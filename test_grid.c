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
    zaml_add_child(pFe, pBtnFe);
    FrameworkElement *pChild = zaml_get_child(pFe, 0);
    ck_assert_msg(pBtnFe == pChild, "child is not the same");

    FeSize size = grid_get_size(pFe);
    ck_assert_int_eq(size.width, 10);
    ck_assert_int_eq(size.height, 20);

    FrameworkElement *pBtnFe2 = button_create_ex(0, 0, 20, 30, L"test2", L"", NULL);
    zaml_add_child(pFe, pBtnFe2);
    size = grid_get_size(pFe);
    ck_assert_int_eq(size.width, 20);
    ck_assert_int_eq(size.height, 30);

    FrameworkElement *pBtnFe3 = button_create_ex(0, 0, 10, 10, L"test3", L"", NULL);
    zaml_add_child(pFe, pBtnFe3);
    size = grid_get_size(pFe);
    ck_assert_int_eq(size.width, 20);
    ck_assert_int_eq(size.height, 30);

    FrameworkElement *pBtnFe4 = button_create_ex(0, 0, 40, 20, L"test4", L"", NULL);
    zaml_add_child(pFe, pBtnFe4);
    size = grid_get_size(pFe);
    ck_assert_int_eq(grid_get_children_count(pFe), 4);
    ck_assert_int_eq(size.width, 40);
    ck_assert_int_eq(size.height, 30);

    FrameworkElement *pBtnFe5 = button_create_ex(0, 0, 10, 40, L"test5", L"", NULL);
    zaml_add_child(pFe, pBtnFe5);
    size = grid_get_size(pFe);
    ck_assert_int_eq(size.width, 40);
    ck_assert_int_eq(size.height, 40);
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

Suite* make_add_suit(void)
{
    Suite *s = suite_create("ngf");

    TCase *tc_grid = tcase_create("grid");
    tcase_add_test(tc_grid, test_grid_create);
    tcase_add_test(tc_grid, test_grid_add_child);
    tcase_add_test(tc_grid, test_grid_size);
    tcase_add_test(tc_grid, test_grid_size2);
    tcase_add_test(tc_grid, test_grid_add_def);
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
