#include <stdint.h>

#define _CR_IN_  1
#define _LF_IN_  0
#define _CR_OUT_ 1
#define _LF_OUT_ 1

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

int putcharNonblock(int c);

int kprintf(const char *format, ...);
