#include "main.h"
#include "userTasks.h"
#include "launchPadUIO.h"
#include "isr.h"
#include "LMCterminal.h"
#include "global_state.h"

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
	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOB);
	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOC);
	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOD);
	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOE);
	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOF);
	
	while(!SysCtlPeripheralReady(SYSCTL_PERIPH_GPIOA));
	while(!SysCtlPeripheralReady(SYSCTL_PERIPH_GPIOB));
	while(!SysCtlPeripheralReady(SYSCTL_PERIPH_GPIOC));
	while(!SysCtlPeripheralReady(SYSCTL_PERIPH_GPIOD));
	while(!SysCtlPeripheralReady(SYSCTL_PERIPH_GPIOE));
	while(!SysCtlPeripheralReady(SYSCTL_PERIPH_GPIOF));
	
	// initialize LaunchPad RGB LED and push buttons
	initUIO();
	
	initPWM(pwm0,pwm_gen0);
	initPWM(pwm0,pwm_gen1);
	initPWM(pwm0,pwm_gen2);
	initPWM(pwm0,pwm_gen3);
	initPWM(pwm1,pwm_gen0);
	initPWM(pwm1,pwm_gen1);
	
	initQEI(qei0);
	initQEI(qei1);
	
	// add all the existing tasks
	initTaskMaster();
	
	//////////////////
	// Kernel Tasks //
	//////////////////
	
	// The UART Manager processes all UART I/O
	addTask(uartManager, 2);
	
	////////////////
	// User Tasks //
	////////////////
	
	// Read global given speeds and QEI input, run PID loop, set wheel PWMs
	addTask(ctrlLoop, 0);
	// parse UART packets and set global state such as given wheel speeds
	addTask(SerialReader, 0);
	// the Blinky task toggles the RGB LED once every seconds to indicate the
	// scheduler is working as intended
	addTask(blinkyTask, 0);
	// Bluetooth radio used for localization
	addTask(radio_init, 0);
	
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
		
		if (qeiIntMask != 0) {
			for (i=0; i<NUM_INT_CALLBACKS; i++) {
				if (qeiIntVector[i] != 0 && taskTable[qeiIntVector[i]].interruptCallback != 0) {
					currTaskID = qeiIntVector[i];
					currTask.frame.R0 = (uint32_t)Quadrature;
					currTask.frame.R1 = qeiIntMask;
					currTask.frame.LR = (uint32_t)userReturn;
					currTask.frame.SP = (uint32_t)(frameBase(i));
					runTask(&(currTask.frame), &kframe, (int (*)(uint32_t))currTask.interruptCallback);
				}
			}
			qeiIntMask = 0;
		}
	}
}
