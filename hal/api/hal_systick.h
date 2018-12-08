#ifndef HAL_SYSTICK_H
#define HAL_SYSTICK_H

#include "hal_timer_types.h"

#ifdef __cplusplus
extern "C" {
#endif // #ifdef __cplusplus

/**
 * \brief Initialize the System Tick timer.
 *
 * \param [in] sysTickHandler  Pointer to the System Tick interrupt handler.
 * \param [in] sysTickInterval System tick interval.
 */
void hal_systick_init(hal_timer_isr_t sysTickHandler, int sysTickInterval);

#ifdef __cplusplus
}
#endif // #ifdef __cplusplus

#endif // #ifndef HAL_SYSTICK_H
