#include <stdarg.h>

#include "LMCterminal.h"
#include "badgerRMCRTOS.h"
#include "circular_buffer.h"

#include "launchPadUIO.h"

#define BUFF_SIZE 16
#define CONVERSION_BUFFER_SIZE 24

circularBuffer_t txBuff;
uint8_t txData[BUFF_SIZE];

circularBuffer_t rxBuff;
uint8_t rxData[BUFF_SIZE];

//*****************************************************************************
//
//! As long as the UART FIFO has room and there are characters available in the
//! transmit buffer to write, transfer characters from the buffer to the UART
//! hardware FIFO. As long as there are characters in the receive hardware
//! FIFO, transfer them to the receive memory buffer. If there is no room in
//! the receive memory buffer, the character is discarded. 
//!
//! \param arg is ignored.
//!
//! \return 0, always
//
//*****************************************************************************
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
//! Called whenever a UART Tx or Rx interrupt happens. If UART0 caused the
//! interrupt, call uartFlow() to handle I/O.
//!
//! \param interruptType is the type of interrupt received. Only accepts UART
//!
//! \param deviceMask is a bit mask identifying which UART modules caused
//! interrupts. Bit-0, the LSB, being set indicates UART0, bit 7 being set
//! indicates UART7. Only handle UART0. 
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
	usbUART.baud = arg;
	usbUART.wlen = 8;
	usbUART.parity = false;
	usbUART.twoStopBits = false;
	initUART(uart0, &usbUART);
	
	// every millisecond, poll the UART
	timerCallbackRegister(5, uartFlow);
	
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
	
	while (!circularBufferRemoveItem(&rxBuff, &c)) taskYield();
	
	return c;
}


int getchar_nonblock(void){
	char c;
	if (!circularBufferRemoveItem(&rxBuff, &c)) {
		return -1;
	} else {
		return c;
	}
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

int printlit(const char *strlit) {
	int numChars;
	
	numChars = 0;
	while (*strlit != '\0') {
		putchar(*strlit);
		numChars++;
		if (*strlit == '\n') {
			putchar('\r');
			numChars++;
		}
		strlit++;
	}
	
	return numChars;
}

int kprintf(const char *format, ...) {
	char *rdChar;
	char convBuff[CONVERSION_BUFFER_SIZE];
	char printChar;
	int intVal;
	int i;
	int charCnt;
	uint32_t hexVal;
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
				putchar(*rdChar);
				putchar('\r');
				charCnt += 2;
				break;
			// number literal conversions
			case '%' :
				rdChar++;
				switch (*rdChar) {
					case '\0' :
						goto END;
					case 'c' :
						printChar = va_arg(va, int);
						putchar(printChar);
						charCnt++;
						break;
					case 'd' :
					case 'i' :
						// grab the argument value
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
					case 'X' :
						hexVal = va_arg(va, uint32_t);
						i = CONVERSION_BUFFER_SIZE - 1;
						while ((hexVal >= 16) && (i >= 0)) {
							if (hexVal % 16 < 10) {
								convBuff[i] = (hexVal % 16) + '0';
							} else {
								convBuff[i] = (hexVal % 16) - 10 + 'A';
							}
							hexVal /= 16;
							i--;
						}
						if (hexVal % 16 < 10) {
							convBuff[i] = (hexVal % 16) + '0';
						} else {
							convBuff[i] = (hexVal % 16) - 10 + 'A';
						}
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
				break;
		}
		
		rdChar++;
	}
	
	END:
	
	va_end(va);
	
	return charCnt;
}


