#ifndef ARTOS_UART_TYPES_H
#define ARTOS_UART_TYPES_H

#include <stdint.h>
#include <stddef.h>

/**
 * \brief Alpha RTOS UART device identifiers.
 */
typedef enum ARTOS_eUART_ {
    ARTOS_eUART_00 = 0,
    ARTOS_eUART_01 = 1,
    ARTOS_eUART_02 = 2,
    ARTOS_eUART_03 = 3,
    ARTOS_eUART_04 = 4,
    ARTOS_eUART_05 = 5,
    ARTOS_eUART_06 = 6,
    ARTOS_eUART_07 = 7
} ARTOS_eUART;

/**
 * \brief Alpha RTOS UART event type identifiers.
 */
typedef enum ARTOS_eUARTEvent_ {
    ARTOS_eUARTEvent_RX_DONE    = 0, //!< A UART read buffer has been filled.
    ARTOS_eUARTEvent_RX_TIMEOUT = 1, //!< A UART read has timed out.
    ARTOS_eUARTEvent_TX_DONE    = 2, //!< A UART write buffer has finished.
    ARTOS_eUARTEvent_TX_TIMEOUT = 3  //!< A UART write has timed out.
} ARTOS_eUARTEvent;

/**
 * \brief Alpha RTOS UART event information.
 */
typedef struct ARTOS_Event_UART_ {
    ARTOS_eUART      dev;      //!< The identifier of the device that triggered the event.
    ARTOS_eUARTEvent event;    //!< The type of event that occurred.
    size_t           data_len; //!< The length of the data buffer.
    uint8_t          data[];   //!< The buffer containing data transceived over UART.
} ARTOS_Event_UART;

#endif // #ifndef ARTOS_UART_TYPES_H
