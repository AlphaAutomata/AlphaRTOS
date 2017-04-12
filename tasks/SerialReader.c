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
uint8_t byteNum;

// does not return unless it fails
int parse(uint32_t arg) {
	unsigned int leftWidth;
	unsigned int rightWidth;
	
	// continuously read UART bytes
	// uses getchar(), which yields the CPU when blocking
	while (byteNum < 16) {
		packet[byteNum] = getchar();
		if (packet[byteNum] == 0x00) {
			ReadPacketHeader(&packet[0], byteNum, &opcode, &payload_ptr);
			
			switch (opcode) {
				case DRIVE_OPCODE :
					ParseDrivePayload(payload_ptr, &dr);
					
					leftWidth = (unsigned int) ((ONE_MS_PULSE_WIDTH*((int)dr.left+100))/200 + ONE_MS_PULSE_WIDTH/2);
					DrivingWidth->left = leftWidth;

					rightWidth = (unsigned int) ((ONE_MS_PULSE_WIDTH*((int)dr.right+100))/200 + ONE_MS_PULSE_WIDTH/2);
					DrivingWidth->right = rightWidth;
					break;
				
				default :
					break;
			}
			
			byteNum = 0;
		}
	}
	
	return 0;
}

int SerialReader(uint32_t arg) {
	timerCallbackRegister(3, parse);
	DrivingWidth = (struct width *)arg;
	
	dr.left = 0;
	dr.right = 0;
	DrivingWidth->left = (3*ONE_MS_PULSE_WIDTH)/2;
	DrivingWidth->right = (3*ONE_MS_PULSE_WIDTH)/2;
	
	byteNum = 0;
	
	return 0;
}
