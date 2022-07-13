#ifndef EVENT_H
#define EVENT_H

#include <stdio.h>
#include <stdlib.h>
#include "lib/list.h"
#include "ngf.h"

typedef void (*EventCallback)(char *pEventArg);

typedef struct _event
{
    SList listeners;
} EventInfo;

typedef struct _eventCallback
{
    EventCallback fnCallback;
    char *pEvtArg;
} EventListener;

void evt_init(EventInfo *pInfo);
void evt_destroy(EventInfo *pInfo);

void evt_subscribe_ex(EventInfo *pInfo, EventCallback fnCallback, char *pEvtArg, size_t iArgSize);
void evt_notify(EventInfo *pInfo);

void evtlistener_init_ex(EventListener *pListener, EventCallback fnCallback, char *pEvtArg, size_t iArgSize);
void evtlistener_destroy(EventListener *pListener);

#endif
