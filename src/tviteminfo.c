#include "tviteminfo.h"

FrameworkElement* tvitem_create()
{
    FrameworkElement *pFe = create_fe(FE_TVITEM);

    TvItemInfo *pInfo = malloc(sizeof(TvItemInfo));
    pInfo->bExpanded = false;
    pFe->pElement = (char*)pInfo;

    return pFe;
}

FrameworkElement* tvitem_create_ex(int x, int y, int width, int height, FrameworkElement *pHeaderFe)
{
    FrameworkElement *pFe = tvitem_create();
    TvItemInfo *pInfo = (TvItemInfo*)(pFe->pElement);

    pInfo->controlInfo.visualInfo.x = x;
    pInfo->controlInfo.visualInfo.y = y;
    pInfo->controlInfo.visualInfo.width = width;
    pInfo->controlInfo.visualInfo.height = height;
    pInfo->pHeaderFe = pHeaderFe;

    return pFe;
}

/*
FrameworkElement* tvitem_set_child(FrameworkElement *pItem, FrameworkElement *pChild)
{
    FrameworkElement *pOldChild = NULL;
    if (pItem != NULL && pChild != NULL)
    {
        if (pItem->iType == FE_TVITEM && pChild->iType == FE_TREEVIEW)
        {
            TvItemInfo *pItemInfo = (TvItemInfo*)(pItem->pElement);
            TreeViewInof *pChildInfo = (TreeViewInfo*)(pChild->pElement);
            if (pItemInfo != NULL && pChildInfo != NULL)
            {
                pOldChild = pItemInfo->controlInfo.pChild;
                pItemInfo->controlInfo.pChild = pChild;
            }
        }
    }
    return pOldChild;
}
*/
