#include "badgerRMCRTOS.h"
#include "launchPadHwAbstraction.h"
#include "LMCterminal.h"
#include "global_state.h"
#define QEI_RANGE 1500 // Upper bound of QEI data range (0 - 1500)
#define KP 87 //Proportional constant for PID contriller
#define KI 56	//Integration constant for PID controller
#define KD 22	//Differential constant for PID controller


int32_t curLeftError = 0;
int32_t integLeftError = 0;
int32_t difLeftError = 0;
int32_t curRightError = 0;
int32_t integRightError = 0;
int32_t difRightError = 0;

int32_t inputLeftPWM = 3*ONE_MS_PULSE_WIDTH/2;// input at speed of 0
int32_t inputRightPWM = 3*ONE_MS_PULSE_WIDTH/2;// input at speed of 0


int SetWheels(uint32_t arg) {
	wheelQEI.leftSpd = getQEISpeed(qei0);
	wheelQEI.rightSpd = getQEISpeed(qei1);
	wheelQEI.leftDir = getQEIDirection(qei0);
	wheelQEI.rightDir = getQEIDirection(qei1);
	
	int32_t measuredLeftSpd;
	int32_t measuredRightSpd;
	int32_t targetLeftSpd;
	int32_t targetRightSpd;
	
	targetLeftSpd = wheelPWM.left;
	targetRightSpd = wheelPWM.right;
	
	// scaled measuredSpeeds based on QEI range
	if (wheelQEI.leftDir > 0){
		measuredLeftSpd = 3*ONE_MS_PULSE_WIDTH/2 + wheelQEI.leftSpd*ONE_MS_PULSE_WIDTH/(QEI_RANGE*2);
	} else {
		measuredLeftSpd = 3*ONE_MS_PULSE_WIDTH/2 - wheelQEI.leftSpd*ONE_MS_PULSE_WIDTH/(QEI_RANGE*2);
	}
	
	if (wheelQEI.rightDir > 0){
		measuredRightSpd = 3*ONE_MS_PULSE_WIDTH/2 + wheelQEI.rightSpd*ONE_MS_PULSE_WIDTH/(QEI_RANGE*2);
	} else {
		measuredRightSpd = 3*ONE_MS_PULSE_WIDTH/2 - wheelQEI.rightSpd*ONE_MS_PULSE_WIDTH/(QEI_RANGE*2);
	}

	
	//calculates the new PWM method input for left wheel; PID controller
	if (targetLeftSpd == measuredLeftSpd) {
		difLeftError = 0;
		curLeftError = 0;
		integLeftError = 0;
		setPWM(pwm0, pwm_gen0, inputLeftPWM);
	}// reset erros once actual left wheel speed matches the target speed
	difLeftError = ((targetLeftSpd - measuredLeftSpd) - curLeftError);
	curLeftError = targetLeftSpd - measuredLeftSpd;
	integLeftError += curLeftError;
	int32_t leftDifference = (KP*curLeftError + KI*integLeftError + KD*difLeftError)/100;
	inputLeftPWM += leftDifference;
	
	//calculates the new PWM method input for right wheel; PID controller
	if (targetRightSpd == measuredRightSpd) {
		difRightError = 0;
		curRightError = 0;
		integRightError = 0;
		setPWM(pwm0, pwm_gen1, inputRightPWM);
	}// reset erros once actual right wheel speed matches the target speed
	difRightError = ((targetRightSpd - measuredRightSpd) - curRightError);
	curRightError = targetRightSpd - measuredRightSpd;
	integRightError += curRightError;
	int32_t rightDifference = (KP*curRightError + KI*integRightError + KD*difRightError)/100;
	inputRightPWM += rightDifference;

	setPWM(pwm0, pwm_gen0, inputLeftPWM);
	setPWM(pwm0, pwm_gen1, inputRightPWM);
	
	return 0;
}

int ctrlLoop(uint32_t arg) {
	timerCallbackRegister(3, SetWheels);
	setPWM(pwm0, pwm_gen0, inputLeftPWM);
	setPWM(pwm0, pwm_gen1, inputRightPWM);
	return 0;
}


