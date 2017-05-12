#ifndef _LAUNCHPAD_HW_ABSTRACTION_H_
#define _LAUNCHPAD_HW_ABSTRACTION_H_

#include <stdbool.h>
#include <stdint.h>

#ifndef PART_TM4C123GH6PM
#define PART_TM4C123GH6PM
#endif

#include "TM4C123GH6PM.h"
#include "pwm.h"
#include "sysctl.h"
#include "gpio.h"
#include "pin_map.h"
#include "uart.h"
#include "qei.h"

///////////////////////////////////////////////////////////////////////////////
///////////////// General Purpose Input/Output Ports and Pins /////////////////
///////////////////////////////////////////////////////////////////////////////

typedef enum {
	portA,
	portB,
	portC,
	portD,
	portE,
	portF
} eLaunchPadGPIOPort;

typedef enum {
	PA0, PA1, PA2, PA3, PA4, PA5, PA6, PA7,
	PB0, PB1, PB2, PB3, PB4, PB5, PB6, PB7,
	PC0, PC1, PC2, PC3, PC4, PC5, PC6, PC7,
	PD0, PD1, PD2, PD3, PD4, PD5, PD6, PD7,
	PE0, PE1, PE2, PE3, PE4, PE5, PE6, PE7,
	PF0, PF1, PF2, PF3, PF4, PF5, PF6, PF7
} eLaunchPadPin;

void initPort(eLaunchPadGPIOPort port);
void resetPort(eLaunchPadGPIOPort port);


///////////////////////////////////////////////////////////////////////////////
///////////////// Universal Asynchronous Receiver/Transmitter /////////////////
///////////////////////////////////////////////////////////////////////////////

// Explicitly defined in order to facilitate array access to UART data buffers
typedef enum {
	uart0 = 0,
	uart1 = 1,
	uart2 = 2,
	uart3 = 3,
	uart4 = 4,
	uart5 = 5,
	uart6 = 6,
	uart7 = 7
} eUartController;

typedef struct {
	uint32_t baud;
	uint8_t wlen;
	bool parity;
	bool twoStopBits;
} uartInfo;

int uartManager(uint32_t arg);

bool initUART(eUartController controller, uartInfo *info);
bool disableUART(eUartController controller);

int uart_getchar(eUartController controller);
int uart_getchar_nonblock(eUartController controller);
int uart_putchar(eUartController controller, int c);
int uart_putchar_nonblock(eUartController controller, int c);

///////////////////////////////////////////////////////////////////////////////
//////////////////////////// Pulse-Width Modulator ////////////////////////////
///////////////////////////////////////////////////////////////////////////////

#define ONE_MS_PULSE_WIDTH 781

typedef enum {
	pwm0,
	pwm1
} ePwmController;

typedef enum {
	pwm_gen0,
	pwm_gen1,
	pwm_gen2,
	pwm_gen3
} ePwmGenerator;

bool initPWM(ePwmController controller, ePwmGenerator generator);
bool disablePWM(ePwmController controller);
bool setPWM(ePwmController controller, ePwmGenerator generator, unsigned int duty);

///////////////////////////////////////////////////////////////////////////////
////////////////////////////// Quadrature Encoder /////////////////////////////
///////////////////////////////////////////////////////////////////////////////

typedef enum {
	qei0,
	qei1
} eQuadrature;

bool initQEI(eQuadrature encoder);
uint32_t getQEISpeed(eQuadrature encoder);
int32_t getQEIDirection(eQuadrature encoder);
int32_t getQEIPosition(eQuadrature encoder);

///////////////////////////////////////////////////////////////////////////////
/////////////////////////////// Interrupt Alert ///////////////////////////////
///////////////////////////////////////////////////////////////////////////////

// explicitly define the values, since these get passed through context switches
typedef enum {
	gpTimer    = 0x00000000,
	UART       = 0x00000001,
	Quadrature = 0x00000002
} eInterrupt;

#endif
