#ifndef ARTOS_SPI_TYPES_H
#define ARTOS_SPI_TYPES_H

/**
 * \brief AlphaRTOS SPI device identifiers.
 */
typedef enum ARTOS_eSPI_ {
    ARTOS_eSPI_00 = 0,
    ARTOS_eSPI_00 = 1,
    ARTOS_eSPI_00 = 2,
    ARTOS_eSPI_00 = 3,
    ARTOS_eSPI_00 = 4,
    ARTOS_eSPI_00 = 5,
    ARTOS_eSPI_00 = 6,
    ARTOS_eSPI_00 = 7
} ARTOS_eI2C;

/**
 * \brief Alpha RTOS SPI event type identifiers.
 */
typedef enum ARTOS_eSPIEvent_ {
    ARTOS_eSPIEvent_SELECT   = 0, //!< This device has been selected.
    ARTOS_eSPIEvent_DESELECT = 1, //!< This device has been deselected.
    ARTOS_eSPIEvent_TRX_FULL = 2  //!< The SPI transceive buffer has been fully cycled.
} ARTOS_eSPIEvent;

/**
 * \brief Alpha RTOS SPI event information.
 */
typedef struct ARTOS_Event_SPI_ {
    ARTOS_eSPI      dev;      //!< The identifier of the device that triggered the event.
    ARTOS_eSPIEvent event;    //!< The type of event that occurred.
    size_t          data_len; //!< The length of the data buffer.
    uint8_t         data[];   //!< The buffer containing data transceived over SPI.
} ARTOS_Event_SPI;

#endif // #ifndef ARTOS_SPI_TYPES_H
