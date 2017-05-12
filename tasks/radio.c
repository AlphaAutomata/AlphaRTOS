#include "badgerRMCRTOS.h"
#include "LMCTerminal.h"

uint8_t string[8] = {'A', 'T', '+', 'N', 'A', 'M', 'E', '?'};

static void putLineEnding(eUartController uart) {
	uart_putchar(uart, '\n');
	uart_putchar(uart, '\r');
}

int timerCallback(uint32_t arg) {
	while(1) putchar(uart_getchar(uart1));
}

int radio_init(uint32_t arg) {
	uartInfo uinfo;
	
	uinfo.baud = 115200;
	uinfo.wlen = 8;
	uinfo.parity = false;
	uinfo.twoStopBits = false;
	initUART(uart1, &uinfo);
	
	timerCallbackRegister(10, timerCallback);
	
	return 0;
}
