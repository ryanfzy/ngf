#ifndef STACKPANELINFO_H
#define STACKPANELINFO_H

#include <stdio.h>
#include <stdlib.h>
#include "feinfo.h"

typedef struct _stackPanelInfo
{
    LayoutInfo layoutInfo;
    OrientationType eOrientation;
} StackPanelInfo;

FrameworkElement* stackpanel_create();
FrameworkElement* statckpanel_create_ex(int x, int y, int width, int height);

#endif
