#ifndef ALPHA_RTOS_H
#define ALPHA_RTOS_H

#include <stdint.h>
#include <stdbool.h>

#include "event_types.h"

#ifdef __cplusplus
extern "C" {
#endif // #ifdef __cplusplus

/**
 * \brief Alpha RTOS API status codes.
 */
typedef enum {
	ARTOS_eStatus_OK          =   0, //!< Operation completed successfully.
	ARTOS_eStatus_BAD_ARGS    =   1, //!< Invalid arguments to RTOS API function.
	ARTOS_eStatus_NO_RSRC     =   2, //!< Not enough resources to complete the requested operation.
	ARTOS_eStatus_UNSUPPORTED =   3, //!< The requested operation is not supported.
	ARTOS_eStatus_OVERRUN     =   4, //!< An overflow or overrun occurred.
	ARTOS_eStatus_GENERIC_ERR = 255  //!< Operation encountered an error.
} ARTOS_eStatus;

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

/**
 * \brief Alpha RTOS generic handler.
 *
 * \param [in,out] arg Usage-defined argument.
 *
 * \retval ::ARTOS_eStatus_OK          Function completed with no errors.
 * \retval ::ARTOS_eStatus_GENERIC_ERR Function did not exit properly.
 */
typedef ARTOS_eStatus (*pFn_taskFunction)(void* arg);

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
ARTOS_eStatus ARTOS_timedServiceRegister(
	int*             handlerID,
	unsigned int     interval,
	pFn_taskFunction handler,
	void*            arg
);

/**
 * \brief Unregister a periodic service.
 *
 * \param handlerID The ID assigned by ::ARTOS_timedServiceRegister() when the handler was first
 *                  registered.
 *
 * \retval ::ARTOS_eStatus_OK       The specified handler was successfully unregistered.
 * \retval ::ARTOS_eStatus_BAD_ARGS No handler with the specified ID was found.
 */
ARTOS_eStatus ARTOS_timedServiceUnregister(int handlerID);

/**
 * \brief Set the periodic interval between successive invocations of a periodic service handler.
 *
 * The new interval takes effect after the handler is next invoked. To force the handler to be
 * invoked immediately, use ::ARTOS_timedServiceIntervalSync().
 *
 * \param handlerID The ID returned by ::ARTOS_timedServiceRegister() when the handler was first
 *                  registered.
 * \param interval  The system tick interval at which the handler should be invoked.
 *
 * \retval ::ARTOS_eStatus_OK       The specified handler's invocation interval has been updated.
 * \retval ::ARTOS_eStatus_BAD_ARGS No handler with the specified ID was found.
 */
ARTOS_eStatus ARTOS_timedServiceIntervalSet(int handlerID, unsigned int interval);

/**
 * \brief Get the periodic interval between successive invocations of a service handler.
 *
 * \param       handlerID The ID returned by ::ARTOS_timedServiceRegister() when the handler was
 *                        first registered.
 * \param [out] interval  The number of system tick interval at which the handler is invoked is
 *                        written here.
 *
 * \retval ::ARTOS_eStatus_OK       The specified handler's invocation interval has been retrieved.
 * \retval ::ARTOS_eStatus_BAD_ARGS No handler with the specified ID was found.
 */
ARTOS_eStatus ARTOS_timedServiceIntervalGet(int handlerID, unsigned int* interval);

/**
 * \brief Synchronize the specified periodic service invocation interval to the current system tick.
 *
 * Causes the specified periodic callback's interval timer to be set such that it is called at the
 * `(current system tick) + (offset)` system tick. Calling this function on a number of periodic
 * callbacks in the same system tick synchronizes their service intervals with each other.
 *
 * \param handlerID The ID returned by ::ARTOS_timedServiceRegister() when the handler was first
 *                  registered.
 * \param offset    The number of system ticks to wait before invoking the specified handler.
 */
ARTOS_eStatus ARTOS_timedServiceIntervalSync(int handlerID, unsigned int offset);

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
ARTOS_eStatus ARTOS_eventServiceRegister(
	int*             handlerID,
	ARTOS_Event*     event,
	pFn_taskFunction handler,
	void*            arg
);

/**
 * \brief Unregister an event service.
 *
 * \param handlerID The ID assigned by ::ARTOS_eventServiceRegister() when the handler was first
 *                  registered.
 *
 * \retval ::ARTOS_eStatus_OK       The specified handler was successfully unregistered.
 * \retval ::ARTOS_eStatus_BAD_ARGS No handler with the specified ID was found.
 */
ARTOS_eStatus ARTOS_eventServiceUnregister(int handlerID);

/**
 * \brief Yield the processor for the remainder of the scheduling cycle.
 *
 * \retval ::ARTOS_eStatus_OK Always.
 */
ARTOS_eStatus ARTOS_taskYield(void);

/**
 * \brief Sleep for the specified number of system ticks.
 *
 * \param time The number of system ticks to sleep for.
 *
 * \return ::ARTOS_eStatus_OK Always.
 */
ARTOS_eStatus ARTOS_taskSleep(unsigned int time);

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

#ifdef __cplusplus
}
#endif // #ifdef __cplusplus

#endif // #ifndef ALPHA_RTOS_H
