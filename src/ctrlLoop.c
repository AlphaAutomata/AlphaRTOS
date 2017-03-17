#include "badgerRMCRTOS.h"
#include "launchPadHwAbstraction.h"
#include "LMCterminal.h"

int output(uint32_t arg) {
	int numBytes;
	int readChar;
	int8_t leftSpd;
	int8_t rightSpd;
	unsigned int leftWidth;
	unsigned int rightWidth;
	
	numBytes = 0;
	
	while (1){
		readChar = getchar_nonblock();
		if (readChar == -1) return 0;
		
		switch (numBytes) {
			case 0 : 
				if (readChar != 0xBE){
					return 0;
				} else {
					break;
				}
			
			case 1 : 
				if (readChar != 0xEF){
					return 0;
				} else {
					break;
				}
			
			case 3 : 
				if (readChar != 0x00){
					return 0;
				} else {
					break;
				}
				
			case 4 : 
					if (readChar != 0x02){
					return 0;
				} else {
					break;
				}
			
			case 5 : 
				break;
			
			case 6 :
				leftSpd = (int8_t)readChar;
				leftWidth = (unsigned int) ONE_MS_PULSE_WIDTH * (leftSpd+100)/200;
				break;
			case 7 :
				rightSpd = (int8_t)readChar;
				rightWidth = (unsigned int) ONE_MS_PULSE_WIDTH * (rightSpd+100)/200;
				break;
			case 8 :
				setPWM(pwm0, pwm_gen0, leftWidth);
				setPWM(pwm0, pwm_gen1, rightWidth);
				return 0;
			default :
				return 0;
		}
		numBytes ++;
	}
}

int ctrlLoop(uint32_t arg) {
	timerCallbackRegister(3, output);
	return 0;
}
