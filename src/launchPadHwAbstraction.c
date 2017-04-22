#include "inc/hw_types.h"
#include "inc/hw_gpio.h"

#include "launchPadHwAbstraction.h"
#include "isr.h"
#include "pwm.h"
#include "qei.h"
#include "badgerRMCRTOS.h"

#include "launchPadUIO.h"
#include "circular_buffer.h"

#define BUFF_SIZE 16

circularBuffer_t uart_txBuffs[8];
uint8_t uart_txData[8][BUFF_SIZE];
circularBuffer_t uart_rxBuffs[8];
uint8_t uart_rxData[8][BUFF_SIZE];

bool initPWM(ePwmController controller, ePwmGenerator generator){
	uint32_t Base;
	uint32_t Gen;	
	uint32_t rxPinConfigMask;
	uint32_t port;
	uint8_t pin;
	uint32_t SysCtlBase;
	
	switch (controller) {
		case pwm0 :
			Base = PWM0_BASE;
			SysCtlBase = SYSCTL_PERIPH_PWM0;
			switch (generator) {
				case pwm_gen0:
					Gen = PWM_GEN_0;
					rxPinConfigMask = GPIO_PB6_M0PWM0;
					pin = GPIO_PIN_6;
					break;
				case pwm_gen1:
					Gen = PWM_GEN_1;
					rxPinConfigMask = GPIO_PB7_M0PWM1;
					pin = GPIO_PIN_7;
					break;
				case pwm_gen2:
					Gen = PWM_GEN_2;
					rxPinConfigMask = GPIO_PB4_M0PWM2;
					pin = GPIO_PIN_4;
					break;
				case pwm_gen3:
					Gen = PWM_GEN_3;
					rxPinConfigMask = GPIO_PB5_M0PWM3;
					pin = GPIO_PIN_5;
					break;
			}
			port = GPIOB_BASE;
			break;
	
		case pwm1 :
			Base = PWM1_BASE;
			SysCtlBase = SYSCTL_PERIPH_PWM1;
			switch (generator) {
				case pwm_gen0:
					Gen = PWM_GEN_0;
					rxPinConfigMask = GPIO_PD0_M1PWM0;
					port = GPIOD_BASE;
					pin = GPIO_PIN_0;
					break;
				case pwm_gen1:
					Gen = PWM_GEN_1;
					rxPinConfigMask = GPIO_PD1_M1PWM1;
					port = GPIOD_BASE;
					pin = GPIO_PIN_1;
					break;
				case pwm_gen2:
					Gen = PWM_GEN_2;
					rxPinConfigMask = GPIO_PE4_M1PWM2;
					port = GPIOE_BASE;
					pin = GPIO_PIN_4;
					break;
				case pwm_gen3:
					Gen = PWM_GEN_3;
					rxPinConfigMask = GPIO_PE5_M1PWM3;
					port = GPIOE_BASE;
					pin = GPIO_PIN_5;
					break;
			}
			break;
		}
	
	// GPIO Port E needs to be unlocked before setting pin configurations
	HWREG(port + GPIO_O_LOCK) = GPIO_LOCK_KEY;
	HWREG(port + GPIO_O_CR) = pin;
	
	GPIOPinTypePWM(port, pin);
	GPIOPinConfigure(rxPinConfigMask);
	SysCtlPeripheralEnable(SysCtlBase);
	while(!SysCtlPeripheralReady(SysCtlBase));
	PWMGenEnable(Base, Gen);
	PWMGenConfigure(Base, Gen, PWM_GEN_MODE_DOWN);
	PWMGenPeriodSet(Base, Gen, 3*ONE_MS_PULSE_WIDTH);
	PWMPulseWidthSet(Base, Gen, 3*ONE_MS_PULSE_WIDTH/2);
	PWMOutputState(Base, (PWM_OUT_0_BIT | PWM_OUT_1_BIT), true);
	return true;
}

