#include <stdlib.h>
#include <check.h>
#include "gridinfo.h"
#include "btninfo.h"
#include "zaml_structs.h"

typedef struct _TestData
{
    int ivalue;
    double dvalue;
} TestData;

START_TEST(test_grid_create)
{
    FrameworkElement *pFe = grid_create_ex(0, 0, 10, 10);
    ck_assert_msg(pFe->iType == FE_GRID, "iType is not FE_GRID");
    ck_assert_msg(pFe->pElement != NULL, "pElement is NULL");
    
    GridInfo *pInfo = (GridInfo*)(pFe->pElement);
    ck_assert_int_eq(pInfo->layoutInfo.visualInfo.x, 0);
    ck_assert_int_eq(pInfo->layoutInfo.visualInfo.y, 0);
    ck_assert_int_eq(pInfo->layoutInfo.visualInfo.width, 10);
    ck_assert_int_eq(pInfo->layoutInfo.visualInfo.height, 10);
    ck_assert_int_eq(slist_get_count(&(pInfo->layoutInfo.children)), 0);
}
END_TEST

START_TEST(test_grid_add_child)
{
    FrameworkElement *pFe = grid_create_ex(0, 0, 10, 10);
    ck_assert_int_eq(grid_get_children_count(pFe), 0);

    FrameworkElement *pFe2 = grid_create_ex(0, 0, 20, 20);
    zaml_add_child(pFe, pFe2);
    ck_assert_int_eq(grid_get_children_count(pFe), 1);

    FrameworkElement *pFe3 = grid_get_child(pFe, 0);
    ck_assert_msg(pFe2 == pFe3, "not same child");

    FrameworkElement *pFe4 = grid_get_child(pFe, 1);
    ck_assert_msg(pFe4 == NULL, "not null");

}
END_TEST

START_TEST(test_grid_size)
{
    FrameworkElement *pFe = grid_create_ex(0, 0, SIZE_AUTO, SIZE_AUTO);
    GridInfo *pInfo = (GridInfo*)(pFe->pElement);
    ck_assert_msg(pInfo->layoutInfo.visualInfo.width == SIZE_AUTO, "width is not auto");
    ck_assert_msg(pInfo->layoutInfo.visualInfo.height == SIZE_AUTO, "height is not auto");
    ck_assert_msg(pInfo->layoutInfo.visualInfo.actualWidth == 0, "actual width is not 0");
    ck_assert_msg(pInfo->layoutInfo.visualInfo.actualHeight == 0, "actual height is not 0");

    FeSize size = grid_get_size(pFe);
    ck_assert_int_eq(size.width, 0);
    ck_assert_int_eq(size.height, 0);

    FrameworkElement *pFe2 = grid_create_ex(0, 0, 10, 20);
    size = grid_get_size(pFe2);
    ck_assert_int_eq(size.width, 10);
    ck_assert_int_eq(size.height, 20);
}
END_TEST

START_TEST(test_grid_size2)
{
    FrameworkElement *pFe = grid_create_ex(0, 0, SIZE_AUTO, SIZE_AUTO);
    FrameworkElement *pBtnFe = button_create_ex(0, 0, 10, 20, L"test", L"", NULL);
    grid_add_child(pFe, pBtnFe);
    FrameworkElement *pChild = zaml_get_child(pFe, 0);
    ck_assert_msg(pBtnFe == pChild, "child is not the same");

    FeSize size = grid_get_size(pFe);
    ck_assert_int_eq(size.width, 10);
    ck_assert_int_eq(size.height, 20);

    FrameworkElement *pBtnFe2 = button_create_ex(0, 0, 20, 30, L"test2", L"", NULL);
    grid_add_child(pFe, pBtnFe2);
    size = grid_get_size(pFe);
    ck_assert_int_eq(size.width, 20);
    ck_assert_int_eq(size.height, 30);

    FrameworkElement *pBtnFe3 = button_create_ex(0, 0, 10, 10, L"test3", L"", NULL);
    grid_add_child(pFe, pBtnFe3);
    size = grid_get_size(pFe);
    ck_assert_int_eq(size.width, 20);
    ck_assert_int_eq(size.height, 30);

    FrameworkElement *pBtnFe4 = button_create_ex(0, 0, 40, 20, L"test4", L"", NULL);
    grid_add_child(pFe, pBtnFe4);
    size = grid_get_size(pFe);
    ck_assert_int_eq(grid_get_children_count(pFe), 4);
    ck_assert_int_eq(size.width, 40);
    ck_assert_int_eq(size.height, 30);

    FrameworkElement *pBtnFe5 = button_create_ex(0, 0, 10, 40, L"test5", L"", NULL);
    grid_add_child(pFe, pBtnFe5);
    size = grid_get_size(pFe);
    ck_assert_int_eq(size.width, 40);
    ck_assert_int_eq(size.height, 40);
}
END_TEST

