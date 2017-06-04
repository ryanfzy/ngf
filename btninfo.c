#include "btninfo.h"

FrameworkElement* button_create()
{
    FrameworkElement *pBtnFe = create_fe(FE_BUTTON);

    ButtonInfo *pButtonInfo = malloc(sizeof(ButtonInfo));
    pBtnFe->pElement = (char*)pButtonInfo;

    return pBtnFe;
}

FrameworkElement* button_create_ex(int x, int y, int width, int height, wchar_t *szContent, wchar_t *szCommand, DataContext *pDc)
{
    FrameworkElement *pBtnFe = button_create();
    ButtonInfo *pButtonInfo = (ButtonInfo*)(pBtnFe->pElement);

    pButtonInfo->controlInfo.visualInfo.x = x;
    pButtonInfo->controlInfo.visualInfo.y = y;
    pButtonInfo->controlInfo.visualInfo.width = width;
    pButtonInfo->controlInfo.visualInfo.height = height;
    pButtonInfo->szContent = szContent;
    pButtonInfo->controlInfo.pDc = pDc;
    pButtonInfo->szCommand = szCommand;

    return pBtnFe;
}