bool initQEI(eQuadrature encoder) {
	uint32_t QEIPinAConfigMask, QEIPinBConfigMask;
	uint32_t QEIbase;
	uint32_t GPIObase;
	uint32_t SysCtlMask;
	uint32_t typePinMask;
	
	switch (encoder) {
		case qei0:
			QEIbase = QEI0_BASE;
			GPIObase = GPIOD_BASE;
			SysCtlMask = SYSCTL_PERIPH_QEI0;
			QEIPinAConfigMask = GPIO_PD6_PHA0;
			QEIPinBConfigMask = GPIO_PD7_PHB0;
			typePinMask = GPIO_PIN_6 | GPIO_PIN_7;
			break;
	
		case qei1:
			QEIbase = QEI1_BASE;
			GPIObase = GPIOC_BASE;
			SysCtlMask = SYSCTL_PERIPH_QEI1;
			QEIPinAConfigMask = GPIO_PC5_PHA1;
			QEIPinBConfigMask = GPIO_PC6_PHB1;
			typePinMask = GPIO_PIN_5 | GPIO_PIN_6;
			break;
		
		default :
			return false;
	}
	
	// GPIO Port C needs to be unlocked before setting pin configurations
	HWREG(GPIObase + GPIO_O_LOCK) = GPIO_LOCK_KEY;
	HWREG(GPIObase + GPIO_O_CR) = typePinMask;
	
	GPIOPinConfigure(QEIPinAConfigMask);
	GPIOPinConfigure(QEIPinBConfigMask);
	GPIOPinTypeQEI(GPIObase, typePinMask);
	
	SysCtlPeripheralEnable(SysCtlMask);
	while(!SysCtlPeripheralReady(SysCtlMask));
	
	QEIConfigure(
		QEIbase,
		(QEI_CONFIG_CAPTURE_A_B | QEI_CONFIG_NO_RESET | QEI_CONFIG_QUADRATURE | QEI_CONFIG_NO_SWAP),
		1023
	);
	QEIEnable(QEIbase);
	QEIVelocityConfigure(QEIbase, QEI_VELDIV_1, 5000000);
	QEIVelocityEnable(QEIbase);
	
	return true;
}

uint32_t getQEISpeed(eQuadrature encoder) {
	switch (encoder) {
		case qei0 :
			return QEIVelocityGet(QEI0_BASE);
		case qei1 :
			return QEIVelocityGet(QEI1_BASE);
		default :
			return 0;
	}
}

int32_t getQEIDirection(eQuadrature encoder) {
	switch (encoder) {
		case qei0 :
			return QEIDirectionGet(QEI0_BASE);
		case qei1 :
			return QEIDirectionGet(QEI1_BASE);
		default :
			return 0;
	}
}

int32_t getQEIPosition(eQuadrature encoder) {
	switch (encoder) {
		case qei0 :
			return QEIPositionGet(QEI0_BASE);
		case qei1 :
			return QEIPositionGet(QEI1_BASE);
		default :
			return 0;
	}
}

bool setPWM(ePwmController controller, ePwmGenerator generator, unsigned int duty) {
	uint32_t baseAddr;
	uint32_t pwmOutput;
	
	switch (controller) {
		case pwm0 :
			baseAddr = PWM0_BASE;
			break;
		case pwm1 :
			baseAddr = PWM1_BASE;
			break;
	}
	
	switch (generator) {
		case pwm_gen0 :
			pwmOutput = PWM_OUT_0;
			break;
		case pwm_gen1 :
			pwmOutput = PWM_OUT_1;
			break;
		case pwm_gen2 :
			pwmOutput = PWM_OUT_2;
			break;
		case pwm_gen3 :
			pwmOutput = PWM_OUT_3;
			break;
	}
	
	PWMPulseWidthSet(baseAddr, pwmOutput, duty);
	
	return true;
}

