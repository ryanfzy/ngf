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

static void _btn_click_evt_callback(FrameworkElement *pFe, char *pClickEvtArg)
{
    ButtonInfo *pInfo = _btn_getinfo(pFe);
    if (pInfo != NULL)
    {
        CommandFn fnCommand = button_get_command(pFe);
        if (fnCommand != NULL)
            fnCommand(button_get_cmdparameter(pFe));
    }
}

static void _btn_init(ButtonInfo *pInfo)
{
    if (pInfo != NULL)
    {
        cinfo_init(&pInfo->controlInfo);
        cinfo_sub_evt(&(pInfo->controlInfo), EventType_Click, _btn_click_evt_callback);
        propinfo_init_str(&(pInfo->TextProperty), NULL);
        propinfo_init_cmd(&(pInfo->CommandProperty), NULL);
        propinfo_init_ptr(&(pInfo->CommandParameterProperty), NULL);
    }
}

static void _btn_destroy(ButtonInfo *pInfo)
{
    if (pInfo != NULL)
    {
        cinfo_destroy(&(pInfo->controlInfo));
        propinfo_destroy(&(pInfo->TextProperty));
        propinfo_destroy(&(pInfo->CommandProperty));
        propinfo_destroy(&(pInfo->CommandParameterProperty));
    }
}

FrameworkElement* button_create()
{
    FrameworkElement *pFe = create_fe(FE_BUTTON);

    ButtonInfo *pInfo = malloc(sizeof(ButtonInfo));
    _btn_init(pInfo);

    pFe->pElement = (char*)pInfo;

    return pFe;
}

FrameworkElement* button_create_ex(int x, int y, int width, int height, wchar_t *szContent)
{
    FrameworkElement *pFe = button_create();
    ButtonInfo *pInfo = (ButtonInfo*)(pFe->pElement);

    vinfo_init_ex(&(pInfo->controlInfo.visualInfo), x, y, width, height);
    button_set_text(pFe, szContent);

    return pFe;
}

void button_free(FrameworkElement *pFe)
{
    ButtonInfo *pInfo = _btn_getinfo(pFe);
    if (pInfo != NULL)
    {
        _btn_destroy(pInfo);
        free(pInfo);
        fe_free(pFe);
    }
}

void button_set_text(FrameworkElement *pFe, wchar_t *szText)
{
    ButtonInfo *pInfo = _btn_getinfo(pFe);
    if (pInfo != NULL && szText != NULL)
        propinfo_set(&(pInfo->TextProperty), (char*)szText);
}

CommandFn button_get_command(FrameworkElement *pFe)
{
    ButtonInfo *pInfo = _btn_getinfo(pFe);
    if (pInfo != NULL)
        return (CommandFn)propinfo_get(&(pInfo->CommandProperty));
    return NULL;
}

void button_bind_command(FrameworkElement *pFe, CmdItem *pItem)
{
    ButtonInfo *pInfo = _btn_getinfo(pFe);
    if (pInfo != NULL && pItem != NULL)
        propinfo_bind(&(pInfo->CommandProperty), &(pItem->item));
}

void button_set_cmdparameter(FrameworkElement *pFe, char *pDataPtr)
{
    ButtonInfo *pInfo = _btn_getinfo(pFe);
    if (pInfo != NULL && pDataPtr != NULL)
        propinfo_set(&(pInfo->CommandParameterProperty), pDataPtr);
}

char* button_get_cmdparameter(FrameworkElement *pFe)
{
    ButtonInfo *pInfo = _btn_getinfo(pFe);
    if (pInfo != NULL)
        return propinfo_get(&(pInfo->CommandParameterProperty));
    return NULL;
}

wchar_t* button_get_text(FrameworkElement *pFe)
{
    ButtonInfo *pInfo = _btn_getinfo(pFe);
    if (pInfo != NULL)
        return (wchar_t*)propinfo_get(&(pInfo->TextProperty));
    return NULL;
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
