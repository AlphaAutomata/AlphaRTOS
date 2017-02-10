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
	int callbackID;
	
	ledOn = false;
	
	callbackID = callbackRegister(100, taskCallback);
	setCallbackInterval(callbackID, 1000);
	
	setLED(off);
	
	return 0;
}
