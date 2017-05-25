#include <stdlib.h>
#include <check.h>
#include "datacontext.h"

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
    char *pExpect = "test value";
    DataContext *pDc = create_datacontext();
    DataContext_add_str(pDc, "test", pExpect, strlen(pExpect));
    DataContext_add_str(pDc, "test2", "test2 value", 11);

    char *pActual = NULL;
    int iLen = DataContext_get_str(pDc, "test", &pActual);

    ck_assert_int_eq(iLen, 10);
    ck_assert_msg(pActual != NULL, "pActual is NULL");
    ck_assert_msg(strcmp(pActual, "test value") == 0, "pActual is wrong");
    ck_assert_msg(pExpect != pActual, "pExpect and pActual refers to the same memory location");

    iLen = DataContext_get_str(pDc, "test2", &pActual);
    ck_assert_int_eq(iLen, 11);
    ck_assert_msg(strcmp(pActual, "test2 value") == 0, "pActual is wrong");

    pActual = NULL;
    iLen = DataContext_get_str(pDc, "test3", &pActual);
    ck_assert_int_eq(iLen, 0);
    ck_assert_msg(pActual == NULL, "pActual is not NULL");

    DataContext_set_str(pDc, "test", "test3 value", 11);
    iLen = DataContext_get_str(pDc, "test", &pActual);
    ck_assert_int_eq(iLen, 11);
    ck_assert_msg(strcmp(pActual, "test3 value") == 0, "pActual is wrong");

    free_datacontext(pDc);
}
END_TEST

START_TEST(test_dc_add_object)
{
    TestData t1, t2;
    t1.ivalue = 1;
    t2.ivalue = 2;
    DataContext *pDc = create_datacontext();

    DataContext_add_object(pDc, "test1", (char*)&t1, sizeof(TestData));
    DataContext_add_object(pDc, "test2", (char*)&t2, sizeof(TestData));

    TestData *pt = (TestData*)DataContext_get_object(pDc, "test1");
    ck_assert_msg(pt != NULL, "pt is NULL");
    ck_assert_msg(pt != &t1, "pt and t1 refers to the same memory location");
    ck_assert_int_eq(pt->ivalue, 1);

    pt = (TestData*)DataContext_get_object(pDc, "test2");
    ck_assert_int_eq(pt->ivalue, 2);

    pt = (TestData*)DataContext_get_object(pDc, "test3");
    ck_assert_msg(pt == NULL, "pt is not NULL");

    free_datacontext(pDc);
}
END_TEST

static void fnTestCommand(DataContext *pDc)
{
    TestData *pt = (TestData*)DataContext_get_object(pDc, "test par");
    ck_assert_int_eq(pt->ivalue, 1);
    pt->ivalue = 2;
}

START_TEST(test_dc_add_cmd)
{
    TestData t;
    t.ivalue = 1;
    DataContext *pDc = create_datacontext();
    DataContext_add_command(pDc, "test cmd", fnTestCommand);
    DataContext_add_object(pDc, "test par", (char*)&t, sizeof(TestData));

    DataContext_run_command(pDc, "test cmd");
    TestData *pt = (TestData*)DataContext_get_object(pDc, "test par");
    ck_assert_int_eq(pt->ivalue, 2);

    free_datacontext(pDc);
}
END_TEST

START_TEST(test_dc_update1)
{
    DataContext *pDc = create_datacontext();
    DataContext_add_str(pDc, "test", "test str", 8);
    DataContext_update(pDc, "test");
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
    DataContext_add_str(pDc, "test", "test str", 8);
    DataContext_observe(pDc, "test", (char*)&t, sizeof(TestData));
    DataContext_update(pDc, "test");

    ck_assert_int_eq(t.ivalue, 1);

    free_datacontext(pDc);
}
END_TEST

Suite* make_add_suit(void)
{
    Suite *s = suite_create("dc");
    TCase *tc_dc = tcase_create("dc");
    tcase_add_test(tc_dc, test_dc_create1);
    tcase_add_test(tc_dc, test_dc_create2);
    tcase_add_test(tc_dc, test_dc_add_str);
    tcase_add_test(tc_dc, test_dc_add_object);
    tcase_add_test(tc_dc, test_dc_add_cmd);
    tcase_add_test(tc_dc, test_dc_update1);
    tcase_add_test(tc_dc, test_dc_observe);
    suite_add_tcase(s, tc_dc);
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
