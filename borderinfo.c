#include "borderinfo.h"
#include "zaml_structs.h"

static bool _bdr_is_valid(FrameworkElement *pFe)
{
    return pFe != NULL && pFe->iType == FE_BORDER && pFe->pElement != NULL;
}

static BorderInfo* _bdr_getinfo(FrameworkElement *pFe)
{
    if (_bdr_is_valid(pFe))
        return (BorderInfo*)(pFe->pElement);
    return NULL;
}

FrameworkElement* border_create()
{
    FrameworkElement *pFe = create_fe(FE_BORDER);

    BorderInfo *pInfo = malloc(sizeof(BorderInfo));
    dinfo_init(&(pInfo->decoratorInfo));

    pFe->pElement = (char*)pInfo;

    return pFe;
}

FrameworkElement* border_create_ex(int x, int y, int width, int height)
{
    FrameworkElement *pFe = border_create();
    BorderInfo* pInfo = (BorderInfo*)(pFe->pElement);

    vinfo_init_ex(&(pInfo->decoratorInfo.visualInfo), x, y, width, height);

    return pFe;
}

void border_free(FrameworkElement *pFe)
{
    BorderInfo *pInfo = _bdr_getinfo(pFe);
    if (pInfo != NULL)
    {
        dinfo_destroy(&(pInfo->decoratorInfo));
    }
}

FeSize border_get_size(FrameworkElement *pFe)
{
    FeSize size = {0, 0};
    BorderInfo *pInfo = _bdr_getinfo(pFe);
    if (pInfo != NULL)
    {
        size = vinfo_get_size(&(pInfo->decoratorInfo.visualInfo));

        FeSize childSize = {0, 0};
        if (pInfo->decoratorInfo.pChildFe != NULL)
           childSize = fe_get_size(pInfo->decoratorInfo.pChildFe);

        if (size.width == SIZE_AUTO)
            size.width = childSize.width;
        if (size.height == SIZE_AUTO)
            size.height = childSize.height;
    }
    return size;
}

FePos border_get_pos(FrameworkElement *pFe)
{
    FePos pos = {0, 0};
    BorderInfo *pInfo = _bdr_getinfo(pFe);
    if (pInfo != NULL)
        pos = vinfo_get_pos(&(pInfo->decoratorInfo.visualInfo));
    return pos;
}

void border_set_pos(FrameworkElement *pFe, FePos pos)
{
    BorderInfo *pInfo = _bdr_getinfo(pFe);
    if (pInfo != NULL)
        vinfo_set_pos(&(pInfo->decoratorInfo.visualInfo), pos);
}

