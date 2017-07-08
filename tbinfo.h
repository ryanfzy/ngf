#ifndef TEXTBLOCKINFO_H
#define TEXTBLOCKINFO_H

#include <stdio.h>
#include <stdlib.h>
#include "feinfo.h"
#include "property.h"

typedef struct _textBlockInfo
{
    StaticInfo staticInfo;
    PropertyInfo ContentHorizontalProperty;
    PropertyInfo TextProperty;
} TextBlockInfo;

FrameworkElement* textblock_create();
FrameworkElement* textblock_create_ex(int x, int y, int width, int height);

void textblock_free(FrameworkElement *pFe);

void textblock_set_contenthorizontal(FrameworkElement *pFe, HorizontalAlignmentType eType);
HorizontalAlignmentType textblock_get_contenthorizontal(FrameworkElement *pFe);

void textblock_set_text(FrameworkElement *pFe, wchar_t *szText);
wchar_t* textblock_get_text(FrameworkElement *pFe);
void textblock_bind_text(FrameworkElement *pFe, StrItem *pItem);

FeSize textblock_get_size(FrameworkElement *pTb);
void textblock_set_pos(FrameworkElement *pFe, FePos fePos);
FePos textblock_get_pos(FrameworkElement *pFe);

#endif
