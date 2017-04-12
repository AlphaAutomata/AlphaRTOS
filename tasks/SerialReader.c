#include "badgerRMCRTOS.h"
#include "launchPadHwAbstraction.h"
#include "LMCterminal.h"
#include "Packet.h"
#include "global_state.h"

struct DrivePayload dr;

uint8_t packet[16];
uint8_t opcode;
uint8_t *payload_ptr;
uint8_t byteNum;

// does not return unless it fails
int parse(uint32_t arg) {
	unsigned int leftWidth;
	unsigned int rightWidth;
	int i;
	
	// continuously read UART bytes
	// uses getchar(), which yields the CPU when blocking
	while (byteNum < 16) {
		packet[byteNum] = getchar();
		if (packet[byteNum] == 0x00) {
			kprintf("Received Packet Dump\n");
			for (i=0; i<16; i++) {
				kprintf(" 0x%X", packet[i]);
			}
			kprintf("\n");
			
			if (ReadPacketHeader(&packet[0], 16, &opcode, &payload_ptr)) {
				lastOp = opcode;
			}
			
			kprintf("Decoded Packet Dump\n");
			for (i=0; i<16; i++) {
				kprintf(" 0x%X", packet[i]);
			}
			kprintf("\n");
			
			switch (opcode) {
				case DRIVE_OPCODE :
					ParseDrivePayload(payload_ptr, &dr);
					
					leftWidth = (unsigned int) (((ONE_MS_PULSE_WIDTH * (((int)dr.left)+100)) / 200) + ONE_MS_PULSE_WIDTH);
					wheelPWM.left = leftWidth;

					rightWidth = (unsigned int) (((ONE_MS_PULSE_WIDTH * (((int)dr.right)+100)) / 200) + ONE_MS_PULSE_WIDTH);
					wheelPWM.right = rightWidth;
					break;
				
				default :
					break;
			}
			
			byteNum = 0xFF;
		}
		
		byteNum++;
	}
	
	return 0;
}

int SerialReader(uint32_t arg) {
	timerCallbackRegister(3, parse);
	
	dr.left = 0;
	dr.right = 0;
	wheelPWM.left = (3*ONE_MS_PULSE_WIDTH)/2;
	wheelPWM.right = (3*ONE_MS_PULSE_WIDTH)/2;
	
	byteNum = 0;
	
	return 0;
}
