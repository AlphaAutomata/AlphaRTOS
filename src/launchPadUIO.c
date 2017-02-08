#include "launchPadUIO.h"

void initUIO(void) {
	// set LED pins as output
	GPIOPinTypeGPIOOutput(GPIOF_BASE, GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3);
	// set push button pins as input
	GPIOPinTypeGPIOInput(GPIOF_BASE, GPIO_PIN_0|GPIO_PIN_4);
}

void setLED(enum ledColor color) {
	uint32_t pinColorMap;
	
	// convert the enumerated type to a pin map
	switch (color) {
		case off :
			pinColorMap = 0;
			break;
		case red :
			pinColorMap = GPIO_PIN_1;
			break;
		case green :
			pinColorMap = GPIO_PIN_3;
			break;
		case blue :
			pinColorMap = GPIO_PIN_2;
			break;
		case yellow :
			pinColorMap = GPIO_PIN_1|GPIO_PIN_3;
			break;
		case magenta :
			pinColorMap = GPIO_PIN_1|GPIO_PIN_2;
			break;
		case cyan :
			pinColorMap = GPIO_PIN_2|GPIO_PIN_3;
			break;
		case white :
			pinColorMap = GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3;
			break;
	}
	
	// do GPIO write to set LED color
	GPIOPinWrite(GPIOF_BASE, GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3, pinColorMap);
}

void buttonsPressed(bool *lPressed, bool *rPressed) {
	uint32_t pinData;
	
	// read the button pins
	GPIOPinRead(GPIOF_BASE, GPIO_PIN_0|GPIO_PIN_4);
	
	// convert the pin data to output booleans
	if (pinData | GPIO_PIN_4) {
		*lPressed = false;
	} else {
		*lPressed = true;
	}
	if (pinData | GPIO_PIN_0) {
		*lPressed = false;
	} else {
		*lPressed = true;
	}
}
