#include "LMCterminal.h"
#include "uart.h"
#include "TM4C123GH6PM.h"

//*****************************************************************************
//
//! MicroLIB's stdio library calls this function.  Implementing it allows
//! MicroLIB's getchar(), scanf(), etc to work.
//!
//! \param stream is unused
//!
//! \return the character read from USB UART
//
// *****************************************************************************
int fgetc(FILE *stream) {
	return UARTCharGet(UART0_BASE);
}

//*****************************************************************************
//
//! MicroLIB's stdio library calls this function.  Implementing it allows
//! MicroLIB's putchar(), puts(), printf(), etc to work.
//!
//! \param c is the character to write to USB UART
//!
//! \param stream is unused
//!
//! \return the character written to USB UART
//
// *****************************************************************************
int fputc(int c, FILE *stream) {
	UARTCharPut(UART0_BASE, c);
	
	return c;
}
