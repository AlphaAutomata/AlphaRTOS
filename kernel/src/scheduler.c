#include <string.h>
#include <stdint.h>
#include <stdbool.h>

#include "gp_timer_port.h"

#include "core_ca.h"

#include "task.h"

#include "scheduler.h"

static volatile uint64_t uptime;

static schedTable_t allTables[NUM_CORES];
static regframe_t   kInstFrames[NUM_CORES];

static inline regframe_t* getKernelContext(void) {
	uint32_t coreID;
	
	coreID = __get_MPIDR();

	return &(kInstFrames[coreID]);
}

static inline regframe_t* getMyContext(void) {
	uint32_t coreID;
	uint32_t mySP;
	
	coreID = __get_MPIDR();
	mySP = __get_SP();

	for (int i=0; i<NUM_UNITS; i++) {
		intptr_t myStackBase;

		myStackBase = (intptr_t)(allTables[coreID].units[i].attributes.stack_mem);

		if (mySP >= myStackBase && mySP <= myStackBase+STACK_SIZE) {
			return &(allTables[coreID].units[i].context);
		}
	}

	return NULL;
}

/**
 * \brief Interrupt service routine to make sure no task overruns its allotted scheduling slot.
 */
static void schedWatchdog_handler(void) {
	regframe_t* kContext = getKernelContext();
	regframe_t* myContext = getMyContext();
	switchContext(kContext, myContext);
}

void initScheduler(schedTable_t* table) {
	uptime = 0;
	
	// Set up a General Purpose Timer which will be used to ensure each task only runs for a limited
	// amount of time. The time slots are set and the interrupts enabled at the beginning of each
	// scheduling round, and the timer is disabled at the end of each scheduling round.
	gpTimer_info info = {
		.loadValue         = SYSTICK_INTERVAL,
		.tripValue         = 0,
		.cntDir            = gpTimer_cntDir_DOWN,
		.rpt               = gpTimer_rpt_ONESHOT,
		.start_immediately = false
	};
	gpTimer_init(gpTimer_inst_00, schedWatchdog_handler, &info);
}

void schedule(schedTable_t* table) {
	unsigned int i;
	
	// Set timer so each task has the same amount of time scheduled for it. currTasks+1 sets
	// aside time for OS operations.
	gpTimer_info info = {
		.loadValue         = SYSTICK_INTERVAL / NUM_UNITS,
		.tripValue         = 0,
		.cntDir            = gpTimer_cntDir_DOWN,
		.rpt               = gpTimer_rpt_ONESHOT,
		.start_immediately = false
	};
	gpTimer_cfg_info(gpTimer_inst_00, &info);
	
	for (i=1; i<=NUM_UNITS; i++) {
		// Reload the timer at the start of each task.
		gpTimer_arm(gpTimer_inst_00);
		
		// For each task, determine action based on its status
		if (table->units[i].state == task_state_READY) {
			table->units[i].state = task_state_RUNNING;
			// perform context switch and run the task
			table->units[i].context.LR = (uint32_t)userReturn;
			table->units[i].context.SP = (uint32_t)(frameBase(i));
			switchContext(getKernelContext(), &(table->units[i].context));
			table->units[i].state = task_state_READY;
		}
	}
	
	// Disable the timer interrupt when done scheduling to avoid unnecessary CPU load from ISRs
	gpTimer_disarm(gpTimer_inst_00);
}
