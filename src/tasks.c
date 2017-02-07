#include "tasks.h"

volatile unsigned int currTasks;

bool initTaskMaster(void) {
	// Initially, we have no tasks.
	currTasks = 0;
	
	// Start the SysTick
	SysTickEnable();
	
	// Set SysTick to 1ms
	SysTickPeriodSet(500000);
	
	// Register our scheduler
	SysTickIntRegister(&SysTick_Handler);
	
	// Enable SysTick interrupts
	SysTickIntEnable();
	
	return true;
}

unsigned int addTask(uint32_t interval, bool (*taskInitCallback)(uint32_t), bool (*taskCallback)(uint32_t)) {
	if (taskInitCallback == 0 || taskCallback == 0) {
		return 0;
	}
	
	currTasks++;
	
	taskTable[currTasks].ticksInterval = interval;
	taskTable[currTasks].ticksRemaining = interval;
	taskTable[currTasks].status = TASK_STATUS_SLEEPING;
	taskTable[currTasks].initCallback = taskInitCallback;
	taskTable[currTasks].callback = taskCallback;
	
	return currTasks;
}

bool initTask(unsigned int taskNum) {
	// check for invalid task IDs
	if (taskNum > currTasks || taskNum == 0) {
		return false;
	}
	
	taskTable[taskNum].ticksRemaining = taskTable[taskNum].ticksInterval;
	taskTable[taskNum].status = TASK_STATUS_RUNNING;
	
	return taskTable[taskNum].initCallback(0);
}
