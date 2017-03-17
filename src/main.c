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
	// Set the LED to red to indicate a hard fault
	setLED(red);
	while(1);
}

int main(void) {
	int i;
	
	// Initialize GPIO Port A for UART over USB
	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOA);
	while(!SysCtlPeripheralReady(SYSCTL_PERIPH_GPIOA));
	
	// GPIO Port F needs to be unlocked for use
	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOF);
	while(!SysCtlPeripheralReady(SYSCTL_PERIPH_GPIOF));
	
	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOB);
	while(!SysCtlPeripheralReady(SYSCTL_PERIPH_GPIOB));
	
	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOE);
	while(!SysCtlPeripheralReady(SYSCTL_PERIPH_GPIOE));
	
	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOD);
	while(!SysCtlPeripheralReady(SYSCTL_PERIPH_GPIOD));
	
	
	
	// initialize LaunchPad RGB LED and push buttons
	initUIO();
	
	initPWM(pwm0,pwm_gen0);
	initPWM(pwm0,pwm_gen1);
	initPWM(pwm0,pwm_gen2);
	initPWM(pwm0,pwm_gen3);
	
	// add all the existing tasks
	// TODO: when booting in debug mode, delay initTask calls until a command
	// is received over UART
	initTaskMaster();
	
	// the Blinky task just toggles the RGB LED once every seconds to indicate
	// the scheduler is working as intended
	addTask(blinkyTask, 0);
	
	// the LMC Terminal communicates over USB UART, and here we initalize it to 1Mbaud
	addTask(initLMCterminal, 1000000);
	
	while(1) {
		// SysTick triggers this every millisecond
		if (runScheduler) {
			schedule();
			runScheduler = false;
		}
		
		// triggered from isr.c by UART interrupts
		if (uartIntMask != 0) {
			// search through the UART interrupt vector to see if we need to
			// call any callbacks
			for (i=0; i<NUM_INT_CALLBACKS; i++) {
				if (uartIntVector[i] != 0 && taskTable[uartIntVector[i]].interruptCallback != 0) {
					currTaskID = uartIntVector[i];
					// conforming to ARM EABI, put the arguments to the user
					// function in R0 and R1
					currTask.frame.R0 = (uint32_t)UART;
					currTask.frame.R1 = uartIntMask;
					// set up user frame's LR and SP
					currTask.frame.LR = (uint32_t)userReturn;
					currTask.frame.SP = (uint32_t)(frameBase(i));
					// have the SysTick ISR return into the kernel at userReturn() when preempting tasks
					runTask(&(currTask.frame), &kframe, (int (*)(uint32_t))currTask.interruptCallback);
				}
				uartIntMask = 0;
			}
		}
	}
}
