#include "tviteminfo.h"

FrameworkElement* tvitem_create()
{
    FrameworkElement *pFe = create_fe(FE_TVITEM);

    TvItemInfo *pInfo = malloc(sizeof(TvItemInfo));
    pInfo->bExpanded = false;
    pFe->pElement = (char*)pInfo;

    return pFe;
}

FrameworkElement* tvitem_create_ex(int x, int y, int width, int height, FrameworkElement *pHeaderFe, DataContext *pDc)
{
    FrameworkElement *pFe = tvitem_create();
    TvItemInfo *pInfo = (TvItemInfo*)(pFe->pElement);

    pInfo->controlInfo.visualInfo.x = x;
    pInfo->controlInfo.visualInfo.y = y;
    pInfo->controlInfo.visualInfo.width = width;
    pInfo->controlInfo.visualInfo.height = height;
    pInfo->controlInfo.pDc = pDc;
    pInfo->pHeaderFe = pHeaderFe;

    return pFe;
}

