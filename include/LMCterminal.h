#include <stdint.h>

#define _PRINTF_CR_LF

//*****************************************************************************
//
//! Initialize the LMC Terminal. Initialize UART0, set up interrupts, and
//! and register the terminal's interrupt callback. Initializes the internal
//! circularBuffer_t used to store data form the UART FIFOs. 
//!
//! \param arg is interpreted as an int internally, and used to set the UART
//! baud rate
//!
//! \return 0, always
//
//*****************************************************************************
int initLMCterminal(void *arg);

int getchar(void);

int putchar(int c);

int kprintf(const char *format, ...);
