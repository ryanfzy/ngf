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
        pInfo->pValue = NULL;
        propinfo_set(pInfo, pValue);
    }
}

char* _pinfo_get(PropertyInfo *pInfo)
{
    if (pInfo != NULL)
    {
        if (pInfo->eType == PropertyType_Cmd)
        { 
            Command *pCmd = (Command*)(pInfo->pValue);
            if (pCmd != NULL)
                return (char*)(pCmd->fnCommand);
        }
        else if (pInfo->eType == PropertyType_Ptr)
            return (char*)(*(char**)(pInfo->pValue));
        else
            return pInfo->pValue;
    }
    return NULL;
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
    Command command;
    command.fnCommand = fnCommand;
    _pinfo_init(pInfo, PropertyType_Cmd, (char*)&command);
}

void propinfo_init_ptr(PropertyInfo *pInfo, char *pDataPtr)
{
    _pinfo_init(pInfo, PropertyType_Ptr, (char*)&pDataPtr);
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
        char *pOldValue = pInfo->pValue;
        if (pValue == NULL)
            pInfo->pValue = NULL;
        else
        {
            size_t iSize = _pinfo_get_memsize(pInfo->eType, pValue);
            if (iSize > 0)
            {
                if (pInfo->eType == PropertyType_Binding)
                    dcitem_set_value((DcItem*)(pInfo->pValue), pValue, iSize);
                else
                {
                    pInfo->pValue = malloc(iSize);
                    memset(pInfo->pValue, 0, iSize);
                    memcpy(pInfo->pValue, pValue, iSize);
                }
            }
        }
        free(pOldValue);
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

void propinfo_bind(PropertyInfo *pInfo, DcItem *pItem)
{
    if (pInfo != NULL && pItem != NULL)
    {
        pInfo->eType = PropertyType_Binding;
        if (pInfo->pValue != NULL)
            free(pInfo->pValue);
        pInfo->pValue = (char*)pItem;
    }
}
