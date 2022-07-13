#include <string.h>
#include "evtinfo.h"

static void _evtlisteners_destroy(SList *pListeners)
{
    if (pListeners != NULL)
    {
        int iListeners = slist_get_count(pListeners);
        for (int i = 0; i < iListeners; i++)
        {
            EventListener *pListener = (EventListener*)slist_get(pListeners, i);
            if (pListener != NULL)
                evtlistener_destroy(pListener);
        }
    }
}

void evt_init(EventInfo *pInfo)
{
    if (pInfo != NULL)
        slist_init(&(pInfo->listeners));
}

void evt_destroy(EventInfo *pInfo)
{
    if (pInfo != NULL)
    {
        _evtlisteners_destroy(&(pInfo->listeners));
        slist_destroy(&(pInfo->listeners));
    }
}

void evt_subscribe_ex(EventInfo *pInfo, EventCallback fnCallback, char *pEvtArg, size_t iArgSize)
{
    if (pInfo != NULL)
    {
        EventListener listener;
        evtlistener_init_ex(&listener, fnCallback, pEvtArg, iArgSize);
        slist_push(&(pInfo->listeners), (char*)&listener, sizeof(EventListener));
    }
}

void evt_notify(EventInfo *pInfo)
{
    if (pInfo != NULL)
    {
        int iCallbacks = slist_get_count(&(pInfo->listeners));
        for (int i = 0; i < iCallbacks; i++)
        {
            EventListener *pListener = (EventListener*)slist_get(&(pInfo->listeners), i);
            if (pListener != NULL)
                pListener->fnCallback(pListener->pEvtArg);
        }
    }
}

static void _evtlistener_init(EventListener *pListener)
{
    pListener->fnCallback = NULL;
    pListener->pEvtArg = NULL;
}

void evtlistener_init_ex(EventListener *pListener, EventCallback fnCallback, char *pEvtArg, size_t iArgSize)
{
    _evtlistener_init(pListener);
    pListener->fnCallback = fnCallback;
    if (pEvtArg != NULL && iArgSize > 0)
    {
        pListener->pEvtArg = malloc(iArgSize);
        memset(pListener->pEvtArg, 0, iArgSize);
        memcpy(pListener->pEvtArg, pEvtArg, iArgSize);
    }
}

void evtlistener_destroy(EventListener *pListener)
{
    if (pListener != NULL)
        free(pListener->pEvtArg);
}
