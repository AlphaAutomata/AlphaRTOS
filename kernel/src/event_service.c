#include "AlphaRTOS_config.h"

#include "event_service.h"

#if (ARTOS_CFG_USE_C_DYNAMIC_MEM == 1)

static pFn_serviceHandler* handlers;
static volatile int numHandlers;

#else // #if (ARTOS_CFG_USE_C_DYNAMIC_MEM == 1)

static pFn_serviceHandler handlers[ARTOS_NUM_EVENT_SERVICES];
#define numHandlers ARTOS_NUM_EVENT_SERVICES

#endif // #if (ARTOS_CFG_USE_C_DYNAMIC_MEM == 1)

bool isEventActive(ARTOS_Event* event) {

}

void activateEvent(ARTOS_Event* event) {

}

void deactivateEvent(ARTOS_Event* event) {

}

int registerEventHandler(ARTOS_Event* event, pFn_serviceHandler handler) {

}

void unregisterEventHandler(int handlerID) {

}

bool eventHandlerExists(int handlerID) {

}

bool eventHasHandler(ARTOS_Event* event) {

}
