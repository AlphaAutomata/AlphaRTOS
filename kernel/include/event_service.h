#ifndef ARTOS_EVENT_SERVICE_H
#define ARTOS_EVENT_SERVICE_H

#include <stdbool.h>

#include "event_types.h"
#include "service_types.h"

bool isEventActive(ARTOS_Event* event);

void activateEvent(ARTOS_Event* event);

void deactivateEvent(ARTOS_Event* event);

int registerEventHandler(ARTOS_Event* event, pFn_serviceHandler handler);

void unregisterEventHandler(int handlerID);

bool eventHandlerExists(int handlerID);

bool eventHasHandler(ARTOS_Event* event);

#endif // #ifndef ARTOS_EVENT_SERVICE_H
