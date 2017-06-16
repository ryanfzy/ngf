#include "feinfo.h"
#include <string.h>

FrameworkElement* create_fe(int iFeType)
{
    FrameworkElement *pFe = malloc(sizeof(FrameworkElement));
    pFe->iType = iFeType;
    return pFe;
}

void InitLayoutInfo(LayoutInfo *pLayoutInfo)
{
    slist_init(&(pLayoutInfo->children));
}

void fe_set_value(FrameworkElement *pFe, AttachedPropertyInfo *pInfo, char *pValue)
{
    if (pFe != NULL && pInfo != NULL && pValue != NULL)
    {
        size_t iSize = 0;
        if (pInfo->eType == PropertyType_Int)
            iSize = sizeof(int);

        if (iSize > 0)
        {
            if (dict_contains(&(pFe->attachedProps), (char*)(&pInfo), sizeof(AttachedPropertyInfo*)))
                dict_set(&(pFe->attachedProps), (char*)(&pInfo), sizeof(AttachedPropertyInfo*), pValue, iSize);
            else
                dict_add(&(pFe->attachedProps), (char*)(&pInfo), sizeof(AttachedPropertyInfo*), pValue, iSize);
        }
    }
}

bool fe_get_value(FrameworkElement *pFe, AttachedPropertyInfo *pInfo, char *pValue)
{
    bool bResult = false;
    if (pFe != NULL && pInfo != NULL && pValue != NULL)
    {
        char *pVal = dict_get(&(pFe->attachedProps), (char*)(&pInfo), sizeof(AttachedPropertyInfo*));
        if (pInfo->eType == PropertyType_Int)
        {
            if (pVal != NULL)
                *(int*)pValue = *((int*)pVal);
            else
                *(int*)pValue = *((int*)(pInfo->pDefaultValue));
        }
        bResult = true;
    }
    return bResult;
}

int linfo_get_children_count(LayoutInfo *pInfo)
{
    if (pInfo != NULL)
        return slist_get_count(&(pInfo->children));
    return 0;
}

FrameworkElement* linfo_get_child(LayoutInfo *pInfo, unsigned int iPos)
{
    if (pInfo != NULL && iPos < linfo_get_children_count(pInfo))
        return *(FrameworkElement**)slist_get(&(pInfo->children), iPos);
    return NULL;
}

FeSize vinfo_get_size(VisualInfo *pInfo)
{
    FeSize size = {0, 0};
    if (pInfo != NULL)
    {
        size.width = pInfo->width;
        size.height = pInfo->height;
    }
    return size;
}
