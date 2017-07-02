#include "evtinfo.h"

void evt_init(EventInfo *pInfo)
{
    if (pInfo != NULL)
        slist_init(&(pInfo->eventCallbacks));
}

void evt_destroy(EventInfo *pInfo)
{
    if (pInfo != NULL)
        slist_destroy(&(pInfo->eventCallbacks));
}

void evt_subscribe(EventInfo *pInfo, EventCallback fnCallback)
{
    if (pInfo != NULL)
    {
        EventListener listener;
        listener.fnCallback = fnCallback;
        slist_push(&(pInfo->eventCallbacks), (char*)&listener, sizeof(EventListener));
    }
}

void evt_notify(EventInfo *pInfo, FrameworkElement *pFe, char *pEvtArg)
{
    if (pFe != NULL && pInfo != NULL)
    {
        int iCallbacks = slist_get_count(&(pInfo->eventCallbacks));
        for (int i = 0; i < iCallbacks; i++)
        {
            EventListener *pListener = (EventListener*)slist_get(&(pInfo->eventCallbacks), i);
            if (pListener != NULL)
                pListener->fnCallback(pFe, pEvtArg);
        }
    }
}

