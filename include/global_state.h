#ifndef _GLOBAL_STATE_H_
#define _GLOBAL_STATE_H_

#include <stdint.h>

typedef volatile struct {
	uint32_t left;
	uint32_t right;
} wheelPWM_t;

extern wheelPWM_t wheelPWM;
extern volatile uint8_t lastOp;

#endif
