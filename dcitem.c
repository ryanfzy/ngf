#include <string.h>
#include "dcitem.h"
#include "zaml_structs.h"
#include "types.h"
#include "lib/utils.h"

static void _dcitem_init(DcItem *pItem)
{
    if (pItem != NULL)
    {
        pItem->pData = NULL;
        slist_init(&(pItem->observers));
    }
}

static void _dcitem_destroy(DcItem *pItem)
{
    if (pItem != NULL)
    {
        free(pItem->pData);
        pItem->pData = NULL;
        slist_destroy(&(pItem->observers));
    }
}

static void _dcitem_add_observer(DcItem *pItem, FrameworkElement *pFe)
{
    // only store pointers to fe, not the actual fe
    if (pItem != NULL && pFe != NULL)
        slist_push(&(pItem->observers), (char*)&pFe, sizeof(FrameworkElement**));
}

static void _dcitem_notify(DcItem *pItem)
{
    if (pItem != NULL)
    {
        int iObservers = slist_get_count(&(pItem->observers));
        for (int iObserver = 0; iObserver < iObservers; iObserver++)
        {
            FrameworkElement **ppFe = (FrameworkElement**)slist_get(&(pItem->observers), iObserver);
            if (ppFe != NULL)
                fe_raise_event(*ppFe, EventType_DcItemChanged);
        }
    }
}

void dcitem_set_value(DcItem *pItem, char *pValue, size_t iSize)
{
    if (pItem != NULL)
    {
        char *pOldValue = pItem->pData;
        if (pValue == NULL || iSize < 1)
            pItem->pData = NULL;
        else
        {
            pItem->pData = malloc(iSize);
            memset(pItem->pData, 0, iSize);
            memcpy(pItem->pData, pValue, iSize);
        }
        free(pOldValue);
        _dcitem_notify(pItem);
    }
}

char* dcitem_get_value(DcItem *pItem)
{
    if (pItem != NULL)
        return pItem->pData;
    return NULL;
}

void stritem_init(StrItem *pItem)
{
    if (pItem != NULL)
        _dcitem_init(&(pItem->item));
}

void stritem_destroy(StrItem *pItem)
{
    if (pItem != NULL)
        _dcitem_destroy(&(pItem->item));
}

void stritem_set_value(StrItem *pItem, wchar_t *pStr)
{
    if (pItem != NULL)
        dcitem_set_value(&(pItem->item), (char*)pStr, wstrsize(pStr));
}

wchar_t* stritem_get_value(StrItem *pItem)
{
    if (pItem != NULL)
        return (wchar_t*)dcitem_get_value(&(pItem->item));
    return NULL;
}

void stritem_add_observer(StrItem *pItem, FrameworkElement *pFe)
{
    if (pItem != NULL)
        _dcitem_add_observer(&(pItem->item), pFe);
}

void cmditem_init(CmdItem *pItem)
{
    if (pItem != NULL)
        _dcitem_init(&(pItem->item));
}

void cmditem_destroy(CmdItem *pItem)
{
    if (pItem != NULL)
        _dcitem_destroy(&(pItem->item));
}

void cmditem_set_value(CmdItem *pItem, CommandFn fnCommand)
{
    if (pItem != NULL)
    {
        Command sCmd;
        sCmd.fnCommand = fnCommand;
        dcitem_set_value(&(pItem->item), (char*)&sCmd, sizeof(Command));
    }
}

CommandFn cmditem_get_value(CmdItem *pItem)
{
    if (pItem != NULL)
    {
        Command *pCmd = (Command*)dcitem_get_value(&(pItem->item));
        if (pCmd != NULL)
            return pCmd->fnCommand;
    }
    return NULL;
}

void cmditem_add_observer(CmdItem *pItem, FrameworkElement *pFe)
{
    if (pItem != NULL)
        _dcitem_add_observer(&(pItem->item), pFe);
}
