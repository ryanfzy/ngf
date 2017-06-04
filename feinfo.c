#include "feinfo.h"

FrameworkElement* create_fe(int iFeType)
{
    FrameworkElement *pFe = malloc(sizeof(FrameworkElement));
    pFe->iType = iFeType;
    return pFe;
}

void InitLayoutInfo(LayoutInfo *pLayoutInfo)
{
    slist_init(&(pLayoutInfo->children));
}

