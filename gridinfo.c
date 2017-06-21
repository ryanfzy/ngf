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

static bool _grid_has_child(FrameworkElement *pFe, FrameworkElement *pChildFe)
{
    GridInfo *pInfo = _grid_getinfo(pFe);
    if (pInfo != NULL)
        return linfo_has_child(&(pInfo->layoutInfo), pChildFe);
    return false;
}

int _grid_get_height_ex(FrameworkElement *pFe, int iRow)
{
    int height = 0;
    if (pFe != NULL)
    {
        RowDefinition *pRowDef = grid_get_rowdef(pFe, iRow);
        if (pRowDef != NULL && pRowDef->height != SIZE_AUTO)
            height = pRowDef->height;
        else
        {
            int rowHeight = 0;
            for (int iChild = 0; iChild < grid_get_children_count(pFe); iChild++)
            {
                FrameworkElement *pChild = grid_get_child(pFe, iChild);
                if (pChild != NULL)
                {
                    if (iRow == grid_get_row(pFe, pChild))
                    {
                        FeSize childSize = fe_get_size(pChild);
                        rowHeight = ymax(rowHeight, childSize.height);
                    }
                }
            }
            height = rowHeight;
        }
    }
    return height;
}

static int _grid_get_width_ex(FrameworkElement *pFe, int iCol)
{
    int width = 0;
    if (pFe != NULL)
    {
        ColumnDefinition *pColDef = grid_get_coldef(pFe, iCol);
        if (pColDef != NULL && pColDef->width != SIZE_AUTO)
            width = pColDef->width;
        else
        {
            int colWidth = 0;
            for (int iChild = 0; iChild < grid_get_children_count(pFe); iChild++)
            {
                FrameworkElement *pChild = grid_get_child(pFe, iChild);
                if (pChild != NULL)
                {
                    if (iCol == grid_get_col(pFe, pChild))
                    {
                        FeSize childSize = fe_get_size(pChild);
                        colWidth = ymax(colWidth, childSize.width);
                    }
                }
            }
            width = colWidth;
        }
    }
    return width;
}

static void _grid_invalidate(FrameworkElement *pFe)
{
    GridInfo *pInfo = _grid_getinfo(pFe);
    if (pInfo != NULL)
    {
        FeSize feSize = { 0, 0 };
        int iRows = grid_get_rowdef_count(pFe);
        int iCols = grid_get_coldef_count(pFe);
        int iRowCols = ymax(iRows, iCols);
        int iRowCol = 0;
        int iChildX = 0;
        int iChildY = 0;
        do
        {
            for (int iChild = 0; iChild < grid_get_children_count(pFe); iChild++)
            {
                FrameworkElement *pChildFe = grid_get_child(pFe, iChild);
                if (pChildFe != NULL)
                {
                    FePos pos = fe_get_pos(pFe);
                    if (iRowCol == grid_get_row(pFe, pChildFe))
                        pos.y = iChildY;
                    if (iRowCol == grid_get_col(pFe, pChildFe))
                        pos.x = iChildX;
                    fe_set_pos(pChildFe, pos);
                }
            }

            if (iRowCol == 0 || iRowCol < iRows)
                iChildY += _grid_get_height_ex(pFe, iRowCol);
            
            if (iRowCol == 0 || iRowCol < iCols)
                iChildX += _grid_get_width_ex(pFe, iRowCol);

            iRowCol++;
        } while (iRowCol < iRowCols);

        feSize.width = pInfo->layoutInfo.visualInfo.width;
        if (feSize.width == SIZE_AUTO)
            feSize.width = iChildX;
        
        feSize.height = pInfo->layoutInfo.visualInfo.height;
        if (feSize.height == SIZE_AUTO)
            feSize.height = iChildY;

        vinfo_set_size(&(pInfo->layoutInfo.visualInfo), feSize);
    }
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

    vinfo_init_ex(&(pGridInfo->layoutInfo.visualInfo), x, y, width, height);

    return pGridFe;
}

void grid_add_child(FrameworkElement *pFe, FrameworkElement *pChildFe)
{
    GridInfo *pInfo = _grid_getinfo(pFe);
    if (pInfo != NULL && pChildFe != NULL)
    {
        if (linfo_add_child(&(pInfo->layoutInfo), pChildFe))
            _grid_invalidate(pFe);
    }
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

FeSize grid_get_size(FrameworkElement *pGrid)
{
    FeSize size = {0, 0};
    GridInfo *pInfo = _grid_getinfo(pGrid);
    if (pInfo != NULL)
        size = vinfo_get_size(&(pInfo->layoutInfo.visualInfo));
    return size;
}

FePos grid_get_pos(FrameworkElement *pFe)
{
    FePos pos = {0, 0};
    GridInfo *pInfo = _grid_getinfo(pFe);
    if (pInfo != NULL)
        pos = vinfo_get_pos(&(pInfo->layoutInfo.visualInfo));
    return pos;
}

void grid_set_pos(FrameworkElement *pFe, FePos pos)
{
    GridInfo *pInfo = _grid_getinfo(pFe);
    if (pInfo != NULL)
        vinfo_set_pos(&(pInfo->layoutInfo.visualInfo), pos);
}

void grid_add_rowdef(FrameworkElement *pGrid, RowDefinition *pRowDef)
{
    GridInfo *pInfo = _grid_getinfo(pGrid);
    if (pInfo != NULL && pRowDef != NULL)
    {
        pRowDef->pFe = pGrid;
        slist_push(&(pInfo->RowDefinitions), (char*)pRowDef, sizeof(RowDefinition));
        _grid_invalidate(pGrid);
    }
}

void grid_add_coldef(FrameworkElement *pGrid, ColumnDefinition *pColDef)
{
    GridInfo *pInfo = _grid_getinfo(pGrid);
    if (pInfo != NULL && pColDef != NULL)
    {
        pColDef->pFe = pGrid;
        slist_push(&(pInfo->ColumnDefinitions), (char*)pColDef, sizeof(ColumnDefinition));
        _grid_invalidate(pGrid);
    }
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
    {
        fe_set_value(pChild, &(pInfo->ColProperty), (char*)(&iCol));
        if (_grid_has_child(pGrid, pChild))
            _grid_invalidate(pGrid);
    }
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
    {
        fe_set_value(pChild, &(pInfo->ColSpanProperty), (char*)(&iColSpan));
        if (_grid_has_child(pGrid, pChild))
            _grid_invalidate(pGrid);
    }
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
    {
        fe_set_value(pChild, &(pInfo->RowProperty), (char*)(&iRow));
        if (_grid_has_child(pGrid, pChild))
            _grid_invalidate(pGrid);
    }
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
    {
        fe_set_value(pChild, &(pInfo->RowSpanProperty), (char*)(&iRowSpan));
        if (_grid_has_child(pGrid, pChild))
            _grid_invalidate(pGrid);
    }
}

int grid_get_rowspan(FrameworkElement *pGrid, FrameworkElement *pChild)
{
    int iValue = 0;
    GridInfo *pInfo = _grid_getinfo(pGrid);
    if (pInfo != NULL && pChild != NULL)
        fe_get_value(pChild, &(pInfo->RowSpanProperty), (char*)(&iValue));
    return iValue;
}

void rowdef_set_height(RowDefinition *pRowDef, int height)
{
    if (pRowDef != NULL)
    {
        pRowDef->height = height;
        _grid_invalidate(pRowDef->pFe);
    }
}

void coldef_set_width(ColumnDefinition *pColDef, int width)
{
    if (pColDef != NULL)
    {
        pColDef->width = width;
        _grid_invalidate(pColDef->pFe);
    }
}

