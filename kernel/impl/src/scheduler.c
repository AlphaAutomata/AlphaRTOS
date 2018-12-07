#include <string.h>
#include <stdint.h>
#include <stdbool.h>

#include "hal_timer_gp.h"

#include "AlphaRTOS_config.h"

#include "concurrency.h"

#include "scheduler.h"

/**
 * \brief Number of stack segments available, based on the total and the per-thread stack sizes.
 */
#define NUM_THREAD_STACKS (HAL_STACK_SIZE/ARTOS_THREAD_STACK_SIZE)

/**
 * \brief Maximum number of user threads, based on available stack segments.
 * 
 * One stack segment is reserved for each core's kernel.
 */
#define NUM_THREADS (NUM_THREAD_STACKS-HAL_NUM_CORES)

/**
 * \brief Maximum number of threads assignable to a CPU core.
 */
#define MAX_THREADS_PER_CPU (NUM_THREADS/HAL_NUM_CORES)

#define ACTIVE_TID(cpu) (cpuActiveThread[cpu])
#define ACTIVE_TCB(cpu) (threadVector[ACTIVE_TID(cpu)])
#define ACTIVE_CTX(cpu) (ACTIVE_TCB(cpu).context)

/**
 * \brief A table of schedulable unit entities.
 */
typedef struct schedTable_ {
	spinLock_t lock;
	tcb_t*     units[MAX_THREADS_PER_CPU];
} schedTable_t;

static volatile uint64_t uptime;

/**
 * \brief A table of each CPU's assigned threads.
 */
static schedTable_t cpuThreadTables[HAL_NUM_CORES];

/**
 * \brief A table of each CPU's kernel frame.
 */
static regframe_t   cpuKernelFrames[HAL_NUM_CORES];

/**
 * \brief A table of indices of each CPU's active user thread.
 */
static intptr_t     cpuActiveThread[HAL_NUM_CORES];

/**
 * \brief Master thread table.
 */
static tcb_t        threadVector[NUM_THREADS];

static regframe_t* getKernelContext(void) {
	uint32_t coreID;
	
	coreID = hal_getCurrentCPU();

	return &(cpuKernelFrames[coreID]);
}

static regframe_t* getUserContext(void) {
	uint32_t coreID;
	
	coreID = hal_getCurrentCPU();

	return &(ACTIVE_CTX(coreID));
}

/**
 * \brief Interrupt service routine to make sure no task overruns its allotted scheduling slot.
 * 
 * \todo Add supervisor call pending
 */
static void schedWatchdog_handler(void) {
	// pend supervisor call to trigger context switch
}

