#ifndef HAL_TIMER_GP
#define HAL_TIMER_GP

#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif // #ifdef __cplusplus

/**
 * \brief Available general purpose timer instances.
 */
typedef enum hal_timerGp_inst_ {
    hal_timerGp_inst_00 = 0,
    hal_timerGp_inst_01 = 1,
    hal_timerGp_inst_02 = 2,
    hal_timerGp_inst_03 = 3,
    hal_timerGp_inst_04 = 4,
    hal_timerGp_inst_05 = 5,
    hal_timerGp_inst_06 = 6,
    hal_timerGp_inst_07 = 7
} hal_timerGp_inst;

/**
 * \brief Timer count direction.
 */
typedef enum hal_timerGp_cntDir_ {
    hal_timerGp_cntDir_DOWN = 0, //!< Count down.
    hal_timerGp_cntDir_UP   = 1  //!< Count up.
} hal_timerGp_cntDir;

/**
 * \brief Timer repeat options.
 */
typedef enum hal_timerGp_rpt_ {
    hal_timerGp_rpt_ONESHOT  = 0, //!< Do not automatically reload the timer.
    hal_timerGp_rpt_REPEATED = 1  //!< Automatically reload the timer when it trips.
} hal_timerGp_rpt;

/**
 * \brief Timer configuration information.
 */
typedef struct hal_timerGp_info_ {
    int                loadValue;         //!< The value to load the timer with, in timer clock
                                          //!< ticks.
    int                tripValue;         //!< The value the timer should trip at.
    hal_timerGp_cntDir cntDir;            //!< The timer count direction.
    hal_timerGp_rpt    rpt;               //!< The timer automatic reload configuration.
    bool               start_immediately; //!< Flag to start the timer immediately when
                                          //!< configuration completes.
} hal_timerGp_info;

/**
 * \brief Initialize a general-purpose hardware timer.
 *
 * Each platform vendor provides access to their timers and interrupt controllers differently.
 *
 * \param      instance       The timer peripheral to initialize.
 * \param [in] gpTimerHandler Pointer to the general purpose timer interrupt handler.
 * \param [in] info           General purpose timer information.
 */
void hal_timerGp_init(hal_timerGp_inst instance, void* gpTimerHandler, hal_timerGp_info* info);

/**
 * \brief Configure an initialized general-purpose hardware timer.
 *
 * Each platform vendor provides access to their timers and interrupt controllers differently.
 *
 * \param      instance       The timer peripheral to configure.
 * \param [in] gpTimerHandler Pointer to the general purpose timer interrupt handler to assign.
 * \param [in] info           General purpose timer information.
 */
void hal_timerGp_cfg(hal_timerGp_inst instance, void* gpTimerHandler, hal_timerGp_info* info);

/**
 * \brief Configure an initialized general-purpose hardware timer with a new trip handler.
 *
 * Each platform vendor provides access to their timers and interrupt controllers differently.
 *
 * \param      instance       The timer peripheral to configure.
 * \param [in] gpTimerHandler Pointer to the general purpose timer interrupt handler to assign.
 */
void hal_timerGp_cfg_handler(hal_timerGp_inst instance, void* gpTimerHandler);

/**
 * \brief Configure an initialized general-purpose hardware timer with new settings.
 *
 * Each platform vendor provides access to their timers and interrupt controllers differently.
 *
 * \param      instance The timer peripheral to configure.
 * \param [in] info     General purpose timer information.
 */
void hal_timerGp_cfg_info(hal_timerGp_inst instance, hal_timerGp_info* info);

/**
 * \brief Start a configured timer.
 *
 * Arming an already-running timer resets the timer to the configured initial load value.
 *
 * \param instance The timer peripheral to initialize.
 */
void hal_timerGp_arm(hal_timerGp_inst instance);

/**
 * \brief Stop a configured timer.
 *
 * \param instance The timer peripheral to initialize.
 */
void hal_timerGp_disarm(hal_timerGp_inst instance);

#ifdef __cplusplus
}
#endif // #ifdef __cplusplus

#endif // #ifndef HAL_TIMER_GP
