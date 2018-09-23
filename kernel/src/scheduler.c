#include <string.h>
#include <stdint.h>
#include <stdbool.h>

#include "gp_timer_port.h"

#include "task.h"

#include "scheduler.h"

volatile uint64_t uptime;

volatile bool runScheduler;

/**
//! Interrupt service routine to make sure no task overruns its allotted
//! scheduling slot. Implemented in memoryS.s, since it forces a context switch
//!
//! \param none
//!
//! \return none
//
 */
extern void TIMER0A_Handler(void);

void initScheduler(taskTable_t* tasks) {
	uptime = 0;
	
	runScheduler = false;

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
	gpTimer_init(gpTimer_inst_00, TIMER0A_Handler, &info);
}

void schedule(taskTable_t* tasks) {
	unsigned int i;
	
	if (tasks->currTaskID != 0) {
		// Should never enter the scheduler when there is already a task running
		while(1);
	}

	// Set timer so each task has the same amount of time scheduled for it. currTasks+1 sets
	// aside time for OS operations.
	gpTimer_info info = {
		.loadValue         = SYSTICK_INTERVAL / (tasks->currNumTasks+1),
		.tripValue         = 0,
		.cntDir            = gpTimer_cntDir_DOWN,
		.rpt               = gpTimer_rpt_ONESHOT,
		.start_immediately = false
	};
	gpTimer_cfg_info(gpTimer_inst_00, &info);
	
	for (i=1; i<=tasks->currNumTasks; i++) {
		// Reload the timer at the start of each task.
		gpTimer_arm(gpTimer_inst_00);
		
		// For each task, determine action based on its status
		switch (tasks->tasks[i].status) {
			case TASK_STATUS_UNINITIALIZED :
				break;
			case TASK_STATUS_RUNNING : // fallthrough
			case TASK_STATUS_PREEMPTED :
				// perform context switch and run the task
				tasks->currTaskID = i;
				tasks->tasks[i].frame.LR = (uint32_t)userReturn;
				tasks->tasks[i].frame.SP = (uint32_t)(frameBase(i));
				runTask(&(tasks->tasks[i].frame), &kframe, tasks->tasks[i].taskEntry);
				tasks->currTaskID = 0;
				break;
			case TASK_STATUS_YIELDING :
				// if the task had previously yielded the processor, let it run again
				tasks->currTaskID = i;
				tasks->tasks[i].status = TASK_STATUS_RUNNING;
				switchContext(&(tasks->tasks[i].frame), &kframe);
				tasks->currTaskID = 0;
				break;
			default :
				break;
		}
	}
	
	// Disable the timer interrupt when done scheduling to avoid unnecessary CPU load from ISRs
	gpTimer_disarm(gpTimer_inst_00);
}

void schedulePreempted(taskTable_t* tasks) {
	int i;

	// Set timer so each task has the same amount of time scheduled for it. currTasks+1 sets
	// aside time for OS operations.
	gpTimer_info info = {
		.loadValue         = SYSTICK_INTERVAL / (tasks->currNumTasks+1),
		.tripValue         = 0,
		.cntDir            = gpTimer_cntDir_DOWN,
		.rpt               = gpTimer_rpt_ONESHOT,
		.start_immediately = false
	};
	gpTimer_cfg_info(gpTimer_inst_00, &info);
	
	for (i=1; i<=tasks->currNumTasks; i++) {
		// Reload the timer at the start of each task.
		gpTimer_arm(gpTimer_inst_00);
		
		// For each task, determine action based on its status
		if (tasks->tasks[i].status == TASK_STATUS_PREEMPTED) {
			// if the task had been preempted, let it run again
			tasks->currTaskID = i;
			tasks->tasks[i].status = TASK_STATUS_RUNNING;
			switchContext(&(tasks->tasks[i].frame), &kframe);
			tasks->currTaskID = 0;
		}
	}
	
	// disable the timer interrupt when done scheduling to avoid unnecessary CPU load from ISRs
	gpTimer_disarm(gpTimer_inst_00);
}
