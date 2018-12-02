/**
 * @file SinglyLinkedListTests.c
 * 
 * @author Leonardo Vencovsky (https://github.com/LeoVen)
 *
 * @date 17/10/2018
 */

#include "SinglyLinkedList.h"
#include "UnitTest.h"

// Tests insertions and removals at the middle of the list
Status sll_test_middle(UnitTest ut)
{
    SinglyLinkedList list;

    Status st = sll_init(&list);

    if (st != DS_OK)
        return st;

    for (int i = 0; i < 10; i++)
    {
        st = sll_insert_tail(list, i);

        if (st != DS_OK)
            goto error;
    }

    int j;
    st += sll_insert_at(list, 99, 5);
    st += sll_remove_at(list, &j, 5);

    if (st != DS_OK)
        goto error;

    ut_equals_int(ut, j, 99, __func__);

    sll_delete(&list);

    return DS_OK;

    error:
    sll_delete(&list);
    return st;
}

// Tests limit functionality
Status sll_test_limit(UnitTest ut)
{
    SinglyLinkedList list;

    Status st = sll_init(&list);

    if (st != DS_OK)
        return st;

    st = sll_limit(list, 10);

    if (st != DS_OK)
        goto error;

    for (int i = 0; i < 20; i++)
    {
        st = sll_insert_tail(list, i);
    }

    ut_equals_int(ut, st, DS_ERR_FULL, __func__);
    ut_equals_integer_t(ut, list->length, list->limit, __func__);
    ut_equals_int(ut, sll_limit(list, 9), DS_ERR_INVALID_OPERATION, __func__);

    ut_equals_int(ut, sll_insert_head(list, 1), DS_ERR_FULL, __func__);
    ut_equals_int(ut, sll_insert_at(list, 1, 1), DS_ERR_FULL, __func__);
    ut_equals_int(ut, sll_insert_tail(list, 1), DS_ERR_FULL, __func__);

    // Removes the limit
    ut_equals_int(ut, sll_limit(list, 0), DS_OK, __func__);
    ut_equals_integer_t(ut, list->limit, 0, __func__);
    ut_equals_int(ut, sll_insert_tail(list, 10), DS_OK, __func__);

    sll_delete(&list);

    return DS_OK;

    error:
    sll_delete(&list);
    return st;
}

// Tests indexof functions
Status sll_test_indexof(UnitTest ut)
{
    SinglyLinkedList list;

    Status st = sll_init(&list);

    if (st != DS_OK)
        return st;

    // 0, 1, 2, 0, 1, 2, 0, 1, 2
    for (int i = 0; i < 9; i++)
    {
        st = sll_insert_tail(list, i % 3);

        if (st != DS_OK)
            goto error;
    }

    int r0, r1, r2;
    int d0, d1, d2;
    integer_t f1, f2;

    st += sll_get(list, &r0, 0);
    st += sll_get(list, &d0, sll_index_first(list, 0));
    st += sll_get(list, &r1, 1);
    st += sll_get(list, &d1, sll_index_first(list, 1));
    st += sll_get(list, &r2, 2);
    st += sll_get(list, &d2, sll_index_first(list, 2));

    if (st != DS_OK)
        goto error;

    ut_equals_int(ut, r0, d0, __func__);
    ut_equals_int(ut, r1, d1, __func__);
    ut_equals_int(ut, r2, d2, __func__);

    st += sll_get(list, &r0, list->length - 3);
    st += sll_get(list, &d0, sll_index_last(list, 0));
    st += sll_get(list, &r1, list->length - 2);
    st += sll_get(list, &d1, sll_index_last(list, 1));
    st += sll_get(list, &r2, list->length - 1);
    st += sll_get(list, &d2, sll_index_last(list, 2));

    if (st != DS_OK)
        goto error;

    ut_equals_int(ut, r0, d0, __func__);
    ut_equals_int(ut, r1, d1, __func__);
    ut_equals_int(ut, r2, d2, __func__);

    f1 = sll_index_first(list, 3);
    f2 = sll_index_last(list, 3);

    ut_equals_integer_t(ut, f1, -1, __func__);
    ut_equals_integer_t(ut, f2, -1, __func__);

    sll_delete(&list);

    error:
    sll_delete(&list);
    return st;
}

// Runs all SinglyLinkedList tests
Status SinglyLinkedListTests(void)
{
    UnitTest ut;

    Status st = ut_init(&ut);

    if (st != DS_OK)
        goto error;

    st += sll_test_middle(ut);
    st += sll_test_limit(ut);
    st += sll_test_indexof(ut);

    if (st != DS_OK)
        goto error;

    ut_report(ut, "SinglyLinkedList");

    ut_delete(&ut);

    return DS_OK;

    error:
    ut_report(ut, "SinglyLinkedList");
    ut_delete(&ut);
    return st;
}