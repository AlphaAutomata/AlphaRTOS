#define PART_TM4C123GH6PM

#include "TM4C123.h"
#include "scheduler.h"
#include "main.h"

struct task *currTask;

struct task taskTable[NUM_TASKS];
volatile uint32_t uptime;

volatile bool runScheduler;

void initScheduler(void) {
	unsigned int i;
	
	uptime = 0;
	
	runScheduler = false;
	currTask = 0;
	
	for (i=0; i<NUM_TASKS; i++) {
		taskTable[i].ticksInterval = 0;
		taskTable[i].ticksRemaining = 0;
		taskTable[i].status = TASK_STATUS_UNINITIALIZED;
		taskTable[i].callback = 0;
	}
}

void schedule(void) {
	unsigned int i;
	
	if (currTask != 0) {
		// Should never enter the scheduler when there is already a task running
		while(1);
	}
	
	// Round robin scheduler
	for (i=1; i<=currTasks; i++) {
		// For each task, if it is ready, run its callback. If not, decrement its remaining ticks. 
		if (taskTable[i].ticksRemaining == 0 && taskTable[i].status == TASK_STATUS_RUNNING) {
			// Keep track of what the running task is
			currTask = &(taskTable[i]);
			if (!taskTable[i].callback(0)) {
				// a task failed
				while(1);
			}
			currTask = 0;
			
			// Reload the remaining ticks counter
			taskTable[i].ticksRemaining = taskTable[i].ticksInterval;
		} else {
			taskTable[i].ticksRemaining--;
		}
	}
	//taskTable[1].callback(0);
}

void SysTick_Handler(void) {
	// Update system uptime.
	uptime++;
	
	// Run a round of the scheduler.
	runScheduler = true;
}
