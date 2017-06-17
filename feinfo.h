#ifndef FEINFO_H
#define FEINFO_H

#include <stdio.h>
#include <stdlib.h>
#include <wchar.h>
#include "lib/list.h"
#include "lib/dict.h"
#include "datacontext.h"
#include "types.h"
#include "attachedproperty.h"

typedef struct _feSize
{
    int width;
    int height;
} FeSize;

typedef struct _frameworkElement
{
    int iType;
    char *pElement;
    Dict attachedProps;
} FrameworkElement;

typedef struct _visualInfo
{
    int x;
    int y;
    int width;
    int height;
    int actualWidth;
    int actualHeight;
} VisualInfo;

typedef struct _layoutInfo
{
    VisualInfo visualInfo;
    SList children;
} LayoutInfo;

typedef struct _controlInfo
{
    VisualInfo visualInfo;
    FrameworkElement *pchild;
} ControlInfo;

typedef struct _staticInfo
{
    VisualInfo visualInfo;
} StaticInfo;

typedef struct _decoratorInfo
{
    VisualInfo visualInfo;
    FrameworkElement *pChildFe;
} DecoratorInfo;

FrameworkElement* create_fe(int iFeType);
void fe_free(FrameworkElement *pFe);

void fe_set_value(FrameworkElement *pFe, AttachedPropertyInfo *pInfo, char *pValue);
bool fe_get_value(FrameworkElement *pFe, AttachedPropertyInfo *pInfo, char *pValue);

void linfo_init(LayoutInfo *pInfo);
void linfo_destroy(LayoutInfo *pInfo);
int linfo_get_children_count(LayoutInfo *pInfo);
FrameworkElement* linfo_get_child(LayoutInfo *pInfo, unsigned int iPos);

void vinfo_init(VisualInfo *pInfo);
void vinfo_destroy(VisualInfo *pInfo);
FeSize vinfo_get_size(VisualInfo *pInfo);

#endif
