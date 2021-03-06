#include <stdlib.h>
#include <check.h>
#include "property.h"

typedef struct _TestData
{
    int ivalue;
    double dvalue;
} TestData;

void test_command(char *pCmdPar){}
void test_command2(char *pCmdPar){}

START_TEST(test_init)
{
    StrItem strItem;
    CmdItem cmdItem;
    stritem_init(&strItem);
    cmditem_init(&cmdItem);

    ck_assert_msg(stritem_get(&strItem) == NULL, "str item is not NULL");
    ck_assert_msg(cmditem_get(&cmdItem) == NULL, "cmd item is not NULL");

    stritem_destroy(&strItem);
    cmditem_destroy(&cmdItem);
}
END_TEST

START_TEST(test_set)
{
    StrItem strItem;
    stritem_init(&strItem);
    stritem_set(&strItem, L"test");
    ck_assert_msg(wcscmp(stritem_get(&strItem), L"test") == 0, "str is wrong");

    stritem_set(&strItem, L"test2");
    ck_assert_msg(wcscmp(stritem_get(&strItem), L"test2") == 0, "str2 is wrong");

    stritem_set(&strItem, NULL);
    ck_assert_msg(stritem_get(&strItem) == NULL, "str3 is not NULL");

    CmdItem cmdItem;
    cmditem_init(&cmdItem);
    cmditem_set(&cmdItem, test_command);
    ck_assert_msg(cmditem_get(&cmdItem) == test_command, "cmd is wrong");

    cmditem_set(&cmdItem, test_command2);
    ck_assert_msg(cmditem_get(&cmdItem) == test_command2, "cmd2 is wrong");

    stritem_destroy(&strItem);
    ck_assert_msg(stritem_get(&strItem) == NULL, "str item is not NULL");
    cmditem_destroy(&cmdItem);
    ck_assert_msg(cmditem_get(&cmdItem) == NULL, "cmd item is not NULL");
}
END_TEST

static void prop_changed_evt(char *pEvtArg)
{
    TestData *pt = *(TestData**)pEvtArg;
    if (pt != NULL)
        pt->ivalue = 2;
}

START_TEST(test_set2)
{
    TestData t;
    t.ivalue = 1;
    StrItem strItem;
    stritem_init(&strItem);
    PropertyInfo propInfo;
    propinfo_init_str(&propInfo, NULL);
    propinfo_bind(&propInfo, &(strItem.item));
    TestData *pt = &t;
    propinfo_sub_changed_evt(&propInfo, prop_changed_evt, (char*)&pt, sizeof(TestData**));
    ck_assert_int_eq(t.ivalue, 1);
    stritem_set(&strItem, L"test");
    ck_assert_int_eq(t.ivalue, 2);
}
END_TEST

Suite* make_add_suit(void)
{
    Suite *s = suite_create("ngf");

    TCase *tc = tcase_create("dcitem");
    tcase_add_test(tc, test_init);
    tcase_add_test(tc, test_set);
    tcase_add_test(tc, test_set2);
    suite_add_tcase(s, tc);

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
