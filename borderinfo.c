#include "borderinfo.h"

FrameworkElement* border_create()
{
    FrameworkElement *pBdrFe = create_fe(FE_BORDER);

    BorderInfo *pBorderInfo = malloc(sizeof(BorderInfo));
    InitLayoutInfo(&(pBorderInfo->layoutInfo));
    pBdrFe->pElement = (char*)pBorderInfo;

    return pBdrFe;
}

FrameworkElement* border_create_ex(int x, int y, int width, int height, DataContext *pDc)
{
    FrameworkElement *pBdrFe = border_create();
    BorderInfo* pBorderInfo = (BorderInfo*)(pBdrFe->pElement);

    pBorderInfo->layoutInfo.visualInfo.x = x;
    pBorderInfo->layoutInfo.visualInfo.y = y;
    pBorderInfo->layoutInfo.visualInfo.width = width;
    pBorderInfo->layoutInfo.visualInfo.height = height;
    pBorderInfo->layoutInfo.pDc = pDc;

    return pBdrFe;
}

