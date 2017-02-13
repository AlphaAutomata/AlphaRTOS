#include "main.h"
#include "userTasks.h"
#include "launchPadUIO.h"
#include "isr.h"
#include "LMCterminal.h"

//*****************************************************************************
//
//! Set the LED to red to indicate a hard fault
//!
//! \param none
//!
//! \return none
//
//*****************************************************************************
void HardFault_Handler(void) {
	setLED(red);
	while(1);
}

int main(void) {
	int i;
	
	unsigned int blinkyTaskID;
	unsigned int terminalTaskID;
	
	// Initialize GPIO Port A for UART over USB
	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOA);
	while(!SysCtlPeripheralReady(SYSCTL_PERIPH_GPIOA));
	
	// GPIO Port F needs to be unlocked for use
	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOF);
	while(!SysCtlPeripheralReady(SYSCTL_PERIPH_GPIOF));
	
	// initialize LaunchPad RGB LED and push buttons
	initUIO();
	
	// add all the existing tasks
	// TODO: when booting in debug mode, delay initTask calls until a command
	// is received over UART
	initTaskMaster();
	
	// the Blinky task just toggles the RGB LED once every seconds to indicate
	// the scheduler is working as intended
	blinkyTaskID = addTask(blinkyTask);
	currTaskID = blinkyTaskID;
	initTask(blinkyTaskID, 0);
	currTaskID = 0;
	
	// the LMC Terminal communicates over USB UART
	terminalTaskID = addTask(initLMCterminal);
	currTaskID = terminalTaskID;
	initTask(terminalTaskID, (void *)115200);
	currTaskID = 0;
	
	//kprintf("Line w/15 char\n");
	//kprintf("This is a stupidly long string to test if the circular buffer is working\nIf the buffer is working, then this string will print perfecly and won't break anything\nIs 255 characters a lot? Have I hit it yet?\nMaybe not. This line will definitely do it though.\n");
	
	while(1) {
		// SysTick triggers this every millisecond
		if (runScheduler) {
			schedule(0);
			runScheduler = false;
		}
		
		// triggered from isr.c by UART interrupts
		if (uartIntMask != 0) {
			// search through the UART interrupt vector to see if we need to
			// call any callbacks
			for (i=0; i<NUM_INT_CALLBACKS; i++) {
				if (uartIntVector[i] != 0 && taskTable[uartIntVector[i]].interruptCallback != 0) {
					taskTable[uartIntVector[i]].interruptCallback(UART, uartIntMask);
				}
				uartIntMask = 0;
			}
		}
	}
}
