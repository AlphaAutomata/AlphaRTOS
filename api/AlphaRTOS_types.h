#include "artos_event_types.h"
#include "artos_service_types.h"

#ifndef ALPHARTOS_TYPES_H
#define ALPHARTOS_TYPES_H

#ifdef __cplusplus
extern "C" {
#endif // #ifdef __cplusplus

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

#ifdef __cplusplus
}
#endif // #ifdef __cplusplus

#endif // #ifndef ALPHARTOS_TYPES_H
