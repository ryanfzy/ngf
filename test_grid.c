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
    FrameworkElement *pChild = grid_get_child(pFe, 0);
    ck_assert_msg(pBtnFe == pChild, "child is not the same");

    FeSize size = grid_get_size(pFe);
    ck_assert_int_eq(size.width, 10);
    ck_assert_int_eq(size.height, 20);
}
END_TEST

Suite* make_add_suit(void)
{
    Suite *s = suite_create("ngf");

    TCase *tc_grid = tcase_create("grid");
    tcase_add_test(tc_grid, test_grid_create);
    tcase_add_test(tc_grid, test_grid_size);
    tcase_add_test(tc_grid, test_grid_size2);
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
