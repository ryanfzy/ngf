#ifndef TVITEMINFO_H
#define TVITEMINFO_H

#include <stdio.h>
#include <stdlib.h>
#include "feinfo.h"

typedef struct _tvItemInfo
{
    ControlInfo controlInfo;
    FrameworkElement *pHeaderFe;
    bool bExpanded;
} TvItemInfo;

FrameworkElement* tvitem_create();
FrameworkElement* tvitem_create_ex(int x, int y, int width, int height, FrameworkElement *pHeaderFe);

//FrameworkElement* tvitem_set_child(FrameworkElement *pItem, FrameworkElement *pChild);
#endif
