#include "datacontext.h"
#include <string.h>
#include "lib/dictiter.h"

#define SIZEOF(x) (x), strlen(x)

DataContext* create_datacontext()
{
    DataContext *pDc = malloc(sizeof(DataContext));
    pDc->pEvtHandler = NULL;
    dict_init(&(pDc->dict));
    return pDc;
}

DataContext* create_dc_ex(EventHandler *pEvtHandler)
{
    DataContext *pDc = create_datacontext();
    pDc->pEvtHandler = pEvtHandler;
    return pDc;
}

void free_datacontext(DataContext *pDc)
{
    if (pDc != NULL)
    {
        DictIter *pIter = dict_get_iter(&(pDc->dict));
        while (dictiter_move_next(pIter))
        {
            free(dictiter_get_key(pIter));
            free(dictiter_get_value(pIter));
        }
        dict_destroy(&(pDc->dict));
        free(pDc);
    }
}

static void InitDcItem(DcItem *pDcItem, DcItemType eType, char *szKey, char *pData, size_t iSize)
{
    pDcItem->szKey = szKey;
    if (pData != NULL && iSize > 0)
    {
        pDcItem->pData = malloc(iSize);
        memcpy(pDcItem->pData, pData, iSize);
    }
    pDcItem->iDataSize = iSize;
    pDcItem->eDataType = eType;
    slist_init(&(pDcItem->observers));
}

static DcItem* DataContext_get_item(DataContext *pDc, char *szKey)
{
    if (dict_contains(&(pDc->dict), SIZEOF(szKey)))
        return (DcItem*)dict_get(&(pDc->dict), SIZEOF(szKey));
    return NULL;
}

void DataContext_add_str(DataContext *pDc, char *szKey, char *szStr, size_t iLen)
{
    DcItem item;
    InitDcItem(&item, ITEMDATATYPE_STRING, szKey, szStr, iLen+1);
    item.pData[iLen] = '\0';

    dict_add(&(pDc->dict), SIZEOF(szKey), (char*)&item, sizeof(DcItem));
}

void DataContext_add_object(DataContext *pDc, char *szKey, char *pData, size_t iSize)
{
    DcItem item;
    InitDcItem(&item, ITEMDATATYPE_OBJECT, szKey, pData, iSize);

    dict_add(&(pDc->dict), SIZEOF(szKey), (char*)&item, sizeof(DcItem));
}

char* DataContext_get_object(DataContext *pDc, char *szKey)
{
    DcItem *pItem = DataContext_get_item(pDc, szKey);
    if (pItem != NULL && pItem->eDataType == ITEMDATATYPE_OBJECT)
        return pItem->pData;
    return NULL;
}

bool DataContext_set_str(DataContext *pDc, char *szKey, char *szStr, size_t iLen)
{
    if (dict_contains(&(pDc->dict), SIZEOF(szKey)))
    {
        DcItem *pItem = DataContext_get_item(pDc, szKey);
        if (pItem != NULL)
        {
            if (szStr != pItem->pData)
            {
                free(pItem->pData);

                pItem->pData = malloc(iLen+1);
                memcpy(pItem->pData, szStr, iLen);
                pItem->pData[iLen] = '\0';
            }
            else
                pItem->pData = szStr;

            pItem->iDataSize = iLen+1;

            DataContext_update(pDc, szKey);
            return true;
        }
    }
    return false;
}

void DataContext_add_command(DataContext *pDc, char *szKey, CommandFn fnCommand)
{
    DcItem item;
    InitDcItem(&item, ITEMDATATYPE_FUNC, szKey, NULL, 0);
    item.fnCommand = fnCommand;

    dict_add(&(pDc->dict), SIZEOF(szKey), (char*)&item, sizeof(DcItem));
}

size_t DataContext_get_str(DataContext *pDc, char *szKey, char **szStr)
{
    if (pDc != NULL)
    {
        DcItem *pItem = DataContext_get_item(pDc, szKey);
        if (pItem != NULL && pItem->eDataType == ITEMDATATYPE_STRING)
        {
            if (szStr != NULL)
                *szStr = (char*)(pItem->pData);
            return pItem->iDataSize - 1;
        }
    }
    return 0;
}

void DataContext_update(DataContext *pDc, char *szKey)
{
    if (dict_contains(&(pDc->dict), SIZEOF(szKey)))
    {
        DcItem *pItem = DataContext_get_item(pDc, szKey);
        if (pItem != NULL)
        {
            for (int i = 0; i < slist_get_count(&(pItem->observers)); i++)
            {
                char *pObserver = slist_get(&(pItem->observers), i);
                if (pObserver != NULL && pDc->pEvtHandler != NULL)
                {
                    Event evt;
                    evt.pObserver = pObserver;
                    evt.eEvtType = EVENTTYPE_UPDATE;
                    pDc->pEvtHandler->fnRaiseEvent(&evt);
                }
            }
        }
    }
}

void DataContext_observe(DataContext *pDc, char *szKey, char *pObserver, size_t iObserverSize)
{
    if (dict_contains(&(pDc->dict), SIZEOF(szKey)))
    {
        DcItem *pItem = DataContext_get_item(pDc, szKey);
        if (pItem != NULL)
            slist_push(&(pItem->observers), pObserver, iObserverSize);
    }
}

void DataContext_run_command(DataContext *pDc, char *szKey)
{
    DcItem *pItem = DataContext_get_item(pDc, szKey);
    if (pItem != NULL && pItem->eDataType == ITEMDATATYPE_FUNC)
        pItem->fnCommand(pDc);
}
