#ifndef _GLOBAL_STATE_H_
#define _GLOBAL_STATE_H_

#include <stdint.h>

typedef volatile struct {
	int32_t left;
	int32_t right;
} wheelPWM_t;

typedef volatile struct {
	int32_t leftSpd;
	int32_t rightSpd;
	int leftDir;
	int rightDir;
	int32_t leftPos;
	int32_t rightPos;
} wheelQEI_t;

#define QEI_RANGE 400 // Upper bound of QEI data range

extern wheelPWM_t wheelPWM;
extern wheelPWM_t wheelIntermediate;
extern wheelPWM_t wheelActual;
extern wheelQEI_t wheelQEI;

#endif
