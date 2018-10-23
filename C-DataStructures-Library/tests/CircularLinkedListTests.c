/**
 * @file CircularLinkedListTests.c
 * 
 * @author Leonardo Vencovsky (https://github.com/LeoVen)
 *
 * @date 22/10/2018
 */

#include "CircularLinkedList.h"
#include "UnitTest.h"

// Tests limit functionality
Status cll_test_limit(UnitTest ut)
{
    CircularLinkedList list;

    Status st = cll_init(&list);

    if (st != DS_OK)
        return st;

    st = cll_limit(list, 10);

    if (st != DS_OK)
        goto error;

    for (int i = 0; i < 20; i++)
    {
        st = cll_insert_before(list, i);
    }

    ut_equals_int(ut, st, DS_ERR_FULL, __func__);
    ut_equals_size_t(ut, list->length, list->limit, __func__);
    ut_equals_int(ut, cll_limit(list, 9), DS_ERR_INVALID_OPERATION, __func__);

    ut_equals_int(ut, cll_insert_after(list, 1), DS_ERR_FULL, __func__);
    ut_equals_int(ut, cll_insert_before(list, 1), DS_ERR_FULL, __func__);

    // Removes the limit
    ut_equals_int(ut, cll_limit(list, 0), DS_OK, __func__);
    ut_equals_size_t(ut, list->limit, 0, __func__);
    ut_equals_int(ut, cll_insert_before(list, 10), DS_OK, __func__);

    cll_delete(&list);

    return DS_OK;

    error:
    cll_delete(&list);
    return st;
}

// Runs all CircularLinkedList tests
Status CircularLinkedListTests(void)
{
    UnitTest ut;

    Status st = ut_init(&ut);

    if (st != DS_OK)
        goto error;

    st += cll_test_limit(ut);

    if (st != DS_OK)
        goto error;

    ut_report(ut, "CircularLinkedList");

    ut_delete(&ut);

    return DS_OK;

    error:
    ut_report(ut, "CircularLinkedList");
    ut_delete(&ut);
    return st;
}