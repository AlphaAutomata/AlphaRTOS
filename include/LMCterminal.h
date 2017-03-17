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

//*****************************************************************************
//
//! Get a single ASCII character from USB UART
//!
//! \param none
//!
//! \return the character read from USB UART
//
//*****************************************************************************
int getchar(void);

//*****************************************************************************
//
//! Get a single ASCII character from USB UART. Does not block.
//!
//! \param none
//!
//! \return the character read from USB UART if there is a char. -1 otherwise.
//
//*****************************************************************************
int getchar_nonblock(void);

//*****************************************************************************
//
//! Write a single character to USB UART
//!
//! \param c is the ASCII character to write to USB UART
//!
//! \return the character written to USB UART
//
//*****************************************************************************
int putchar(int c);

//*****************************************************************************
//
//! Write a string literal to USB UART. Converts LF to CRLF.
//!
//! \param strlit is the C-style string literal to write
//!
//! \return the number of characters written to USB UART
//
//*****************************************************************************
int printlit(const char *strlit);

//*****************************************************************************
//
//! Write a string literal to USB UART. Converts LF to CRLF.
//!
//! \param format is the stdio printf-style string literal to write. Currently
//! only supports %c, %d, %i, and %l formatting characters and no formatting
//! parameters. 
//!
//! \return the number of characters written to USB UART
//
//*****************************************************************************
int kprintf(const char *format, ...);

#endif
