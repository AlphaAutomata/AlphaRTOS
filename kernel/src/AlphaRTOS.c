#include "AlphaRTOS.h"

ARTOS_eStatus ARTOS_timedService_register(
	int*             handlerID,
	unsigned int     interval,
	pFn_taskFunction handler,
	void*            arg
) {
    return ARTOS_eStatus_UNSUPPORTED;
}

ARTOS_eStatus ARTOS_timedService_unregister(int handlerID) {
    return ARTOS_eStatus_UNSUPPORTED;
}

ARTOS_eStatus ARTOS_timedService_intervalSet(int handlerID, unsigned int interval) {
    return ARTOS_eStatus_UNSUPPORTED;
}

ARTOS_eStatus ARTOS_timedService_intervalGet(int handlerID, unsigned int* interval) {
    return ARTOS_eStatus_UNSUPPORTED;
}

ARTOS_eStatus ARTOS_timedService_intervalSync(int handlerID, unsigned int offset) {
    return ARTOS_eStatus_UNSUPPORTED;
}

ARTOS_eStatus ARTOS_eventService_register(
	int*             handlerID,
	ARTOS_Event*     event,
	pFn_taskFunction handler,
	void*            arg
) {
    return ARTOS_eStatus_UNSUPPORTED;
}

ARTOS_eStatus ARTOS_eventService_unregister(int handlerID) {
    return ARTOS_eStatus_UNSUPPORTED;
}

ARTOS_eStatus ARTOS_task_yield(void) {
    return ARTOS_eStatus_UNSUPPORTED;
}

ARTOS_eStatus ARTOS_task_sleep(unsigned int time) {
    return ARTOS_eStatus_UNSUPPORTED;
}

ARTOS_eStatus ARTOS_getUptime(unsigned int* uptime) {
    return ARTOS_eStatus_UNSUPPORTED;
}

noreturn void ARTOS_start(void) {
    while (1) {
        // spin
    }
}
