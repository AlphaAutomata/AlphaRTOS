#include <string.h>
#include <stdbool.h>
#include <stdint.h>

#include "systick_port.h"

#include "scheduler.h"

#include "task.h"

taskTable_t taskTable;

int gpTimerIntVector[NUM_INT_CALLBACKS];
int uartIntVector[NUM_INT_CALLBACKS];
int qeiIntVector[NUM_INT_CALLBACKS];

bool initTaskMaster(void) {
	// Initialize the task table.
	concurr_mutex_init(taskTable.mutex);
	taskTable.currNumTasks = 0;
	taskTable.currTaskID = -1;
	memset(&(taskTable.tasks[0]), 0, sizeof(taskTable.tasks));
	
	// clear the interrupt vectors
	memset(gpTimerIntVector, 0, sizeof(gpTimerIntVector));
	memset(uartIntVector, 0, sizeof(gpTimerIntVector));
	memset(qeiIntVector, 0, sizeof(qeiIntVector));
	
	// initialize the scheduler
	initScheduler(&taskTable);
	
	// Initialize the system tick timer to 1ms intervals, and register the handler.
	systick_init(&SysTick_Handler, SYSTICK_INTERVAL);
	
	return true;
}

void userReturn(void) {
	unsigned int ID;
	
	// clear the running task indicator
	ID = currTaskID;
	currTaskID = 0;
	
	// run the kernel context
	switchContext(&kframe, &((taskTable[ID]).frame));
}

int addTask(pFn_taskMain taskEntry, char taskName[]) {
	concurr_mutex_lock(taskTable.mutex);
	
	taskTable[old_num_tasks].status = TASK_STATUS_UNINITIALIZED;
	taskTable[old_num_tasks].taskEntry = taskEntry;
	
	concurr_mutex_unlock(taskTable.mutex);
	return old_num_tasks + 1;
}

int initTask(unsigned int taskNum, uint32_t arg) {
	// check for invalid task IDs
	if (taskNum > currTasks || taskNum == 0) {
		return false;
	}
	
	taskTable[taskNum].status = TASK_STATUS_RUNNING;
	
	taskTable[taskNum].frame.R0 = arg;
	taskTable[taskNum].frame.LR = (uint32_t)(userReturn);
	taskTable[taskNum].frame.SP = (uint32_t)(frameBase(taskNum));
	runTask(
		&(taskTable[taskNum].frame),
		&kframe,
		taskTable[taskNum].taskEntry
	);
	
	return taskTable[taskNum].frame.R0;
}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////// Badger RMC RTOS API /////////////////////////////
///////////////////////////////////////////////////////////////////////////////

int timerCallbackRegister(uint32_t interval, int (*callback)(uint32_t)) {
	if (currTaskID == 0 || callback == 0 || interval == 0) {
		return 0;
	}
	
	currTask.timerCallback = callback;
	currTask.ticksInterval = interval - 1;
	currTask.ticksRemaining = interval - 1;
	
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

void taskYield(void) {
	unsigned int ID;
	
	currTask.status = TASK_STATUS_YIELDING;
	
	ID = currTaskID;
	currTaskID = 0;
	
	switchContext(&kframe, &((taskTable[ID]).frame));
	
	return;
}

uint64_t getUptime(void) {
	return uptime;
}
