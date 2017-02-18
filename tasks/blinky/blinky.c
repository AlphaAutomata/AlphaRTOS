#include "badgerRMCRTOS.h"
#include "launchPadHwAbstraction.h"
#include "launchPadUIO.h"
#include "LMCterminal.h"

volatile bool ledOn;
volatile bool lbpressed;
volatile bool rbpressed;
volatile bool print;

int taskCallback(uint32_t arg) {
	if (ledOn) {
		ledOn = false;
		if (print) kprintf("Uptime = %lms\n", getUptime());
		setLED(off);
	} else {
		ledOn = true;
		if (print) kprintf("Uptime = %lms\n", getUptime());
		setLED(green);
	}
	
	buttonsPressed(&lbpressed, &rbpressed);
	if (lbpressed) {
		if (print) {
			printlit("Blinky: printing disabled\n");
			print = false;
		} else {
			printlit("Blinky: printing enabled\n");
			print = true;
		}
	}
	
	return 0;
}

int blinkyTask(uint32_t arg) {
	ledOn = false;
	print = true;
	
	timerCallbackRegister(1000, taskCallback);
	
	setLED(off);
	
	return 0;
}
