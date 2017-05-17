#include "badgerRMCRTOS.h"
#include "launchPadHwAbstraction.h"
#include "LMCterminal.h"
#include "global_state.h"

#define clamp(var,ceil,floor) \
	do { \
		if (var > ceil) { \
			var = ceil; \
		} else if (var < floor) { \
			var = floor; \
		} \
	} while(0)

#define scale(var,oldRange,oldBase,newRange,newBase) \
	((((var)-(oldBase))*(newRange)) + ((newBase)*(oldRange)))/(oldRange)

#define STOP_PULSE_WIDTH (3*ONE_MS_PULSE_WIDTH/2)
#define PWM_RANGE ONE_MS_PULSE_WIDTH
#define PWM_BASE (STOP_PULSE_WIDTH-PWM_RANGE/2)

typedef volatile struct {
	int32_t kp;
	int32_t ki;
	int32_t kd;
	int32_t kRange;
	int32_t lastErr;
	int32_t sumErr;
	int32_t sumErrCeil;
	int32_t sumErrFloor;
	int32_t deadZone;
} pidParams_t;

pidParams_t slowLoopLeftParams;
pidParams_t slowLoopRightParams;
pidParams_t fastLoopParams;

volatile int32_t measuredLeftSpd;
volatile int32_t measuredRightSpd;

int32_t pidLoop(pidParams_t *params, int32_t measured, int32_t target) {
	int32_t deltaCtrlVar;
	int32_t currErr;
	
	currErr = target - measured;
	if (currErr < params->deadZone && currErr > -params->deadZone) currErr = 0;
	params->sumErr += currErr;
	
	clamp(
		params->sumErr,
		params->sumErrCeil,
		params->sumErrFloor
	);
	
	deltaCtrlVar = (params->kp*currErr + params->ki*params->sumErr + params->kd*(currErr-params->lastErr)) / params->kRange;
	
	clamp(
		deltaCtrlVar,
		QEI_RANGE,
		-QEI_RANGE
	);
	
	params->lastErr = currErr;
	
	return deltaCtrlVar;
}

int runLoop(eInterrupt intType, uint32_t devMask) {
	int32_t leftTarget;
	int32_t rightTarget;
	int32_t deltaCtrlVar;
	
	if (wheelActual.left < STOP_PULSE_WIDTH) {
		wheelQEI.leftSpd = -getQEISpeed(qei0);
	} else {
		wheelQEI.leftSpd = getQEISpeed(qei0);
	}
	leftTarget = scale(
		wheelPWM.left,
		PWM_RANGE,
		PWM_BASE,
		2*QEI_RANGE,
		-QEI_RANGE
	);
	deltaCtrlVar = pidLoop(&slowLoopLeftParams, wheelQEI.leftSpd, leftTarget);
	wheelActual.left += scale(
		deltaCtrlVar,
		2*QEI_RANGE,
		0,
		PWM_RANGE,
		0
	);
	if (wheelActual.right < STOP_PULSE_WIDTH) {
		wheelQEI.rightSpd = -getQEISpeed(qei1);
	} else {
		wheelQEI.rightSpd = getQEISpeed(qei1);
	}
	rightTarget = scale(
		wheelPWM.right,
		PWM_RANGE,
		PWM_BASE,
		2*QEI_RANGE,
		-QEI_RANGE
	);
	deltaCtrlVar = pidLoop(&slowLoopRightParams, wheelQEI.rightSpd, rightTarget);
	wheelActual.right += scale(
		deltaCtrlVar,
		2*QEI_RANGE,
		0,
		PWM_RANGE,
		0
	);
	
	clamp(
		wheelActual.left,
		STOP_PULSE_WIDTH + 15*PWM_RANGE/20,
		STOP_PULSE_WIDTH - 15*PWM_RANGE/20
	);
	clamp(
		wheelActual.right,
		STOP_PULSE_WIDTH + 15*PWM_RANGE/20,
		STOP_PULSE_WIDTH - 15*PWM_RANGE/20
	);
	
	setPWM(pwm0, pwm_gen0, wheelActual.left);
	setPWM(pwm0, pwm_gen1, wheelActual.right);
	//setPWM(pwm0, pwm_gen0, wheelPWM.left);
	//setPWM(pwm0, pwm_gen1, wheelPWM.right);
	
	return 0;
}

int setWheels(uint32_t arg) {
	setPWM(pwm0, pwm_gen0, wheelPWM.left);
	setPWM(pwm0, pwm_gen1, wheelPWM.right);
}

int ctrlLoop(uint32_t arg) {
	interruptCallbackRegister(Quadrature, runLoop, 0);
	interruptCallbackRegister(Quadrature, runLoop, 1);
	
	//timerCallbackRegister(10, setWheels);
	
	slowLoopLeftParams.kp = 200;
	slowLoopLeftParams.ki = 1;
	slowLoopLeftParams.kd = 1;
	slowLoopLeftParams.kRange = 1000;
	slowLoopLeftParams.lastErr = 0;
	slowLoopLeftParams.sumErr = 0;
	slowLoopLeftParams.sumErrCeil = 100*QEI_RANGE;
	slowLoopLeftParams.sumErrFloor = -100*QEI_RANGE;
	slowLoopLeftParams.deadZone = scale(
		ONE_MS_PULSE_WIDTH+1,
		PWM_RANGE,
		PWM_BASE,
		2*QEI_RANGE,
		-QEI_RANGE
	);
	
	slowLoopRightParams.kp = 200;
	slowLoopRightParams.ki = 5;
	slowLoopRightParams.kd = 10;
	slowLoopRightParams.kRange = 1000;
	slowLoopRightParams.lastErr = 0;
	slowLoopRightParams.sumErr = 0;
	slowLoopRightParams.sumErrCeil = 10*QEI_RANGE;
	slowLoopRightParams.sumErrFloor = -10*QEI_RANGE;
	slowLoopRightParams.deadZone = scale(
		ONE_MS_PULSE_WIDTH+1,
		PWM_RANGE,
		PWM_BASE,
		2*QEI_RANGE,
		-QEI_RANGE
	);
	
	wheelActual.left = STOP_PULSE_WIDTH;
	wheelActual.right = STOP_PULSE_WIDTH;
	
	setPWM(pwm0, pwm_gen0, STOP_PULSE_WIDTH);
	setPWM(pwm0, pwm_gen1, STOP_PULSE_WIDTH);
	
	return 0;
}


