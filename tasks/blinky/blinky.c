#include "badgerRMCRTOS.h"
#include "launchPadHwAbstraction.h"
#include "launchPadUIO.h"
#include "LMCterminal.h"
#include "global_state.h"

volatile bool ledOn;
volatile bool lbpressed;
volatile bool rbpressed;
volatile bool print;

int taskCallback(uint32_t arg) {
	if (ledOn) {
		ledOn = false;
		setLED(off);
	} else {
		ledOn = true;
		setLED(green);
	}
	
	if (print) {
		kprintf("Uptime = %ls\n", getUptime()/1000);
		kprintf("  Left Speed  = %d", wheelQEI.leftSpd);
		if (wheelQEI.leftDir > 0) {
			kprintf(" fwd |\n");
		} else if (wheelQEI.leftDir < 0) {
			kprintf("     | bkw\n");
		} else {
			kprintf("\n");
		}
		kprintf("  Right Speed = %d", wheelQEI.rightSpd);
		if (wheelQEI.rightDir > 0) {
			kprintf(" fwd |\n");
		} else if (wheelQEI.rightDir < 0) {
			kprintf("     | bkw\n");
		} else {
			kprintf("\n");
		}
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
	print = false;
	
	timerCallbackRegister(1000, taskCallback);
	
	setLED(off);
	
	return 0;
}
