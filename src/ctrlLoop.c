#include "badgerRMCRTOS.h"
#include "launchPadHwAbstraction.h"
#include "LMCterminal.h"
#include "global_state.h"

int SetWheels(uint32_t arg){
	setPWM(pwm0, pwm_gen0, wheelPWM.left);
	setPWM(pwm0, pwm_gen1, wheelPWM.right);
	return 0;
}

int ctrlLoop(uint32_t arg) {
	timerCallbackRegister(3, SetWheels);
	return 0;
}
