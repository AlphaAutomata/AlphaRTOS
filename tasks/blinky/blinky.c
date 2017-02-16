#include "badgerRMCRTOS.h"
#include "launchPadHwAbstraction.h"
#include "launchPadUIO.h"
#include "LMCterminal.h"

volatile bool ledOn;

int taskCallback(void *arg) {
	if (ledOn) {
		ledOn = false;
		//kprintf("Uptime = %lms\n", getUptime());
		setLED(off);
	} else {
		ledOn = true;
		//kprintf("Uptime = %lms\n", getUptime());
		setLED(green);
	}
	
	return 0;
}

int blinkyTask(void *arg) {
	ledOn = false;
	
	timerCallbackRegister(1000, taskCallback);
	
	setLED(off);
	
	return 0;
}
