#ifndef ARTOS_EVENT_TYPES_H
#define ARTOS_EVENT_TYPES_H

#include "artos_event_data_types.h"

/**
 * \brief Alpha RTOS event categories.
 */
typedef enum ARTOS_eEventCat_ {
	ARTOS_eEventCat_RESERVED_00 = 0, //!< Reserved.
	ARTOS_eEventCat_UART        = 1, //!< A UART event.
	ARTOS_eEventCat_I2C         = 2, //!< An I2C event.
	ARTOS_eEventCat_SPI         = 3, //!< A SPI event.
	ARTOS_eEventCat_GPIO        = 4, //!< A GPIO event.
	ARTOS_eEventCat_TIMER_HW_GP = 5, //!< A general-purpose hardware timer event.
	ARTOS_eEventCat_TASK_MSG    = 6  //!< A message received from another task.
} ARTOS_eEventCat;

/**
 * \brief Alpha RTOS detailed event information.
 */
typedef union ARTOS_EventInfo_ {
	ARTOS_Event_UART        uart;     //!< A UART event.
	ARTOS_Event_I2C         i2c;      //!< An I2C event.
	ARTOS_Event_SPI         spi;      //!< A SPI event.
	ARTOS_Event_GPIO        gpio;     //!< A GPIO event.
	ARTOS_Event_TIMER_HW_GP timer;    //!< A general-purpose hardware timer event.
	ARTOS_Event_TASK_MSG    task_msg; //!< A message received from another task.
} ARTOS_EventInfo;

/**
 * \brief An Alpha RTOS event descriptor.
 */
typedef struct ARTOS_Event_ {
	ARTOS_eEventCat category; //!< The event category.
	ARTOS_EventInfo info;     //!< Detailed event specifications.
} ARTOS_Event;

#endif // #ifndef ARTOS_EVENT_TYPES_H
