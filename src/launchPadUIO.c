#include "launchPadUIO.h"
#include "inc/hw_types.h"
#include "inc/hw_gpio.h"

void initUIO(void) {
	// unlock the GPIO port so we can change the pad configuration
	HWREG(GPIOF_BASE + GPIO_O_LOCK) = GPIO_LOCK_KEY;
	HWREG(GPIOF_BASE + GPIO_O_CR) = GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_3 | GPIO_PIN_4;
	
	// set LED pins as output
	GPIOPinTypeGPIOOutput(GPIOF_BASE, GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3);
	// set push button pins as input
	GPIOPinTypeGPIOInput(GPIOF_BASE, GPIO_PIN_0|GPIO_PIN_4);
	// set pull-up resistors for the push buttons
	GPIOPadConfigSet(GPIOF_BASE, GPIO_PIN_0|GPIO_PIN_4, GPIO_STRENGTH_2MA, GPIO_PIN_TYPE_STD_WPU);
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

void buttonsPressed(volatile bool *lPressed, volatile bool *rPressed) {
	uint32_t pinData;
	
	// read the button pins
	pinData = GPIOPinRead(GPIOF_BASE, GPIO_PIN_0|GPIO_PIN_4);
	
	// convert the pin data to output booleans
	if (pinData & GPIO_PIN_0) {
		*lPressed = false;
	} else {
		*lPressed = true;
	}
	if (pinData & GPIO_PIN_4) {
		*lPressed = false;
	} else {
		*lPressed = true;
	}
}
