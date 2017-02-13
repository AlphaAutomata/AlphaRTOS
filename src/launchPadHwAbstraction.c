#include "launchPadHwAbstraction.h"
#include "isr.h"

bool initUART(eUartController controller, uartInfo info) {
	uint32_t sysctlMask;
	uint32_t gpioBase;
	uint32_t uartBase;
	uint32_t uartConfigMask;
	uint32_t rxPinConfigMask;
	uint32_t txPinConfigMask;
	uint32_t typePinMask;
	void (*uartISR)(void);
	
	// set bit masks to pass into lower level functions
	switch (controller) {
		case uart0 :
			sysctlMask = SYSCTL_PERIPH_UART0;
			gpioBase = GPIO_PORTA_BASE;
			uartBase = UART0_BASE;
			rxPinConfigMask = GPIO_PA0_U0RX;
			txPinConfigMask = GPIO_PA1_U0TX;
			typePinMask = GPIO_PIN_0 | GPIO_PIN_1;
			uartISR = uart0ISR;
			break;
		case uart1 :
			sysctlMask = SYSCTL_PERIPH_UART1;
			gpioBase = GPIO_PORTB_BASE;
			uartBase = UART1_BASE;
			rxPinConfigMask = GPIO_PB0_U1RX;
			txPinConfigMask = GPIO_PB1_U1TX;
			typePinMask = GPIO_PIN_0 | GPIO_PIN_1;
			uartISR = uart1ISR;
			break;
		case uart2 :
			sysctlMask = SYSCTL_PERIPH_UART2;
			gpioBase = GPIO_PORTD_BASE;
			uartBase = UART2_BASE;
			rxPinConfigMask = GPIO_PD6_U2RX;
			txPinConfigMask = GPIO_PD7_U2TX;
			typePinMask = GPIO_PIN_6 | GPIO_PIN_7;
			uartISR = uart2ISR;
			break;
		case uart3 :
			sysctlMask = SYSCTL_PERIPH_UART3;
			gpioBase = GPIO_PORTC_BASE;
			uartBase = UART3_BASE;
			rxPinConfigMask = GPIO_PC6_U3RX;
			txPinConfigMask = GPIO_PC7_U3TX;
			typePinMask = GPIO_PIN_6 | GPIO_PIN_7;
			uartISR = uart3ISR;
			break;
		case uart4 :
			sysctlMask = SYSCTL_PERIPH_UART4;
			gpioBase = GPIO_PORTC_BASE;
			uartBase = UART4_BASE;
			rxPinConfigMask = GPIO_PC4_U4RX;
			txPinConfigMask = GPIO_PC5_U4TX;
			typePinMask = GPIO_PIN_4 | GPIO_PIN_5;
			uartISR = uart4ISR;
			break;
		case uart5 :
			sysctlMask = SYSCTL_PERIPH_UART5;
			gpioBase = GPIO_PORTE_BASE;
			uartBase = UART5_BASE;
			rxPinConfigMask = GPIO_PE4_U5RX;
			txPinConfigMask = GPIO_PE5_U5TX;
			typePinMask = GPIO_PIN_4 | GPIO_PIN_5;
			uartISR = uart5ISR;
			break;
		case uart6 :
			sysctlMask = SYSCTL_PERIPH_UART6;
			gpioBase = GPIO_PORTD_BASE;
			uartBase = UART6_BASE;
			rxPinConfigMask = GPIO_PD4_U6RX;
			txPinConfigMask = GPIO_PD5_U6TX;
			typePinMask = GPIO_PIN_4 | GPIO_PIN_5;
			uartISR = uart6ISR;
			break;
		case uart7 :
			sysctlMask = SYSCTL_PERIPH_UART7;
			gpioBase = GPIO_PORTE_BASE;
			uartBase = UART7_BASE;
			rxPinConfigMask = GPIO_PE0_U7RX;
			txPinConfigMask = GPIO_PE1_U7TX;
			typePinMask = GPIO_PIN_0 | GPIO_PIN_1;
			uartISR = uart7ISR;
			break;
		default :
			return false;
	}
	// set write length, parity, and stop bits based on the info passed in
	switch (info.wlen) {
		case 5 :
			uartConfigMask = UART_CONFIG_WLEN_5;
			break;
		case 6 :
			uartConfigMask = UART_CONFIG_WLEN_6;
			break;
		case 7 :
			uartConfigMask = UART_CONFIG_WLEN_7;
			break;
		case 8 :
			uartConfigMask = UART_CONFIG_WLEN_8;
			break;
		default :
			return false;
	}
	if (info.parity) {
		uartConfigMask |= UART_CONFIG_PAR_ONE;
	}
	if (info.twoStopBits) {
		uartConfigMask |= UART_CONFIG_STOP_TWO;
	}
	
	// Set the GPIO pins to UART hardware control
	GPIOPinTypeUART(gpioBase, typePinMask);
	GPIOPinConfigure(rxPinConfigMask);
	GPIOPinConfigure(txPinConfigMask);
	
	// turn on the clock gate to the UART peripheral
	SysCtlPeripheralEnable(sysctlMask);
	while(!SysCtlPeripheralReady(sysctlMask));
	
	// enable the UART transmit and receive FIFOs
	UARTFIFOEnable(uartBase);
	// register the appropririate UART interrupt service routine
	UARTIntRegister(uartBase, uartISR);
	// Set the trasmit FIFO to trigger on FIFO level
	UARTTxIntModeSet(uartBase, UART_TXINT_MODE_FIFO);
	// set both FIFOs levels. Tx interrupts when almost empty, Rx interrupts when almost full.
	UARTFIFOLevelSet(uartBase, UART_FIFO_TX1_8, UART_FIFO_RX7_8);
	// enable interrupts for receive and transmit
	UARTIntEnable(uartBase, UART_INT_RX | UART_INT_TX);
	// configure and initialize the UART peripheral
	UARTConfigSetExpClk(uartBase, SysCtlClockGet(), info.baud, uartConfigMask);
	
	return true;
}
