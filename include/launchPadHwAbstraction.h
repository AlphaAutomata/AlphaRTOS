#ifndef _LAUNCHPAD_HW_ABSTRACTION_H_
#define _LAUNCHPAD_HW_ABSTRACTION_H_

#include <stdbool.h>
#include <stdint.h>

///////////////////////////////////////////////////////////////////////////////
///////////////// General Purpose Input/Output Ports and Pins /////////////////
///////////////////////////////////////////////////////////////////////////////

enum launchPadGPIOPort {
	portA,
	portB,
	portC,
	portD,
	portE,
	portF
};

enum launchPadPin {
	PA0, PA1, PA2, PA3, PA4, PA5, PA6, PA7,
	PB0, PB1, PB2, PB3, PB4, PB5, PB6, PB7,
	PC0, PC1, PC2, PC3, PC4, PC5, PC6, PC7,
	PD0, PD1, PD2, PD3, PD4, PD5, PD6, PD7,
	PE0, PE1, PE2, PE3, PE4, PE5, PE6, PE7,
	PF0, PF1, PF2, PF3, PF4, PF5, PF6, PF7
};

void initPort(enum launchPadGPIOPort port);
void resetPort(enum launchPadGPIOPort port);


///////////////////////////////////////////////////////////////////////////////
///////////////// Universal Asynchronous Receiver/Transmitter /////////////////
///////////////////////////////////////////////////////////////////////////////

enum uartController {
	uart0,
	uart1,
	uart2,
	uart3,
	uart4,
	uart5,
	uart6,
	uart7
};

struct uartInfo {
	unsigned int baud;
	uint8_t wlen;
	bool parity;
	bool twoStopBits;
};

bool initUART(enum uartController controller, struct uartInfo info);
bool disableUART(enum uartController controller);


///////////////////////////////////////////////////////////////////////////////
//////////////////////////// Pulse-Width Modulator ////////////////////////////
///////////////////////////////////////////////////////////////////////////////

enum pwmController {
	pwm0,
	pwm1
};

bool initPWM(enum pwmController controller);
bool disablePWM(enum pwmController controller);
bool setPWM(enum pwmController, unsigned int duty);

#endif
