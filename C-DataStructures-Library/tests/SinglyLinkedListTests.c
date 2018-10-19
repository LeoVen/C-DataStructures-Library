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

    st = sll_limit_add(list, 10);

    if (st != DS_OK)
        goto error;

    for (int i = 0; i < 20; i++)
    {
        st = sll_insert_tail(list, i);
    }

    ut_equals_int(ut, st, DS_ERR_FULL, __func__);
    ut_equals_size_t(ut, list->length, list->limit, __func__);
    ut_equals_int(ut, sll_limit_add(list, 9), DS_ERR_INVALID_OPERATION, __func__);

    ut_equals_int(ut, sll_insert_head(list, 1), DS_ERR_FULL, __func__);
    ut_equals_int(ut, sll_insert_at(list, 1, 1), DS_ERR_FULL, __func__);
    ut_equals_int(ut, sll_insert_tail(list, 1), DS_ERR_FULL, __func__);

    sll_limit_remove(list);

    ut_equals_size_t(ut, list->limit, 0, __func__);
    ut_equals_int(ut, sll_insert_tail(list, 10), DS_OK, __func__);

    sll_delete(&list);

    return DS_OK;

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