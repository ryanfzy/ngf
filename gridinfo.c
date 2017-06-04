#include "gridinfo.h"

FrameworkElement* grid_create()
{
    FrameworkElement *pGridFe = create_fe(FE_GRID);
    
    GridInfo *pGridInfo = malloc(sizeof(GridInfo));
    InitLayoutInfo(&(pGridInfo->layoutInfo));
    pGridFe->pElement = (char*)pGridInfo;

    return pGridFe;
}

FrameworkElement* grid_create_ex(int x, int y, int width, int height, DataContext *pDc)
{
    FrameworkElement *pGridFe = grid_create();
    GridInfo *pGridInfo = (GridInfo*)(pGridFe->pElement);

    pGridInfo->layoutInfo.visualInfo.x = x;
    pGridInfo->layoutInfo.visualInfo.y = y;
    pGridInfo->layoutInfo.visualInfo.width = width;
    pGridInfo->layoutInfo.visualInfo.height = height;
    pGridInfo->layoutInfo.pDc = pDc;

    return pGridFe;
}

