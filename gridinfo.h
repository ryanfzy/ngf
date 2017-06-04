#ifndef GRIDINFO_H
#define GRIDINFO_H

#include <stdio.h>
#include <stdlib.h>
#include "feinfo.h"

typedef struct _gridInfo
{
    LayoutInfo layoutInfo;
    //SList rowDefinitions;
    //SList columnDefinitions;
} GridInfo;

typedef struct _rowDefinition
{
    VisualInfo visualInfo;
} RowDefinition;

typedef struct _columnDefinition
{
    VisualInfo visualInfo;
} ColumnDefinition;

FrameworkElement* grid_create();
FrameworkElement* grid_create_ex(int x, int y, int width, int height, DataContext *pDc);

#endif
