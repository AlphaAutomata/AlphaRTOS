#include "badgerRMCRTOS.h"
#include "launchPadHwAbstraction.h"
#include "launchPadUIO.h"

volatile bool ledOn;

int taskCallback(void *arg) {
	if (ledOn) {
		ledOn = false;
		setLED(off);
	} else {
		ledOn = true;
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
