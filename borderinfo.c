#include "borderinfo.h"
#include "zaml_structs.h"

static bool _border_is_valid(FrameworkElement *pFe)
{
    return pFe != NULL && pFe->iType == FE_BORDER && pFe->pElement != NULL;
}

static BorderInfo* _border_getinfo(FrameworkElement *pFe)
{
    if (_border_is_valid(pFe))
        return (BorderInfo*)(pFe->pElement);
    return NULL;
}

FrameworkElement* border_create()
{
    FrameworkElement *pBdrFe = create_fe(FE_BORDER);

    BorderInfo *pBorderInfo = malloc(sizeof(BorderInfo));
    pBdrFe->pElement = (char*)pBorderInfo;

    return pBdrFe;
}

FrameworkElement* border_create_ex(int x, int y, int width, int height)
{
    FrameworkElement *pFe = border_create();
    BorderInfo* pBorderInfo = (BorderInfo*)(pFe->pElement);

    pBorderInfo->decoratorInfo.visualInfo.x = x;
    pBorderInfo->decoratorInfo.visualInfo.y = y;
    pBorderInfo->decoratorInfo.visualInfo.width = width;
    pBorderInfo->decoratorInfo.visualInfo.height = height;

    return pFe;
}

FeSize border_get_size(FrameworkElement *pFe)
{
    FeSize size = {0, 0};
    BorderInfo *pInfo = _border_getinfo(pFe);
    if (pInfo != NULL)
    {
        size = vinfo_get_size(&(pInfo->decoratorInfo.visualInfo));
        if (size.width == SIZE_AUTO)
        {
            if (pInfo->decoratorInfo.pChildFe != NULL)
            {
                FeSize childSize = fe_get_size(pInfo->decoratorInfo.pChildFe);
                size.width = childSize.width;
            }
            else
                size.width = 0;
        }
        if (size.height == SIZE_AUTO)
        {
            if (pInfo->decoratorInfo.pChildFe != NULL)
            {
                FeSize childSize = fe_get_size(pInfo->decoratorInfo.pChildFe);
                size.height = childSize.height;
            }
            else
                size.height = 0;
        }
    }
    return size;
}
