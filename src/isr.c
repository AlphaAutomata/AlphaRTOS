#include "inc/hw_memmap.h"
#include "isr.h"
#include "uart.h"

volatile uint32_t uartIntMask;

void uart0ISR(void) {
	uartIntMask |= 0x00000001;
	
	UARTIntClear(UART0_BASE, UART_INT_TX | UART_INT_RX);
	
	return;
}

void uart1ISR(void) {
	uartIntMask |= 0x00000002;
	
	UARTIntClear(UART1_BASE, UART_INT_TX | UART_INT_RX);
	
	return;
}

void uart2ISR(void) {
	uartIntMask |= 0x00000004;
	
	UARTIntClear(UART2_BASE, UART_INT_TX | UART_INT_RX);
	
	return;
}

void uart3ISR(void) {
	uartIntMask |= 0x00000008;
	
	UARTIntClear(UART3_BASE, UART_INT_TX | UART_INT_RX);
	
	return;
}

void uart4ISR(void) {
	uartIntMask |= 0x00000010;
	
	UARTIntClear(UART4_BASE, UART_INT_TX | UART_INT_RX);
	
	return;
}

void uart5ISR(void) {
	uartIntMask |= 0x00000020;
	
	UARTIntClear(UART5_BASE, UART_INT_TX | UART_INT_RX);
	
	return;
}

void uart6ISR(void) {
	uartIntMask |= 0x00000040;
	
	UARTIntClear(UART6_BASE, UART_INT_TX | UART_INT_RX);
	
	return;
}

void uart7ISR(void) {
	uartIntMask |= 0x00000080;
	
	UARTIntClear(UART7_BASE, UART_INT_TX | UART_INT_RX);
	
	return;
}
