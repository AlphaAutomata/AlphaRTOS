#ifndef ARTOS_TIMER_TYPES_H
#define ARTOS_TIMER_TYPES_H

/**
 * \brief AlphaRTOS general-purpose timer identifiers.
 */
typedef enum ARTOS_eTimer_ {
    ARTOS_eTimer_00 = 0,
    ARTOS_eTimer_01 = 1,
    ARTOS_eTimer_02 = 2,
    ARTOS_eTimer_03 = 3,
    ARTOS_eTimer_04 = 4,
    ARTOS_eTimer_05 = 5,
    ARTOS_eTimer_06 = 6,
    ARTOS_eTimer_07 = 7
} ARTOS_eTimer;

/**
 * \brief Alpha RTOS general-purpose timer event information.
 */
typedef struct ARTOS_Event_TIMER_HW_GP_ {
    ARTOS_eTimer dev; //!< The timer device that tripped.
} ARTOS_Event_TIMER_HW_GP;

#endif // #ifndef ARTOS_TIMER_TYPES_H
