#include <string.h>
#include <stdint.h>
#include <stdbool.h>

#include "gp_timer_port.h"
#include "port_concurrent.h"

#include "mem_ARMCA9.h"
#include "ARMCA9.h"

#include "scheduler.h"

#define STACK_SIZE (__STACK_SIZE)
#define STACK_BASE (__RAM_BASE+__RAM_SIZE)

#define NUM_THREADS (NUM_CORES*NUM_UNITS)

/**
 * \brief A table of schedulable unit entities.
 */
typedef struct schedTable_ {
	concurr_mutex mutex;
	tcb_t*        units[NUM_UNITS];
} schedTable_t;

static volatile uint64_t uptime;

static schedTable_t cpuThreadTables[NUM_CORES];
static regframe_t   cpuKernelFrames[NUM_CORES];
static intptr_t     cpuActiveThread[NUM_CORES];
static tcb_t        threadVector[NUM_THREADS];

regframe_t* getKernelContext(void) {
	uint32_t coreID;
	
	coreID = __get_MPIDR();

	return &(cpuKernelFrames[coreID]);
}

regframe_t* getMyContext(void) {
	uint32_t coreID;
	
	coreID = __get_MPIDR();

	return threadVector[cpuActiveThread[coreID]].stack;
}

/**
 * \brief Interrupt service routine to make sure no task overruns its allotted scheduling slot.
 */
static void schedWatchdog_handler(void) {
	regframe_t* kContext = getKernelContext();
	regframe_t* myContext = getMyContext();
	switchContext(kContext, myContext);
}

void userReturn(void) {
	// run the kernel context
	regframe_t* kContext = getKernelContext();
	regframe_t* myContext = getMyContext();
	switchContext(kContext, myContext);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////// Unchecked Task API ////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////

void task_register(intptr_t* tcbIndex, ARTOS_pFn_taskMain taskMain, const char* taskName) {
	intptr_t i;

	for (i=0; i<NUM_THREADS; i++) {
		if (threadVector[i].stack == NULL) {
			threadVector[i].name       = taskName;
			threadVector[i].stack      = (void*)(STACK_BASE + (i+1)*STACK_SIZE);
			threadVector[i].priority   = ARTOS_thread_pri_NORMAL;
			threadVector[i].state      = thread_state_UNINITIALIZED;
			threadVector[i].context.SP = (uint32_t)(threadVector[i].stack);
			threadVector[i].context.LR = (uint32_t)taskMain;
			threadVector[i].parent     = NULL;
			threadVector[i].schedGroup = NULL;

			*tcbIndex = i;

			return;
		}
	}

	*tcbIndex = (intptr_t)NULL;
}

void task_exec(intptr_t tcbIndex, int argc, char** argv) {
	// Pass initial arguments to the task entry point using ARM ABI.
	threadVector[tcbIndex].context.R0 = argc;
	threadVector[tcbIndex].context.R1 = (uint32_t)argv;
	threadVector[tcbIndex].state      = thread_state_READY;
}

void task_kill(intptr_t tcbIndex) {
	threadVector[tcbIndex].state = thread_state_ZOMBIE;
}

void task_getHandle(intptr_t* tcbIndex) {
	uint32_t coreID;
	
	coreID = __get_MPIDR();

	*tcbIndex = cpuActiveThread[coreID];
}

bool task_handleValid(intptr_t handle) {
	return (
		(threadVector[handle].stack != NULL) &&
		(
			(threadVector[handle].state == thread_state_READY  ) ||
			(threadVector[handle].state == thread_state_RUNNING) ||
			(threadVector[handle].state == thread_state_BLOCKED)
		)
	);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////// Unchecked Thread API ///////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////

void initScheduler(int cpu) {
	schedTable_t* table;

	table = &(cpuThreadTables[cpu]);

	concurr_mutex_init(table->mutex);
	memset(&(table->units), 0, sizeof(table->units));

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

void schedule(int cpu) {
	schedTable_t* table;
	unsigned int  i;

	table = &(cpuThreadTables[cpu]);
	
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
	
	for (i=0; i<=NUM_UNITS; i++) {
		// Reload the timer at the start of each task.
		gpTimer_arm(gpTimer_inst_00);
		
		// For each task, determine action based on its status
		if (table->units[i]->state == thread_state_READY) {
			table->units[i]->state = thread_state_RUNNING;
			// perform context switch and run the task
			table->units[i]->context.LR = (uint32_t)userReturn;
			table->units[i]->context.SP = (uint32_t)(frameBase(i));
			switchContext(getKernelContext(), &(table->units[i]->context));
			table->units[i]->state = thread_state_READY;
		}
	}
	
	// Disable the timer interrupt when done scheduling to avoid unnecessary CPU load from ISRs
	gpTimer_disarm(gpTimer_inst_00);
}
