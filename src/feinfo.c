#include "feinfo.h"
#include <string.h>
#include "lib/utils.h"

FrameworkElement* create_fe(int iFeType)
{
    FrameworkElement *pFe = malloc(sizeof(FrameworkElement));
    pFe->iType = iFeType;
    dict_init(&(pFe->attachedProps));
    return pFe;
}

void fe_free(FrameworkElement *pFe)
{
    if (pFe != NULL)
    {
        // currently all info will free pElement
        //free(pFe->pElement);
        dict_destroy(&(pFe->attachedProps));
        free(pFe);
    }
}

void linfo_init(LayoutInfo *pInfo)
{
    if (pInfo != NULL)
    {
        vinfo_init(&(pInfo->visualInfo));
        slist_init(&(pInfo->children));
    }
}

void linfo_destroy(LayoutInfo *pInfo)
{
    if (pInfo != NULL)
    {
        slist_destroy(&(pInfo->children));
        vinfo_destroy(&(pInfo->visualInfo));
    }
}

bool linfo_add_child(LayoutInfo *pInfo, FrameworkElement *pChildFe)
{
    if (pInfo != NULL && pChildFe)
    {
        slist_push(&(pInfo->children), (char*)(&pChildFe), sizeof(pChildFe));
        return true;
    }
    return false;
}


bool linfo_has_child(LayoutInfo *pInfo, FrameworkElement *pChildFe)
{
    for (int iChild = 0; iChild < slist_get_count(&(pInfo->children)); iChild++)
    {
        if (*((FrameworkElement**)slist_get(&(pInfo->children), iChild)) == pChildFe)
            return true;
    }
    return false;
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

void vinfo_init(VisualInfo *pInfo)
{
    if (pInfo != NULL)
    {
        pInfo->x = 0;
        pInfo->y = 0;
        pInfo->width = SIZE_AUTO;
        pInfo->height = SIZE_AUTO;
        pInfo->actualWidth = 0;
        pInfo->actualHeight = 0;
    }
}

void vinfo_destroy(VisualInfo *pInfo)
{
    // no pointer to free
}

FeSize vinfo_get_size(VisualInfo *pInfo)
{
    FeSize size = {0, 0};
    if (pInfo != NULL)
    {
        size.width = pInfo->actualWidth;
        size.height = pInfo->actualHeight;
    }
    return size;
}

void vinfo_set_size(VisualInfo *pInfo, FeSize size)
{
    if (pInfo != NULL)
    {
        pInfo->actualWidth = size.width;
        pInfo->actualHeight = size.height;
    }
}

FePos vinfo_get_pos(VisualInfo *pInfo)
{
    FePos pos = {0, 0};
    if (pInfo != NULL)
    {
        pos.x = pInfo->x;
        pos.y = pInfo->y;
    }
    return pos;
}

void vinfo_set_pos(VisualInfo *pInfo, FePos pos)
{
    if (pInfo != NULL)
    {
        pInfo->x = pos.x;
        pInfo->y = pos.y;
    }
}

void vinfo_init_ex(VisualInfo *pInfo, int x, int y, int width, int height)
{
    if (pInfo != NULL)
    {
        pInfo->x = x;
        pInfo->y = y;
        pInfo->width = width;
        pInfo->actualWidth = ymax(width, 0);
        pInfo->height = height;
        pInfo->actualHeight = ymax(height, 0);
    }
}

void cinfo_init(ControlInfo *pInfo)
{
    if (pInfo != NULL)
    {
        vinfo_init(&(pInfo->visualInfo));
        evt_init(&(pInfo->ClickEvent));
        pInfo->pChildFe = NULL;
    }
}

void cinfo_destroy(ControlInfo *pInfo)
{
    if (pInfo != NULL)
    {
        vinfo_destroy(&(pInfo->visualInfo));
        evt_destroy(&(pInfo->ClickEvent));
        // don't free child because it doesn't create it
        // free(pInfo->pChildFe)
    }
}

void cinfo_sub_evt(ControlInfo *pInfo, EventCallback fnCallback, char *pEvtArg, size_t iArgSize)
{
    if (pInfo != NULL)
        evt_subscribe_ex(&(pInfo->ClickEvent), fnCallback, pEvtArg, iArgSize);
}

void cinfo_raise_evt(ControlInfo *pInfo)
{
    if (pInfo != NULL)
        evt_notify(&(pInfo->ClickEvent));
}

void sinfo_init(StaticInfo *pInfo)
{
    if (pInfo != NULL)
        vinfo_init(&(pInfo->visualInfo));
}

void sinfo_destroy(StaticInfo *pInfo)
{
    if (pInfo != NULL)
        vinfo_destroy(&(pInfo->visualInfo));
}

void dinfo_init(DecoratorInfo *pInfo)
{
    if (pInfo != NULL)
    {
        vinfo_init(&(pInfo->visualInfo));
        pInfo->pChildFe = NULL;
    }
}

void dinfo_destroy(DecoratorInfo *pInfo)
{
    if (pInfo != NULL)
    {
        vinfo_destroy(&(pInfo->visualInfo));
    }
}

void dinfo_set_child(DecoratorInfo *pInfo, FrameworkElement *pChildFe)
{
    if (pInfo != NULL)
        pInfo->pChildFe = pChildFe;
}

FrameworkElement* dinfo_get_child(DecoratorInfo *pInfo)
{
    if (pInfo != NULL)
        return pInfo->pChildFe;
    return NULL;
}
