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
    // todo, make it not a pointer
    PropertyInfo *pTextProperty;
} TextBlockInfo;

FrameworkElement* textblock_create();
FrameworkElement* textblock_create_ex(int x, int y, int width, int height, PropertyInfo *pTextProp);

FeSize textblock_get_size(FrameworkElement *pTb);
void textblock_set_pos(FrameworkElement *pFe, FePos fePos);
FePos textblock_get_pos(FrameworkElement *pFe);

#endif
