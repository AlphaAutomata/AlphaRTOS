#ifndef GP_TIMER_PORT_H
#define GP_TIMER_PORT_H

#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif // #ifdef __cplusplus

/**
 * \brief Available general purpose timer instances.
 */
typedef enum gpTimer_inst_ {
    gpTimer_inst_00 = 0,
    gpTimer_inst_01 = 1,
    gpTimer_inst_02 = 2,
    gpTimer_inst_03 = 3,
    gpTimer_inst_04 = 4,
    gpTimer_inst_05 = 5,
    gpTimer_inst_06 = 6,
    gpTimer_inst_07 = 7
} gpTimer_inst;

/**
 * \brief Timer count direction.
 */
typedef enum gpTimer_cntDir_ {
    gpTimer_cntDir_DOWN = 0, //!< Count down.
    gpTimer_cntDir_UP   = 1  //!< Count up.
} gpTimer_cntDir;

/**
 * \brief Timer repeat options.
 */
typedef enum gpTimer_rpt_ {
    gpTimer_rpt_ONESHOT  = 0, //!< Do not automatically reload the timer.
    gpTimer_rpt_REPEATED = 1  //!< Automatically reload the timer when it trips.
} gpTimer_rpt;

/**
 * \brief Timer configuration information.
 */
typedef struct gpTimer_info_ {
    int            loadValue;         //!< The value to load the timer with, in timer clock ticks.
    int            tripValue;         //!< The value the timer should trip at.
    gpTimer_cntDir cntDir;            //!< The timer count direction.
    gpTimer_rpt    rpt;               //!< The timer automatic reload configuration.
    bool           start_immediately; //!< Flag to start the timer immediately when configuration
                                      //!< completes.
} gpTimer_info;

/**
 * \brief Initialize a general-purpose hardware timer.
 *
 * Each platform vendor provides access to their timers and interrupt controllers differently.
 *
 * \param      instance       The timer peripheral to initialize.
 * \param [in] gpTimerHandler Pointer to the general purpose timer interrupt handler.
 * \param [in] info           General purpose timer information.
 */
void gpTimer_init(gpTimer_inst instance, void* gpTimerHandler, gpTimer_info* info);

/**
 * \brief Configure an initialized general-purpose hardware timer.
 *
 * Each platform vendor provides access to their timers and interrupt controllers differently.
 *
 * \param      instance       The timer peripheral to configure.
 * \param [in] gpTimerHandler Pointer to the general purpose timer interrupt handler to assign.
 * \param [in] info           General purpose timer information.
 */
void gpTimer_cfg(gpTimer_inst instance, void* gpTimerHandler, gpTimer_info* info);

/**
 * \brief Configure an initialized general-purpose hardware timer with a new trip handler.
 *
 * Each platform vendor provides access to their timers and interrupt controllers differently.
 *
 * \param      instance       The timer peripheral to configure.
 * \param [in] gpTimerHandler Pointer to the general purpose timer interrupt handler to assign.
 */
void gpTimer_cfg_handler(gpTimer_inst instance, void* gpTimerHandler);

/**
 * \brief Configure an initialized general-purpose hardware timer with new settings.
 *
 * Each platform vendor provides access to their timers and interrupt controllers differently.
 *
 * \param      instance The timer peripheral to configure.
 * \param [in] info     General purpose timer information.
 */
void gpTimer_cfg_info(gpTimer_inst instance, gpTimer_info* info);

/**
 * \brief Start a configured timer.
 *
 * Arming an already-running timer resets the timer to the configured initial load value.
 *
 * \param instance The timer peripheral to initialize.
 */
void gpTimer_arm(gpTimer_inst instance);

/**
 * \brief Stop a configured timer.
 *
 * \param instance The timer peripheral to initialize.
 */
void gpTimer_disarm(gpTimer_inst instance);

#ifdef __cplusplus
}
#endif // #ifdef __cplusplus

#endif // #ifndef GP_TIMER_PORT_H
