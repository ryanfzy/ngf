#ifndef BORDERINFO_H
#define BORDERINFO_H

#include <stdio.h>
#include <stdlib.h>
#include "feinfo.h"

typedef struct _borderInfo
{
    LayoutInfo layoutInfo;
} BorderInfo;

FrameworkElement* border_create();
FrameworkElement* border_create_ex(int x, int y, int width, int height, DataContext *pDc);

#endif
