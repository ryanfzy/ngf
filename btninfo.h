#ifndef BUTTONINFO_H
#define BUTTONINFO_H

#include <stdio.h>
#include <stdlib.h>
#include "feinfo.h"
#include "types.h"
#include "evtinfo.h"
#include "property.h"

typedef struct _buttonInfo
{
    ControlInfo controlInfo;
    PropertyInfo TextProperty;
    PropertyInfo CommandProperty;
} ButtonInfo;

typedef struct _clickEventArg
{
} ClickEventArg;

FrameworkElement* button_create();
FrameworkElement* button_create_ex(int x, int y, int width, int height, wchar_t *szContent);

void button_free(FrameworkElement *pFe);

void button_set_text(FrameworkElement *pFe, wchar_t *szText);
wchar_t* button_get_text(FrameworkElement *pFe);

CommandFn button_get_command(FrameworkElement *pFe);
void button_bind_command(FrameworkElement *pFe, CmdItem *pItem);

FeSize button_get_size(FrameworkElement *pBtn);
FePos button_get_pos(FrameworkElement *pFe);
void button_set_pos(FrameworkElement *pBtn, FePos pos);

void btn_sub_click_evt(FrameworkElement *pFe, EventCallback fnCallback);
void btn_raise_click_evt(FrameworkElement *pFe);

#endif
