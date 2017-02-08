#include "LMCterminal.h"

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
int fgetc(FILE *stream)
{
	char c;
	
	// TODO: read a character from the USB UART controller and return it
	
	return c;
}

//*****************************************************************************
//
//! MicroLIB's stdio library calls this function.  Implementing it allows
//! MicroLIB's MicroLIB's putchar(), puts(), printf(), etc to work.
//!
//! \param c is the character to write to USB UART
//!
//! \param stream is unused
//!
//! \return the character written to USB UART
//
// *****************************************************************************
int fputc(int c, FILE *stream)
{
	// TODO: write a character to the USB UART controller
	
	return c;
}
