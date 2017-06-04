#include <stdlib.h>
#include <stdio.h>
#include "zaml_structs.h"
#include "datacontext.h"
#include <windows.h>

static LayoutInfo* get_layoutinfo(FrameworkElement *pFe)
{
    LayoutInfo *pLayoutInfo = NULL;
    if (pFe->iType == FE_GRID)
    {
        GridInfo *pGridInfo = (GridInfo*)(pFe->pElement);
        pLayoutInfo = &(pGridInfo->layoutInfo);
    }
    else if (pFe->iType == FE_BORDER)
    {
        BorderInfo *pBorderInfo = (BorderInfo*)(pFe->pElement);
        pLayoutInfo = &(pBorderInfo->layoutInfo);
    }
    else if (pFe->iType == FE_TREEVIEW)
    {
        TreeViewInfo *pInfo = (TreeViewInfo*)(pFe->pElement);
        pLayoutInfo = &(pInfo->layoutInfo);
    }
    return pLayoutInfo;
}

bool zaml_add_child(FrameworkElement *pParentFe, FrameworkElement *pChildFe)
{
    if (pParentFe != NULL && pChildFe != NULL)
    {
        LayoutInfo *pLayoutInfo = get_layoutinfo(pParentFe);
        if (pLayoutInfo != NULL)
        {
            slist_push(&(pLayoutInfo->children), (char*)(&pChildFe), sizeof(pChildFe));
            return true;
        }
    }
    return false;
}

int zaml_get_children_count(FrameworkElement *pFe)
{
    if (pFe != NULL)
    {
        LayoutInfo *pLayoutInfo = get_layoutinfo(pFe);
        if (pLayoutInfo != NULL)
            return slist_get_count(&(pLayoutInfo->children));
    }
    return 0;
}

FrameworkElement* zaml_get_child(FrameworkElement *pFe, int iPos)
{
    if (pFe != NULL)
    {
        LayoutInfo *pLayoutInfo = get_layoutinfo(pFe);
        if (pLayoutInfo != NULL)
        {
            FrameworkElement **ppFe = (FrameworkElement**)slist_get(&(pLayoutInfo->children), iPos);
            return *ppFe;
        }
    }
    return NULL;
}

