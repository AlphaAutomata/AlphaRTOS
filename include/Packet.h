#ifndef _PACKET_H
#define _PACKET_H

#ifdef _cplusplus
extern "C" {
#endif

#include <stdint.h>

#define __CLAMP(val, max, min) { if (val > max) val = max; if (val < min) val = min; }

#define PKT_HEADER_BYTE						0xAB 
#define PKT_END_BYTE    					0x7F
#define PKT_MIN_SIZE						4
#define PKT_HDR_INDEX						0
#define PKT_OP_INDEX						1
#define PKT_PAYLOAD_SIZE_INDEX				2
#define PKT_PAYLOAD_START_INDEX				3

// opcodes //
#define DRIVE_OPCODE						0x99

#define DIRECT_SWITCH_OPCODE				0x90
#define AI_SWITCH_OPCODE					0x91
#define REMOTE_SWITCH_OPCODE				0x92
#define SWITCH_MODE_ACK_OPCODE              0x93

#define AI_INIT_OPCODE                      0x02
#define QUERY_LOCATION_OPCODE				0x44
#define REPORT_LOCATION_OPCODE				0x45
#define SET_TARGET_LOCATION_OPCODE          0x46

#define QUERY_ENCODER_OPCODE                0x50
#define REPORT_ENCODER_OPCODE               0x47

#define QUERY_CAMERA_IMAGE_OPCODE			0x13

#define QUERY_HEARTBEAT_OPCODE				0x14
#define REPORT_HEARTBEAT_OPCODE				0x44

#define ESTOP_OPCODE                        0xAA
#define CLEAR_ESTOP_OPCODE                  0xAB

// payload structs //
struct DrivePayload
{
	int8_t left;
	int8_t right;
};

struct LocationPayload
{
    int16_t x;
    int16_t y;
    int16_t heading; // capped between +/- 180
};

struct EncoderPayload
{
    int16_t front_left;
    int16_t front_right;
    int16_t back_left;
    int16_t back_right;
    int16_t left_actuator;
    int16_t right_actuator;
};

// functions //

static int encodeCOBS(uint8_t *buffer, uint8_t length) {
	uint8_t i;
	uint8_t j;
	uint8_t sec_hd;
	
	sec_hd = 0;
	for (i=0; i<=length; i++) {
		if (buffer[i] == 0x00) {
			for (j=i; j>sec_hd; j--) {
				buffer[j] = buffer[j-1];
			}
			buffer[sec_hd] = i - sec_hd + 1;
			sec_hd = i + 1;
		}
	}
	
	buffer[length+1] = 0x00;
	
	return length + 2;
}

static int decodeCOBS(uint8_t *buffer, uint8_t length) {
	uint8_t i;
	uint8_t sec_end;
	
	sec_end = buffer[0];
	for (i=0; i<length-2; i++) {
		if (i < sec_end-1) {
			buffer[i] = buffer[i+1];
			if (buffer[i] == 0x00) {
				return i;
			}
		} else {
			buffer[i] = 0x00;
			if (buffer[i+1] != 0x00) {
				sec_end += buffer[i+1];
			} else {
				return i;
			}
		}
	}
}

/**
 * Does simplistic verification of the packet. Returns 1 on success, 0 otherwise.
 * Also (optionally) gives back the opcode and pointer to payload. The pointer to payload
 * is set to NULL if there is no payload.
 */
static int ReadPacketHeader(uint8_t *buffer, uint8_t length, uint8_t *opcode, uint8_t **payload_ptr)
{
	if (!buffer || length < PKT_MIN_SIZE)
		return 0;

	length = decodeCOBS(buffer, length);
	
	if (buffer[PKT_HDR_INDEX] != PKT_HEADER_BYTE)
		return 0;

	if (buffer[length - 1] != PKT_END_BYTE)
		return 0;

	if (opcode)
		*opcode = buffer[PKT_OP_INDEX];

	if (payload_ptr)
		*payload_ptr = (length == PKT_MIN_SIZE) ? 0 : buffer + PKT_PAYLOAD_START_INDEX;
	return length;
}

// serialize functions //
static int CreateNoPayloadPacket(uint8_t *buffer, uint8_t length, uint8_t opcode)
{
	if (length < PKT_MIN_SIZE + 2)
		return 0;
	buffer[PKT_HDR_INDEX] = PKT_HEADER_BYTE;
	buffer[PKT_OP_INDEX] = opcode;
	buffer[PKT_PAYLOAD_SIZE_INDEX] = 0;
	buffer[PKT_PAYLOAD_START_INDEX] = PKT_END_BYTE;
	
	return encodeCOBS(buffer, PKT_MIN_SIZE);
}

static int CreateQueryHeartbeatPacket(uint8_t *buffer, uint8_t length)
{
	return CreateNoPayloadPacket(buffer, length, QUERY_HEARTBEAT_OPCODE);
}

static int CreateReportHeartbeatPacket(uint8_t *buffer, uint8_t length)
{
	return CreateNoPayloadPacket(buffer, length, REPORT_HEARTBEAT_OPCODE);
}

static int CreateSwitchModeAckPacket(uint8_t *buffer, uint8_t length)
{
    return CreateNoPayloadPacket(buffer, length, SWITCH_MODE_ACK_OPCODE);
}

static int CreateRemoteSwitchPacket(uint8_t *buffer, uint8_t length)
{
    return CreateNoPayloadPacket(buffer, length, REMOTE_SWITCH_OPCODE);
}

static int CreateAiInitPacket(uint8_t *buffer, uint8_t length)
{
    return CreateNoPayloadPacket(buffer, length, AI_INIT_OPCODE);
}

static int CreateQueryLocationPacket(uint8_t *buffer, uint8_t length)
{
    return CreateNoPayloadPacket(buffer, length, QUERY_LOCATION_OPCODE);
}

static int CreateQueryEncoderPacket(uint8_t *buffer, uint8_t length)
{
    return CreateNoPayloadPacket(buffer, length, QUERY_ENCODER_OPCODE);
}

static int CreateQueryCameraImagePacket(uint8_t *buffer, uint8_t length)
{
    return CreateNoPayloadPacket(buffer, length, QUERY_CAMERA_IMAGE_OPCODE);
}

static int CreateDrivePacket(uint8_t *buffer, uint8_t length, struct DrivePayload payload)
{
	if (length < PKT_MIN_SIZE + 2)
		return 0;
	buffer[PKT_HDR_INDEX] = PKT_HEADER_BYTE;
	buffer[PKT_OP_INDEX] = DRIVE_OPCODE;
	buffer[PKT_PAYLOAD_SIZE_INDEX] = 2;
	buffer[PKT_PAYLOAD_START_INDEX] = payload.left;
	buffer[PKT_PAYLOAD_START_INDEX + 1] = payload.right;
	buffer[PKT_PAYLOAD_START_INDEX + 2] = PKT_END_BYTE;
	
	return encodeCOBS(buffer, PKT_MIN_SIZE + 2);
}

static int CreateReportLocationPacket(uint8_t *buffer, uint8_t length, struct LocationPayload payload)
{
    if (length < PKT_MIN_SIZE + 3 * sizeof(int16_t) + 2)
        return 0;

    __CLAMP(payload.heading, 180, -180);

    buffer[PKT_HDR_INDEX] = PKT_HEADER_BYTE;
    buffer[PKT_OP_INDEX] = REPORT_LOCATION_OPCODE;
    buffer[PKT_PAYLOAD_SIZE_INDEX] = 3 * sizeof(int16_t);
    buffer[PKT_PAYLOAD_START_INDEX] = payload.x >> 8;
    buffer[PKT_PAYLOAD_START_INDEX + 1] = payload.x & 0xFF;
    buffer[PKT_PAYLOAD_START_INDEX + 2] = payload.y >> 8;
    buffer[PKT_PAYLOAD_START_INDEX + 3] = payload.y & 0xFF;
    buffer[PKT_PAYLOAD_START_INDEX + 4] = payload.heading >> 8;
    buffer[PKT_PAYLOAD_START_INDEX + 5] = payload.heading & 0xFF;
	
    return encodeCOBS(buffer, PKT_MIN_SIZE + 3 * sizeof(int16_t));
}

static int CreateSetLocationPacket(uint8_t *buffer, uint8_t length, struct LocationPayload payload)
{
    return CreateReportLocationPacket(buffer, length, payload);
}

static int CreateReportEncoderPacket(uint8_t *buffer, uint8_t length, struct EncoderPayload payload)
{
    if (length < PKT_MIN_SIZE + 6 * sizeof(int16_t) + 2)
        return 0;

    buffer[PKT_HDR_INDEX] = PKT_HEADER_BYTE;
    buffer[PKT_OP_INDEX] = REPORT_ENCODER_OPCODE;
    buffer[PKT_PAYLOAD_SIZE_INDEX] = 6 * sizeof(int16_t);
    buffer[PKT_PAYLOAD_START_INDEX + 0] = payload.front_left >> 8;
    buffer[PKT_PAYLOAD_START_INDEX + 1] = payload.front_left & 0xFF;

    buffer[PKT_PAYLOAD_START_INDEX + 2] = payload.front_right >> 8;
    buffer[PKT_PAYLOAD_START_INDEX + 3] = payload.front_right & 0xFF;

    buffer[PKT_PAYLOAD_START_INDEX + 4] = payload.back_left >> 8;
    buffer[PKT_PAYLOAD_START_INDEX + 5] = payload.back_left & 0xFF;

    buffer[PKT_PAYLOAD_START_INDEX + 6] = payload.back_right >> 8;
    buffer[PKT_PAYLOAD_START_INDEX + 7] = payload.back_right & 0xFF;

    buffer[PKT_PAYLOAD_START_INDEX + 8] = payload.left_actuator >> 8;
    buffer[PKT_PAYLOAD_START_INDEX + 9] = payload.left_actuator & 0xFF;

    buffer[PKT_PAYLOAD_START_INDEX + 10] = payload.right_actuator >> 8;
    buffer[PKT_PAYLOAD_START_INDEX + 11] = payload.right_actuator & 0xFF;

    return encodeCOBS(buffer, PKT_MIN_SIZE + 6 * sizeof(int16_t));
}

// deserialize functions - only for packets that have payloads //
// yes, there is an assumption that you won't feed a bad buffer //

static void ParseDrivePayload(uint8_t *payload, struct DrivePayload *drive)
{
	if (!payload || !drive)
		return;

	drive->left = payload[0];
	drive->right = payload[1];
}

static void ParseLocationPayload(uint8_t *payload, struct LocationPayload *loc)
{
    if (!payload || !loc)
        return;

    loc->x = (payload[0] << 8) | payload[1];
    loc->y = (payload[2] << 8) | payload[3];
    loc->heading = (payload[4] << 8) | payload[5];
}

static void ParseEncoderPayload(uint8_t *payload, struct EncoderPayload *enc)
{
    if (!payload || !enc)
        return;

    enc->front_left = (payload[0] << 8) | payload[1];
    enc->front_right = (payload[2] << 8) | payload[3];
    enc->back_left = (payload[4] << 8) | payload[5];
    enc->back_right = (payload[6] << 8) | payload[7];
    enc->left_actuator = (payload[8] << 8) | payload[9];
    enc->right_actuator = (payload[10] << 8) | payload[11];
}

#ifdef _cplusplus
}
#endif

#undef __CLAMP
#endif
