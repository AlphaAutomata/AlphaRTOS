#include <string.h>

#include "tasks.h"
#include "launchPadHwAbstraction.h"
#include "isr.h"

volatile unsigned int currTasks;

int gpTimerIntVector[NUM_INT_CALLBACKS];
int uartIntVector[NUM_INT_CALLBACKS];

bool initTaskMaster(void) {
	// Initially, we have no tasks.
	currTasks = 0;
	
	// Start the SysTick
	SysTickEnable();
	
	// Set SysTick to 1ms
	SysTickPeriodSet(50000);
	
	// Register our scheduler
	SysTickIntRegister(&SysTick_Handler);
	
	// Enable SysTick interrupts
	SysTickIntEnable();
	
	// clearn the interrupt vectors
	memset(gpTimerIntVector, 0, sizeof(gpTimerIntVector));
	memset(uartIntVector, 0, sizeof(gpTimerIntVector));
	
	// initialize the scheduler
	initScheduler();
	
	return true;
}

unsigned int addTask(int (*taskEntry)(void *)) {
	if (taskEntry == 0 || currTasks >= NUM_TASKS) {
		return 0;
	}
	
	currTasks++;
	
	taskTable[currTasks].status = TASK_STATUS_UNINITIALIZED;
	taskTable[currTasks].taskEntry = taskEntry;
	
	return currTasks;
}

int initTask(unsigned int taskNum, void *arg) {
	// check for invalid task IDs
	if (taskNum > currTasks || taskNum == 0) {
		return false;
	}
	
	taskTable[taskNum].status = TASK_STATUS_RUNNING;
	
	return taskTable[taskNum].taskEntry(arg);
}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////// Badger RMC RTOS API /////////////////////////////
///////////////////////////////////////////////////////////////////////////////

int timerCallbackRegister(uint32_t interval, int (*callback)(void *)) {
	if (currTaskID == 0 || callback == 0 || interval == 0) {
		return 0;
	}
	
	currTask.timerCallback = callback;
	currTask.ticksInterval = interval;
	currTask.ticksRemaining = interval;
	
	return 1;
}

int timerCallbackUnregister(int callbackID) {
	if (currTaskID == 0 || callbackID == 0) {
		return -1;
	}
	
	currTask.timerCallback = 0;
	
	return callbackID;
}

int setTimerCallbackInterval(int callbackID, uint32_t interval) {
	if (currTaskID == 0 || callbackID != 1) return -1;
	
	currTask.ticksInterval = interval;
	
	return callbackID;
}

uint32_t getTimerCallbackInterval(int callbackID) {
	if (currTaskID == 0) return 0;
	
	return currTask.ticksInterval;
}

int interruptCallbackRegister(eInterrupt interrupt, int (*callback)(eInterrupt interruptType, uint8_t deviceMask), int deviceNumber) {
	int *iVector;
	int i;
	uint32_t deviceBase;
	void (*isrAddr)(void);
	
	if (currTaskID == 0) return -1;
	
	// select the appropriate interrupt vector to search through
	switch (interrupt) {
		case gpTimer :
			iVector = gpTimerIntVector;
			switch (deviceNumber) {
				case 0 :
					deviceBase = TIMER0_BASE;
					break;
				case 1 :
					deviceBase = TIMER1_BASE;
					break;
				default :
					return -1;
			}
			break;
		case UART :
			iVector = uartIntVector;
			switch (deviceNumber) {
				case 0 :
					deviceBase = UART0_BASE;
					isrAddr = uart0ISR;
					break;
				case 1 :
					deviceBase = UART1_BASE;
					break;
				case 2 :
					deviceBase = UART2_BASE;
					break;
				case 3 :
					deviceBase = UART3_BASE;
					break;
				case 4 :
					deviceBase = UART4_BASE;
					break;
				case 5 :
					deviceBase = UART5_BASE;
					break;
				case 6 :
					deviceBase = UART6_BASE;
					break;
				case 7 :
					deviceBase = UART7_BASE;
					break;
				default :
					return -1;
			}
			
			UARTIntRegister(deviceBase, isrAddr);
			UARTIntEnable(deviceBase, UART_INT_TX | UART_INT_RX);
			
			break;
		default :
			return -1;
	}
	
	// locate an empty slot in the vector
	for (i=0; i<NUM_INT_CALLBACKS; i++) {
		if (iVector[i] == 0) {
			// put the callback in the empty slot
			iVector[i] = currTaskID;
			currTask.interruptCallback = callback;
			
			return i;
		}
	}
	
	return -1;
}

int interruptCallbackUnregister(eInterrupt interrupt, int callbackID) {
	int *iVector;
	
	if (currTaskID == 0) return -1;
	
	switch (interrupt) {
		case gpTimer :
			iVector = gpTimerIntVector;
			break;
		case UART :
			iVector = uartIntVector;
			break;
	}
	
	iVector[callbackID] = 0;
	
	return callbackID;
}

void taskYield(void) {
	return;
}

uint64_t getUptime(void) {
	return uptime;
}
