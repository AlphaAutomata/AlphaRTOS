#include <string.h>

#include "scheduler.h"
#include "launchPadUIO.h"

#include "tasks.h"
#include "isr.h"

int currTaskID;

struct task taskTable[NUM_TASKS];
volatile uint64_t uptime;

volatile bool runScheduler;

void initScheduler(void) {
	uptime = 0;
	
	runScheduler = false;
	currTaskID = 0;
	
	memset(taskTable, 0, sizeof(taskTable));
}

void schedule(struct task *oldTask) {
	unsigned int i;
	
	if (currTaskID != 0) {
		// Should never enter the scheduler when there is already a task running
		setLED(magenta);
		while(1);
	}
	
	for (i=1; i<=currTasks; i++) {
		// For each task, if it is ready, run its callback. If not, decrement its remaining ticks. 
		if (taskTable[i].ticksRemaining == 0 && taskTable[i].status == TASK_STATUS_RUNNING) {
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
			taskTable[i].ticksRemaining = taskTable[i].ticksInterval;
		} else if (taskTable[i].status == TASK_STATUS_YIELDING) {
			// if the task had previously yielded the processor, let it run again
			currTaskID = i;
			currTask.status = TASK_STATUS_RUNNING;
			switchContext(&(currTask.frame), &kframe);
			currTaskID = 0;
		} else {
			taskTable[i].ticksRemaining--;
		}
	}
}

void SysTick_Handler(void) {
	// Update system uptime.
	uptime++;
	
	// Run a round of the scheduler.
	runScheduler = true;
	
	// If the SysTick interrupts when a task is running, kill the task
	if (currTaskID != 0) {
		// Gotta figure out how to do this
	}
	
	return;
}
