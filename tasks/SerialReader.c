#include "badgerRMCRTOS.h"
#include "launchPadHwAbstraction.h"
#include "LMCterminal.h"
#include "Packet.h"

struct width {
	uint32_t left;
	uint32_t right;
};

struct width *DrivingWidth;

struct DrivePayload dr;

int parse(uint32_t arg) {
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
				if (readChar != 0xAB){
					return 0;
				} else {
					break;
				}
			
			case 1 : 
				if (readChar != 0x00){
					return 0;
				} else {
					break;
				}
				
			case 2 : 
					if (readChar != 0x02){
					return 0;
				} else {
					break;
				}
			
			case 3 :
				leftSpd = (int8_t)readChar;
				leftWidth = (unsigned int) (ONE_MS_PULSE_WIDTH*(leftSpd+100))/200 + ONE_MS_PULSE_WIDTH/2;
				DrivingWidth -> left = leftWidth;
				break;
			
			case 4 :
				rightSpd = (int8_t)readChar;
				rightWidth = (unsigned int) (ONE_MS_PULSE_WIDTH*(rightSpd+100))/200 + ONE_MS_PULSE_WIDTH/2;
				DrivingWidth -> right = rightWidth;
				break;
			
			case 5 :
				return 0;
			
			default :
				return 0;
		}
		numBytes++;
		
	}
}

int SerialReader(uint32_t arg) {
	timerCallbackRegister(3, parse);
	DrivingWidth = (struct width *)arg;
	DrivingWidth->left = (3*ONE_MS_PULSE_WIDTH)/2;
	DrivingWidth->right = (3*ONE_MS_PULSE_WIDTH)/2;
	return 0;
}
