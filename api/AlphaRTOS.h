#include <stdint.h>
#include <stdbool.h>
#include <stdnoreturn.h>

#include "artos_event_types.h"
#include "artos_service_types.h"

#ifndef ALPHA_RTOS_H
#define ALPHA_RTOS_H

////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////// AlphaRTOS API Common Types /////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////

/**
 * \brief Alpha RTOS API status codes.
 */
typedef enum ARTOS_eStatus_ {
	ARTOS_eStatus_OK          =   0, //!< Operation completed successfully.
	ARTOS_eStatus_BAD_ARGS    =   1, //!< Invalid arguments to RTOS API function.
	ARTOS_eStatus_NO_RSRC     =   2, //!< Not enough resources to complete the requested operation.
	ARTOS_eStatus_UNSUPPORTED =   3, //!< The requested operation is not supported.
	ARTOS_eStatus_OVERRUN     =   4, //!< An overflow or overrun occurred.
	ARTOS_eStatus_GENERIC_ERR = 255  //!< Operation encountered an error.
} ARTOS_eStatus;

#ifdef __cplusplus
extern "C" {
#endif // #ifdef __cplusplus

/**
 * \brief Alpha RTOS task entry point.
 *
 * \param      argc The number of elements in the arguments vector \a argv.
 * \param [in] argv A vector of C-string arguments. Arguments usage is task-defined.
 *
 * \retval ::ARTOS_eStatus_OK          Task completed with no errors.
 * \retval ::ARTOS_eStatus_GENERIC_ERR Task did not exit properly.
 */
typedef ARTOS_eStatus (*pFn_taskMain)(int argc, char** argv);

////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////// AlphaRTOS Common API ///////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////

/**
 * \brief Get the number of system ticks the RTOS has been running for.
 *
 * \param [out] uptime The number of system ticks since startup will be written here.
 *
 * \retval ::ARTOS_eStatus_OK       Successfully retrieved the number of system ticks since startup.
 * \retval ::ARTOS_eStatus_BAD_ARGS \a uptime is `NULL`.
 * \retval ::ARTOS_eStatus_OVERRUN  The system tick counter has overflowed since the last time this
 *                                  function has been called.
 */
ARTOS_eStatus ARTOS_getUptime(unsigned int* uptime);

/**
 * \brief Start the RTOS.
 */
noreturn void ARTOS_start(void);

////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////// Timer-Based Services API /////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////

/**
 * \brief Register a periodic service.
 *
 * The periodic handler will be automatically invoked every \a interval number of system ticks.
 *
 * \param [out] handlerID The unique ID assigned to this handler is written here.
 * \param       interval  The system tick interval at which the handler function is called.
 * \param       handler   The handler to invoke every given periodic interval.
 * \param [in]  arg       The argument to pass to the handler.
 *
 * \retval ::ARTOS_eStatus_OK       Handler successfully registered.
 * \retval ::ARTOS_eStatus_BAD_ARGS \a handlerID is `NULL`.
 * \retval ::ARTOS_eStatus_NO_RSRC  The maximum number of periodic services have already been
 *                                  registered.
 */
ARTOS_eStatus ARTOS_timedService_register(
	int*               handlerID,
	unsigned int       interval,
	pFn_serviceHandler handler,
	void*              arg
);

/**
 * \brief Unregister a periodic service.
 *
 * \param handlerID The ID assigned by ::ARTOS_timedService_register() when the handler was first
 *                  registered.
 *
 * \retval ::ARTOS_eStatus_OK       The specified handler was successfully unregistered.
 * \retval ::ARTOS_eStatus_BAD_ARGS No handler with the specified ID was found.
 */
ARTOS_eStatus ARTOS_timedService_unregister(int handlerID);

/**
 * \brief Set the periodic interval between successive invocations of a periodic service handler.
 *
 * The new interval takes effect after the handler is next invoked. To force the handler to be
 * invoked immediately, use ::ARTOS_timedService_intervalSync().
 *
 * \param handlerID The ID returned by ::ARTOS_timedService_register() when the handler was first
 *                  registered.
 * \param interval  The system tick interval at which the handler should be invoked.
 *
 * \retval ::ARTOS_eStatus_OK       The specified handler's invocation interval has been updated.
 * \retval ::ARTOS_eStatus_BAD_ARGS No handler with the specified ID was found.
 */
ARTOS_eStatus ARTOS_timedService_intervalSet(int handlerID, unsigned int interval);

/**
 * \brief Get the periodic interval between successive invocations of a service handler.
 *
 * \param       handlerID The ID returned by ::ARTOS_timedService_register() when the handler was
 *                        first registered.
 * \param [out] interval  The number of system tick interval at which the handler is invoked is
 *                        written here.
 *
 * \retval ::ARTOS_eStatus_OK       The specified handler's invocation interval has been retrieved.
 * \retval ::ARTOS_eStatus_BAD_ARGS No handler with the specified ID was found.
 */
ARTOS_eStatus ARTOS_timedService_intervalGet(int handlerID, unsigned int* interval);

/**
 * \brief Synchronize the specified periodic service invocation interval to the current system tick.
 *
 * Causes the specified periodic callback's interval timer to be set such that it is called at the
 * `(current system tick) + (offset)` system tick. Calling this function on a number of periodic
 * callbacks in the same system tick synchronizes their service intervals with each other.
 *
 * \param handlerID The ID returned by ::ARTOS_timedService_register() when the handler was first
 *                  registered.
 * \param offset    The number of system ticks to wait before invoking the specified handler.
 */
ARTOS_eStatus ARTOS_timedService_intervalSync(int handlerID, unsigned int offset);

////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////// Event-Based Services API /////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////

/**
 * \brief Register an event service handler.
 *
 * \param [out] handlerID The unique ID assigned to this event service handler will be written here.
 * \param [in]  event     Specification of the event to register a handler for.
 * \param       handler   The handler function to invoke on event occurrence.
 * \param [in]  arg       The argument to pass to the event handler.
 *
 * \retval ::ARTOS_eStatus_OK          Successfully registered an event service handler.
 * \retval ::ARTOS_eStatus_BAD_ARGS    \a handlerID, \a event, or \a handler is `NULL`.
 * \retval ::ARTOS_eStatus_UNSUPPORTED Event service handlers for the requested event are not
 *                                     supported.
 */
ARTOS_eStatus ARTOS_eventService_register(
	int*               handlerID,
	ARTOS_Event*       event,
	pFn_serviceHandler handler,
	void*              arg
);

/**
 * \brief Unregister an event service.
 *
 * \param handlerID The ID assigned by ::ARTOS_eventService_register() when the handler was first
 *                  registered.
 *
 * \retval ::ARTOS_eStatus_OK       The specified handler was successfully unregistered.
 * \retval ::ARTOS_eStatus_BAD_ARGS No handler with the specified ID was found.
 */
ARTOS_eStatus ARTOS_eventService_unregister(int handlerID);

////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////// Task API /////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////

/**
 * \brief Register a task.
 * 
 * \param [out] taskID   The unique ID assigned to this task will be written here.
 * \param       taskMain The task entry point.
 * 
 * \retval ::ARTOS_eStatus_OK       Successfully registered the task.
 * \retval ::ARTOS_eStatus_BAD_ARGS \a taskID or \a taskMain is `NULL`.
 * \retval ::ARTOS_eStatus_NO_RSRC  Insufficient resources to register another task.
 */
ARTOS_eStatus ARTOS_task_register(int* taskID, pFn_taskMain taskMain);

/**
 * \brief Execute the specified task.
 * 
 * \param      taskID The ID, assigned by ::ARTOS_task_register(), of the task to execute.
 * \param      argc   The number of elements in the arguments vector \a argv.
 * \param [in] argv   A vector of C-string arguments to pass to the task.
 * 
 * \retval ::ARTOS_eStatus_OK       Successfully started task execution.
 * \retval ::ARTOS_eStatus_BAD_ARGS The task with the given ID was not found.
 * \retval ::ARTOS_eStatus_NO_RSRC  Insufficient resources to execute another task.
 */
ARTOS_eStatus ARTOS_task_exec(int taskID, int argc, char** argv);

/**
 * \brief Kill the specified task.
 * 
 * \param taskID The ID, assigned by ::ARTOS_task_register(), of the task to kill.
 * 
 * \retval ::ARTOS_eStatus_OK       Successfully killed the specified task.
 * \retval ::ARTOS_eStatus_BAD_ARGS The task with the given ID was not found.
 */
ARTOS_eStatus ARTOS_task_kill(int taskID);

/**
 * \brief Yield the processor for the remainder of the scheduling cycle.
 *
 * \retval ::ARTOS_eStatus_OK Always.
 */
ARTOS_eStatus ARTOS_task_yield(void);

/**
 * \brief Sleep for the specified number of system ticks.
 *
 * \param time The number of system ticks to sleep for.
 *
 * \return ::ARTOS_eStatus_OK Always.
 */
ARTOS_eStatus ARTOS_task_sleep(unsigned int time);

#ifdef __cplusplus
}
#endif // #ifdef __cplusplus

#endif // #ifndef ALPHA_RTOS_H
