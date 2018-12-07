#ifndef HAL_TIMER_TYPES_H
#define HAL_TIMER_TYPES_H

#ifdef __cplusplus
extern "C" {
#endif // #ifdef __cplusplus

/**
 * \brief Available general purpose timer instances.
 */
typedef enum hal_timer_inst_ {
    hal_timer_inst_00 = 0,
    hal_timer_inst_01 = 1,
    hal_timer_inst_02 = 2,
    hal_timer_inst_03 = 3,
    hal_timer_inst_04 = 4,
    hal_timer_inst_05 = 5,
    hal_timer_inst_06 = 6,
    hal_timer_inst_07 = 7
} hal_timer_inst_t;

/**
 * \brief Timer count direction.
 */
typedef enum hal_timer_cntDir_ {
    hal_timer_cntDir_DOWN = 0, //!< Count down.
    hal_timer_cntDir_UP   = 1  //!< Count up.
} hal_timer_cntDir_t;

/**
 * \brief Timer repeat options.
 */
typedef enum hal_timer_rpt_ {
    hal_timer_rpt_ONESHOT  = 0, //!< Do not automatically reload the timer.
    hal_timer_rpt_REPEATED = 1  //!< Automatically reload the timer when it trips.
} hal_timer_rpt_t;

typedef void (*hal_timer_isr_t)(void);

#ifdef __cplusplus
}
#endif // #ifdef __cplusplus

#endif // #ifndef HAL_TIMER_TYPES_H
