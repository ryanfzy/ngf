#include <stdlib.h>
#include <check.h>
#include "property.h"
#include "dcitem.h"

typedef struct _TestData
{
    int ivalue;
    double dvalue;
} TestData;

void test_command(char *pCmdPar){}
void test_command2(char *pCmdPar){}

START_TEST(test_pr_init)
{
    PropertyInfo strInfo, strInfo2;
    PropertyInfo intInfo;
    PropertyInfo cmdInfo, cmdInfo2;
    PropertyInfo ptrInfo, ptrInfo2;

    propinfo_init_str(&strInfo, L"test");
    wchar_t *szStr = (wchar_t*)propinfo_get(&(strInfo));
    ck_assert_msg(wcscmp(szStr, L"test") == 0, "str is wrong");

    propinfo_init_str(&strInfo2, NULL);
    ck_assert_msg(propinfo_get(&strInfo2) == NULL, "str info is not NULL");

    propinfo_init_int(&intInfo, 10);
    int iNum = *((int*)propinfo_get(&intInfo));
    ck_assert_int_eq(iNum, 10);

    propinfo_init_cmd(&cmdInfo, test_command);
    CommandFn fnCmd = (CommandFn)propinfo_get(&cmdInfo);
    ck_assert_msg(fnCmd == test_command, "cmd is wrong");

    propinfo_init_cmd(&cmdInfo2, NULL);
    ck_assert_msg(propinfo_get(&cmdInfo2) == NULL, "cmd info is not NULL");

    propinfo_init_ptr(&ptrInfo, (char*)&ptrInfo);
    ck_assert_msg(propinfo_get(&ptrInfo) == (char*)&ptrInfo, "ptr is wrong");

    propinfo_init_ptr(&ptrInfo2, NULL);
    ck_assert_msg(propinfo_get(&ptrInfo2) == NULL, "ptr info is not NULL");

    propinfo_destroy(&strInfo);
    propinfo_destroy(&strInfo2);
    propinfo_destroy(&intInfo);
    propinfo_destroy(&cmdInfo);
    propinfo_destroy(&cmdInfo2);
    propinfo_destroy(&ptrInfo);
    propinfo_destroy(&ptrInfo2);
}
END_TEST

START_TEST(test_pr_set)
{
    PropertyInfo strInfo;
    propinfo_init_str(&strInfo, NULL);
    wchar_t *szTest = L"test";
    propinfo_set(&strInfo, (char*)szTest);
    wchar_t *szStr = (wchar_t*)propinfo_get(&(strInfo));
    ck_assert_msg(wcscmp(szStr, szTest) == 0, "str is wrong");

    PropertyInfo intInfo;
    propinfo_init_int(&intInfo, 10);
    int iTest = 20;
    propinfo_set(&intInfo, (char*)&iTest);
    int iNum = *((int*)propinfo_get(&intInfo));
    ck_assert_int_eq(iNum, iTest);

    PropertyInfo cmdInfo;
    propinfo_init_cmd(&cmdInfo, NULL);
    propinfo_set(&cmdInfo, (char*)test_command);
    CommandFn fnCmd = (CommandFn)propinfo_get(&cmdInfo);
    ck_assert_msg(fnCmd == test_command, "cmd is wrong");

    PropertyInfo ptrInfo;
    propinfo_init_ptr(&ptrInfo, NULL);
    propinfo_set(&ptrInfo, (char*)&ptrInfo);
    ck_assert_msg(propinfo_get(&ptrInfo) == (char*)&ptrInfo, "ptr is wrong");
}
END_TEST

START_TEST(test_bind)
{
    PropertyInfo pInfo;
    propinfo_init_str(&pInfo, NULL);
    StrItem strItem;
    stritem_init(&strItem);
    propinfo_bind(&pInfo, &(strItem.item));

    stritem_set(&strItem, L"test");
    wchar_t *szTest = (wchar_t*)propinfo_get(&pInfo);
    ck_assert_msg(wcscmp(szTest, L"test") == 0, "property is wrong");

    CmdItem cmdItem;
    cmditem_init(&cmdItem);
    propinfo_bind(&pInfo, &(cmdItem.item));
    
    cmditem_set(&cmdItem, test_command);
    ck_assert_msg(wcscmp(stritem_get(&strItem), L"test") == 0, "str is wrong");
    CommandFn fnCommand = (CommandFn)propinfo_get(&pInfo);
    ck_assert_msg(fnCommand == test_command, "property is wrong");

    cmditem_set(&cmdItem, test_command2);
    fnCommand = (CommandFn)propinfo_get(&pInfo);
    ck_assert_msg(fnCommand == test_command2, "property 2 is wrong");
}
END_TEST

Suite* make_add_suit(void)
{
    Suite *s = suite_create("ngf");

    TCase *tc_pr = tcase_create("property");
    tcase_add_test(tc_pr, test_pr_init);
    tcase_add_test(tc_pr, test_pr_set);
    tcase_add_test(tc_pr, test_bind);
    suite_add_tcase(s, tc_pr);

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
