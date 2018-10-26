/**
 * @file DoublyLinkedListTests.c
 * 
 * @author Leonardo Vencovsky (https://github.com/LeoVen)
 *
 * @date 18/10/2018
 */

#include "DoublyLinkedList.h"
#include "UnitTest.h"

// Tests dll_get
Status dll_test_get(UnitTest ut)
{
    DoublyLinkedList list;

    Status st = dll_init(&list);

    if (st != DS_OK)
        return st;

    for (int i = 0; i < 10; i++)
    {
        st = dll_insert_tail(list, i);

        if (st != DS_OK)
            goto error;
    }

    // Tests all edge cases
    int j, k, l, m;
    st += dll_get(list, &j, 9);
    st += dll_get(list, &k, 5);
    st += dll_get(list, &l, 4);
    st += dll_get(list, &m, 0);

    if (st != DS_OK)
        goto error;

    ut_equals_int(ut, j, 9, __func__);
    ut_equals_int(ut, k, 5, __func__);
    ut_equals_int(ut, l, 4, __func__);
    ut_equals_int(ut, m, 0, __func__);

    // Tests on odd list size
    st += dll_insert_tail(list, 11);
    st += dll_get(list, &j, 5);

    if (st != DS_OK)
        goto error;

    ut_equals_int(ut, j, 5, __func__);

    dll_delete(&list);

    return DS_OK;

    error:
    dll_delete(&list);
    return st;
}

// Tests limit functionality
Status dll_test_limit(UnitTest ut)
{
    DoublyLinkedList list;

    Status st = dll_init(&list);

    if (st != DS_OK)
        return st;

    st = dll_limit(list, 10);

    if (st != DS_OK)
        goto error;

    for (int i = 0; i < 20; i++)
    {
        st = dll_insert_tail(list, i);
    }

    ut_equals_int(ut, st, DS_ERR_FULL, __func__);
    ut_equals_index_t(ut, list->length, list->limit, __func__);
    ut_equals_int(ut, dll_limit(list, 9), DS_ERR_INVALID_OPERATION, __func__);

    ut_equals_int(ut, dll_insert_head(list, 1), DS_ERR_FULL, __func__);
    ut_equals_int(ut, dll_insert_at(list, 1, 1), DS_ERR_FULL, __func__);
    ut_equals_int(ut, dll_insert_tail(list, 1), DS_ERR_FULL, __func__);

    // Removes the limit
    ut_equals_int(ut, dll_limit(list, 0), DS_OK, __func__);
    ut_equals_index_t(ut, list->limit, 0, __func__);
    ut_equals_int(ut, dll_insert_tail(list, 10), DS_OK, __func__);

    dll_delete(&list);

    return DS_OK;

    error:
    dll_delete(&list);
    return st;
}

// Tests indexof functions
Status dll_test_indexof(UnitTest ut)
{
    DoublyLinkedList list;

    Status st = dll_init(&list);

    if (st != DS_OK)
        return st;

    // 0, 1, 2, 0, 1, 2, 0, 1, 2
    for (int i = 0; i < 9; i++)
    {
        st = dll_insert_tail(list, i % 3);

        if (st != DS_OK)
            goto error;
    }

    int r0, r1, r2;
    int d0, d1, d2;
    index_t f1, f2;

    st += dll_get(list, &r0, 0);
    st += dll_get(list, &d0, dll_index_first(list, 0));
    st += dll_get(list, &r1, 1);
    st += dll_get(list, &d1, dll_index_first(list, 1));
    st += dll_get(list, &r2, 2);
    st += dll_get(list, &d2, dll_index_first(list, 2));

    if (st != DS_OK)
        goto error;

    ut_equals_int(ut, r0, d0, __func__);
    ut_equals_int(ut, r1, d1, __func__);
    ut_equals_int(ut, r2, d2, __func__);

    st += dll_get(list, &r0, list->length - 3);
    st += dll_get(list, &d0, dll_index_last(list, 0));
    st += dll_get(list, &r1, list->length - 2);
    st += dll_get(list, &d1, dll_index_last(list, 1));
    st += dll_get(list, &r2, list->length - 1);
    st += dll_get(list, &d2, dll_index_last(list, 2));

    if (st != DS_OK)
        goto error;

    ut_equals_int(ut, r0, d0, __func__);
    ut_equals_int(ut, r1, d1, __func__);
    ut_equals_int(ut, r2, d2, __func__);

    f1 = dll_index_first(list, 3);
    f2 = dll_index_last(list, 3);

    ut_equals_index_t(ut, f1, -1, __func__);
    ut_equals_index_t(ut, f2, -1, __func__);

    dll_delete(&list);

    error:
    dll_delete(&list);
    return st;
}

// Runs all DoublyLinkedList tests
Status DoublyLinkedListTests(void)
{
    UnitTest ut;

    Status st = ut_init(&ut);

    if (st != DS_OK)
        goto error;

    st += dll_test_get(ut);
    st += dll_test_limit(ut);
    st += dll_test_indexof(ut);

    if (st != DS_OK)
        goto error;

    ut_report(ut, "DoublyLinkedList");

    ut_delete(&ut);

    return DS_OK;

    error:
    ut_report(ut, "DoublyLinkedList");
    ut_delete(&ut);
    return st;
}