START_TEST(test_grid_size3)
{
    FrameworkElement *pFe = grid_create_ex(0, 0, 10, SIZE_AUTO);
    FrameworkElement *pBtnFe = button_create_ex(0, 0, 20, 20, L"test", L"", NULL);
    grid_add_child(pFe, pBtnFe);

    FeSize size = grid_get_size(pFe);
    ck_assert_int_eq(size.width, 10);
    ck_assert_int_eq(size.height, 20);

    FrameworkElement *pFe2 = grid_create_ex(0, 0, SIZE_AUTO, 10);
    grid_add_child(pFe2, pBtnFe);
    size = grid_get_size(pFe2);
    ck_assert_int_eq(size.width, 20);
    ck_assert_int_eq(size.height, 10);

    FrameworkElement *pBtnFe2 = button_create_ex(0, 0, 40, 20, L"test", L"", NULL);
    grid_add_child(pFe2, pBtnFe2);
    size = grid_get_size(pFe2);
    ck_assert_int_eq(size.width, 40);
    ck_assert_int_eq(size.height, 10);
}
END_TEST

START_TEST(test_grid_add_def)
{
    FrameworkElement *pFe = grid_create_ex(0, 0, 10, 10);
    RowDefinition rowDef;
    rowDef.height = 20;
    grid_add_rowdef(pFe, &rowDef);

    ck_assert_int_eq(grid_get_rowdef_count(pFe), 1);
    ck_assert_int_eq(grid_get_coldef_count(pFe), 0);

    RowDefinition *pRowDef = grid_get_rowdef(pFe, 0);
    ColumnDefinition *pColDef = grid_get_coldef(pFe, 0);
    ck_assert_msg(pRowDef != NULL, "rowdef is NULL");
    ck_assert_msg(pColDef == NULL, "coldef is not NULL");
    ck_assert_msg(pRowDef != &rowDef, "rowdef is not copied");
    ck_assert_int_eq(pRowDef->height, 20);

    rowDef.height = 30;
    grid_add_rowdef(pFe, &rowDef);
    ck_assert_int_eq(grid_get_rowdef_count(pFe), 2);
    pRowDef = grid_get_rowdef(pFe, 0);
    ck_assert_int_eq(pRowDef->height, 20);
    pRowDef = grid_get_rowdef(pFe, 1);
    ck_assert_int_eq(pRowDef->height, 30);
    pRowDef = grid_get_rowdef(pFe, 2);
    ck_assert_msg(pRowDef == NULL, "rowdef is not NULL");

    ColumnDefinition colDef;
    colDef.width = 10;
    grid_add_coldef(pFe, &colDef);

    pColDef = grid_get_coldef(pFe, 0);
    ck_assert_int_eq(grid_get_coldef_count(pFe), 1);
    ck_assert_msg(pColDef != NULL, "coldef is NULL");
    ck_assert_int_eq(pColDef->width, 10);
}
END_TEST

START_TEST(test_grid_set_attachprop)
{
    FrameworkElement *pFe = grid_create_ex(0, 0, 10, 10);
    FrameworkElement *pBtnFe = button_create_ex(0, 0, 10, 20, L"test", L"", NULL);
    grid_set_col(pFe, pBtnFe, 1);
    ck_assert_int_eq(grid_get_col(pFe, pBtnFe), 1);
    ck_assert_int_eq(grid_get_row(pFe, pBtnFe), 0);

    FrameworkElement *pBtnFe2 = button_create_ex(0, 0, 10, 20, L"test", L"", NULL);
    grid_set_row(pFe, pBtnFe2, 2);
    ck_assert_int_eq(grid_get_col(pFe, pBtnFe2), 0);
    ck_assert_int_eq(grid_get_row(pFe, pBtnFe2), 2);

    grid_set_colspan(pFe, pBtnFe, 3);
    ck_assert_int_eq(grid_get_colspan(pFe, pBtnFe), 3);
    ck_assert_int_eq(grid_get_rowspan(pFe, pBtnFe), 1);
    grid_set_rowspan(pFe, pBtnFe2, 3);
    ck_assert_int_eq(grid_get_colspan(pFe, pBtnFe2), 1);
    ck_assert_int_eq(grid_get_rowspan(pFe, pBtnFe2), 3);

    grid_set_col(pFe, pBtnFe, 2);
    ck_assert_int_eq(grid_get_col(pFe, pBtnFe), 2);
    grid_set_row(pFe, pBtnFe2, 3);
    ck_assert_int_eq(grid_get_row(pFe, pBtnFe2), 3);
}
END_TEST

