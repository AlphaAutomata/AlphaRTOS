#ifndef _LMC_TERMINAL_H_
#define _LMC_TERMINAL_H_

#include <stdint.h>

//*****************************************************************************
//
//! Initialize the LMC Terminal. Initialize UART0, set up interrupts, and
//! and register the terminal's interrupt callback. Initializes the internal
//! circularBuffer_t used to store data form the UART FIFOs. 
//!
//! \param arg is used to set the UART baud rate. This is currently unsupported
//!
//! \return 0, always
//!
//! \note sets UART mode to 8N1. baud currently defaults to 115200
//
//*****************************************************************************
int initLMCterminal(uint32_t arg);

int getchar(void);

int putchar(int c);

int putcharNonblock(int c);

int printlit(const char *strlit);

int kprintf(const char *format, ...);

#endif
