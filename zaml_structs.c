#include <stdlib.h>
#include <stdio.h>
#include "zaml_structs.h"
#include "datacontext.h"

bool fe_add_child(FrameworkElement *pFe, FrameworkElement *pChildFe)
{
    if (pFe != NULL && pChildFe != NULL)
    {
        if (pFe->iType == FE_GRID)
            grid_add_child(pFe, pChildFe);
        return true;
    }
    return false;
}

int zaml_get_children_count(FrameworkElement *pFe)
{
    if (pFe != NULL)
    {
        if (pFe->iType == FE_GRID)
            return grid_get_children_count(pFe);
    }
    return 0;
}

FrameworkElement* zaml_get_child(FrameworkElement *pFe, int iPos)
{
    if (pFe != NULL && iPos >= 0)
    {
        if (pFe->iType == FE_GRID && iPos < grid_get_children_count(pFe))
            return grid_get_child(pFe, iPos);
    }
    return NULL;
}

FeSize fe_get_size(FrameworkElement *pFe)
{
    FeSize size = {0, 0};
    if (pFe != NULL && pFe->pElement != NULL)
    {
        if (pFe->iType == FE_GRID)
            size = grid_get_size(pFe);
        else if (pFe->iType == FE_BUTTON)
            size =  button_get_size(pFe);
        else if (pFe->iType == FE_TEXTBLOCK)
            size = textblock_get_size(pFe);
        else if (pFe->iType == FE_BORDER)
            size = border_get_size(pFe);
    }
    return size;
}

void fe_set_pos(FrameworkElement *pFe, FePos pos)
{
    if (pFe != NULL && pFe->pElement != NULL)
    {
        if (pFe->iType == FE_GRID)
            grid_set_pos(pFe, pos);
        else if (pFe->iType == FE_BUTTON)
            button_set_pos(pFe, pos);
        else if (pFe->iType == FE_TEXTBLOCK)
            textblock_set_pos(pFe, pos);
        else if (pFe->iType == FE_BORDER)
            border_set_pos(pFe, pos);
    }
}

FePos fe_get_pos(FrameworkElement *pFe)
{
    FePos pos = {0, 0};
    if (pFe != NULL && pFe->pElement != NULL)
    {
        if (pFe->iType == FE_GRID)
            pos = grid_get_pos(pFe);
        else if (pFe->iType == FE_BUTTON)
            pos = button_get_pos(pFe);
        else if (pFe->iType == FE_TEXTBLOCK)
            pos = textblock_get_pos(pFe);
        else if (pFe->iType == FE_BORDER)
            pos = border_get_pos(pFe);
    }
    return pos;
}
