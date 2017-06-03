#include <stdlib.h>
#include <check.h>
#include "datacontext.h"
#include "property.h"

typedef struct _TestData
{
    int ivalue;
    double dvalue;
} TestData;

static void fnTestRaiseEvent(Event *pEvt)
{
}

START_TEST(test_dc_create1)
{
    DataContext *pDc = create_datacontext();

    ck_assert_msg(pDc->pEvtHandler == NULL, "evt handler is not NULL");
    for (int i = 0; i < 256; i++)
        ck_assert_int_eq(pDc->dict.nodes[i].icount, 0);

    free_datacontext(pDc);
}
END_TEST

START_TEST(test_dc_create2)
{
    EventHandler sEvtHandler;
    sEvtHandler.fnRaiseEvent = fnTestRaiseEvent;
    DataContext *pDc = create_dc_ex(&sEvtHandler);

    ck_assert_msg(pDc->pEvtHandler == &sEvtHandler, "evt handler is wrong");
    ck_assert_msg(pDc->pEvtHandler->fnRaiseEvent == fnTestRaiseEvent, "evt handler fn is wrong");
    for (int i = 0; i < 256; i++)
        ck_assert_int_eq(pDc->dict.nodes[i].icount, 0);

    free_datacontext(pDc);
}
END_TEST

START_TEST(test_dc_add_str)
{
    wchar_t *pExpect = L"test value";
    DataContext *pDc = create_datacontext();
    DataContext_add_str(pDc, L"test", pExpect);
    DataContext_add_str(pDc, L"test2", L"test2 value");

    wchar_t *pActual = NULL;
    int iLen = DataContext_get_str(pDc, L"test", &pActual);

    ck_assert_int_eq(iLen, 10);
    ck_assert_msg(pActual != NULL, "pActual is NULL");
    ck_assert_msg(wcscmp(pActual, L"test value") == 0, "pActual is wrong");
    ck_assert_msg(pExpect != pActual, "pExpect and pActual refers to the same memory location");

    iLen = DataContext_get_str(pDc, L"test2", &pActual);
    ck_assert_int_eq(iLen, 11);
    ck_assert_msg(wcscmp(pActual, L"test2 value") == 0, "pActual is wrong");

    pActual = NULL;
    iLen = DataContext_get_str(pDc, L"test3", &pActual);
    ck_assert_int_eq(iLen, 0);
    ck_assert_msg(pActual == NULL, "pActual is not NULL");

    DataContext_set_str(pDc, L"test", L"test3 value");
    iLen = DataContext_get_str(pDc, L"test", &pActual);
    ck_assert_int_eq(iLen, 11);
    ck_assert_msg(wcscmp(pActual, L"test3 value") == 0, "pActual is wrong");

    free_datacontext(pDc);
}
END_TEST

START_TEST(test_dc_add_object)
{
    TestData t1, t2;
    t1.ivalue = 1;
    t2.ivalue = 2;
    DataContext *pDc = create_datacontext();

    DataContext_add_object(pDc, L"test1", (char*)&t1, sizeof(TestData));
    DataContext_add_object(pDc, L"test2", (char*)&t2, sizeof(TestData));

    TestData *pt = (TestData*)DataContext_get_object(pDc, L"test1");
    ck_assert_msg(pt != NULL, "pt is NULL");
    ck_assert_msg(pt != &t1, "pt and t1 refers to the same memory location");
    ck_assert_int_eq(pt->ivalue, 1);

    pt = (TestData*)DataContext_get_object(pDc, L"test2");
    ck_assert_int_eq(pt->ivalue, 2);

    pt = (TestData*)DataContext_get_object(pDc, L"test3");
    ck_assert_msg(pt == NULL, "pt is not NULL");

    free_datacontext(pDc);
}
END_TEST

static void fnTestCommand(DataContext *pDc)
{
    TestData *pt = (TestData*)DataContext_get_object(pDc, L"test par");
    ck_assert_int_eq(pt->ivalue, 1);
    pt->ivalue = 2;
}

START_TEST(test_dc_add_cmd)
{
    TestData t;
    t.ivalue = 1;
    DataContext *pDc = create_datacontext();
    DataContext_add_command(pDc, L"test cmd", fnTestCommand);
    DataContext_add_object(pDc, L"test par", (char*)&t, sizeof(TestData));

    DataContext_run_command(pDc, L"test cmd");
    TestData *pt = (TestData*)DataContext_get_object(pDc, L"test par");
    ck_assert_int_eq(pt->ivalue, 2);

    free_datacontext(pDc);
}
END_TEST

START_TEST(test_dc_update1)
{
    DataContext *pDc = create_datacontext();
    DataContext_add_str(pDc, L"test", L"test str");
    DataContext_update(pDc, L"test");
    free_datacontext(pDc);
}
END_TEST

