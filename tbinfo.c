#include "tbinfo.h"

static bool _tb_is_valid(FrameworkElement *pFe)
{
    return pFe != NULL && pFe->iType == FE_TEXTBLOCK && pFe->pElement != NULL;
}

static TextBlockInfo* _tb_getinfo(FrameworkElement *pFe)
{
    if (_tb_is_valid(pFe))
        return (TextBlockInfo*)(pFe->pElement);
    return NULL;
}

void _tb_init(TextBlockInfo *pInfo)
{
    if (pInfo != NULL)
    {
        sinfo_init(&(pInfo->staticInfo));
        propinfo_init_int(&(pInfo->ContentHorizontalProperty), HorizontalAlignment_Left);
        propinfo_init_str(&(pInfo->TextProperty), NULL);
    }
}

void _tb_destroy(TextBlockInfo *pInfo)
{
    if (pInfo != NULL)
    {
        sinfo_destroy(&(pInfo->staticInfo));
        propinfo_destroy(&(pInfo->ContentHorizontalProperty));
        propinfo_destroy(&(pInfo->TextProperty));
    }
}

FrameworkElement* textblock_create()
{
    FrameworkElement *pFe = create_fe(FE_TEXTBLOCK);

    TextBlockInfo *pInfo = malloc(sizeof(TextBlockInfo));
    _tb_init(pInfo);

    pFe->pElement = (char*)pInfo;

    return pFe;
}

FrameworkElement* textblock_create_ex(int x, int y, int width, int height)
{
    FrameworkElement *pFe = textblock_create();
    TextBlockInfo *pInfo = (TextBlockInfo*)(pFe->pElement);

    vinfo_init_ex(&(pInfo->staticInfo.visualInfo), x, y, width, height);

    return pFe;
}

void textblock_free(FrameworkElement *pFe)
{
    TextBlockInfo *pInfo = _tb_getinfo(pFe);
    if (pInfo != NULL)
    {
        _tb_destroy(pInfo);
        free(pInfo);
        fe_free(pFe);
    }
}

void textblock_set_contenthorizontal(FrameworkElement *pFe, HorizontalAlignmentType eType)
{
    TextBlockInfo *pInfo = _tb_getinfo(pFe);
    if (pInfo != NULL)
        propinfo_set(&(pInfo->ContentHorizontalProperty), (char*)&eType);
}

HorizontalAlignmentType textblock_get_contenthorizontal(FrameworkElement *pFe)
{
    TextBlockInfo *pInfo = _tb_getinfo(pFe);
    if (pInfo != NULL)
        return (HorizontalAlignmentType)(*propinfo_get(&(pInfo->ContentHorizontalProperty)));
    return HorizontalAlignment_NoType;
}

void textblock_set_text(FrameworkElement *pFe, wchar_t *pText)
{
    TextBlockInfo *pInfo = _tb_getinfo(pFe);
    if (pInfo != NULL)
        propinfo_set(&(pInfo->TextProperty), (char*)pText);
}

wchar_t* textblock_get_text(FrameworkElement *pFe)
{
    TextBlockInfo *pInfo = _tb_getinfo(pFe);
    if (pInfo != NULL)
        return (wchar_t*)propinfo_get(&(pInfo->TextProperty));
    return NULL;
}

void textblock_bind_text(FrameworkElement *pFe, StrItem *pItem)
{
    TextBlockInfo *pInfo = _tb_getinfo(pFe);
    if (pInfo != NULL && pItem != NULL)
        propinfo_bind(&(pInfo->TextProperty), &(pItem->item));
}

FeSize textblock_get_size(FrameworkElement *pTb)
{
    FeSize size = {0, 0};
    TextBlockInfo *pInfo = _tb_getinfo(pTb);
    if (pInfo != NULL)
        size = vinfo_get_size(&(pInfo->staticInfo.visualInfo));
    return size;
}

void textblock_set_pos(FrameworkElement *pFe, FePos pos)
{
    TextBlockInfo *pInfo = _tb_getinfo(pFe);
    if (pInfo != NULL)
        vinfo_set_pos(&(pInfo->staticInfo.visualInfo), pos);
}

FePos textblock_get_pos(FrameworkElement *pFe)
{
    FePos pos = {0, 0};
    TextBlockInfo *pInfo = _tb_getinfo(pFe);
    if (pInfo != NULL)
        pos = vinfo_get_pos(&(pInfo->staticInfo.visualInfo));
    return pos;
}

void tb_sub_textchanged_evt(FrameworkElement *pFe, EventCallback fnCallback, char *pEvtArg, size_t iArgSize)
{
    TextBlockInfo *pInfo = _tb_getinfo(pFe);
    if (pInfo != NULL)
        propinfo_sub_changed_evt(&(pInfo->TextProperty), fnCallback, pEvtArg, iArgSize);
}

void tb_raise_textchanged_evt(FrameworkElement *pFe)
{
    TextBlockInfo *pInfo = _tb_getinfo(pFe);
    if (pInfo != NULL)
        propinfo_raise_changed_evt(&(pInfo->TextProperty));
}
