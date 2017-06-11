#include "tbinfo.h"

static bool _textblock_is_valid(FrameworkElement *pFe)
{
    return pFe != NULL && pFe->iType == FE_TEXTBLOCK && pFe->pElement != NULL;
}

static TextBlockInfo* _textblock_getinfo(FrameworkElement *pFe)
{
    if (_textblock_is_valid(pFe))
        return (TextBlockInfo*)(pFe->pElement);
    return NULL;
}

FrameworkElement* textblock_create()
{
    FrameworkElement *pTbFe = create_fe(FE_TEXTBLOCK);

    TextBlockInfo *pTbInfo = malloc(sizeof(TextBlockInfo));
    pTbInfo->pTextProperty = NULL;
    pTbInfo->eContentHorizontal = HorizontalAlignment_Left;
    pTbFe->pElement = (char*)pTbInfo;

    return pTbFe;
}

FrameworkElement* textblock_create_ex(int x, int y, int width, int height, PropertyInfo *pTextProp)
{
    FrameworkElement *pTbFe = textblock_create();
    TextBlockInfo *pTbInfo = (TextBlockInfo*)(pTbFe->pElement);

    pTbInfo->staticInfo.visualInfo.x = x;
    pTbInfo->staticInfo.visualInfo.y = y;
    pTbInfo->staticInfo.visualInfo.width = width;
    pTbInfo->staticInfo.visualInfo.height = height;
    pTbInfo->pTextProperty = pTextProp;

    return pTbFe;
}

FeSize textblock_get_size(FrameworkElement *pTb)
{
    FeSize size = {0, 0};
    TextBlockInfo *pInfo = _textblock_getinfo(pTb);
    if (pInfo != NULL)
        size = vinfo_get_size(&(pInfo->staticInfo.visualInfo));
    return size;
}
