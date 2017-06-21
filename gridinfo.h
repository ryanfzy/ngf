#ifndef GRIDINFO_H
#define GRIDINFO_H

#include <stdio.h>
#include <stdlib.h>
#include "lib/list.h"
#include "feinfo.h"
#include "attachedproperty.h"

typedef struct _gridInfo
{
    LayoutInfo layoutInfo;
    AttachedPropertyInfo ColProperty;
    AttachedPropertyInfo RowProperty;
    AttachedPropertyInfo ColSpanProperty;
    AttachedPropertyInfo RowSpanProperty;
    SList RowDefinitions;
    SList ColumnDefinitions;
} GridInfo;

typedef struct _rowDefinition
{
    int height;
    FrameworkElement *pFe;
} RowDefinition;

typedef struct _columnDefinition
{
    int width;
    FrameworkElement *pFe;
} ColumnDefinition;

FrameworkElement* grid_create();
FrameworkElement* grid_create_ex(int x, int y, int width, int height);
void grid_free(FrameworkElement *pFe);

void grid_add_child(FrameworkElement *pGridFe, FrameworkElement *pChildFe);
int grid_get_children_count(FrameworkElement *pGrid);
FrameworkElement* grid_get_child(FrameworkElement *pGrid, unsigned int iPos);

FeSize grid_get_size(FrameworkElement *pGrid);

FePos grid_get_pos(FrameworkElement *pFe);
void grid_set_pos(FrameworkElement *pFe, FePos);

void grid_add_rowdef(FrameworkElement *pGrid, RowDefinition *pRowDef);
void grid_add_coldef(FrameworkElement *pGrid, ColumnDefinition *pColDef);

int grid_get_rowdef_count(FrameworkElement *pGrid);
int grid_get_coldef_count(FrameworkElement *pGrid);

RowDefinition* grid_get_rowdef(FrameworkElement *pGrid, unsigned int iPos);
ColumnDefinition* grid_get_coldef(FrameworkElement *pGrid, unsigned int iPos);

void grid_set_col(FrameworkElement *pGrid, FrameworkElement *pChild, int iCol);
int grid_get_col(FrameworkElement *pGrid, FrameworkElement *pChild);

void grid_set_colspan(FrameworkElement *pGrid, FrameworkElement *pChild, int iColSpan);
int grid_get_colspan(FrameworkElement *pGrid, FrameworkElement *pChild);

void grid_set_row(FrameworkElement *pGrid, FrameworkElement *pChild, int iRow);
int grid_get_row(FrameworkElement *pGrid, FrameworkElement *pChild);

void grid_set_rowspan(FrameworkElement *pGrid, FrameworkElement *pChild, int iRowSpan);
int grid_get_rowspan(FrameworkElement *pGrid, FrameworkElement *pChild);

void rowdef_set_height(RowDefinition *pRowDef, int height);
void coldef_set_width(ColumnDefinition *pColDef, int width);

#endif