bool initUART(eUartController controller, uartInfo *info) {
	uint32_t sysctlMask;
	uint32_t gpioBase;
	uint32_t uartBase;
	uint32_t uartConfigMask;
	uint32_t rxPinConfigMask;
	uint32_t txPinConfigMask;
	uint32_t typePinMask;
	
	// set bit masks to pass into lower level functions
	switch (controller) {
		case uart0 :
			sysctlMask = SYSCTL_PERIPH_UART0;
			gpioBase = GPIOA_BASE;
			uartBase = UART0_BASE;
			rxPinConfigMask = GPIO_PA0_U0RX;
			txPinConfigMask = GPIO_PA1_U0TX;
			typePinMask = GPIO_PIN_0 | GPIO_PIN_1;
			break;
		case uart1 :
			sysctlMask = SYSCTL_PERIPH_UART1;
			gpioBase = GPIOB_BASE;
			uartBase = UART1_BASE;
			rxPinConfigMask = GPIO_PB0_U1RX;
			txPinConfigMask = GPIO_PB1_U1TX;
			typePinMask = GPIO_PIN_0 | GPIO_PIN_1;
			break;
		case uart2 :
			sysctlMask = SYSCTL_PERIPH_UART2;
			gpioBase = GPIOD_BASE;
			uartBase = UART2_BASE;
			rxPinConfigMask = GPIO_PD6_U2RX;
			txPinConfigMask = GPIO_PD7_U2TX;
			typePinMask = GPIO_PIN_6 | GPIO_PIN_7;
			break;
		case uart3 :
			sysctlMask = SYSCTL_PERIPH_UART3;
			gpioBase = GPIOC_BASE;
			uartBase = UART3_BASE;
			rxPinConfigMask = GPIO_PC6_U3RX;
			txPinConfigMask = GPIO_PC7_U3TX;
			typePinMask = GPIO_PIN_6 | GPIO_PIN_7;
			break;
		case uart4 :
			sysctlMask = SYSCTL_PERIPH_UART4;
			gpioBase = GPIOC_BASE;
			uartBase = UART4_BASE;
			rxPinConfigMask = GPIO_PC4_U4RX;
			txPinConfigMask = GPIO_PC5_U4TX;
			typePinMask = GPIO_PIN_4 | GPIO_PIN_5;
			break;
		case uart5 :
			sysctlMask = SYSCTL_PERIPH_UART5;
			gpioBase = GPIOE_BASE;
			uartBase = UART5_BASE;
			rxPinConfigMask = GPIO_PE4_U5RX;
			txPinConfigMask = GPIO_PE5_U5TX;
			typePinMask = GPIO_PIN_4 | GPIO_PIN_5;
			break;
		case uart6 :
			sysctlMask = SYSCTL_PERIPH_UART6;
			gpioBase = GPIOD_BASE;
			uartBase = UART6_BASE;
			rxPinConfigMask = GPIO_PD4_U6RX;
			txPinConfigMask = GPIO_PD5_U6TX;
			typePinMask = GPIO_PIN_4 | GPIO_PIN_5;
			break;
		case uart7 :
			sysctlMask = SYSCTL_PERIPH_UART7;
			gpioBase = GPIOE_BASE;
			uartBase = UART7_BASE;
			rxPinConfigMask = GPIO_PE0_U7RX;
			txPinConfigMask = GPIO_PE1_U7TX;
			typePinMask = GPIO_PIN_0 | GPIO_PIN_1;
			break;
		default :
			return false;
	}
	// set write length, parity, and stop bits based on the info passed in
	switch (info->wlen) {
		case 5 :
			uartConfigMask = UART_CONFIG_WLEN_5;
			break;
		case 6 :
			uartConfigMask = UART_CONFIG_WLEN_6;
			break;
		case 7 :
			uartConfigMask = UART_CONFIG_WLEN_7;
			break;
		case 8 :
			uartConfigMask = UART_CONFIG_WLEN_8;
			break;
		default :
			return false;
	}
	if (info->parity) {
		uartConfigMask |= UART_CONFIG_PAR_ONE;
	}
	if (info->twoStopBits) {
		uartConfigMask |= UART_CONFIG_STOP_TWO;
	}
	
	initCircularBuffer(&(uart_txBuffs[(int)controller]), 1, BUFF_SIZE, uart_txData[(int)controller]);
	initCircularBuffer(&(uart_rxBuffs[(int)controller]), 1, BUFF_SIZE, uart_rxData[(int)controller]);
	
	// Set the GPIO pins to UART hardware control
	GPIOPinTypeUART(gpioBase, typePinMask);
	GPIOPinConfigure(rxPinConfigMask);
	GPIOPinConfigure(txPinConfigMask);
	
	// turn on the clock gate to the UART peripheral
	SysCtlPeripheralEnable(sysctlMask);
	while(!SysCtlPeripheralReady(sysctlMask));
	
	// configure and initialize the UART peripheral
	UARTConfigSetExpClk(uartBase, SysCtlClockGet(), info->baud, uartConfigMask);
	
	return true;
}

//*****************************************************************************
//
//! As long as the UART FIFO has room and there are characters available in the
//! transmit buffer to write, transfer characters from the buffer to the UART
//! hardware FIFO. As long as there are characters in the receive hardware
//! FIFO, transfer them to the receive memory buffer. If there is no room in
//! the receive memory buffer, the character is discarded. 
//!
//! @param controller specifies which UART controller to service
//!
//! @param base is the base address of the UART controller
//
//*****************************************************************************
static void uartFlow(int controller, uint32_t base) {
	char uartChar;
	
	while (UARTSpaceAvail(base)) {
		if (circularBufferRemoveItem(&(uart_txBuffs[controller]), &uartChar)) {
			UARTCharPut(base, uartChar);
		} else {
			break;
		}
	}
	
	if (UARTCharsAvail(base)) {
		uartChar = UARTCharGet(base);

		if (controller == 0 && uartChar == '\r') {
			UARTCharPut(UART0_BASE, '\n');
		}
		
		UARTCharPut(base, uartChar);
		
		circularBufferAddItem(&(uart_rxBuffs[controller]), &uartChar);
	}
}

int uart_getchar(eUartController controller) {
	char c;
	
	while (!circularBufferRemoveItem(&(uart_rxBuffs[(int)controller]), &c)) taskYield();
	
	return c;
}

int uart_getchar_nonblock(eUartController controller) {
	char c;
	
	if (!circularBufferRemoveItem(&(uart_rxBuffs[(int)controller]), &c)) {
		return -1;
	} else {
		return c;
	}
}

int uart_putchar(eUartController controller, int c) {
	char in;
	
	in = c;
	while (!circularBufferAddItem(&(uart_txBuffs[(int)controller]), &in)) taskYield();
	
	return c;
}

int uart_putchar_nonblock(eUartController controller, int c) {
	char in;
	
	in = c;
	if (!circularBufferAddItem(&(uart_txBuffs[(int)controller]), &in)) {
		return -1;
	} else {
		return c;
	}
}
