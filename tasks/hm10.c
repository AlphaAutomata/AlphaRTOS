#include "badgerRMCRTOS.h"

#include "LMCterminal.h"
#include "isr.h"

int timerCallback(uint32_t arg) {
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
	
	for (i=0; i<2; i++) {
		uart_putchar(uart1, string[i]);
	}
	
	uartFlow(uart1, UART1_BASE);
	
	return 0;
}

int hm10_init(uint32_t arg) {
	uartInfo uinfo;
	
	uinfo.baud = 9600;
	uinfo.wlen = 8;
	uinfo.parity = false;
	uinfo.twoStopBits = false;
	initUART(uart1, &uinfo);
	
	timerCallbackRegister(100, timerCallback);
	
	return 0;
}
