#include "btninfo.h"

static bool _button_is_valid(FrameworkElement *pBtn)
{
    return pBtn != NULL && pBtn->iType == FE_BUTTON && pBtn->pElement != NULL;
}

static ButtonInfo* _button_getinfo(FrameworkElement *pBtn)
{
    if (_button_is_valid(pBtn))
        return (ButtonInfo*)(pBtn->pElement);
    return NULL;
}

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

    vinfo_init_ex(&(pButtonInfo->controlInfo.visualInfo), x, y, width, height);
    pButtonInfo->szContent = szContent;
    //pButtonInfo->controlInfo.pDc = pDc;
    pButtonInfo->szCommand = szCommand;

    return pBtnFe;
}

FeSize button_get_size(FrameworkElement *pBtn)
{
    FeSize size = {0, 0};
    ButtonInfo *pInfo = _button_getinfo(pBtn);
    if (pInfo != NULL)
        size = vinfo_get_size(&(pInfo->controlInfo.visualInfo));
    return size;
}

void button_set_pos(FrameworkElement *pFe, FePos pos)
{
    ButtonInfo *pInfo = _button_getinfo(pFe);
    if (pInfo != NULL)
        vinfo_set_pos(&(pInfo->controlInfo.visualInfo), pos);
}

FePos button_get_pos(FrameworkElement *pFe)
{
    FePos pos = {0, 0};
    ButtonInfo *pInfo = _button_getinfo(pFe);
    if (pInfo != NULL)
        pos = vinfo_get_pos(&(pInfo->controlInfo.visualInfo));
    return pos;
}
