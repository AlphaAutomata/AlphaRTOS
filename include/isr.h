#ifndef _ISR_H_
#define _ISR_H_

#include <stdbool.h>
#include <stdint.h>

#include "circular_buffer.h"

// This bit mask identifies which UART module triggered the interrupt.
// Least significant bit is set by UART0, most significant bit is set by UART7.
extern volatile uint32_t uartIntMask;
// Hardware masked interrupt statuses from the last interrupt trigger
extern volatile uint32_t uartHWIntMask[8];
// Hardware error mask from last interrupt
extern volatile uint32_t uartHWErrMask[8];

#define BUFF_SIZE 32

extern circularBuffer_t uart_txBuffs[8];
extern uint8_t uart_txData[8][BUFF_SIZE];
extern circularBuffer_t uart_rxBuffs[8];
extern uint8_t uart_rxData[8][BUFF_SIZE];

void uartFlow(int controller, uint32_t base);

void uart0ISR(void);
void uart1ISR(void);
void uart2ISR(void);
void uart3ISR(void);
void uart4ISR(void);
void uart5ISR(void);
void uart6ISR(void);
void uart7ISR(void);

#endif
