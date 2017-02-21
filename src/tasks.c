#include <string.h>

#include "tasks.h"
#include "launchPadHwAbstraction.h"
#include "isr.h"
#include "memory.h"

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

void userReturn(void) {
	unsigned int ID;
	
	// clear the running task indicator
	ID = currTaskID;
	currTaskID = 0;
	
	// run the kernel context
	switchContext(&kframe, &((taskTable[ID]).frame));
}

unsigned int addTask(int (*taskEntry)(uint32_t), uint32_t initArg) {
	if (taskEntry == 0 || currTasks >= NUM_TASKS) {
		return 0;
	}
	
	currTasks++;
	
	taskTable[currTasks].status = TASK_STATUS_UNINITIALIZED;
	taskTable[currTasks].initArg = initArg;
	taskTable[currTasks].taskEntry = taskEntry;
	
	return currTasks;
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

int interruptCallbackRegister(eInterrupt interrupt, int (*callback)(eInterrupt interruptType, uint32_t deviceMask), int deviceNumber) {
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
					isrAddr = uart1ISR;
					break;
				case 2 :
					deviceBase = UART2_BASE;
					isrAddr = uart2ISR;
					break;
				case 3 :
					deviceBase = UART3_BASE;
					isrAddr = uart3ISR;
					break;
				case 4 :
					deviceBase = UART4_BASE;
					isrAddr = uart4ISR;
					break;
				case 5 :
					deviceBase = UART5_BASE;
					isrAddr = uart5ISR;
					break;
				case 6 :
					deviceBase = UART6_BASE;
					isrAddr = uart6ISR;
					break;
				case 7 :
					deviceBase = UART7_BASE;
					isrAddr = uart7ISR;
					break;
				default :
					return -1;
			}
			
			// enable the UART transmit and receive FIFOs
			UARTFIFOEnable(deviceBase);
			// register the appropririate UART interrupt service routine
			UARTIntRegister(deviceBase, isrAddr);
			// Set the trasmit FIFO to trigger on FIFO level
			UARTTxIntModeSet(deviceBase, UART_TXINT_MODE_FIFO);
			// set both FIFOs levels. Tx interrupts when almost empty, Rx interrupts when almost full.
			UARTFIFOLevelSet(deviceBase, UART_FIFO_TX1_8, UART_FIFO_RX7_8);
			// enable interrupts for receive and transmit
			UARTIntEnable(deviceBase, UART_INT_RX | UART_INT_TX);
			
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
