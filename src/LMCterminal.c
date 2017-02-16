#include <stdarg.h>

#include "LMCterminal.h"
#include "badgerRMCRTOS.h"
#include "circular_buffer.h"

#define BUFF_SIZE 16
#define CONVERSION_BUFFER_SIZE 24

circularBuffer_t txBuff;
uint8_t txData[BUFF_SIZE];

circularBuffer_t rxBuff;
uint8_t rxData[BUFF_SIZE];

int uartFlow(uint32_t arg) {
	char uartChar;
	
	while (UARTSpaceAvail(UART0_BASE)) {
		if (circularBufferRemoveItem(&txBuff, &uartChar)) {
			UARTCharPut(UART0_BASE, uartChar);
		} else {
			break;
		}
	}
	
	if (UARTCharsAvail(UART0_BASE)) {
		uartChar = UARTCharGet(UART0_BASE);

		if (uartChar == '\r') {
			UARTCharPut(UART0_BASE, '\n');
		}
		
		UARTCharPut(UART0_BASE, uartChar);
		
		circularBufferAddItem(&rxBuff, &uartChar);
	}
	
	return 0;
}

//*****************************************************************************
//
//! Initialize the LMC Terminal. Initialize UART0, set up interrupts, and
//! and register the terminal's interrupt callback.
//!
//! \param interruptType is the type of interrupt received. Only accepts UART
//!
//! \param deviceMask is a bit mask identifying which UART modules caused
//! interrupts. Only handle UART0.
//!
//! \return 0, always
//
//*****************************************************************************
int interruptCallback(eInterrupt interruptType, uint32_t deviceMask) {
	// we only care about the UART0 controller. Reject all other interrupts.
	if (interruptType == UART && (deviceMask & 0x00000001)) uartFlow(0);
	
	return 0;
}

int initLMCterminal(uint32_t arg) {
	uartInfo usbUART;
	
	// initialize USB UART to 8N1
	usbUART.baud = 115200;
	usbUART.wlen = 8;
	usbUART.parity = false;
	usbUART.twoStopBits = false;
	initUART(uart0, &usbUART);
	
	// every millisecond, poll the UART
	timerCallbackRegister(1, uartFlow);
	
	// every time a UART FIFO interrupt happens, handle it
	interruptCallbackRegister(UART, interruptCallback, 0);
	initCircularBuffer(&rxBuff, 1, BUFF_SIZE, rxData);
	initCircularBuffer(&txBuff, 1, BUFF_SIZE, txData);
	
	return 0;
}

//*****************************************************************************
//
//! Get character from specified UART0
//!
//! \param none
//!
//! \return the character read from USB UART
//
// *****************************************************************************
int getchar(void) {
	char c;
	
	while (!circularBufferRemoveItem(&rxBuff, &c));
	
	return c;
}

//*****************************************************************************
//
//! Put character to a UART0
//!
//! \param c is the character to write to USB UART
//!
//! \return the character written to USB UART
//
// *****************************************************************************
int putchar(int c) {
	char in;
	
	in = c;
	while (!circularBufferAddItem(&txBuff, &in)) taskYield();
	
	return c;
}

//#define va_start(va,arg0) va = (char *)(&arg0) - sizeof(arg0)
//#define va_arg(va,type) *((type *)va); va = (char *)va - sizeof(type)
//#define va_end(va) va = 0

int printlit(const char *strlit) {
	while (*strlit != '\0') {
		putchar(*strlit);
		if (*strlit == '\n') putchar('\r');
		strlit++;
	}
	
	return 0;
}

//*****************************************************************************
//
//!
//
// *****************************************************************************
int kprintf(const char *format, ...) {
	char *rdChar;
	char convBuff[CONVERSION_BUFFER_SIZE];
	char printChar;
	int intVal;
	int i;
	int charCnt;
	uint64_t longVal;
	
	// process variable arguments
	va_list va;
	va_start(va, format);
	
	rdChar = (char *)format;
	charCnt = 0;
	
	// while this does not check the input string length, the function also
	// does not write to memory but out to UART0
	while (*rdChar != '\0') {
		switch (*rdChar) {
			// special character considerations
			case '\n' :
				putchar('\n');
				putchar('\r');
				charCnt++;
				break;
			// number literal conversions
			case '%' :
				rdChar++;
				switch (*rdChar) {
					case '\0' :
						goto END;
					case 'c' :
						printChar = va_arg(va, unsigned int);
						putchar(printChar);
						charCnt++;
						break;
					case 'd' :
					case 'i' :
						// grap the argument value
						// print negative sign if necessary
						intVal = va_arg(va, int);
						if (intVal < 0) {
							putchar('-');
							intVal = -intVal;
						}
						// convert to integer literal
						i = CONVERSION_BUFFER_SIZE - 1;
						while ((intVal >= 10) && (i >= 0)) {
							convBuff[i] = (intVal % 10) + '0';
							intVal /= 10;
							i--;
						}
						convBuff[i] = (char)(intVal + '0');
						// print buffer
						while (i < CONVERSION_BUFFER_SIZE) {
							putchar(convBuff[i]);
							charCnt++;
							i++;
						}
						break;
					case 'l' :
						// grap the argument value
						longVal = va_arg(va, uint64_t);
						// convert to integer literal
						i = CONVERSION_BUFFER_SIZE - 1;
						while ((longVal >= 10) && (i >= 0)) {
							convBuff[i] = (longVal % 10) + '0';
							longVal /= 10;
							i--;
						}
						convBuff[i] = (char)(longVal + '0');
						// print buffer
						while (i < CONVERSION_BUFFER_SIZE) {
							putchar(convBuff[i]);
							charCnt++;
							i++;
						}
						break;
				}
				break;
			default :
				putchar(*rdChar);
				charCnt++;
		}
		
		rdChar++;
	}
	
	END:
	
	va_end(va);
	
	return charCnt;
}
