#define PART_TM4C123GH6PM

#include <string.h>

#include "scheduler.h"
#include "launchPadUIO.h"

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
	
	while (i <= currTasks) {
		// For each task, if it is ready, run its callback. If not, decrement its remaining ticks. 
		if (taskTable[i].ticksRemaining == 0 && taskTable[i].status == TASK_STATUS_RUNNING) {
			// Keep track of what the running task is
			currTaskID = i;
			// call the tasks's periodic timer callback
			if (taskTable[i].timerCallback != 0 && taskTable[i].timerCallback(0) < 0) {
				// Indicate task failure by setting LED to magenta
				setLED(magenta);
				while(1);
			}
			currTaskID = 0;
			
			// Reload the remaining ticks counter
			taskTable[i].ticksRemaining = taskTable[i].ticksInterval;
		} else {
			taskTable[i].ticksRemaining--;
		}
		
		i++;
	}
}

void SysTick_Handler(void) {
	// Update system uptime.
	uptime++;
	
	// Run a round of the scheduler.
	runScheduler = true;
	
	return;
}
