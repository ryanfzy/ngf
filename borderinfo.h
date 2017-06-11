#ifndef BORDERINFO_H
#define BORDERINFO_H

#include <stdio.h>
#include <stdlib.h>
#include "feinfo.h"

typedef struct _borderInfo
{
    //LayoutInfo layoutInfo;
    DecoratorInfo decoratorInfo;
} BorderInfo;

FrameworkElement* border_create();
FrameworkElement* border_create_ex(int x, int y, int width, int height);

FeSize border_get_size(FrameworkElement *pFe);

#endif
