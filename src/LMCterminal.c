#include <stdarg.h>

#include "LMCterminal.h"
#include "badgerRMCRTOS.h"
#include "circular_buffer.h"

#include "isr.h"

#define CONVERSION_BUFFER_SIZE 24

int initLMCterminal(uint32_t arg) {
	uartInfo usbUART;
	
	// initialize USB UART to 8N1
	usbUART.baud = arg;
	usbUART.wlen = 8;
	usbUART.parity = false;
	usbUART.twoStopBits = false;
	initUART(uart0, &usbUART);
	
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
	return uart_getchar(uart0);
}

int getchar_nonblock(void){
	return uart_getchar_nonblock(uart0);
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
	return uart_putchar(uart0, c);
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


