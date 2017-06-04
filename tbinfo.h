#ifndef TEXTBLOCKINFO_H
#define TEXTBLOCKINFO_H

#include <stdio.h>
#include <stdlib.h>
#include "feinfo.h"
#include "property.h"

typedef struct _textBlockInfo
{
    StaticInfo staticInfo;
    HorizontalAlignmentType eContentHorizontal;
    PropertyInfo *pTextProperty;
} TextBlockInfo;

FrameworkElement* textblock_create();
FrameworkElement* textblock_create_ex(int x, int y, int width, int height, PropertyInfo *pTextProp);

#endif
