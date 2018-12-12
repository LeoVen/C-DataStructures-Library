/**
 * @file CircularLinkedListTests.c
 * 
 * @author Leonardo Vencovsky (https://github.com/LeoVen)
 *
 * @date 22/10/2018
 */

#include "CircularLinkedList.h"
#include "UnitTest.h"
#include "Utility.h"

// Tests limit functionality
Status cll_test_limit(UnitTest ut)
{
    CircularLinkedList list;

    Status st = cll_create(&list, compare_int32_t, copy_int32_t, display_int32_t, free);

    if (st != DS_OK)
        return st;

    st = cll_set_limit(list, 10);

    if (st != DS_OK)
        goto error;

    void *elem;
    for (int i = 0; i < 20; i++)
    {
        elem = new_int32_t(i);

        st = cll_insert_before(list, elem);

        if (st == DS_ERR_FULL)
        {
            free(elem);
        }
    }

    ut_equals_int(ut, st, DS_ERR_FULL, __func__);
    ut_equals_integer_t(ut, cll_length(list), cll_limit(list), __func__);
    ut_equals_int(ut, cll_set_limit(list, 9), DS_ERR_INVALID_OPERATION, __func__);

    int *t = new_int32_t(1);
    ut_equals_int(ut, cll_insert_after(list, t), DS_ERR_FULL, __func__);
    ut_equals_int(ut, cll_insert_before(list, t), DS_ERR_FULL, __func__);

    // Removes the limit
    ut_equals_int(ut, cll_set_limit(list, 0), DS_OK, __func__);
    ut_equals_integer_t(ut, cll_limit(list), 0, __func__);
    ut_equals_int(ut, cll_insert_before(list, t), DS_OK, __func__);

    cll_free(&list);

    return DS_OK;

    error:
    printf("Error at %s\n", __func__);
    cll_free(&list);
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
    printf("Error at %s\n", __func__);
    ut_report(ut, "CircularLinkedList");
    ut_delete(&ut);
    return st;
}