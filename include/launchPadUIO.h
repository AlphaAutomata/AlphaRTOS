#ifndef _LAUNCHPAD_UIO_H_
#define _LAUNCHPAD_UIO_H_

#include <stdbool.h>
#include <stdint.h>

#include "TM4C123GH6PM.h"

#include "gpio.h"

enum ledColor {
	off,
	red,
	green,
	blue,
	yellow,
	magenta,
	cyan,
	white
};

//*****************************************************************************
//
//! Initialize the GPIO pins for the RGB LED and the two push buttons. Port F
//! must already be initialized and unlocked.
//!
//! \param none
//!
//! \return none
//
//*****************************************************************************
void initUIO(void);

//*****************************************************************************
//
//! Set the RGB LED to a specific color
//!
//! \param color is an enumerated variable specifying the LED color
//!
//! \return none
//
//*****************************************************************************
void setLED(enum ledColor color);

//*****************************************************************************
//
//! Check to see if the left and right buttons are pressed. No debouncing done.
//!
//! \param lPressed is a pointer to a boolean to store whether the left button
//! is pressed
//!
//! \param rPressed is a pointe rto a boolean to store whether the right button
//! is pressed
//!
//! \return none
//
//*****************************************************************************
void buttonsPressed(volatile bool *lPressed, volatile bool *rPressed);

#endif
