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

FrameworkElement* textblock_create()
{
    FrameworkElement *pFe = create_fe(FE_TEXTBLOCK);

    TextBlockInfo *pInfo = malloc(sizeof(TextBlockInfo));
    sinfo_init(&(pInfo->staticInfo));
    pInfo->eContentHorizontal = HorizontalAlignment_Left;
    pInfo->pTextProperty = NULL;

    pFe->pElement = (char*)pInfo;

    return pFe;
}

FrameworkElement* textblock_create_ex(int x, int y, int width, int height, PropertyInfo *pTextProp)
{
    FrameworkElement *pFe = textblock_create();
    TextBlockInfo *pInfo = (TextBlockInfo*)(pFe->pElement);

    vinfo_init_ex(&(pInfo->staticInfo.visualInfo), x, y, width, height);
    pInfo->pTextProperty = pTextProp;

    return pFe;
}

void textblock_free(FrameworkElement *pFe)
{
    TextBlockInfo *pInfo = _tb_getinfo(pFe);
    if (pInfo != NULL)
    {
        sinfo_destroy(&(pInfo->staticInfo));
        free(pInfo->pTextProperty);
        free(pInfo);
        fe_free(pFe);
    }
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
