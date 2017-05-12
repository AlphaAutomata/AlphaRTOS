#include "badgerRMCRTOS.h"
#include "launchPadHwAbstraction.h"
#include "LMCterminal.h"
#include "Packet.h"
#include "global_state.h"

#define PARSE_BUFF_SIZE 16

struct DrivePayload dr;

uint8_t packet[PARSE_BUFF_SIZE];
uint8_t opcode;
uint8_t *payload_ptr;
uint8_t byteNum;

// does not return unless it fails
int parse(uint32_t arg) {
	unsigned int leftWidth;
	unsigned int rightWidth;
#ifdef DEBUG_SERIAL_READER
	int i;
#endif
	
	// this function should never return, so reset read state on entry
	byteNum = 0;
	
	// continuously read UART bytes
	// uses getchar(), which yields the CPU when blocking
	while (byteNum < PARSE_BUFF_SIZE) {
		packet[byteNum] = getchar();
		if (packet[byteNum] == 0x00) {
#ifdef DEBUG_SERIAL_READER
			kprintf("Received Packet Dump\n");
			for (i=0; i<16; i++) {
				kprintf(" 0x%X", packet[i]);
			}
			kprintf("\n");
#endif
			
			ReadPacketHeader(&packet[0], PARSE_BUFF_SIZE, &opcode, &payload_ptr);
#ifdef DEBUG_SERIAL_READER
			kprintf("Decoded Packet Dump\n");
			for (i=0; i<16; i++) {
				kprintf(" 0x%X", packet[i]);
			}
			kprintf("\n");
#endif
			
			switch (opcode) {
				case DRIVE_OPCODE :
					ParseDrivePayload(payload_ptr, &dr);
					
					leftWidth = (unsigned int) (((ONE_MS_PULSE_WIDTH * (((int)(dr.left))+100)) / 200) + ONE_MS_PULSE_WIDTH);
					wheelPWM.left = leftWidth;

					rightWidth = (unsigned int) (((ONE_MS_PULSE_WIDTH * (((int)(dr.right))+100)) / 200) + ONE_MS_PULSE_WIDTH);
					wheelPWM.right = rightWidth;
					break;
				
				case QUERY_RSSI_OPCODE :
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