static void userReturn(void) {
	// run the kernel context
	regframe_t* kContext = getKernelContext();
	regframe_t* myContext = getUserContext();
	switchContext(kContext, myContext);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////// Unchecked Task API ////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////

void task_register(intptr_t* tcbIndex, ARTOS_pFn_taskMain taskMain, const char* taskName) {
	intptr_t i;

	for (i=0; i<NUM_THREADS; i++) {
		if (threadVector[i].stack == NULL) {
			threadVector[i].name         = taskName;
			threadVector[i].stack        = (void*)(HAL_STACK_BASE + (i+1)*HAL_STACK_SIZE);
			threadVector[i].priority     = ARTOS_thread_pri_NORMAL;
			threadVector[i].state        = thread_state_UNINITIALIZED;
			threadVector[i].parent       = NULL;
			threadVector[i].schedGroup   = NULL;
			threadVector[i].entryFn.task = taskMain;

			assignStackToRegframe(&(threadVector[i].context), threadVector[i].stack, taskMain);

			*tcbIndex = i;

			return;
		}
	}

	*tcbIndex = (intptr_t)NULL;
}

void task_exec(intptr_t tcbIndex, int argc, char** argv) {
	// Pass initial arguments to the task entry point using ARM ABI.
	assignParams2(&(threadVector[tcbIndex].context), argc, (int)((intptr_t)argv));

	threadVector[tcbIndex].state = thread_state_READY;
}

void task_kill(intptr_t tcbIndex) {
	threadVector[tcbIndex].state = thread_state_ZOMBIE;
}

void task_getHandle(intptr_t* tcbIndex) {
	uint32_t coreID;
	
	coreID = hal_getCurrentCPU();

	*tcbIndex = ACTIVE_TID(coreID);
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

bool task_handleEmbryo(intptr_t handle) {
	return (
		(threadVector[handle].stack != NULL                      ) &&
		(threadVector[handle].state == thread_state_UNINITIALIZED)
	);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////// Unchecked Thread API ///////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////

void thread_create(
	intptr_t*             handle,
	ARTOS_thread_attr_t*  attributes,
	ARTOS_pFn_threadEntry threadEntry,
	void*                 arg
) {
	intptr_t i;
	uint32_t coreID;
	
	coreID = hal_getCurrentCPU();

	for (i=0; i<NUM_THREADS; i++) {
		if (threadVector[i].stack == NULL) {
			threadVector[i].name           = attributes->name;
			threadVector[i].stack          = (void*)(HAL_STACK_BASE + (i+1)*HAL_STACK_SIZE);
			threadVector[i].priority       = attributes->priority;
			threadVector[i].state          = thread_state_UNINITIALIZED;
			threadVector[i].parent         = &(ACTIVE_TCB(coreID));
			threadVector[i].schedGroup     = NULL;
			threadVector[i].entryFn.thread = threadEntry;

			assignStackToRegframe(&(threadVector[i].context), threadVector[i].stack, threadEntry);

			*handle = i;

			return;
		}
	}

	*handle = (intptr_t)NULL;
}

//! \todo Add indication of what this thread is waiting on
//! \todo Add supervisor call
void thread_join(intptr_t handle) {
	uint32_t coreID;
	
	coreID = hal_getCurrentCPU();

	ACTIVE_TCB(coreID).state = thread_state_BLOCKED;

	// set thread wake condition

	// perform supervisor call to trigger context switch
}

//! \todo Add supervisor call
void thread_yield(void) {
	// perform supervisor call to trigger context switch
}

//! \todo Add indication of what this thread is waiting on
//! \todo Add supervisor call
void thread_sleep(unsigned int time) {
	uint32_t coreID;
	
	coreID = hal_getCurrentCPU();

	ACTIVE_TCB(coreID).state = thread_state_BLOCKED;

	// set thread wake condition

	// perform supervisor call to trigger context switch
}

void thread_getHandle(intptr_t* handle) {
	uint32_t coreID;
	
	coreID = hal_getCurrentCPU();

	*handle = ACTIVE_TID(coreID);
}

bool thread_handleValid(intptr_t handle) {
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
//////////////////////////////////////// Scheduler Core API ////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////

void initScheduler(int cpu) {
	schedTable_t* table;

	table = &(cpuThreadTables[cpu]);

	spinLock_init(table->lock);
	memset(&(table->units), 0, sizeof(table->units));

	// Set up a General Purpose Timer which will be used to ensure each task only runs for a limited
	// amount of time. The time slots are set and the interrupts enabled at the beginning of each
	// scheduling round, and the timer is disabled at the end of each scheduling round.
	hal_timerGp_info info = {
		.loadValue         = SYSTICK_INTERVAL,
		.tripValue         = 0,
		.cntDir            = hal_timer_cntDir_DOWN,
		.rpt               = hal_timer_rpt_ONESHOT,
		.start_immediately = false
	};
	hal_timerGp_init(hal_timer_inst_00, schedWatchdog_handler, &info);
}

void schedule(int cpu) {
	schedTable_t* table;
	unsigned int  i;

	table = &(cpuThreadTables[cpu]);
	
	// Set timer so each task has the same amount of time scheduled for it. currTasks+1 sets
	// aside time for OS operations.
	hal_timerGp_info info = {
		.loadValue         = SYSTICK_INTERVAL / MAX_THREADS_PER_CPU,
		.tripValue         = 0,
		.cntDir            = hal_timer_cntDir_DOWN,
		.rpt               = hal_timer_rpt_ONESHOT,
		.start_immediately = false
	};
	hal_timerGp_cfg_info(hal_timer_inst_00, &info);
	
	for (i=0; i<MAX_THREADS_PER_CPU; i++) {
		// Reload the timer at the start of each task.
		hal_timerGp_arm(hal_timer_inst_00);
		
		// For each task, determine action based on its status
		//if (table->units[i]->state == thread_state_READY) {
		//	table->units[i]->state = thread_state_RUNNING;
		//	// perform context switch and run the task
		//	switchContext(&(table->units[i]->context), getKernelContext());
		//	table->units[i]->state = thread_state_READY;
		//}
		if (threadVector[i].state == thread_state_READY) {
			threadVector[i].state = thread_state_RUNNING;
			switchContext(&(threadVector[i].context), getKernelContext());
			threadVector[i].state = thread_state_READY;
		}
	}
	
	// Disable the timer interrupt when done scheduling to avoid unnecessary CPU load from ISRs
	hal_timerGp_disarm(hal_timer_inst_00);
}
