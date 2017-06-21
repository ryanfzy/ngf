#ifndef BUTTONINFO_H
#define BUTTONINFO_H

#include <stdio.h>
#include <stdlib.h>
#include "feinfo.h"

typedef struct _buttonInfo
{
    ControlInfo controlInfo;
    wchar_t *szContent;
    wchar_t *szCommand;
} ButtonInfo;

FrameworkElement* button_create();
FrameworkElement* button_create_ex(int x, int y, int width, int height, wchar_t *szContent, wchar_t *szCommand, DataContext *pDc);

FeSize button_get_size(FrameworkElement *pBtn);
FePos button_get_pos(FrameworkElement *pFe);
void button_set_pos(FrameworkElement *pBtn, FePos pos);

#endif
