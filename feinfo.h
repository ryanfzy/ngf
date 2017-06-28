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

typedef struct _fePos
{
    int x;
    int y;
} FePos;

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
    FrameworkElement *pChildFe;
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
bool linfo_add_child(LayoutInfo *pInfo, FrameworkElement *pChildFe);
int linfo_get_children_count(LayoutInfo *pInfo);
FrameworkElement* linfo_get_child(LayoutInfo *pInfo, unsigned int iPos);
bool linfo_has_child(LayoutInfo *pInfo, FrameworkElement *pChildFe);

void vinfo_init(VisualInfo *pInfo);
void vinfo_init_ex(VisualInfo *pInfo, int x, int y, int width, int height);
void vinfo_destroy(VisualInfo *pInfo);
FeSize vinfo_get_size(VisualInfo *pInfo);
void vinfo_set_size(VisualInfo *pInfo, FeSize feSize);
void vinfo_set_pos(VisualInfo *pInfo, FePos fePos);
FePos vinfo_get_pos(VisualInfo *pInfo);

void cinfo_init(ControlInfo *pInfo);
void cinfo_destroy(ControlInfo *pInfo);

void sinfo_init(StaticInfo *pInfo);
void sinfo_destroy(StaticInfo *pInfo);

void dinfo_init(DecoratorInfo *pInfo);
void dinfo_destroy(DecoratorInfo *pInfo);
void dinfo_set_child(DecoratorInfo *pInfo, FrameworkElement *pChildFe);
FrameworkElement* dinfo_get_child(DecoratorInfo *pInfo);

#endif
