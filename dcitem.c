#include <string.h>
#include "dcitem.h"
#include "zaml_structs.h"
#include "types.h"
#include "lib/utils.h"

static int _dcitem_get_memsize(DcItemType eType, char *pValue)
{
    if (pValue != NULL)
    {
        if (eType == DcItemType_Str)
            return wstrsize((wchar_t*)pValue);
        else if (eType == DcItemType_Cmd)
            return sizeof(Command);
    }
    return 0;
}

static void _dcitem_init(DcItem *pItem, DcItemType eType)
{
    if (pItem != NULL)
    {
        pItem->eType = eType;
        pItem->pData = NULL;
        evt_init(&(pItem->ValueChangedEvent));
    }
}

static void _dcitem_destroy(DcItem *pItem)
{
    if (pItem != NULL)
    {
        free(pItem->pData);
        pItem->pData = NULL;
        evt_destroy(&(pItem->ValueChangedEvent));
    }
}

void dcitem_set_value(DcItem *pItem, char *pValue)
{
    if (pItem != NULL)
    {
        if (pItem->pData != NULL)
            free(pItem->pData);
        pItem->pData = NULL;
        int iSize = _dcitem_get_memsize(pItem->eType, pValue);
        if (iSize > 0)
        {
            pItem->pData = malloc(iSize);
            memset(pItem->pData, 0, iSize);
            if (pItem->eType == DcItemType_Cmd)
            {
                Command cmd;
                cmd.fnCommand = (CommandFn)pValue;
                pValue = (char*)&cmd;
            }
            memcpy(pItem->pData, pValue, iSize);
        }
        dcitem_raise_changed_evt(pItem);
    }
}

char* dcitem_get_value(DcItem *pItem)
{
    if (pItem != NULL)
    {
        if (pItem->eType == DcItemType_Cmd)
        {
            if (pItem->pData != NULL)
            {
                Command *pCmd = (Command*)pItem->pData;
                return (char*)(pCmd->fnCommand);
            }
        }
        else
            return pItem->pData;
    }
    return NULL;
}

void dcitem_sub_changed_evt(DcItem *pItem, EventCallback fnCallback, char *pEvtArg, size_t iArgSize)
{
    if (pItem != NULL)
        evt_subscribe_ex(&(pItem->ValueChangedEvent), fnCallback, pEvtArg, iArgSize);
}

void dcitem_raise_changed_evt(DcItem *pItem)
{
    if (pItem != NULL)
        evt_notify(&(pItem->ValueChangedEvent));
}

void stritem_init(StrItem *pItem)
{
    if (pItem != NULL)
        _dcitem_init(&(pItem->item), DcItemType_Str);
}

void stritem_destroy(StrItem *pItem)
{
    if (pItem != NULL)
        _dcitem_destroy(&(pItem->item));
}

void stritem_set(StrItem *pItem, wchar_t *pStr)
{
    if (pItem != NULL)
        dcitem_set_value(&(pItem->item), (char*)pStr);
}

wchar_t* stritem_get(StrItem *pItem)
{
    if (pItem != NULL)
        return (wchar_t*)dcitem_get_value(&(pItem->item));
    return NULL;
}

void cmditem_init(CmdItem *pItem)
{
    if (pItem != NULL)
        _dcitem_init(&(pItem->item), DcItemType_Cmd);
}

void cmditem_destroy(CmdItem *pItem)
{
    if (pItem != NULL)
        _dcitem_destroy(&(pItem->item));
}

void cmditem_set(CmdItem *pItem, CommandFn fnCommand)
{
    if (pItem != NULL)
        dcitem_set_value(&(pItem->item), (char*)fnCommand);
}

CommandFn cmditem_get(CmdItem *pItem)
{
    if (pItem != NULL)
        return (CommandFn)dcitem_get_value(&(pItem->item));
    return NULL;
}