static void fnTestEventHandler(Event *pEvt)
{
    ck_assert_msg(pEvt->eEvtType == EVENTTYPE_UPDATE, "event is not update event");
    TestData *pt = (TestData*)pEvt->pObserver;
    ck_assert_int_eq(pt->ivalue, 1);
    pt->ivalue = 2;
}

START_TEST(test_dc_observe)
{
    EventHandler sEvtHandler;
    sEvtHandler.fnRaiseEvent = fnTestEventHandler;
    DataContext *pDc = create_dc_ex(&sEvtHandler);

    TestData t;
    t.ivalue = 1;
    DataContext_add_str(pDc, L"test", L"test str");
    DataContext_observe(pDc, L"test", (char*)&t, sizeof(TestData));
    DataContext_update(pDc, L"test");

    ck_assert_int_eq(t.ivalue, 1);

    free_datacontext(pDc);
}
END_TEST

START_TEST(test_prop_create)
{
    PropertyInfo *pInfo = propinfo_create_ex(PropertyType_Str);
    ck_assert_msg(pInfo->eType == PropertyType_Str, "property type is not string");
    ck_assert_msg(pInfo->pBinding == NULL, "binding is not NULL");
    ck_assert_msg(pInfo->pValue == NULL, "value is not NULL");
    propinfo_free(pInfo);
}
END_TEST

START_TEST(test_prop_set_str)
{
    PropertyInfo *pInfo = propinfo_create_ex(PropertyType_Str);
    wchar_t *szText = L"123";
    propinfo_set(pInfo, (char*)szText);
    ck_assert_msg(wcscmp((wchar_t*)(pInfo->pValue), szText) == 0, "set is not working");
}
END_TEST

START_TEST(test_prop_get_str)
{
    PropertyInfo *pInfo = propinfo_create_ex(PropertyType_Str);
    wchar_t *szText = L"test";
    propinfo_set(pInfo, (char*)szText);

    wchar_t *szExp = NULL;
    bool ret = propinfo_get(pInfo, (char*)(&szExp));
    ck_assert_msg(ret == true, "get return false");
    ck_assert_msg(szExp != NULL, "exp is NULL");
    ck_assert_msg(wcscmp((wchar_t*)szExp, szText) == 0, "get is not working");
}
END_TEST

START_TEST(test_binding_create)
{
    DataContext *pDc = create_datacontext();
    DataContext_add_str(pDc, L"key", L"value");
    BindingInfo *pBinding = binding_create(L"key", pDc);

    ck_assert_msg(pBinding->pDc != NULL, "dc is NULL");
    ck_assert_msg(pBinding->pDc == pDc, "dc is not correct");
    ck_assert_msg(wcscmp(pBinding->szKey, L"key") == 0, "key is wrong");
}
END_TEST

START_TEST(test_prop_binding)
{
    DataContext *pDc = create_datacontext();
    DataContext_add_str(pDc, L"key", L"value");
    BindingInfo *pBinding = binding_create(L"key", pDc);

    PropertyInfo *pInfo = propinfo_create_ex(PropertyType_Str);
    propinfo_set_binding(pInfo, pBinding);

    ck_assert_msg(pInfo->pBinding != NULL, "binding is NULL");
    ck_assert_msg(pInfo->pBinding == pBinding, "binding is not correct");
    ck_assert_msg(pInfo->pValue == NULL, "value is not NULL");

    wchar_t* szExp = NULL;
    bool ret = propinfo_get(pInfo, (char*)(&szExp));
    ck_assert_msg(ret == true, "get returns false");
    ck_assert_msg(szExp != NULL, "returned value is NULL");
    ck_assert_msg(wcscmp(szExp, L"value") == 0, "returned value is not correct");

    DataContext_set_str(pDc, L"key", L"value 2");
    propinfo_get(pInfo, (char*)(&szExp));
    ck_assert_msg(wcscmp(szExp, L"value 2") == 0, "returned value is not correct");
}
END_TEST

Suite* make_add_suit(void)
{
    Suite *s = suite_create("ngf");

    TCase *tc_dc = tcase_create("dc");
    tcase_add_test(tc_dc, test_dc_create1);
    tcase_add_test(tc_dc, test_dc_create2);
    tcase_add_test(tc_dc, test_dc_add_str);
    tcase_add_test(tc_dc, test_dc_add_object);
    tcase_add_test(tc_dc, test_dc_add_cmd);
    tcase_add_test(tc_dc, test_dc_update1);
    tcase_add_test(tc_dc, test_dc_observe);
    suite_add_tcase(s, tc_dc);

    TCase *tc_prop = tcase_create("prop");
    tcase_add_test(tc_prop, test_prop_create);
    tcase_add_test(tc_prop, test_prop_set_str);
    tcase_add_test(tc_prop, test_prop_get_str);
    tcase_add_test(tc_prop, test_binding_create);
    tcase_add_test(tc_prop, test_prop_binding);
    suite_add_tcase(s, tc_prop);

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
