#ifndef FEINFO_H
#define FEINFO_H

#include <stdio.h>
#include <stdlib.h>
#include "lib/list.h"
#include "datacontext.h"

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

typedef struct _staticInfo
{
    VisualInfo visualInfo;
    DataContext *pDc;
} StaticInfo;

FrameworkElement* create_fe(int iFeType);
void InitLayoutInfo(LayoutInfo *pInfo);

#endif
