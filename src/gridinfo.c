#include "gridinfo.h"
#include "lib/utils.h"
#include "zaml_structs.h"

typedef struct _spanInfo
{
    int iSpan;
    int iLength;
} SpanInfo;

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

static void _grid_add_spaninfo(SList *pSpanList, int iSpan, int iLength)
{
    if (pSpanList != NULL)
    {
        SpanInfo sInfo;
        sInfo.iSpan = iSpan;
        sInfo.iLength = iLength;
        slist_push(pSpanList, (char*)&sInfo, sizeof(SpanInfo));
    }
}

static int _grid_get_span_length(SList *pSpanList, int iLength)
{
    int result = 0;
    if (pSpanList)
    {
        SList spanList;
        slist_init(&spanList);
        for (int iSpanInfo = 0; iSpanInfo < slist_get_count(pSpanList); iSpanInfo++)
        {
            SpanInfo *pInfo = (SpanInfo*)slist_get(pSpanList, iSpanInfo);
            if (pInfo != NULL)
            {
                pInfo->iSpan--;
                pInfo->iLength -= iLength;
                if (pInfo->iSpan == 0)
                    result = ymax(result, pInfo->iLength);
                else
                    slist_push(&spanList, (char*)pInfo, sizeof(SpanInfo));
            }
        }
        slist_destroy(pSpanList);
        *pSpanList = spanList;
    }
    return result;
}

static int _grid_get_max_height(FrameworkElement *pFe, int iRow, SList *pRowList)
{
    int rowHeight = 0;
    if (pFe != NULL)
    {
        for (int iChild = 0; iChild < grid_get_children_count(pFe); iChild++)
        {
            FrameworkElement *pChild = grid_get_child(pFe, iChild);
            if (pChild != NULL)
            {
                if (iRow == grid_get_row(pFe, pChild))
                {
                    FeSize childSize = fe_get_size(pChild);
                    if (grid_get_rowspan(pFe, pChild) > 1)
                    {
                        if (pRowList != NULL)
                            _grid_add_spaninfo(pRowList, grid_get_rowspan(pFe, pChild), childSize.height);
                    }
                    else
                        rowHeight = ymax(rowHeight, childSize.height);
                }
            }
        }
    }
    return rowHeight;
}

static int _grid_get_height_ex(FrameworkElement *pFe, int iRow, SList *pRowList)
{
    int height = 0;
    if (pFe != NULL)
    {
        RowDefinition *pRowDef = grid_get_rowdef(pFe, iRow);
        if (pRowDef != NULL && pRowDef->height != SIZE_AUTO)
            height = pRowDef->height;
        else
            height = _grid_get_max_height(pFe, iRow, pRowList);
        height = ymax(height, _grid_get_span_length(pRowList, height));
    }
    return height;
}

static int _grid_get_max_width(FrameworkElement *pFe, int iCol, SList *pColList)
{
    int colWidth = 0;
    if (pFe != NULL)
    {
        for (int iChild = 0; iChild < grid_get_children_count(pFe); iChild++)
        {
            FrameworkElement *pChild = grid_get_child(pFe, iChild);
            if (pChild != NULL)
            {
                if (iCol == grid_get_col(pFe, pChild))
                {
                    FeSize childSize = fe_get_size(pChild);
                    if (grid_get_colspan(pFe, pChild) > 1)
                    {
                        if (pColList != NULL)
                            _grid_add_spaninfo(pColList, grid_get_colspan(pFe, pChild), childSize.width);
                    }
                    else
                        colWidth = ymax(colWidth, childSize.width);
                }
            }
        }
    }
    return colWidth;
}

static int _grid_get_width_ex(FrameworkElement *pFe, int iCol, SList *pColList)
{
    int width = 0;
    if (pFe != NULL)
    {
        ColumnDefinition *pColDef = grid_get_coldef(pFe, iCol);
        if (pColDef != NULL && pColDef->width != SIZE_AUTO)
            width = pColDef->width;
        else
            width = _grid_get_max_width(pFe, iCol, pColList);

        width = ymax(width, _grid_get_span_length(pColList, width));
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
        SList rowList, colList;
        slist_init(&rowList);
        slist_init(&colList);
        do
        {
            for (int iChild = 0; iChild < grid_get_children_count(pFe); iChild++)
            {
                FrameworkElement *pChildFe = grid_get_child(pFe, iChild);
                if (pChildFe != NULL)
                {
                    FePos pos = fe_get_pos(pChildFe);
                    if (iRowCol == grid_get_row(pFe, pChildFe))
                        pos.y = iChildY;
                    if (iRowCol == grid_get_col(pFe, pChildFe))
                        pos.x = iChildX;
                    fe_set_pos(pChildFe, pos);
                }
            }

            if (iRowCol == 0 || iRowCol < iRows)
                iChildY += _grid_get_height_ex(pFe, iRowCol, &rowList);
            
            if (iRowCol == 0 || iRowCol < iCols)
                iChildX += _grid_get_width_ex(pFe, iRowCol, &colList);

            iRowCol++;
        } while (iRowCol < iRowCols);

        feSize.width = pInfo->layoutInfo.visualInfo.width;
        if (feSize.width == SIZE_AUTO)
            feSize.width = iChildX;
        
        feSize.height = pInfo->layoutInfo.visualInfo.height;
        if (feSize.height == SIZE_AUTO)
            feSize.height = iChildY;

        vinfo_set_size(&(pInfo->layoutInfo.visualInfo), feSize);

        slist_destroy(&rowList);
        slist_destroy(&colList);
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
        if (grid_get_children_count(pGrid) > 0)
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
        if (grid_get_children_count(pGrid) > 0)
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

