#ifndef ZAMLSTRUCTS_H
#define ZAMLSTRUCTS_H

#include <stdbool.h>
#include "lib/list.h"
#include "datacontext.h"
#include "property.h"
#include "feinfo.h"
#include "treeviewinfo.h"
#include "tviteminfo.h"

typedef struct _windowInfo
{
} WindowInfo;

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

typedef struct _buttonInfo
{
    ControlInfo controlInfo;
    wchar_t *szContent;
    wchar_t *szCommand;
} ButtonInfo;

typedef struct _staticInfo
{
    VisualInfo visualInfo;
    DataContext *pDc;
} StaticInfo;

typedef struct _textBlockInfo
{
    StaticInfo staticInfo;
    HorizontalAlignmentType eContentHorizontal;
    PropertyInfo *pTextProperty;
} TextBlockInfo;

typedef struct _borderInfo
{
    LayoutInfo layoutInfo;
} BorderInfo;

FrameworkElement* zaml_create_grid();
FrameworkElement* zaml_create_grid_ex(int x, int y, int width, int height, DataContext *pDc);

FrameworkElement* zaml_create_button();
FrameworkElement* zaml_create_button_ex(int x, int y, int width, int height, wchar_t *szContent, wchar_t *szCommand, DataContext *pDc);

FrameworkElement* zaml_create_textblock();
FrameworkElement* zaml_create_textblock_ex(int x, int y, int width, int height, PropertyInfo *pTextProp);

FrameworkElement* zaml_create_border();
FrameworkElement* zaml_create_border_ex(int x, int y, int width, int height, DataContext *pDc);

bool zaml_add_child(FrameworkElement *pParentFe, FrameworkElement *pChildFe);
int zaml_get_children_count(FrameworkElement *pFe);
FrameworkElement* zaml_get_child(FrameworkElement *pFe, int iPos);

#endif
