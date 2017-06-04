#include <stdlib.h>
#include <stdio.h>
#include "zaml_structs.h"
#include "datacontext.h"
#include <windows.h>

FrameworkElement* zaml_create_grid()
{
    FrameworkElement *pGridFe = create_fe(FE_GRID);
    
    GridInfo *pGridInfo = malloc(sizeof(GridInfo));
    InitLayoutInfo(&(pGridInfo->layoutInfo));
    pGridFe->pElement = (char*)pGridInfo;

    return pGridFe;
}

FrameworkElement* zaml_create_grid_ex(int x, int y, int width, int height, DataContext *pDc)
{
    FrameworkElement *pGridFe = zaml_create_grid();
    GridInfo *pGridInfo = (GridInfo*)(pGridFe->pElement);

    pGridInfo->layoutInfo.visualInfo.x = x;
    pGridInfo->layoutInfo.visualInfo.y = y;
    pGridInfo->layoutInfo.visualInfo.width = width;
    pGridInfo->layoutInfo.visualInfo.height = height;
    pGridInfo->layoutInfo.pDc = pDc;

    return pGridFe;
}

FrameworkElement* zaml_create_button()
{
    FrameworkElement *pBtnFe = create_fe(FE_BUTTON);

    ButtonInfo *pButtonInfo = malloc(sizeof(ButtonInfo));
    pBtnFe->pElement = (char*)pButtonInfo;

    return pBtnFe;
}

FrameworkElement* zaml_create_button_ex(int x, int y, int width, int height, wchar_t *szContent, wchar_t *szCommand, DataContext *pDc)
{
    FrameworkElement *pBtnFe = zaml_create_button();
    ButtonInfo *pButtonInfo = (ButtonInfo*)(pBtnFe->pElement);

    pButtonInfo->controlInfo.visualInfo.x = x;
    pButtonInfo->controlInfo.visualInfo.y = y;
    pButtonInfo->controlInfo.visualInfo.width = width;
    pButtonInfo->controlInfo.visualInfo.height = height;
    pButtonInfo->szContent = szContent;
    pButtonInfo->controlInfo.pDc = pDc;
    pButtonInfo->szCommand = szCommand;

    return pBtnFe;
}

FrameworkElement* zaml_create_textblock()
{
    FrameworkElement *pTbFe = create_fe(FE_TEXTBLOCK);

    TextBlockInfo *pTbInfo = malloc(sizeof(TextBlockInfo));
    pTbInfo->pTextProperty = NULL;
    pTbInfo->eContentHorizontal = HorizontalAlignment_Left;
    pTbFe->pElement = (char*)pTbInfo;

    return pTbFe;
}

FrameworkElement* zaml_create_textblock_ex(int x, int y, int width, int height, PropertyInfo *pTextProp)
{
    FrameworkElement *pTbFe = zaml_create_textblock();
    TextBlockInfo *pTbInfo = (TextBlockInfo*)(pTbFe->pElement);

    pTbInfo->staticInfo.visualInfo.x = x;
    pTbInfo->staticInfo.visualInfo.y = y;
    pTbInfo->staticInfo.visualInfo.width = width;
    pTbInfo->staticInfo.visualInfo.height = height;
    pTbInfo->pTextProperty = pTextProp;

    return pTbFe;
}

FrameworkElement* zaml_create_border()
{
    FrameworkElement *pBdrFe = create_fe(FE_BORDER);

    BorderInfo *pBorderInfo = malloc(sizeof(BorderInfo));
    InitLayoutInfo(&(pBorderInfo->layoutInfo));
    pBdrFe->pElement = (char*)pBorderInfo;

    return pBdrFe;
}

FrameworkElement* zaml_create_border_ex(int x, int y, int width, int height, DataContext *pDc)
{
    FrameworkElement *pBdrFe = zaml_create_border();
    BorderInfo* pBorderInfo = (BorderInfo*)(pBdrFe->pElement);

    pBorderInfo->layoutInfo.visualInfo.x = x;
    pBorderInfo->layoutInfo.visualInfo.y = y;
    pBorderInfo->layoutInfo.visualInfo.width = width;
    pBorderInfo->layoutInfo.visualInfo.height = height;
    pBorderInfo->layoutInfo.pDc = pDc;

    return pBdrFe;
}

static LayoutInfo* get_layoutinfo(FrameworkElement *pFe)
{
    LayoutInfo *pLayoutInfo = NULL;
    if (pFe->iType == FE_GRID)
    {
        GridInfo *pGridInfo = (GridInfo*)(pFe->pElement);
        pLayoutInfo = &(pGridInfo->layoutInfo);
    }
    else if (pFe->iType == FE_BORDER)
    {
        BorderInfo *pBorderInfo = (BorderInfo*)(pFe->pElement);
        pLayoutInfo = &(pBorderInfo->layoutInfo);
    }
    else if (pFe->iType == FE_TREEVIEW)
    {
        TreeViewInfo *pInfo = (TreeViewInfo*)(pFe->pElement);
        pLayoutInfo = &(pInfo->layoutInfo);
    }
    return pLayoutInfo;
}

bool zaml_add_child(FrameworkElement *pParentFe, FrameworkElement *pChildFe)
{
    if (pParentFe != NULL && pChildFe != NULL)
    {
        LayoutInfo *pLayoutInfo = get_layoutinfo(pParentFe);
        if (pLayoutInfo != NULL)
        {
            slist_push(&(pLayoutInfo->children), (char*)(&pChildFe), sizeof(pChildFe));
            return true;
        }
    }
    return false;
}

int zaml_get_children_count(FrameworkElement *pFe)
{
    if (pFe != NULL)
    {
        LayoutInfo *pLayoutInfo = get_layoutinfo(pFe);
        if (pLayoutInfo != NULL)
            return slist_get_count(&(pLayoutInfo->children));
    }
    return 0;
}

FrameworkElement* zaml_get_child(FrameworkElement *pFe, int iPos)
{
    if (pFe != NULL)
    {
        LayoutInfo *pLayoutInfo = get_layoutinfo(pFe);
        if (pLayoutInfo != NULL)
        {
            FrameworkElement **ppFe = (FrameworkElement**)slist_get(&(pLayoutInfo->children), iPos);
            return *ppFe;
        }
    }
    return NULL;
}

