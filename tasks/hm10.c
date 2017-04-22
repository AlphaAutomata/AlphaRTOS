#include "badgerRMCRTOS.h"
#include "isr.h"
#include "circular_buffer.h"

#include "LMCterminal.h"

#define BUFF_SIZE 32

circularBuffer_t u1_txBuff;
uint8_t u1_txData[BUFF_SIZE];

circularBuffer_t u1_rxBuff;
uint8_t u1_rxData[BUFF_SIZE];

static void uartFlow(void) {
	char uartChar;
	
	while (UARTSpaceAvail(UART1_BASE)) {
		if (circularBufferRemoveItem(&u1_txBuff, &uartChar)) {
			UARTCharPut(UART1_BASE, uartChar);
		} else {
			break;
		}
	}
	
	if (UARTCharsAvail(UART1_BASE)) {
		uartChar = UARTCharGet(UART1_BASE);
		circularBufferAddItem(&u1_rxBuff, &uartChar);
	}
}

static int interruptCallback(eInterrupt interruptType, uint32_t deviceMask) {
	if (!(interruptType == UART && (deviceMask & 0x00000002))) return 0;
	
	if (uartHWIntMask[1] & (UART_INT_TX | UART_INT_RX)) {
		if (uartHWErrMask[1] != 0) {
			while (UARTSpaceAvail(UART1_BASE)) UARTCharGet(UART1_BASE);
		}
	}
	
	uartFlow();
	
	deviceMask &= !0x00000002;
	uartHWIntMask[1] = 0;
	
	return 0;
}

int timerCallback(uint32_t arg) {
	char a;
	char t;
	char data;
	int i;
	
	uint8_t string[10];
	
	string[0] = 'A';
	string[1] = 'T';
	string[2] = '+';
	string[3] = 'N';
	string[4] = 'A';
	string[5] = 'M';
	string[6] = 'E';
	string[7] = '?';
	string[8] = '\n';
	string[9] = '\r';
	
	for (i=0; i<8; i++) {
		circularBufferAddItem(&u1_txBuff, &(string[i]));
	}
	
	uartFlow();
	
//	while (circularBufferRemoveItem(&u1_rxBuff, &data)) {
//		switch (data >> 4) {
//			case 0x0 :
//				putchar('0');
//				break;
//			case 0x1 :
//				putchar('1');
//				break;
//			case 0x2 :
//				putchar('2');
//				break;
//			case 0x3 :
//				putchar('3');
//				break;
//			case 0x4 :
//				putchar('4');
//				break;
//			case 0x5 :
//				putchar('5');
//				break;
//			case 0x6 :
//				putchar('6');
//				break;
//			case 0x7 :
//				putchar('7');
//				break;
//			case 0x8 :
//				putchar('8');
//				break;
//			case 0x9 :
//				putchar('9');
//				break;
//			case 0xA :
//				putchar('A');
//				break;
//			case 0xB :
//				putchar('B');
//				break;
//			case 0xC :
//				putchar('C');
//				break;
//			case 0xD :
//				putchar('D');
//				break;
//			case 0xE :
//				putchar('E');
//				break;
//			case 0xF :
//				putchar('F');
//				break;
//			
//			default :
//				break;
//		}
//		switch (data & 0x0F) {
//			case 0x0 :
//				putchar('0');
//				break;
//			case 0x1 :
//				putchar('1');
//				break;
//			case 0x2 :
//				putchar('2');
//				break;
//			case 0x3 :
//				putchar('3');
//				break;
//			case 0x4 :
//				putchar('4');
//				break;
//			case 0x5 :
//				putchar('5');
//				break;
//			case 0x6 :
//				putchar('6');
//				break;
//			case 0x7 :
//				putchar('7');
//				break;
//			case 0x8 :
//				putchar('8');
//				break;
//			case 0x9 :
//				putchar('9');
//				break;
//			case 0xA :
//				putchar('A');
//				break;
//			case 0xB :
//				putchar('B');
//				break;
//			case 0xC :
//				putchar('C');
//				break;
//			case 0xD :
//				putchar('D');
//				break;
//			case 0xE :
//				putchar('E');
//				break;
//			case 0xF :
//				putchar('F');
//				break;
//			
//			default :
//				break;
//		}
//	}
	
	return 0;
}

int hm10_init(uint32_t arg) {
	uartInfo uinfo;
	
	uinfo.baud = 9600;
	uinfo.wlen = 8;
	uinfo.parity = false;
	uinfo.twoStopBits = false;
	initUART(uart1, &uinfo);
	
	initCircularBuffer(&u1_txBuff, 1, BUFF_SIZE, u1_txData);
	initCircularBuffer(&u1_rxBuff, 1, BUFF_SIZE, u1_rxData);
	
	timerCallbackRegister(100, timerCallback);
	interruptCallbackRegister(UART, interruptCallback, 1);
	
	return 0;
}
