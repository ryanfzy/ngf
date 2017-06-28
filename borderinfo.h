#ifndef BORDERINFO_H
#define BORDERINFO_H

#include <stdio.h>
#include <stdlib.h>
#include "feinfo.h"

typedef struct _borderInfo
{
    DecoratorInfo decoratorInfo;
} BorderInfo;

FrameworkElement* border_create();
FrameworkElement* border_create_ex(int x, int y, int width, int height);
void border_free(FrameworkElement *pFe);

FeSize border_get_size(FrameworkElement *pFe);

FePos border_get_pos(FrameworkElement *pFe);
void border_set_pos(FrameworkElement*pFe, FePos pos);

#endif
