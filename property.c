#include "property.h"
#include <string.h>

static PropertyInfo* propinfo_create()
{
    PropertyInfo *pInfo = malloc(sizeof(PropertyInfo));
    memset(pInfo, 0, sizeof(PropertyInfo));
    return pInfo;
}

PropertyInfo* propinfo_create_ex(PropertyType eType)
{
    PropertyInfo *pInfo = propinfo_create();
    pInfo->eType = eType;
    return pInfo;
}

void propinfo_free(PropertyInfo *pInfo)
{
    free(pInfo->pBinding);
    free(pInfo->pValue);
    free(pInfo);
}

void propinfo_set(PropertyInfo *pInfo, char *pValue)
{
    if (pInfo != NULL && pValue != NULL)
    {
        if (pInfo->pBinding != NULL)
            binding_set(pInfo->pBinding, pInfo->eType, pValue);
        else
        {
            size_t iSize = 0;
            if (pInfo->eType == PropertyType_Str)
                iSize = (wcslen((wchar_t*)pValue)+1) * sizeof(wchar_t) / sizeof(char);
            else if (pInfo->eType == PropertyType_Int)
                iSize = sizeof(int);

            pInfo->pValue = malloc(iSize);
            memset(pInfo->pValue, 0, iSize);
            memcpy(pInfo->pValue, pValue, iSize);
        }
    }
}

bool propinfo_get(PropertyInfo *pInfo, char *pValue)
{
    bool bResult = false;
    if (pInfo != NULL)
    {
        if (pInfo->pBinding != NULL)
            return binding_get(pInfo->pBinding, pInfo->eType, pValue);
        else if (pInfo->pValue != NULL)
        {
            if (pInfo->eType == PropertyType_Str)
            {
                *(wchar_t**)pValue = (wchar_t*)(pInfo->pValue);
                bResult = true;
            }
            else if (pInfo->eType == PropertyType_Int)
            {
                *(int*)pValue = *((int*)(pInfo->pValue));
                bResult = true;
            }
        }
    }
    return bResult;
}

void propinfo_set_binding(PropertyInfo *pInfo, BindingInfo *pBinding)
{
    if (pInfo != NULL && pBinding != NULL)
        pInfo->pBinding = pBinding;
}

void propinfo_observe(PropertyInfo *pInfo, char *pObserver, size_t iObserverSize)
{
    if (pInfo != NULL && pInfo->pBinding != NULL && pObserver != NULL && iObserverSize > 0)
        DataContext_observe(pInfo->pBinding->pDc, pInfo->pBinding->szKey, pObserver, iObserverSize);
}

BindingInfo* binding_create(wchar_t *szKey, DataContext *pDc)
{
    BindingInfo *pBinding = malloc(sizeof(BindingInfo));
    pBinding->szKey = szKey;
    pBinding->pDc = pDc;
    return pBinding;
}

void binding_set(BindingInfo *pInfo, PropertyType eType, char *pValue)
{
    if (pInfo != NULL && pValue != NULL && pInfo->pDc != NULL && pInfo->szKey != NULL && wcslen(pInfo->szKey) > 0)
    {
        if (eType == PropertyType_Str)
        {
            wchar_t *pstr = (wchar_t*)pValue;
            DataContext_set_str(pInfo->pDc, pInfo->szKey, pstr);
        }
        else if (eType == PropertyType_Int)
        {
            // todo
        }
    }
}

bool binding_get(BindingInfo *pInfo, PropertyType eType, char *pValue)
{
    if (pInfo != NULL && pValue != NULL && pInfo->pDc != NULL && pInfo->szKey != NULL && wcslen(pInfo->szKey) > 0)
    {
        if (eType == PropertyType_Str)
            return DataContext_get_str(pInfo->pDc, pInfo->szKey, (wchar_t**)pValue);
        else if (eType == PropertyType_Int)
        {
            // todo
        }
    }
    return false;
}

