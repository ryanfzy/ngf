#ifndef BUTTONINFO_H
#define BUTTONINFO_H

#include <stdio.h>
#include <stdlib.h>
#include "feinfo.h"
#include "types.h"
#include "evtinfo.h"

typedef struct _buttonInfo
{
    ControlInfo controlInfo;
    wchar_t *szContent;
    wchar_t *szCommand;
    //PropertyInfo CommandProperty;
} ButtonInfo;

typedef struct _clickEventArg
{
} ClickEventArg;

FrameworkElement* button_create();
FrameworkElement* button_create_ex(int x, int y, int width, int height, wchar_t *szContent, wchar_t *szCommand, DataContext *pDc);

FeSize button_get_size(FrameworkElement *pBtn);
FePos button_get_pos(FrameworkElement *pFe);
void button_set_pos(FrameworkElement *pBtn, FePos pos);

void btn_sub_click_evt(FrameworkElement *pFe, EventCallback fnCallback);
void btn_raise_click_evt(FrameworkElement *pFe);

#endif
