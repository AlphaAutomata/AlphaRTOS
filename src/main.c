#include "main.h"

volatile bool ledOn;

bool blinkyTaskInit(uint32_t arg) {
	ledOn = false;
	
	GPIOPinTypeGPIOOutput(GPIOF_BASE, GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3);
	GPIOPinWrite(GPIOF_BASE, GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3, 0x0);
	
	return true;
}

bool blinkyTaskCallback(uint32_t arg) {
	if (ledOn) {
		GPIOPinWrite(GPIOF_BASE, GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3, 0x0);
		ledOn = false;
	} else {
		GPIOPinWrite(GPIOF_BASE, GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3, GPIO_PIN_1);
		ledOn = true;
	}
	
	return true;
}

void systickInit(uint32_t period) {
	SysTickPeriodSet(period);
	SysTickIntEnable();
	SysTickIntRegister(&SysTick_Handler);
	SysTickEnable();
}

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
	HWREG(GPIOF_BASE + GPIO_O_CR) = 0x02;
	HWREG(GPIOF_BASE + GPIO_O_CR) |= 0x04;
	HWREG(GPIOF_BASE + GPIO_O_CR) |= 0x08;
	
	initTaskMaster();
	blinkyTaskID = addTask(100, &blinkyTaskInit, &blinkyTaskCallback);
	initTask(blinkyTaskID);
	
	while(1);
}
