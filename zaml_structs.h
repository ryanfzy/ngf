#ifndef ZAMLSTRUCTS_H
#define ZAMLSTRUCTS_H

#include <stdbool.h>
#include "lib/list.h"
#include "datacontext.h"
#include "property.h"

static const int FE_WINDOW = 1;
static const int FE_GRID = 2;
static const int FE_BUTTON = 3;
static const int FE_TEXTBLOCK = 4;
static const int FE_BORDER = 5;
static const int FE_TREEVIEW = 6;
static const int FE_TVITEM = 7;

static const int SIZE_AUTO = -1;

typedef enum _horizontalAlignmentType
{
    HorizontalAlignment_Left,
    HorizontalAlignment_Center,
    HorizontalAlignment_Right,
} HorizontalAlignmentType;

typedef struct _frameworkElement
{
    int iType;
    char *pElement;
} FrameworkElement;

/*
typedef struct _propertInfo
{
    bool bIsBinding;
    char *szBindingKey;
    int iValue;
    int szValue;
} PropertyInfo;
*/

typedef struct _visualInfo
{
    int x;
    int y;
    int width;
    int height;
} VisualInfo;

typedef struct _layoutInfo
{
    VisualInfo visualInfo;
    DataContext *pDc;
    SList children;
} LayoutInfo;

typedef struct _controlInfo
{
    VisualInfo visualInfo;
    long iId;
    DataContext *pDc;
    FrameworkElement child;
} ControlInfo;

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

typedef struct _treeViewInfo
{
    LayoutInfo layoutInfo;
} TreeViewInfo;

typedef struct _tvItemInfo
{
    ControlInfo controlInfo;
    FrameworkElement *pHeaderFe;
    bool bExpanded;
} TvItemInfo;

FrameworkElement* zaml_create_grid();
FrameworkElement* zaml_create_grid_ex(int x, int y, int width, int height, DataContext *pDc);

FrameworkElement* zaml_create_button();
FrameworkElement* zaml_create_button_ex(int x, int y, int width, int height, wchar_t *szContent, wchar_t *szCommand, DataContext *pDc);

FrameworkElement* zaml_create_textblock();
FrameworkElement* zaml_create_textblock_ex(int x, int y, int width, int height, PropertyInfo *pTextProp);

FrameworkElement* zaml_create_border();
FrameworkElement* zaml_create_border_ex(int x, int y, int width, int height, DataContext *pDc);

FrameworkElement* zaml_create_treeview();
FrameworkElement* zaml_create_treeview_ex(int x, int y, int width, int height, DataContext *pDc);

FrameworkElement* zaml_create_tvitem();
FrameworkElement* zaml_create_tvitem_ex(int x, int y, int width, int height, FrameworkElement *pHeaderFe, DataContext *pDc);

bool zaml_add_child(FrameworkElement *pParentFe, FrameworkElement *pChildFe);
int zaml_get_children_count(FrameworkElement *pFe);
FrameworkElement* zaml_get_child(FrameworkElement *pFe, int iPos);

#endif
