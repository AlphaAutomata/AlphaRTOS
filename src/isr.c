#include "inc/hw_memmap.h"
#include "isr.h"
#include "uart.h"

volatile uint8_t uartIntMask;

void uart0ISR(void) {
	uartIntMask |= 0x01;
	
	UARTIntClear(UART0_BASE, UART_INT_TX | UART_INT_RX);
	
	return;
}

void uart1ISR(void) {
	uartIntMask |= 0x02;
	
	UARTIntClear(UART1_BASE, UART_INT_TX | UART_INT_RX);
	
	return;
}

void uart2ISR(void) {
	uartIntMask |= 0x04;
	
	UARTIntClear(UART2_BASE, UART_INT_TX | UART_INT_RX);
	
	return;
}

void uart3ISR(void) {
	uartIntMask |= 0x08;
	
	UARTIntClear(UART3_BASE, UART_INT_TX | UART_INT_RX);
	
	return;
}

void uart4ISR(void) {
	uartIntMask |= 0x10;
	
	UARTIntClear(UART4_BASE, UART_INT_TX | UART_INT_RX);
	
	return;
}

void uart5ISR(void) {
	uartIntMask |= 0x20;
	
	UARTIntClear(UART5_BASE, UART_INT_TX | UART_INT_RX);
	
	return;
}

void uart6ISR(void) {
	uartIntMask |= 0x40;
	
	UARTIntClear(UART6_BASE, UART_INT_TX | UART_INT_RX);
	
	return;
}

void uart7ISR(void) {
	uartIntMask |= 0x80;
	
	UARTIntClear(UART7_BASE, UART_INT_TX | UART_INT_RX);
	
	return;
}
