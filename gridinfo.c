#include "gridinfo.h"
#include "lib/utilities.h"
#include "zaml_structs.h"

static bool _grid_is_valid(FrameworkElement *pGridFe)
{
    return pGridFe != NULL && pGridFe->iType == FE_GRID && pGridFe->pElement != NULL;
}

static GridInfo* _grid_getinfo(FrameworkElement *pGridFe)
{
    if (_grid_is_valid(pGridFe))
        return (GridInfo*)(pGridFe->pElement);
    return NULL;
}

FrameworkElement* grid_create()
{
    FrameworkElement *pGridFe = create_fe(FE_GRID);
    
    GridInfo *pGridInfo = malloc(sizeof(GridInfo));
    linfo_init(&(pGridInfo->layoutInfo));
    attachedpropinfo_init_int(&(pGridInfo->ColProperty), 0);
    attachedpropinfo_init_int(&(pGridInfo->RowProperty), 0);
    attachedpropinfo_init_int(&(pGridInfo->ColSpanProperty), 1);
    attachedpropinfo_init_int(&(pGridInfo->RowSpanProperty), 1);
    slist_init(&(pGridInfo->RowDefinitions));
    slist_init(&(pGridInfo->ColumnDefinitions));
    pGridFe->pElement = (char*)pGridInfo;

    return pGridFe;
}

void grid_free(FrameworkElement *pFe)
{
    GridInfo *pInfo = _grid_getinfo(pFe);
    if (pInfo != NULL)
    {
        linfo_destroy(&(pInfo->layoutInfo));
        attachedpropinfo_destroy(&(pInfo->ColProperty));
        attachedpropinfo_destroy(&(pInfo->RowProperty));
        attachedpropinfo_destroy(&(pInfo->ColSpanProperty));
        attachedpropinfo_destroy(&(pInfo->RowSpanProperty));
        slist_destroy(&(pInfo->RowDefinitions));
        slist_destroy(&(pInfo->ColumnDefinitions));
        free(pInfo);
        fe_free(pFe);
    }
}

FrameworkElement* grid_create_ex(int x, int y, int width, int height)
{
    FrameworkElement *pGridFe = grid_create();
    GridInfo *pGridInfo = (GridInfo*)(pGridFe->pElement);

    pGridInfo->layoutInfo.visualInfo.x = x;
    pGridInfo->layoutInfo.visualInfo.y = y;
    pGridInfo->layoutInfo.visualInfo.width = width;
    pGridInfo->layoutInfo.visualInfo.height = height;

    return pGridFe;
}

int grid_get_children_count(FrameworkElement *pGrid)
{
    GridInfo *pInfo = _grid_getinfo(pGrid);
    if (pInfo != NULL)
        return linfo_get_children_count(&(pInfo->layoutInfo));
    return 0;
}

FrameworkElement* grid_get_child(FrameworkElement *pGrid, unsigned int iPos)
{
    GridInfo *pInfo = _grid_getinfo(pGrid);
    if (pInfo != NULL && iPos < grid_get_children_count(pGrid))
        return (FrameworkElement*)linfo_get_child(&(pInfo->layoutInfo), iPos);
    return NULL;
}

int _grid_get_height(FrameworkElement *pGrid)
{
    int height = 0;
    if (pGrid != NULL)
    {
        int iRow = 0;
        do
        {
            RowDefinition *pRowDef = grid_get_rowdef(pGrid, iRow);
            if (pRowDef != NULL && pRowDef->height != SIZE_AUTO)
                height += pRowDef->height;
            else
            {
                int rowHeight = 0;
                for (int iChild = 0; iChild < grid_get_children_count(pGrid); iChild++)
                {
                    FrameworkElement *pChild = grid_get_child(pGrid, iChild);
                    if (pChild != NULL)
                    {
                        if (iRow == grid_get_row(pGrid, pChild))
                        {
                            FeSize childSize = fe_get_size(pChild);
                            rowHeight = ymax(rowHeight, childSize.height);
                        }
                    }
                }
                height += rowHeight;
            }
            iRow++;
        } while (iRow < grid_get_rowdef_count(pGrid));
    }
    return height;
}

int _grid_get_width(FrameworkElement *pGrid)
{
    int width = 0;
    if (pGrid != NULL)
    {
        int iCol = 0;
        do
        {
            ColumnDefinition *pColDef = grid_get_coldef(pGrid, iCol);
            if (pColDef != NULL && pColDef->width != SIZE_AUTO)
                width += pColDef->width;
            else
            {
                int colWidth = 0;
                for (int iChild = 0; iChild < grid_get_children_count(pGrid); iChild++)
                {
                    FrameworkElement *pChild = grid_get_child(pGrid, iChild);
                    if (pChild != NULL)
                    {
                        if (iCol == grid_get_col(pGrid, pChild))
                        {
                            FeSize childSize = fe_get_size(pChild);
                            colWidth = ymax(colWidth, childSize.width);
                        }
                    }
                }
                width += colWidth;
            }
            iCol++;
        } while (iCol < grid_get_coldef_count(pGrid));
    }
    return width;
}

