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

uint8_t packet[16];
uint8_t opcode;
uint8_t *payload_ptr;
int byteNum;

int parse(uint32_t arg) {
	int readChar;
	unsigned int leftWidth;
	unsigned int rightWidth;
	
	readChar = getchar_nonblock();
	if (readChar == PKT_HEADER_BYTE) {
		packet[0] = readChar;
		
		byteNum = 1;
		while(1) {
			readChar = getchar_nonblock();
			
			if (readChar >= 0) {
				packet[byteNum] = readChar;
				byteNum++;
			}
			
			if (byteNum > 4) break;
		}
	}
	
	ReadPacketHeader(&packet[0], byteNum, &opcode, &payload_ptr);
	if (opcode == DRIVE_OPCODE) {
		ParseDrivePayload(payload_ptr, &dr);
	}
	
	leftWidth = (unsigned int) ((ONE_MS_PULSE_WIDTH*((int)dr.left+100))/200 + ONE_MS_PULSE_WIDTH/2);
	DrivingWidth -> left = leftWidth;

	rightWidth = (unsigned int) ((ONE_MS_PULSE_WIDTH*((int)dr.right+100))/200 + ONE_MS_PULSE_WIDTH/2);
	DrivingWidth -> right = rightWidth;
	
	return 0;
}

int SerialReader(uint32_t arg) {
	timerCallbackRegister(3, parse);
	DrivingWidth = (struct width *)arg;
	dr.left = 0;
	dr.right = 0;
	DrivingWidth->left = (3*ONE_MS_PULSE_WIDTH)/2;
	DrivingWidth->right = (3*ONE_MS_PULSE_WIDTH)/2;
	return 0;
}
