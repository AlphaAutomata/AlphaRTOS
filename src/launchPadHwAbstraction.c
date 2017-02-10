#include "launchPadHwAbstraction.h"

bool initUART(eUartController controller, uartInfo info) {
	uint32_t uartBase;
	uint32_t configMask;
	
	switch (controller) {
		case uart0 :
			uartBase = UART0_BASE;
			break;
		case uart1 :
			uartBase = UART0_BASE;
			break;
		case uart2 :
			uartBase = UART0_BASE;
			break;
		case uart3 :
			uartBase = UART0_BASE;
			break;
		case uart4 :
			uartBase = UART0_BASE;
			break;
		case uart5 :
			uartBase = UART0_BASE;
			break;
		case uart6 :
			uartBase = UART0_BASE;
			break;
		case uart7 :
			uartBase = UART0_BASE;
			break;
	}
	
	switch (info.wlen) {
		case 5 :
			configMask = UART_CONFIG_WLEN_5;
			break;
		case 6 :
			configMask = UART_CONFIG_WLEN_6;
			break;
		case 7 :
			configMask = UART_CONFIG_WLEN_7;
			break;
		case 8 :
			configMask = UART_CONFIG_WLEN_8;
			break;
		default :
			return false;
	}
	if (info.parity) {
		configMask |= UART_CONFIG_PAR_ONE;
	}
	if (info.twoStopBits) {
		configMask |= UART_CONFIG_STOP_TWO;
	}
	
	GPIOPinTypeUART(GPIOA_BASE, GPIO_PIN_0 | GPIO_PIN_1);
	GPIOPinConfigure(GPIO_PA0_U0RX);
	GPIOPinConfigure(GPIO_PA1_U0TX);
	
	SysCtlPeripheralEnable(SYSCTL_PERIPH_UART0);
	while(!SysCtlPeripheralReady(SYSCTL_PERIPH_UART0));
	
	UARTConfigSetExpClk(uartBase, SysCtlClockGet(), info.baud, configMask);
	
	return true;
}
