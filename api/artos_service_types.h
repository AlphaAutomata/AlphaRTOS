#ifndef ARTOS_SERVICE_TYPES_H
#define ARTOS_SERVICE_TYPES_H

#ifdef __cplusplus
extern "C" {
#endif // #ifdef __cplusplus

/**
 * \brief Alpha RTOS generic handler.
 *
 * \param [in,out] arg Usage-defined argument.
 *
 * \retval ::ARTOS_eStatus_OK          Function completed with no errors.
 * \retval ::ARTOS_eStatus_GENERIC_ERR Function did not exit properly.
 */
typedef void (*pFn_serviceHandler)(void* arg);

#ifdef __cplusplus
}
#endif // #ifdef __cplusplus

#endif // #ifndef ARTOS_SERVICE_TYPES_H
