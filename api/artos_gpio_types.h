#ifndef ARTOS_GPIO_TYPES_H
#define ARTOS_GPIO_TYPES_H

/**
 * \brief AlphaRTOS GPIO bank identifiers.
 */
typedef enum ARTOS_eGPIO_ {
    ARTOS_eGPIO_00 = 0,
    ARTOS_eGPIO_01 = 1,
    ARTOS_eGPIO_02 = 2,
    ARTOS_eGPIO_03 = 3,
    ARTOS_eGPIO_04 = 4,
    ARTOS_eGPIO_05 = 5,
    ARTOS_eGPIO_06 = 6,
    ARTOS_eGPIO_07 = 7
} ARTOS_eGPIO;

/**
 * \brief Alpha RTOS GPIO event type identifiers.
 */
typedef enum ARTOS_eGPIOEvent_ {
    ARTOS_eGPIOEvent_NEGEDGE = 0, //!< A GPIO pin transitioned from high to low.
    ARTOS_eGPIOEvent_POSEDGE = 1  //!< A GPIO pin transitioned from low to high.
} ARTOS_eGPIOEvent;

/**
 * \brief Alpha RTOS GPIO event information.
 */
typedef struct ARTOS_eEventCat_GPIO_ {
    ARTOS_eGPIO      bank;  //!< The GPIO bank that triggered the event.
    ARTOS_eGPIOEvent event; //!< The type of event that occurred.
    uint32_t         pins;  //!< The GPIO pin states after the triggering event.
} ARTOS_eEventCat_GPIO;

#endif // #ifndef ARTOS_GPIO_TYPES_H
