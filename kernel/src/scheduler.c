#include <string.h>
#include <stdint.h>
#include <stdbool.h>

#include "gp_timer_port.h"

#include "task.h"

#include "scheduler.h"

int currTaskID;

struct task taskTable[NUM_TASKS];
volatile uint64_t uptime;

volatile bool runScheduler;

void initScheduler(void) {
	uptime = 0;
	
	runScheduler = false;
	currTaskID = 0;
	
	memset(taskTable, 0, sizeof(taskTable));
	
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

void schedule(void) {
	unsigned int i;
	
	if (currTaskID != 0) {
		// Should never enter the scheduler when there is already a task running
		while(1);
	}

	// Set timer so each task has the same amount of time scheduled for it. currTasks+1 sets
	// aside time for OS operations.
	gpTimer_info info = {
		.loadValue         = SYSTICK_INTERVAL / (currTasks+1),
		.tripValue         = 0,
		.cntDir            = gpTimer_cntDir_DOWN,
		.rpt               = gpTimer_rpt_ONESHOT,
		.start_immediately = false
	};
	gpTimer_cfg_info(gpTimer_inst_00, &info);
	
	for (i=1; i<=currTasks; i++) {
		// Reload the timer at the start of each task.
		gpTimer_arm(gpTimer_inst_00);
		
		// For each task, determine action based on its status
		switch (taskTable[i].status) {
			case TASK_STATUS_UNINITIALIZED :
				currTaskID = i;
				initTask(i, currTask.initArg);
				currTaskID = 0;
				break;
			case TASK_STATUS_RUNNING :
				if (taskTable[i].ticksRemaining == 0) {
					// Keep track of what the running task is
					// call the tasks's periodic timer callback
					if (taskTable[i].timerCallback != 0) {
						// perform context switch and run the task
						currTaskID = i;
						currTask.frame.LR = (uint32_t)userReturn;
						currTask.frame.SP = (uint32_t)(frameBase(i));
						runTask(&(currTask.frame), &kframe, currTask.timerCallback);
						currTaskID = 0;
					}
					// Reload the remaining ticks counter
					taskTable[i].ticksRemaining = taskTable[i].ticksInterval - 1;
				} else {
					taskTable[i].ticksRemaining--;
				}
				break;
			case TASK_STATUS_YIELDING :
					// if the task had previously yielded the processor, let it run again
					currTaskID = i;
					currTask.status = TASK_STATUS_RUNNING;
					switchContext(&(currTask.frame), &kframe);
					currTaskID = 0;
				break;
			default :
				break;
		}
	}
	
	// Disable the timer interrupt when done scheduling to avoid unnecessary CPU load from ISRs
	gpTimer_disarm(gpTimer_inst_00);
}

void schedulePreempted(void) {
	int i;

	// Set timer so each task has the same amount of time scheduled for it. currTasks+1 sets
	// aside time for OS operations.
	gpTimer_info info = {
		.loadValue         = SYSTICK_INTERVAL / (currTasks+1),
		.tripValue         = 0,
		.cntDir            = gpTimer_cntDir_DOWN,
		.rpt               = gpTimer_rpt_ONESHOT,
		.start_immediately = false
	};
	gpTimer_cfg_info(gpTimer_inst_00, &info);
	
	for (i=1; i<=currTasks; i++) {
		// Reload the timer at the start of each task.
		gpTimer_arm(gpTimer_inst_00);
		
		// For each task, determine action based on its status
		if (taskTable[i].status == TASK_STATUS_PREEMPTED) {
			// if the task had been preempted, let it run again
			currTaskID = i;
			currTask.status = TASK_STATUS_RUNNING;
			switchContext(&(currTask.frame), &kframe);
			currTaskID = 0;
		}
	}
	
	// disable the timer interrupt when done scheduling to avoid unnecessary CPU load from ISRs
	gpTimer_disarm(gpTimer_inst_00);
}
