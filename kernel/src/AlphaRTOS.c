#include "AlphaRTOS.h"

#include "task.h"

////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////// AlphaRTOS Common API ///////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////

ARTOS_eStatus ARTOS_getUptime(unsigned int* uptime) {
    return ARTOS_eStatus_UNSUPPORTED;
}

noreturn void ARTOS_start(void) {
    while (1) {
        // spin
    }
}

////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////// Timer-Based Services API /////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////

ARTOS_eStatus ARTOS_timedService_register(
	int*               handlerID,
	unsigned int       interval,
	pFn_serviceHandler handler,
	void*              arg
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

////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////// Event-Based Services API /////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////

ARTOS_eStatus ARTOS_eventService_register(
	int*               handlerID,
	ARTOS_Event*       event,
	pFn_serviceHandler handler,
	void*              arg
) {
    return ARTOS_eStatus_UNSUPPORTED;
}

ARTOS_eStatus ARTOS_eventService_unregister(int handlerID) {
    return ARTOS_eStatus_UNSUPPORTED;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////// Task API /////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////

ARTOS_eStatus ARTOS_task_register(int* taskID, pFn_taskMain taskMain, const char* taskName) {
    int temp_id;

    if (taskID == NULL || taskMain == NULL) {
        return ARTOS_eStatus_BAD_ARGS;
    }

    temp_id = addTask(taskMain, taskName);
    if (temp_id == 0) {
        return ARTOS_eStatus_NO_RSRC;
    }

    return ARTOS_eStatus_OK;
}

ARTOS_eStatus ARTOS_task_exec(int taskID, int argc, char** argv) {
    return ARTOS_eStatus_UNSUPPORTED;
}

ARTOS_eStatus ARTOS_task_kill(int taskID) {
    return ARTOS_eStatus_UNSUPPORTED;
}

ARTOS_eStatus ARTOS_task_yield(void) {
    return ARTOS_eStatus_UNSUPPORTED;
}

ARTOS_eStatus ARTOS_task_sleep(unsigned int time) {
    return ARTOS_eStatus_UNSUPPORTED;
}

ARTOS_eStatus ARTOS_task_getID(int* taskID) {
    return ARTOS_eStatus_UNSUPPORTED;
}