START_TEST(test_grid_size4_pos)
{
    FrameworkElement *pFe = grid_create_ex(0, 0, SIZE_AUTO, SIZE_AUTO);
    FrameworkElement *pBtnFe = button_create_ex(0, 0, 10, 10, L"test", L"", NULL);
    grid_add_child(pFe, pBtnFe);

    ButtonInfo *pBtnInfo = (ButtonInfo*)(pBtnFe->pElement);
    ck_assert_int_eq(pBtnInfo->controlInfo.visualInfo.x, 0);
    ck_assert_int_eq(pBtnInfo->controlInfo.visualInfo.y, 0);

    ColumnDefinition colDef;
    colDef.width = SIZE_AUTO;
    grid_add_coldef(pFe, &colDef);

    FeSize size = grid_get_size(pFe);
    ck_assert_int_eq(size.width, 10);
    ck_assert_int_eq(size.height, 10);

    ColumnDefinition *pColDef = grid_get_coldef(pFe, 0);
    coldef_set_width(pColDef, 20);
    size = grid_get_size(pFe);
    ck_assert_int_eq(size.width, 20);

    RowDefinition rowDef;
    rowDef.height = 20;
    grid_add_rowdef(pFe, &rowDef);
    size = grid_get_size(pFe);
    ck_assert_int_eq(size.height, 20);

    RowDefinition *pRowDef = grid_get_rowdef(pFe, 0);
    pRowDef->height = SIZE_AUTO;

    pColDef->width = SIZE_AUTO;
    grid_add_coldef(pFe, &colDef);
    FrameworkElement *pBtnFe2 = button_create_ex(0, 0, 20, 20, L"test", L"", NULL);
    grid_add_child(pFe, pBtnFe2);
    grid_set_col(pFe, pBtnFe, 0);
    grid_set_col(pFe, pBtnFe2, 1);

    pBtnInfo = (ButtonInfo*)(pBtnFe2->pElement);
    ck_assert_int_eq(pBtnInfo->controlInfo.visualInfo.x, 10);
    ck_assert_int_eq(pBtnInfo->controlInfo.visualInfo.y, 0);

    size = grid_get_size(pFe);
    ck_assert_int_eq(size.width, 30);
    ck_assert_int_eq(size.height, 20);

    rowDef.height = SIZE_AUTO;
    grid_add_rowdef(pFe, &rowDef);
    FrameworkElement *pBtnFe3 = button_create_ex(0, 0, 10, 20, L"test", L"", NULL);
    grid_add_child(pFe, pBtnFe3);
    grid_set_col(pFe, pBtnFe3, 0);
    grid_set_row(pFe, pBtnFe3, 1);
    size = grid_get_size(pFe);
    ck_assert_int_eq(size.width, 30);
    ck_assert_int_eq(size.height, 40);

    pBtnInfo = (ButtonInfo*)(pBtnFe3->pElement);
    ck_assert_int_eq(pBtnInfo->controlInfo.visualInfo.x, 0);
    ck_assert_int_eq(pBtnInfo->controlInfo.visualInfo.y, 20);

    pColDef = grid_get_coldef(pFe, 1);
    pColDef->width = 5;
    grid_add_coldef(pFe, &colDef);
    grid_set_col(pFe, pBtnFe2, 2);
    size = grid_get_size(pFe);
    ck_assert_int_eq(size.width, 35);
}
END_TEST

Suite* make_add_suit(void)
{
    Suite *s = suite_create("ngf");

    TCase *tc_grid = tcase_create("grid");
    tcase_add_test(tc_grid, test_grid_create);
    tcase_add_test(tc_grid, test_grid_add_child);
    tcase_add_test(tc_grid, test_grid_size);
    tcase_add_test(tc_grid, test_grid_size2);
    tcase_add_test(tc_grid, test_grid_size3);
    tcase_add_test(tc_grid, test_grid_add_def);
    tcase_add_test(tc_grid, test_grid_set_attachprop);
    tcase_add_test(tc_grid, test_grid_size4_pos);
    suite_add_tcase(s, tc_grid);

    return s;
}

int main(void)
{
    int n;
    SRunner *sr;
    Suite *s = make_add_suit();
    sr = srunner_create(s);
    srunner_run_all(sr, CK_NORMAL);
    n = srunner_ntests_failed(sr);
    srunner_free(sr);
    return n == 0 ? EXIT_SUCCESS : EXIT_FAILURE;
}
