#ifndef ARTOS_SERVICE_TYPES_H
#define ARTOS_SERVICE_TYPES_H

/**
 * \brief Alpha RTOS generic handler.
 *
 * \param [in,out] arg Usage-defined argument.
 *
 * \retval ::ARTOS_eStatus_OK          Function completed with no errors.
 * \retval ::ARTOS_eStatus_GENERIC_ERR Function did not exit properly.
 */
typedef ARTOS_eStatus (*pFn_serviceHandler)(void* arg);

#endif // #ifndef ARTOS_SERVICE_TYPES_H
