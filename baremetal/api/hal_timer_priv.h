#ifndef HAL_TIMER_PRIV
#define HAL_TIMER_PRIV

#include "hal_timer_types.h"

#ifdef __cplusplus
extern "C" {
#endif // #ifdef __cplusplus

/**
 * \brief Timer configuration information.
 */
typedef struct hal_timerPriv_info_ {
    int             loadValue;         //!< The value to load the timer with, in timer clock ticks.
    hal_timer_rpt_t rpt;               //!< The timer automatic reload configuration.
    bool            start_immediately; //!< Flag to start the timer immediately when configuration
                                       //!< completes.
} hal_timerPriv_info;

/**
 * \brief Initialize a CPU-private hardware timer.
 *
 * Each platform vendor provides access to their timers and interrupt controllers differently.
 *
 * \param      inst    The timer peripheral to initialize.
 * \param [in] handler Pointer to the general purpose timer interrupt handler.
 * \param [in] info    General purpose timer information.
 */
void hal_timerPriv_init(hal_timer_inst_t inst, hal_timer_isr_t handler, hal_timerPriv_info* info);

/**
 * \brief Configure an initialized CPU-private hardware timer.
 *
 * Each platform vendor provides access to their timers and interrupt controllers differently.
 *
 * \param      inst    The timer peripheral to configure.
 * \param [in] handler Pointer to the general purpose timer interrupt handler to assign.
 * \param [in] info    General purpose timer information.
 */
void hal_timerPriv_cfg(hal_timer_inst_t inst, hal_timer_isr_t handler, hal_timerPriv_info* info);

/**
 * \brief Configure an initialized CPU-private hardware timer with a new trip handler.
 *
 * Each platform vendor provides access to their timers and interrupt controllers differently.
 *
 * \param      inst    The timer peripheral to configure.
 * \param [in] handler Pointer to the general purpose timer interrupt handler to assign.
 */
void hal_timerPriv_cfg_handler(hal_timer_inst_t inst, hal_timer_isr_t handler);

/**
 * \brief Configure an initialized CPU-private hardware timer with new settings.
 *
 * Each platform vendor provides access to their timers and interrupt controllers differently.
 *
 * \param      inst The timer peripheral to configure.
 * \param [in] info General purpose timer information.
 */
void hal_timerPriv_cfg_info(hal_timer_inst_t inst, hal_timerPriv_info* info);

/**
 * \brief Start a configured timer.
 *
 * Arming an already-running timer resets the timer to the configured initial load value.
 *
 * \param inst The timer peripheral to initialize.
 */
void hal_timerPriv_arm(hal_timer_inst_t inst);

/**
 * \brief Stop a configured timer.
 *
 * \param inst The timer peripheral to initialize.
 */
void hal_timerPriv_disarm(hal_timer_inst_t inst);

#ifdef __cplusplus
}
#endif // #ifdef __cplusplus

#endif // #ifndef HAL_TIMER_PRIV
