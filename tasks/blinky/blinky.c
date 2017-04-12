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
		kprintf("  Last OPCODE = 0x%X\n", (uint8_t)lastOp);
		kprintf("  Left Speed  = 0x%X\n", wheelPWM.left);
		kprintf("  Right Speed = 0x%X\n", wheelPWM.right);
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
