#include "badgerRMCRTOS.h"
#include "launchPadHwAbstraction.h"
#include "launchPadUIO.h"
#include "LMCterminal.h"

volatile bool ledOn;

int taskCallback(uint32_t arg) {
	if (ledOn) {
		ledOn = false;
		printlit("LED off\n");
		//kprintf("Uptime = %lms\n", getUptime());
		setLED(off);
	} else {
		ledOn = true;
		printlit("LED on\n");
		//kprintf("Uptime = %lms\n", getUptime());
		setLED(green);
	}
	
	return 0;
}

int blinkyTask(uint32_t arg) {
	ledOn = false;
	
	timerCallbackRegister(1000, taskCallback);
	
	setLED(off);
	
	return 0;
}
