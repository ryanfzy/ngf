#ifndef EVENT_H
#define EVENT_H

#include <stdio.h>
#include <stdlib.h>
#include "lib/list.h"
#include "ngf.h"

typedef void (*EventCallback)(FrameworkElement *pFe, char *pEventArg);

typedef struct _event
{
    SList eventCallbacks;
} EventInfo;

typedef struct _eventCallback
{
    EventCallback fnCallback;
} EventListener;

void evt_init(EventInfo *pInfo);
void evt_destroy(EventInfo *pInfo);

void evt_subscribe(EventInfo *pInfo, EventCallback fnCallback);
void evt_notify(EventInfo *pInfo, FrameworkElement *pFe, char *pEvtArg);

#endif
