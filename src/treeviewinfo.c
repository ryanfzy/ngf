#include "treeviewinfo.h"
#include "tviteminfo.h"

FrameworkElement* treeview_create()
{
    FrameworkElement *pFe = create_fe(FE_TREEVIEW);

    TreeViewInfo *pInfo = malloc(sizeof(TreeViewInfo));
    linfo_init(&(pInfo->layoutInfo));
    pFe->pElement = (char*)pInfo;

    return pFe;
}

FrameworkElement* treeview_create_ex(int x, int y, int width, int height)
{
    FrameworkElement *pFe = treeview_create();
    TreeViewInfo *pInfo = (TreeViewInfo*)(pFe->pElement);

    pInfo->layoutInfo.visualInfo.x = x;
    pInfo->layoutInfo.visualInfo.y = y;
    pInfo->layoutInfo.visualInfo.width = width;
    pInfo->layoutInfo.visualInfo.height = height;

    return pFe;
}

/*
void treeview_add_item(FrameworkElement *pTree, FrameworkElement *pItem)
{
    if (pTree != NULL && pItem != NULL)
    {
        if (pTree->iType == FE_TREEVIEW && pTree->pElement != NULL && pItem->iType == FE_TVITEM && pItem->pElement != NULL)
        {
            TreeViewInfo *pTreeInfo = (TreeViewInfo*)(pTree->pElement);

            TvItemInfo *pItemInfo = (TvItemInfo*)(pItem->pElement);
            pItemInfo->controlInfo.visualInfo.y = pTreeInfo->layoutInfo.visualInfo.height;

            pTreeInfo->layoutInfo.visualInfo.height += pItemInfo->controlInfo.visualInfo.height + 5;
            pTreeInfo->layoutInfo.visualInfo.width = pTreeInfo->layoutInfo.visualInfo.width > pItemInfo->controlInfo.visualInfo.width 
                                          ? pTreeInfo->layoutInfo.visualInfo.width : pItemInfo->controlInfo.visualInfo.width;

            slist_push(&(pTreeInfo->layoutInfo.children), (char*)(&pItem), sizeof(pItem));
        }
    }
}
*/
