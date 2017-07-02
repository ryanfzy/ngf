#include "btninfo.h"

static bool _btn_is_valid(FrameworkElement *pBtn)
{
    return pBtn != NULL && pBtn->iType == FE_BUTTON && pBtn->pElement != NULL;
}

static ButtonInfo* _btn_getinfo(FrameworkElement *pBtn)
{
    if (_btn_is_valid(pBtn))
        return (ButtonInfo*)(pBtn->pElement);
    return NULL;
}

static void _btn_click_evt_callback(FrameworkElement *pFe, char *pClickEventArg)
{
    ButtonInfo *pInfo = _btn_getinfo(pFe);
    if (pInfo != NULL)
    {
    }
}

FrameworkElement* button_create()
{
    FrameworkElement *pFe = create_fe(FE_BUTTON);

    ButtonInfo *pInfo = malloc(sizeof(ButtonInfo));
    cinfo_init(&(pInfo->controlInfo));
    cinfo_sub_evt(&(pInfo->controlInfo), EventType_Click, _btn_click_evt_callback);

    pFe->pElement = (char*)pInfo;

    return pFe;
}

FrameworkElement* button_create_ex(int x, int y, int width, int height, wchar_t *szContent, wchar_t *szCommand, DataContext *pDc)
{
    FrameworkElement *pBtnFe = button_create();
    ButtonInfo *pButtonInfo = (ButtonInfo*)(pBtnFe->pElement);

    vinfo_init_ex(&(pButtonInfo->controlInfo.visualInfo), x, y, width, height);
    pButtonInfo->szContent = szContent;
    //pButtonInfo->controlInfo.pDc = pDc;
    //pButtonInfo->szCommand = szCommand;

    return pBtnFe;
}

FeSize button_get_size(FrameworkElement *pBtn)
{
    FeSize size = {0, 0};
    ButtonInfo *pInfo = _btn_getinfo(pBtn);
    if (pInfo != NULL)
        size = vinfo_get_size(&(pInfo->controlInfo.visualInfo));
    return size;
}

void button_set_pos(FrameworkElement *pFe, FePos pos)
{
    ButtonInfo *pInfo = _btn_getinfo(pFe);
    if (pInfo != NULL)
        vinfo_set_pos(&(pInfo->controlInfo.visualInfo), pos);
}

FePos button_get_pos(FrameworkElement *pFe)
{
    FePos pos = {0, 0};
    ButtonInfo *pInfo = _btn_getinfo(pFe);
    if (pInfo != NULL)
        pos = vinfo_get_pos(&(pInfo->controlInfo.visualInfo));
    return pos;
}

void btn_sub_click_evt(FrameworkElement *pFe, EventCallback fnCallback)
{
    ButtonInfo *pInfo = _btn_getinfo(pFe);
    if (pInfo != NULL)
        cinfo_sub_evt(&(pInfo->controlInfo), EventType_Click, fnCallback);
}

static void _btn_raise_evt(FrameworkElement *pFe, EventType eType, char *pEvtArg)
{
    ButtonInfo *pInfo = _btn_getinfo(pFe);
    if (pInfo != NULL)
        cinfo_raise_evt(&(pInfo->controlInfo), eType, pFe, pEvtArg);
}

void btn_raise_click_evt(FrameworkElement *pFe)
{
    ButtonInfo *pInfo = _btn_getinfo(pFe);
    if (pInfo != NULL)
    {
        ClickEventArg arg;
        _btn_raise_evt(pFe, EventType_Click, (char*)&arg);
    }
}
