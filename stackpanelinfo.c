#include "stackpanelinfo.h"

FrameworkElement* stackpanel_create()
{
    FrameworkElement *pFe = create_fe(FE_STACKPANEL);

    StackPanelInfo *pInfo = malloc(sizeof(StackPanelInfo));
    pInfo->eOrientation = Orientation_Vertical;
    pFe->pElement = (char*)pInfo;

    return pFe;
}

FrameworkElement* stackpanel_create_ex(int x, int y, int width, int height)
{
    FrameworkElement *pFe = stackpanel_create();
    StackPanelInfo *pInfo = (StackPanelInfo*)(pFe->pElement);

    pInfo->layoutInfo.visualInfo.x = x;
    pInfo->layoutInfo.visualInfo.y = y;
    pInfo->layoutInfo.visualInfo.width = width;
    pInfo->layoutInfo.visualInfo.height = height;

    return pFe;
}
