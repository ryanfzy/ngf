#include <stdlib.h>
#include <check.h>
#include "btninfo.h"
#include "dcitem.h"

typedef struct _TestData
{
    int ivalue;
    double dvalue;
} TestData;

void test_command(char *pCmdPar)
{
    TestData *pt = (TestData*)pCmdPar;
    if (pt != NULL)
        pt->ivalue = 2;
}
void test_command2(char *pCmdPar){}

START_TEST(test_init)
{
    FrameworkElement *pFe = button_create();
    button_free(pFe);
}
END_TEST

START_TEST(test_set)
{
    FrameworkElement *pFe = button_create();
    button_set_text(pFe, L"test");
    wchar_t *szText = button_get_text(pFe);
    ck_assert_msg(wcscmp(szText, L"test") == 0, "text is wrong");

    TestData t;
    button_set_cmdparameter(pFe, (char*)&t);
    TestData *pt = (TestData*)button_get_cmdparameter(pFe);
    ck_assert_msg(pt != NULL, "cmd parameter is NULL");
    ck_assert_msg(pt == &t, "cmd parameter is wrong");
}
END_TEST

START_TEST(test_bind)
{
    FrameworkElement *pFe = button_create();
    CmdItem cmdItem;
    cmditem_init(&cmdItem);
    cmditem_set(&cmdItem, test_command);
    button_bind_command(pFe, &cmdItem);

    TestData t;
    t.ivalue = 1;
    button_set_cmdparameter(pFe, (char*)&t);
    ck_assert_int_eq(t.ivalue, 1);
    btn_raise_click_evt(pFe);
    ck_assert_int_eq(t.ivalue, 2);
}
END_TEST

Suite* make_suit(void)
{
    Suite *s = suite_create("ngf");

    TCase *tc = tcase_create("button");
    tcase_add_test(tc, test_init);
    tcase_add_test(tc, test_set);
    tcase_add_test(tc, test_bind);
    suite_add_tcase(s, tc);

    return s;
}

int main(void)
{
    int n;
    SRunner *sr;
    Suite *s = make_suit();
    sr = srunner_create(s);
    srunner_run_all(sr, CK_NORMAL);
    n = srunner_ntests_failed(sr);
    srunner_free(sr);
    return n == 0 ? EXIT_SUCCESS : EXIT_FAILURE;
}
