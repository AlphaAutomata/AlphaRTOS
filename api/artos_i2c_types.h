#ifndef ARTOS_I2C_TYPES_H
#define ARTOS_I2C_TYPES_H

/**
 * \brief AlphaRTOS I2C device identifiers.
 */
typedef enum ARTOS_eI2C_ {
    ARTOS_eI2C_00 = 0,
    ARTOS_eI2C_00 = 1,
    ARTOS_eI2C_00 = 2,
    ARTOS_eI2C_00 = 3
} ARTOS_eI2C;

/**
 * \brief Alpha RTOS I2C event type identifiers.
 */
typedef enum ARTOS_eI2CEvent_ {
    ARTOS_eI2CEvent_BUS_ERR  = 0, //!< An I2C bus error.
    ARTOS_eI2CEvent_BUS_STOP = 1, //!< An I2C bus STOP event has been detected.
    ARTOS_eI2CEvent_TRX_DONE = 2, //!< An I2C bus transaction has finished.
    ARTOS_eI2CEvent_NO_ACK   = 3  //!< No response to an I2C bus request.
} ARTOS_eI2CEvent;

/**
 * \brief Alpha RTOS I2C event information.
 */
typedef struct ARTOS_Event_I2C_ {
    ARTOS_eI2C      dev;      //!< The identifier of the device that triggered the event.
    ARTOS_eI2CEvent event;    //!< The type of event that occurred.
    size_t          data_len; //!< The length of the data buffer.
    uint8_t         data[];   //!< The buffer containing data transceived over I2C.
} ARTOS_Event_I2C;

#endif // #ifndef ARTOS_I2C_TYPES_H
