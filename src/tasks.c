#include "tasks.h"
#include "launchPadHwAbstraction.h"

volatile unsigned int currTasks;

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

int initTask(unsigned int taskNum) {
	// check for invalid task IDs
	if (taskNum > currTasks || taskNum == 0) {
		return false;
	}
	
	taskTable[taskNum].status = TASK_STATUS_RUNNING;
	
	return taskTable[taskNum].taskEntry(0);
}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////// Badger RMC RTOS API /////////////////////////////
///////////////////////////////////////////////////////////////////////////////

int timerCallbackRegister(uint32_t interval, int (*callback)(void *)) {
	if (currTask == 0 || callback == 0 || interval == 0) {
		return 0;
	}
	
	currTask->timerCallback = callback;
	
	return 1;
}

int timerCallbackUnregister(int callbackID) {
	if (currTask == 0 || callbackID == 0) {
		return -1;
	}
	
	currTask->timerCallback = 0;
	
	return callbackID;
}

int setTimerCallbackInterval(int callbackID, uint32_t interval) {
	if (currTask == 0 || callbackID != 1) return -1;
	
	currTask->ticksInterval = interval;
	
	return callbackID;
}

uint32_t getTimerCallbackInterval(int callbackID) {
	if (currTask == 0) return 0;
	
	return currTask->ticksInterval;
}

int interruptCallbackRegister(eInterrupt interrupt, int (*callback)(void *)) {
	
	return -1;
}

void taskYield(void) {
	return;
}

uint64_t getUptime(void) {
	return uptime;
}
