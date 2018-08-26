#ifndef ARTOS_EVENTS_H
#define ARTOS_EVENTS_H

/**
 * \brief Alpha RTOS event categories.
 */
typedef enum {
	ARTOS_eEventCat_RESERVED_00 = 0, //!< Reserved.
	ARTOS_eEventCat_HW_INT      = 1, //!< A hardware interrupt event.
	ARTOS_eEventCat_UART        = 2, //!< A UART event.
	ARTOS_eEventCat_I2C         = 3, //!< An I2C event.
	ARTOS_eEventCat_SPI         = 4, //!< A SPI event.
	ARTOS_eEventCat_GPIO        = 5, //!< A GPIO event.
	ARTOS_eEventCat_TIMER_HW_GP = 6, //!< A general-purpose hardware timer event.
	ARTOS_eEventCat_TASK_MSG    = 7  //!< A message received from another task.
} ARTOS_eEventCat;

/**
 * \brief Alpha RTOS detailed event information.
 */
typedef union {
	ARTOS_Event_HW_INT      hw_int;   //!< A hardware interrupt event.
	ARTOS_Event_UART        uart；    //!< A UART event.
	ARTOS_Event_I2C         i2c；     //!< An I2C event.
	ARTOS_Event_SPI         spi；     //!< A SPI event.
	ARTOS_Event_GPIO        gpio；    //!< A GPIO event.
	ARTOS_Event_TIMER_HW_GP timer；   //!< A general-purpose hardware timer event.
	ARTOS_Event_TASK_MSG    task_msg；//!< A message received from another task.
} ARTOS_EventInfo;

/**
 * \brief An Alpha RTOS event descriptor.
 */
typedef struct {
	ARTOS_eEventCat category; //!< The event category.
	ARTOS_EventInfo info;     //!< Detailed event specifications.
} ARTOS_Event;

#endif // #ifndef ARTOS_EVENTS_H
