/**
 * @file SortedListTests.c
 * 
 * @author Leonardo Vencovsky (https://github.com/LeoVen)
 *
 * @date 18/11/2018
 */
 
#include "SortedList.h"
#include "UnitTest.h"
#include "Util.h"

// Tests all the functions that shouldn't allow lists without the default
// functions set.
Status sli_test_incomplete(UnitTest ut)
{
    SortedList list, slist;

    Status st = sli_init(&list);

    if (st != DS_OK)
        return st;

    void *R, *E = new_int(1);
    void *EL[2] = {new_int(1), new_int(2)};
    void **RL;

    index_t t;

    ut_equals_int(ut, sli_insert(list, E), DS_ERR_INCOMPLETE_TYPE, __func__);
    ut_equals_int(ut, sli_insert_all(list, EL, 2), DS_ERR_INCOMPLETE_TYPE, __func__);
    ut_equals_int(ut, sli_copy(list, &slist), DS_ERR_INCOMPLETE_TYPE, __func__);
    ut_equals_int(ut, sli_display(list), DS_ERR_INCOMPLETE_TYPE, __func__);
    ut_equals_int(ut, sli_display_array(list), DS_ERR_INCOMPLETE_TYPE, __func__);
    ut_equals_int(ut, sli_display_raw(list), DS_ERR_INCOMPLETE_TYPE, __func__);
    ut_equals_index_t(ut, sli_index_first(list, E), -2, __func__);
    ut_equals_index_t(ut, sli_index_last(list, E), -2, __func__);

    // This function (sli_to_array) require that we have at least one element
    // in the list:
    sli_set_func_compare(list, compare_int);
    sli_insert(list, new_int(1)); // Adding it

    // Little hack...
    sli_set_func_compare(list, NULL); // Like we have never set a comparator...

    // Testing
    ut_equals_int(ut, sli_to_array(list, &RL, &t), DS_ERR_INCOMPLETE_TYPE, __func__);

    ut_equals_int(ut, sli_order(list), DESCENDING, __func__);

    free(E);
    free(EL[0]);
    free(EL[1]);

    sli_free(&list);

    return DS_OK;
}

// Tests limit functionality
Status sli_test_limit(UnitTest ut)
{
    SortedList list;

    Status st = sli_create(&list, ASCENDING, compare_int, copy_int,
            display_int, free);

    if (st != DS_OK)
        return st;

    st = sli_set_limit(list, 10);

    if (st != DS_OK)
        goto error;

    void *elem;

    for (int i = 0; i < 20; i++)
    {
        elem = new_int(i);

        st = sli_insert(list, elem);

        if (st !=  DS_OK)
            free(elem);
    }

    ut_equals_int(ut, st, DS_ERR_FULL, __func__);
    ut_equals_index_t(ut, sli_length(list), sli_limit(list), __func__);
    ut_equals_int(ut, sli_set_limit(list, 9), DS_ERR_INVALID_OPERATION, __func__);

    int *t = new_int(-1);

    ut_equals_int(ut, sli_insert(list, t), DS_ERR_FULL, __func__);

    // Removes the limit
    ut_equals_int(ut, sli_set_limit(list, 0), DS_OK, __func__);
    ut_equals_index_t(ut, sli_limit(list), 0, __func__);
    ut_equals_int(ut, sli_insert(list, t), DS_OK, __func__);

    sli_free(&list);

    return DS_OK;

    error:
    sli_free(&list);
    return st;
}

// Tests indexof functions
Status sli_test_indexof(UnitTest ut)
{
    SortedList list;

    Status st = sli_create(&list, ASCENDING, compare_int, copy_int, display_int, free);

    if (st != DS_OK)
        return st;

    void *elem;

    // 0, 0, 0, 1, 1, 1, 2, 2, 2
    for (int i = 0; i < 9; i++)
    {
        elem = new_int(i % 3);

        st = sli_insert(list, elem);

        if (st != DS_OK)
        {
            free(elem);
            goto error;
        }
    }

    void *r0, *r1, *r2;
    void *d0, *d1, *d2;
    index_t f1, f2;

    void *n0 = new_int(0), *n1 = new_int(1), *n2 = new_int(2);

    st += sli_get(list, &r0, 0);
    st += sli_get(list, &d0, sli_index_first(list, n0));
    st += sli_get(list, &r1, 3);
    st += sli_get(list, &d1, sli_index_first(list, n1));
    st += sli_get(list, &r2, 6);
    st += sli_get(list, &d2, sli_index_first(list, n2));

    if (st != DS_OK)
        goto free_all;

    ut_equals_int(ut, *(int*)r0, *(int*)d0, __func__);
    ut_equals_int(ut, *(int*)r1, *(int*)d1, __func__);
    ut_equals_int(ut, *(int*)r2, *(int*)d2, __func__);

    free(r0);free(r1);free(r2);
    free(d0);free(d1);free(d2);

    st += sli_get(list, &r0, 2);
    st += sli_get(list, &d0, sli_index_last(list, n0));
    st += sli_get(list, &r1, 5);
    st += sli_get(list, &d1, sli_index_last(list, n1));
    st += sli_get(list, &r2, 8);
    st += sli_get(list, &d2, sli_index_last(list, n2));

    if (st != DS_OK)
        goto free_all;

    ut_equals_int(ut, *(int*)r0, *(int*)d0, __func__);
    ut_equals_int(ut, *(int*)r1, *(int*)d1, __func__);
    ut_equals_int(ut, *(int*)r2, *(int*)d2, __func__);

    void *n3 = new_int(3);

    f1 = sli_index_first(list, n3);
    f2 = sli_index_last(list, n3);

    ut_equals_index_t(ut, f1, -1, __func__);
    ut_equals_index_t(ut, f2, -1, __func__);

    free(r0);free(r1);free(r2);
    free(d0);free(d1);free(d2);
    free(n0);free(n1);free(n2);free(n3);

    sli_free(&list);

    free_all:
    free(r0);free(r1);free(r2);
    free(d0);free(d1);free(d2);
    free(n0);free(n1);free(n2);
    goto error;

    error:
    sli_free(&list);
    return st;
}

// Runs all SortedList tests
Status SortedListTests(void)
{
    UnitTest ut;

    Status st = ut_init(&ut);

    if (st != DS_OK)
        goto error;

    st += sli_test_incomplete(ut);
    st += sli_test_limit(ut);
    st += sli_test_indexof(ut);

    if (st != DS_OK)
        goto error;

    ut_report(ut, "SortedList");

    ut_delete(&ut);

    return DS_OK;

    error:
    ut_report(ut, "SortedList");
    ut_delete(&ut);
    status_print(st);
    return st;
}