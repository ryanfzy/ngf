#include <string.h>
#include "lib/utils.h"
#include "property.h"
#include "dcitem.h"

int _pinfo_get_memsize(PropertyType eType, char *pValue)
{
    if (eType == PropertyType_Str)
        return wstrsize((wchar_t*)pValue);
    else if (eType == PropertyType_Int)
        return sizeof(int);
    else if (eType == PropertyType_Cmd)
        return sizeof(Command);
    else if (eType == PropertyType_Ptr)
        return sizeof(char*);
    return 0;
}

void _pinfo_init(PropertyInfo *pInfo, PropertyType eType, char *pValue)
{
    if (pInfo != NULL)
    {
        pInfo->eType = eType;
        evt_init(&pInfo->PropertyChangedEvent);
        pInfo->pValue = NULL;
        if (pValue != NULL)
            propinfo_set(pInfo, pValue);
    }
}

char* _pinfo_get(PropertyInfo *pInfo)
{
    if (pInfo != NULL && pInfo->pValue != NULL)
    {
        if (pInfo->eType == PropertyType_Cmd)
        { 
            Command *pCmd = (Command*)(pInfo->pValue);
            if (pCmd != NULL)
                return (char*)(pCmd->fnCommand);
        }
        else if (pInfo->eType == PropertyType_Ptr)
        {
            return *((char**)(pInfo->pValue));
        }
        else
            return pInfo->pValue;
    }
    return NULL;
}

void _pinfo_set(PropertyInfo *pInfo, char *pValue)
{
    if (pInfo != NULL)
    {
        if (pInfo->pValue != NULL)
            free(pInfo->pValue);
        pInfo->pValue = NULL;
        size_t iSize = _pinfo_get_memsize(pInfo->eType, pValue);
        if (iSize > 0)
        {
            pInfo->pValue = malloc(iSize);
            memset(pInfo->pValue, 0, iSize);
            if (pInfo->eType == PropertyType_Cmd)
            {
                Command cmd;
                cmd.fnCommand = (CommandFn)pValue;
                pValue = (char*)&cmd;
            }
            else if (pInfo->eType == PropertyType_Ptr)
            {
                char *pPtr = pValue;
                pValue = (char*)&pPtr;
            }
            memcpy(pInfo->pValue, pValue, iSize);
        }
    }
}

void propinfo_init_int(PropertyInfo *pInfo, int iValue)
{
    _pinfo_init(pInfo, PropertyType_Int, (char*)&iValue);
}

void propinfo_init_str(PropertyInfo *pInfo, wchar_t *pValue)
{
    _pinfo_init(pInfo, PropertyType_Str, (char*)pValue);
}

void propinfo_init_cmd(PropertyInfo *pInfo, CommandFn fnCommand)
{
    _pinfo_init(pInfo, PropertyType_Cmd, (char*)fnCommand);
}

void propinfo_init_ptr(PropertyInfo *pInfo, char *pDataPtr)
{
    _pinfo_init(pInfo, PropertyType_Ptr, pDataPtr);
}

void propinfo_destroy(PropertyInfo *pInfo)
{
    // if it's a binidng, don't free it
    if (pInfo != NULL && pInfo->eType != PropertyType_Binding)
        free(pInfo->pValue);
}


void propinfo_set(PropertyInfo *pInfo, char *pValue)
{
    if (pInfo != NULL)
    {
        if (pInfo->eType == PropertyType_Binding)
            dcitem_set_value((DcItem*)(pInfo->pValue), pValue);
        else
            _pinfo_set(pInfo, pValue);
    }
}

char* propinfo_get(PropertyInfo *pInfo)
{
    if (pInfo != NULL)
    {
        if (pInfo->eType == PropertyType_Binding)
            return dcitem_get_value((DcItem*)(pInfo->pValue));
        else
            return _pinfo_get(pInfo);
    }
    return NULL;
}

static void dcitem_valuechanged_callback(char *pEvtArg)
{
    PropertyInfo *pInfo = *((PropertyInfo**)pEvtArg);
    if (pInfo != NULL)
        propinfo_raise_changed_evt(pInfo);
}

void propinfo_bind(PropertyInfo *pInfo, DcItem *pItem)
{
    if (pInfo != NULL && pItem != NULL)
    {
        if (pInfo->eType != PropertyType_Binding)
        {
            if (pInfo->pValue != NULL)
                free(pInfo->pValue);
            pInfo->eType = PropertyType_Binding;
        }
        pInfo->pValue = (char*)pItem;
        dcitem_sub_changed_evt(pItem, dcitem_valuechanged_callback, (char*)&pInfo, sizeof(PropertyInfo**));
    }
}

void propinfo_sub_changed_evt(PropertyInfo *pInfo, EventCallback fnCallback, char *pEvtArg, size_t iArgSize)
{
    if (pInfo != NULL)
        evt_subscribe_ex(&(pInfo->PropertyChangedEvent), fnCallback, pEvtArg, iArgSize);
}

void propinfo_raise_changed_evt(PropertyInfo *pInfo)
{
    if (pInfo != NULL)
        evt_notify(&(pInfo->PropertyChangedEvent));
}
