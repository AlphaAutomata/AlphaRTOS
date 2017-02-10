#include "main.h"
#include "userTasks.h"
#include "launchPadUIO.h"

int main(void) {
	unsigned int blinkyTaskID;
	
	// Initialize GPIO Port A for UART over USB
	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOA);
	while(!SysCtlPeripheralReady(SYSCTL_PERIPH_GPIOA));
	
	// GPIO Port F needs to be unlocked for use
	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOF);
	while(!SysCtlPeripheralReady(SYSCTL_PERIPH_GPIOF));
	// Unlock pins 1, 2, and 3 for software control of LaunchPad RGB LED
	HWREG(GPIOF_BASE + GPIO_O_LOCK) = GPIO_LOCK_KEY;
	HWREG(GPIOF_BASE + GPIO_O_CR) = 0x02 | 0x04 | 0x08;
	
	// initialize LaunchPad RGB LED and push buttons
	initUIO();
	
	// add all the existing tasks
	// TODO: when booting in debug mode, delay initTask calls until a command
	// is received over UART
	initTaskMaster();
	blinkyTaskID = addTask(blinkyTask);
	currTask = &(taskTable[blinkyTaskID]);
	initTask(blinkyTaskID);
	currTask = 0;
	
	while(1) {
		if (runScheduler) {
			schedule(0);
			runScheduler = false;
		}
	}
}
