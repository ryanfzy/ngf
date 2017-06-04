#ifndef TREEVIEWINFO_H
#define TREEVIEWINFO_H

#include <stdio.h>
#include <stdlib.h>
#include "feinfo.h"

typedef struct _treeViewInfo
{
    LayoutInfo layoutInfo;
} TreeViewInfo;

FrameworkElement* treeview_create();
FrameworkElement* treeview_create_ex(int x, int y, int width, int height, DataContext *pDc);

void treeview_add_item(FrameworkElement *pTree, FrameworkElement *pItem);

#endif
