#include "datacontext.h"
#include <string.h>
#include "lib/dictiter.h"

static size_t len2size(int iLen)
{
    return iLen * sizeof(wchar_t) / sizeof(char);
}

#define SIZEOF(x) ((char*)x), len2size(wcslen(x))
#define LEN2SIZE(x) len2size(x)
#define SIZE2LEN(x) (x/sizeof(wchar_t))

DataContext* create_datacontext()
{
    DataContext *pDc = malloc(sizeof(DataContext));
    //pDc->pEvtHandler = NULL;
    dict_init(&(pDc->dict));
    return pDc;
}

DataContext* create_dc_ex(EventHandler *pEvtHandler)
{
    DataContext *pDc = create_datacontext();
    pDc->pEvtHandler = pEvtHandler;
    return pDc;
}

static void DestroyDcItem(DcItem *pDcItem)
{
    free(pDcItem->pData);
    for (int i = 0; i < slist_get_count(&(pDcItem->observers)); i++)
        free(slist_get(&(pDcItem->observers), i));
}

void free_datacontext(DataContext *pDc)
{
    if (pDc != NULL)
    {
        DictIter *pIter = dict_get_iter(&(pDc->dict));
        while (dictiter_move_next(pIter))
        {
            free(dictiter_get_key(pIter));
            DcItem *pItem = (DcItem*)dictiter_get_value(pIter);
            DestroyDcItem(pItem);
            free(pItem);
        }
        dict_destroy(&(pDc->dict));
        free(pDc);
    }
}

static void InitDcItem(DcItem *pDcItem, DcItemType eType, char *pData, size_t iSize)
{
    if (pData != NULL && iSize > 0)
    {
        pDcItem->pData = malloc(iSize);
        memcpy(pDcItem->pData, pData, iSize);
    }
    else
        pDcItem->pData = NULL;

    pDcItem->iDataSize = iSize;
    pDcItem->eDataType = eType;
    slist_init(&(pDcItem->observers));
}

static DcItem* DataContext_get_item(DataContext *pDc, wchar_t *szKey)
{
    if (dict_contains(&(pDc->dict), SIZEOF(szKey)))
        return (DcItem*)dict_get(&(pDc->dict), SIZEOF(szKey));
    return NULL;
}

void DataContext_add_str(DataContext *pDc, wchar_t *szKey, wchar_t *szStr)
{
    int iLen = wcslen(szStr);
    DcItem item;
    InitDcItem(&item, ITEMDATATYPE_STRING, (char*)szStr, LEN2SIZE(iLen+1));
    ((wchar_t*)item.pData)[iLen] = L'\0';

    dict_add(&(pDc->dict), SIZEOF(szKey), (char*)&item, sizeof(DcItem));
}

void DataContext_add_object(DataContext *pDc, wchar_t *szKey, char *pData, size_t iSize)
{
    DcItem item;
    InitDcItem(&item, ITEMDATATYPE_OBJECT, pData, iSize);

    dict_add(&(pDc->dict), SIZEOF(szKey), (char*)&item, sizeof(DcItem));
}

char* DataContext_get_object(DataContext *pDc, wchar_t *szKey)
{
    DcItem *pItem = DataContext_get_item(pDc, szKey);
    if (pItem != NULL && pItem->eDataType == ITEMDATATYPE_OBJECT)
        return pItem->pData;
    return NULL;
}

bool DataContext_set_str(DataContext *pDc, wchar_t *szKey, wchar_t *szStr)
{
    if (dict_contains(&(pDc->dict), SIZEOF(szKey)))
    {
        DcItem *pItem = DataContext_get_item(pDc, szKey);
        if (pItem != NULL && pItem->eDataType == ITEMDATATYPE_STRING)
        { 
            int iLen = wcslen(szStr);
            if (szStr != (wchar_t*)pItem->pData)
            {
                free(pItem->pData);

                pItem->pData = malloc(LEN2SIZE(iLen+1));
                memcpy(pItem->pData, (char*)szStr, LEN2SIZE(iLen));
                ((wchar_t*)pItem->pData)[iLen] = L'\0';
            }
            else
                pItem->pData = (char*)szStr;

            pItem->iDataSize = LEN2SIZE(iLen+1);

            //DataContext_update(pDc, szKey);
            return true;
        }
    }
    return false;
}

void DataContext_add_command(DataContext *pDc, wchar_t *szKey, CommandFn fnCommand)
{
    DcItem item;
    InitDcItem(&item, ITEMDATATYPE_FUNC, NULL, 0);
    item.fnCommand = fnCommand;

    dict_add(&(pDc->dict), SIZEOF(szKey), (char*)&item, sizeof(DcItem));
}

size_t DataContext_get_str(DataContext *pDc, wchar_t *szKey, wchar_t **szStr)
{
    if (pDc != NULL)
    {
        DcItem *pItem = DataContext_get_item(pDc, szKey);
        if (pItem != NULL && pItem->eDataType == ITEMDATATYPE_STRING)
        {
            if (szStr != NULL)
                *szStr = (wchar_t*)(pItem->pData);
            return SIZE2LEN(pItem->iDataSize) - 1;
        }
    }
    return 0;
}

void DataContext_update(DataContext *pDc, wchar_t *szKey)
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

void DataContext_observe(DataContext *pDc, wchar_t *szKey, char *pObserver, size_t iObserverSize)
{
    if (dict_contains(&(pDc->dict), SIZEOF(szKey)))
    {
        DcItem *pItem = DataContext_get_item(pDc, szKey);
        if (pItem != NULL)
            slist_push(&(pItem->observers), pObserver, iObserverSize);
    }
}

void DataContext_run_command(DataContext *pDc, wchar_t *szKey)
{
    DcItem *pItem = DataContext_get_item(pDc, szKey);
    if (pItem != NULL && pItem->eDataType == ITEMDATATYPE_FUNC)
        pItem->fnCommand(pDc);
}
