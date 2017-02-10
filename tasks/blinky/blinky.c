#include "badgerRMCRTOS.h"
#include "launchPadHwAbstraction.h"
#include "launchPadUIO.h"

volatile bool ledOn;

int taskCallback(void *arg) {
	if (ledOn) {
		setLED(off);
	} else {
		setLED(green);
	}
	
	return 0;
}

int blinkyTask(void *arg) {
	int callbackID;
	
	ledOn = false;
	
	callbackID = timerCallbackRegister(100, taskCallback);
	setTimerCallbackInterval(callbackID, 1000);
	
	setLED(off);
	
	return 0;
}
