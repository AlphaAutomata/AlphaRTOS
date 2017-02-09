#ifndef _BADGER_RMC_RTOS_H_
#define _BADGER_RMC_RTOS_H_

#include <stdint.h>
#include <stdbool.h>

//*****************************************************************************
//
//! Register a periodic callback. This function will be automatically called
//! every interval number of milliseconds. 
//!
//! \param interval specifies how many milliseconds should pass between calls
//! to the callback function
//!
//! \param callback points to the function that should be called every interval
//!
//! \return a callback ID identifying this particular callback. Currently only
//! supports a single callback. Returns 0 on failure.
//
//*****************************************************************************
int callbackRegister(uint32_t interval, int (*callback)(void *));

//*****************************************************************************
//
//! Unregister a periodic callback 
//!
//! \param callbackID is the ID returned by callbackRegister() when the
//! callback was first registered
//!
//! \return the callback ID on success, -1 on failure
//
//*****************************************************************************
int callbackUnregister(int callbackID);

//*****************************************************************************
//
//! Set the periodic interval between successive calls to a callback
//!
//! \param callbackID is the ID returned by callbackRegister() when the
//! callback was first registered
//!
//! \param interval is the number of milliseconds to wait between calls to the
//! callback
//!
//! \return the callback ID on success, -1 on failure
//
//*****************************************************************************
int setCallbackInterval(int callbackID, uint32_t interval);

//*****************************************************************************
//
//! Get the periodic interval between successive calls to a callback
//!
//! \param callbackID is the ID returned by callbackRegister() when the
//! callback was first registered
//!
//! \return the interval in milliseconds, or 0 on failure
//
//*****************************************************************************
uint32_t getCallbackInterval(int callbackID);

//*****************************************************************************
//
//! Yield the processor for the remainder of the scheduling cycle. Not
//! currently supported. 
//!
//! \param none
//!
//! \return none
//
//*****************************************************************************
void taskYield(void);

//*****************************************************************************
//
//! Sleep for timeMillis number of milliseconds. Not currently supported. 
//!
//! \param none
//!
//! \return none
//
//*****************************************************************************
int taskSleep(int timeMillis);

//*****************************************************************************
//
//! Get the current number of milliseconds the system has been running for.
//!
//! \param none
//!
//! \return the number of milliseconds the system has been running
//
//*****************************************************************************
uint64_t getUptime(void);

#endif
