#include "tbinfo.h"

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

