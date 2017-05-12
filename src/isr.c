#include "inc/hw_memmap.h"
#include "isr.h"
#include "uart.h"
#include "qei.h"

////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////// UART ///////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////

volatile uint32_t uartIntMask;
volatile uint32_t uartHWIntMask[8];
volatile uint32_t uartHWErrMask[8];

circularBuffer_t uart_txBuffs[8];
uint8_t uart_txData[8][BUFF_SIZE];
circularBuffer_t uart_rxBuffs[8];
uint8_t uart_rxData[8][BUFF_SIZE];

//*****************************************************************************
//
//! As long as the UART FIFO has room and there are characters available in the
//! transmit buffer to write, transfer characters from the buffer to the UART
//! hardware FIFO. As long as there are characters in the receive hardware
//! FIFO, transfer them to the receive memory buffer. If there is no room in
//! the receive memory buffer, the character is discarded. 
//!
//! @param controller specifies which UART controller to service
//!
//! @param base is the base address of the UART controller
//
//*****************************************************************************
void uartFlow(int controller, uint32_t base) {
	char uartChar;
	
	while (UARTSpaceAvail(base)) {
		if (circularBufferRemoveItem(&(uart_txBuffs[controller]), &uartChar)) {
			UARTCharPut(base, uartChar);
		} else {
			break;
		}
	}
	
	while (UARTCharsAvail(base)) {
		uartChar = UARTCharGet(base);
		circularBufferAddItem(&(uart_rxBuffs[controller]), &uartChar);
	}
}

void uart0ISR(void) {
	uartIntMask |= 0x00000001;
	
	uartHWIntMask[0] = UARTIntStatus(UART0_BASE, true);
	uartHWErrMask[0] = UARTRxErrorGet(UART0_BASE);
	
	uartFlow(0, UART0_BASE);
	
	UARTIntClear(UART0_BASE, uartHWIntMask[0]);
	if (uartHWErrMask[0]) UARTRxErrorClear(UART0_BASE);
	
	return;
}

void uart1ISR(void) {
	uartIntMask |= 0x00000002;
	
	uartHWIntMask[1] = UARTIntStatus(UART1_BASE, true);
	uartHWErrMask[1] = UARTRxErrorGet(UART1_BASE);
	
	uartFlow(1, UART1_BASE);
	
	UARTIntClear(UART1_BASE, UART_INT_TX | UART_INT_RX);
	if (uartHWErrMask[1]) UARTRxErrorClear(UART1_BASE);
	
	return;
}

void uart2ISR(void) {
	uartIntMask |= 0x00000004;
	
	uartHWIntMask[2] = UARTIntStatus(UART2_BASE, true);
	uartHWErrMask[2] = UARTRxErrorGet(UART2_BASE);
	
	uartFlow(2, UART2_BASE);
	
	UARTIntClear(UART2_BASE, UART_INT_TX | UART_INT_RX);
	if (uartHWErrMask[2]) UARTRxErrorClear(UART2_BASE);
	
	return;
}

void uart3ISR(void) {
	uartIntMask |= 0x00000008;
	
	uartHWIntMask[3] = UARTIntStatus(UART3_BASE, true);
	uartHWErrMask[3] = UARTRxErrorGet(UART3_BASE);
	
	uartFlow(3, UART3_BASE);
	
	UARTIntClear(UART3_BASE, UART_INT_TX | UART_INT_RX);
	if (uartHWErrMask[3]) UARTRxErrorClear(UART3_BASE);
	
	return;
}

void uart4ISR(void) {
	uartIntMask |= 0x00000010;
	
	uartHWIntMask[4] = UARTIntStatus(UART4_BASE, true);
	uartHWErrMask[4] = UARTRxErrorGet(UART4_BASE);
	
	uartFlow(4, UART4_BASE);
	
	UARTIntClear(UART4_BASE, UART_INT_TX | UART_INT_RX);
	if (uartHWErrMask[4]) UARTRxErrorClear(UART4_BASE);
	
	return;
}

void uart5ISR(void) {
	uartIntMask |= 0x00000020;
	
	uartHWIntMask[5] = UARTIntStatus(UART5_BASE, true);
	uartHWErrMask[5] = UARTRxErrorGet(UART5_BASE);
	
	uartFlow(5, UART5_BASE);
	
	UARTIntClear(UART5_BASE, UART_INT_TX | UART_INT_RX);
	if (uartHWErrMask[5]) UARTRxErrorClear(UART5_BASE);
	
	return;
}

void uart6ISR(void) {
	uartIntMask |= 0x00000040;
	
	uartHWIntMask[6] = UARTIntStatus(UART6_BASE, true);
	uartHWErrMask[6] = UARTRxErrorGet(UART6_BASE);
	
	uartFlow(6, UART6_BASE);
	
	UARTIntClear(UART6_BASE, UART_INT_TX | UART_INT_RX);
	if (uartHWErrMask[6]) UARTRxErrorClear(UART6_BASE);
	
	return;
}

void uart7ISR(void) {
	uartIntMask |= 0x00000080;
	
	uartHWIntMask[7] = UARTIntStatus(UART7_BASE, true);
	uartHWErrMask[7] = UARTRxErrorGet(UART7_BASE);
	
	uartFlow(7, UART7_BASE);
	
	UARTIntClear(UART7_BASE, UART_INT_TX | UART_INT_RX);
	if (uartHWErrMask[7]) UARTRxErrorClear(UART7_BASE);
	
	return;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////// Quadrature ////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////

volatile uint32_t qeiIntMask;

void qei0ISR(void) {
	QEIIntClear(QEI0_BASE, QEI_INTTIMER);
	qeiIntMask |= 0x00000001;
}

void qei1ISR(void) {
	QEIIntClear(QEI1_BASE, QEI_INTTIMER);
	qeiIntMask |= 0x00000002;
}
