#define PART_TM4C123GH6PM

#include "TM4C123.h"
#include "pin_map.h"
#include "stdbool.h"
#include "sysctl.h"
#include "uart.h"
#include "gpio.h"

int main(void) {
	char usb_char;
	
	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOA);
	while(!SysCtlPeripheralReady(SYSCTL_PERIPH_GPIOA));
	
	GPIOPinTypeUART(GPIOA_BASE, GPIO_PIN_0 | GPIO_PIN_1);
	GPIOPinConfigure(GPIO_PA0_U0RX);
	GPIOPinConfigure(GPIO_PA1_U0TX);
	
	SysCtlPeripheralEnable(SYSCTL_PERIPH_UART0);
	while(!SysCtlPeripheralReady(SYSCTL_PERIPH_UART0));
	
	UARTConfigSetExpClk(
		UART0_BASE,
		SysCtlClockGet(),
		115200,
		UART_CONFIG_WLEN_8 | UART_CONFIG_PAR_NONE | UART_CONFIG_STOP_ONE
	);
	
	while (1) {
		usb_char = UARTCharGet(UART0_BASE);
		UARTCharPut(UART0_BASE, usb_char);
	}
}
