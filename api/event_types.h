#ifndef ARTOS_EVENTS_H
#define ARTOS_EVENTS_H

/**
 * \brief Alpha RTOS event categories.
 */
typedef enum {
	ARTOS_eEventCat_RESERVED_00 = 0, //!< Reserved.
	ARTOS_eEventCat_ISR         = 1  //!< A hardware interrupt event.
} ARTOS_eEventCat;

/**
 * \brief An Alpha RTOS event descriptor.
 */
typedef struct {
	ARTOS_eEventCat category; //!< The event category.
} ARTOS_Event;

#endif // #ifndef ARTOS_EVENTS_H
