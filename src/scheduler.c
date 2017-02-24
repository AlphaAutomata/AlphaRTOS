#include <string.h>
#include <stdint.h>
#include <stdbool.h>

#include "timer.h"

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
	
	// turn on clock for Timer0 peripheral device
	SysCtlPeripheralEnable(SYSCTL_PERIPH_TIMER0);
	while(!SysCtlPeripheralReady(SYSCTL_PERIPH_TIMER0));
	
	// set up a General Purpose Timer which will be used to ensure each task only runs for a
	// limited amount of time. The time slots are set and the interrupts enabled at the beginning
	// of each scheduling round, and disabled at the end of each scheduling round. 
	// Set timer to be periodic, and have no additional actions other than triggering and interrupt
	// when timing out. 
	TimerConfigure(TIMER0_BASE, TIMER_CFG_ONE_SHOT);
	TimerIntRegister(TIMER0_BASE, TIMER_A, TIMER0A_Handler);
}

void schedule(void) {
	unsigned int i;
	
	if (currTaskID != 0) {
		// Should never enter the scheduler when there is already a task running
		setLED(magenta);
		while(1);
	}
	
	// enable the interrupt right before entering the scheduling loop
	TimerIntEnable(TIMER0_BASE, TIMER_TIMA_TIMEOUT);
	
	for (i=1; i<=currTasks; i++) {
		// set timer so each task has the same amount of time scheduled for it. currTasks+1 sets
		// aside time for OS operations. Reload the timer at the start of each task. 
		TimerLoadSet(TIMER0_BASE, TIMER_A, SYSTICK_INTERVAL / (currTasks+1));
		TimerEnable(TIMER0_BASE, TIMER_A);
		
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
	
	// disable the timer interrupt when done scheduling to avoid unnecessary CPU load from ISRs
	TimerIntDisable(TIMER0_BASE, TIMER_TIMA_TIMEOUT);
}