FeSize grid_get_size(FrameworkElement *pGrid)
{
    FeSize feSize = { 0, 0 };
    GridInfo *pInfo = _grid_getinfo(pGrid);
    if (pInfo != NULL)
    {
        feSize = vinfo_get_size(&(pInfo->layoutInfo.visualInfo));

        if (feSize.height == SIZE_AUTO)
            feSize.height = _grid_get_height(pGrid);

        if (feSize.width == SIZE_AUTO)
            feSize.width = _grid_get_width(pGrid);
    }
    return feSize;
}

void grid_add_rowdef(FrameworkElement *pGrid, RowDefinition *pRowDef)
{
    GridInfo *pInfo = _grid_getinfo(pGrid);
    if (pInfo != NULL && pRowDef != NULL)
        slist_push(&(pInfo->RowDefinitions), (char*)pRowDef, sizeof(RowDefinition));
}

void grid_add_coldef(FrameworkElement *pGrid, ColumnDefinition *pColDef)
{
    GridInfo *pInfo = _grid_getinfo(pGrid);
    if (pInfo != NULL && pColDef != NULL)
        slist_push(&(pInfo->ColumnDefinitions), (char*)pColDef, sizeof(ColumnDefinition));
}

int grid_get_rowdef_count(FrameworkElement *pGrid)
{
    GridInfo *pInfo = _grid_getinfo(pGrid);
    if (pInfo != NULL)
        return slist_get_count(&(pInfo->RowDefinitions));
    return 0;
}

int grid_get_coldef_count(FrameworkElement *pGrid)
{
    GridInfo *pInfo = _grid_getinfo(pGrid);
    if (pInfo != NULL)
        return slist_get_count(&(pInfo->ColumnDefinitions));
    return 0;
}

RowDefinition* grid_get_rowdef(FrameworkElement *pGrid, unsigned int iPos)
{
    GridInfo *pInfo = _grid_getinfo(pGrid);
    if (pInfo != NULL && iPos < grid_get_rowdef_count(pGrid))
        return (RowDefinition*)slist_get(&(pInfo->RowDefinitions), iPos);
    return NULL;
}

ColumnDefinition* grid_get_coldef(FrameworkElement *pGrid, unsigned int iPos)
{
    GridInfo *pInfo = _grid_getinfo(pGrid);
    if (pInfo != NULL && iPos < grid_get_coldef_count(pGrid))
        return (ColumnDefinition*)slist_get(&(pInfo->ColumnDefinitions), iPos);
    return NULL;
}

void grid_set_col(FrameworkElement *pGrid, FrameworkElement *pChild, int iCol)
{
    GridInfo *pInfo = _grid_getinfo(pGrid);
    if (pInfo != NULL && pChild != NULL)
        fe_set_value(pChild, &(pInfo->ColProperty), (char*)(&iCol));
}

int grid_get_col(FrameworkElement *pGrid, FrameworkElement *pChild)
{
    int iValue = 0;
    GridInfo *pInfo = _grid_getinfo(pGrid);
    if (pInfo != NULL && pChild != NULL)
        fe_get_value(pChild, &(pInfo->ColProperty), (char*)(&iValue));
    return iValue;
}

void grid_set_colspan(FrameworkElement *pGrid, FrameworkElement *pChild, int iColSpan)
{
    GridInfo *pInfo = _grid_getinfo(pGrid);
    if (pInfo != NULL && pChild != NULL)
        fe_set_value(pChild, &(pInfo->ColSpanProperty), (char*)(&iColSpan));
}

int grid_get_colspan(FrameworkElement *pGrid, FrameworkElement *pChild)
{
    int iValue = 0;
    GridInfo *pInfo = _grid_getinfo(pGrid);
    if (pInfo != NULL && pChild != NULL)
        fe_get_value(pChild, &(pInfo->ColSpanProperty), (char*)(&iValue));
    return iValue;
}

void grid_set_row(FrameworkElement *pGrid, FrameworkElement *pChild, int iRow)
{
    GridInfo *pInfo = _grid_getinfo(pGrid);
    if (pInfo != NULL && pChild != NULL)
        fe_set_value(pChild, &(pInfo->RowProperty), (char*)(&iRow));
}

int grid_get_row(FrameworkElement *pGrid, FrameworkElement *pChild)
{
    int iValue = 0;
    GridInfo *pInfo = _grid_getinfo(pGrid);
    if (pInfo != NULL && pChild != NULL)
        fe_get_value(pChild, &(pInfo->RowProperty), (char*)(&iValue));
    return iValue;
}

void grid_set_rowspan(FrameworkElement *pGrid, FrameworkElement *pChild, int iRowSpan)
{
    GridInfo *pInfo = _grid_getinfo(pGrid);
    if (pInfo != NULL && pChild != NULL)
        fe_set_value(pChild, &(pInfo->RowSpanProperty), (char*)(&iRowSpan));
}

int grid_get_rowspan(FrameworkElement *pGrid, FrameworkElement *pChild)
{
    int iValue = 0;
    GridInfo *pInfo = _grid_getinfo(pGrid);
    if (pInfo != NULL && pChild != NULL)
        fe_get_value(pChild, &(pInfo->RowSpanProperty), (char*)(&iValue));
    return iValue;
}

