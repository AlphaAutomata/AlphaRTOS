#include "badgerRMCRTOS.h"
#include "launchPadHwAbstraction.h"
#include "LMCterminal.h"

struct speeds {
	uint32_t left;
	uint32_t right;
};

struct speeds *sharedSpeeds;

int SetWheels(uint32_t arg){
	setPWM(pwm0, pwm_gen0, sharedSpeeds->left);
	setPWM(pwm0, pwm_gen1, sharedSpeeds->right);
	return 0;
}

int ctrlLoop(uint32_t arg) {
	sharedSpeeds = (struct speeds *)arg;
	timerCallbackRegister(3, SetWheels);
	return 0;
}
