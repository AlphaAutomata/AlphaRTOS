#ifndef _GLOBAL_STATE_H_
#define _GLOBAL_STATE_H_

#include <stdint.h>

typedef volatile struct {
	uint32_t left;
	uint32_t right;
} wheelPWM_t;

typedef volatile struct {
	uint32_t leftSpd;
	uint32_t rightSpd;
	int leftDir;
	int rightDir;
	int32_t leftPos;
	int32_t rightPos;
} wheelQEI_t;

extern wheelPWM_t wheelPWM;
extern wheelQEI_t wheelQEI;

#endif
