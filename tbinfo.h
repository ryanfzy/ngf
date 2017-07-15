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

void tb_sub_textchanged_evt(FrameworkElement *pFe, EventCallback fnCallback, char *pEvtArg, size_t iArgSize);
void tb_raise_textchanged_evt(FrameworkElement *pFe);

#endif
