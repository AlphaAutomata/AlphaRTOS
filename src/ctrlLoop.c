#include "badgerRMCRTOS.h"
#include "launchPadHwAbstraction.h"
#include "LMCterminal.h"
#include "global_state.h"
#include "qei.h"

int SetWheels(uint32_t arg){
	int wheelSpeed;
	int direction;

	
	setPWM(pwm0, pwm_gen0, wheelPWM.left);
	setPWM(pwm0, pwm_gen1, wheelPWM.right);
	wheelSpeed = getWheelSpeed();
	direction = QEIDirectionGet(QEI0_BASE);
	kprintf("speed = %d direction = %d\n", wheelSpeed, direction);
	
	return 0;
}

int ctrlLoop(uint32_t arg) {
	timerCallbackRegister(3, SetWheels);
	return 0;
}
