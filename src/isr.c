#include "inc/hw_memmap.h"
#include "isr.h"
#include "uart.h"

volatile uint32_t uartIntMask;
volatile uint32_t uartHWIntMask[8];
volatile uint32_t uartHWErrMask[8];

void uart0ISR(void) {
	uartIntMask |= 0x00000001;
	
	uartHWIntMask[0] = UARTIntStatus(UART0_BASE, true);
	uartHWErrMask[0] = UARTRxErrorGet(UART0_BASE);
	
	UARTIntClear(UART0_BASE, uartHWIntMask[0]);
	if (uartHWErrMask[0]) UARTRxErrorClear(UART0_BASE);
	
	return;
}

void uart1ISR(void) {
	uartIntMask |= 0x00000002;
	
	uartHWIntMask[1] = UARTIntStatus(UART1_BASE, true);
	uartHWErrMask[1] = UARTRxErrorGet(UART1_BASE);
	
	UARTIntClear(UART1_BASE, UART_INT_TX | UART_INT_RX);
	if (uartHWErrMask[1]) UARTRxErrorClear(UART1_BASE);
	
	return;
}

void uart2ISR(void) {
	uartIntMask |= 0x00000004;
	
	uartHWIntMask[2] = UARTIntStatus(UART2_BASE, true);
	uartHWErrMask[2] = UARTRxErrorGet(UART2_BASE);
	
	UARTIntClear(UART2_BASE, UART_INT_TX | UART_INT_RX);
	if (uartHWErrMask[2]) UARTRxErrorClear(UART2_BASE);
	
	return;
}

void uart3ISR(void) {
	uartIntMask |= 0x00000008;
	
	uartHWIntMask[3] = UARTIntStatus(UART3_BASE, true);
	uartHWErrMask[3] = UARTRxErrorGet(UART3_BASE);
	
	UARTIntClear(UART3_BASE, UART_INT_TX | UART_INT_RX);
	if (uartHWErrMask[3]) UARTRxErrorClear(UART3_BASE);
	
	return;
}

void uart4ISR(void) {
	uartIntMask |= 0x00000010;
	
	uartHWIntMask[4] = UARTIntStatus(UART4_BASE, true);
	uartHWErrMask[4] = UARTRxErrorGet(UART4_BASE);
	
	UARTIntClear(UART4_BASE, UART_INT_TX | UART_INT_RX);
	if (uartHWErrMask[4]) UARTRxErrorClear(UART4_BASE);
	
	return;
}

void uart5ISR(void) {
	uartIntMask |= 0x00000020;
	
	uartHWIntMask[5] = UARTIntStatus(UART5_BASE, true);
	uartHWErrMask[5] = UARTRxErrorGet(UART5_BASE);
	
	UARTIntClear(UART5_BASE, UART_INT_TX | UART_INT_RX);
	if (uartHWErrMask[5]) UARTRxErrorClear(UART5_BASE);
	
	return;
}

void uart6ISR(void) {
	uartIntMask |= 0x00000040;
	
	uartHWIntMask[6] = UARTIntStatus(UART6_BASE, true);
	uartHWErrMask[6] = UARTRxErrorGet(UART6_BASE);
	
	UARTIntClear(UART6_BASE, UART_INT_TX | UART_INT_RX);
	if (uartHWErrMask[6]) UARTRxErrorClear(UART6_BASE);
	
	return;
}

void uart7ISR(void) {
	uartIntMask |= 0x00000080;
	
	uartHWIntMask[7] = UARTIntStatus(UART7_BASE, true);
	uartHWErrMask[7] = UARTRxErrorGet(UART7_BASE);
	
	UARTIntClear(UART7_BASE, UART_INT_TX | UART_INT_RX);
	if (uartHWErrMask[7]) UARTRxErrorClear(UART7_BASE);
	
	return;
}
