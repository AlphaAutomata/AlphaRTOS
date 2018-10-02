#include "AlphaRTOS.h"

#include "scheduler.h"

#include "api_contract.h"

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

ARTOS_eStatus ARTOS_task_register(
	ARTOS_hTask_t*     pHandle,
	ARTOS_pFn_taskMain taskMain,
	const char*        taskName
) {
    intptr_t* frame;

    CONTRACT_VERIFY((pHandle != NULL) && (taskMain != NULL) && (taskName != NULL));

    frame = (intptr_t*)pHandle;
    task_register(frame, taskMain, taskName);
    if (frame == NULL) {
        return ARTOS_eStatus_NO_RSRC;
    }

    return ARTOS_eStatus_OK;
}

ARTOS_eStatus ARTOS_task_exec(ARTOS_hTask_t handle, int argc, char** argv) {
    intptr_t frame;

    CONTRACT_VERIFY(task_handleValid((intptr_t)handle));

    frame = (intptr_t)handle;
    task_exec(frame, argc, argv);

    return ARTOS_eStatus_OK;
}

ARTOS_eStatus ARTOS_task_kill(ARTOS_hTask_t handle) {
    intptr_t frame;

    CONTRACT_VERIFY(task_handleValid((intptr_t)handle));

    frame = (intptr_t)handle;
    task_kill(frame);

    return ARTOS_eStatus_OK;
}

ARTOS_eStatus ARTOS_task_getHandle(ARTOS_hTask_t* pHandle) {
    intptr_t* frame;

    CONTRACT_VERIFY(pHandle != NULL);

    frame = (intptr_t*)pHandle;
	task_getHandle(frame);

    return ARTOS_eStatus_UNSUPPORTED;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////// Thread API ////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////

ARTOS_eStatus ARTOS_thread_create(
	ARTOS_hThread_t*      pHandle,
	ARTOS_thread_attr_t*  attributes,
	ARTOS_pFn_threadEntry threadEntry,
	void*                 arg
) {
    return ARTOS_eStatus_UNSUPPORTED;
}

ARTOS_eStatus ARTOS_thread_join(ARTOS_hThread_t thread) {
    return ARTOS_eStatus_UNSUPPORTED;
}

ARTOS_eStatus ARTOS_thread_yield(void) {
    return ARTOS_eStatus_UNSUPPORTED;
}

ARTOS_eStatus ARTOS_thread_sleep(unsigned int time) {
    return ARTOS_eStatus_UNSUPPORTED;
}

ARTOS_eStatus ARTOS_thread_getHandle(ARTOS_hThread_t* pHandle) {
    return ARTOS_eStatus_UNSUPPORTED;
}
