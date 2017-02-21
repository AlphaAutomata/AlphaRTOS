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

void schedule(void) {
	unsigned int i;
	
	if (currTaskID != 0) {
		// Should never enter the scheduler when there is already a task running
		setLED(magenta);
		while(1);
	}
	
	for (i=1; i<=currTasks; i++) {
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
					taskTable[i].ticksRemaining = taskTable[i].ticksInterval;
				} else {
					taskTable[i].ticksRemaining--;
				}
				break;
			case TASK_STATUS_YIELDING :
			case TASK_STATUS_PREEMPTED :
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
}
