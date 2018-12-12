/**
 * @file DoublyLinkedListTests.c
 * 
 * @author Leonardo Vencovsky (https://github.com/LeoVen)
 *
 * @date 18/10/2018
 */

#include "DoublyLinkedList.h"
#include "UnitTest.h"
#include "Utility.h"

// Tests dll_get
Status dll_test_get(UnitTest ut)
{
    DoublyLinkedList list;

    Status st = dll_create(&list, compare_int32_t, copy_int32_t, display_int32_t, free);

    if (st != DS_OK)
        return st;

    void *elem;
    for (int i = 0; i < 10; i++)
    {
        elem = new_int32_t(i);
        st = dll_insert_tail(list, elem);

        if (st != DS_OK)
        {
            free(elem);
            goto error;
        }
    }

    // Tests all edge cases
    void *j, *k, *l, *m;
    st += dll_get(list, &j, 9);
    st += dll_get(list, &k, 5);
    st += dll_get(list, &l, 4);
    st += dll_get(list, &m, 0);

    if (st != DS_OK)
        goto error;

    ut_equals_int(ut, *(int*)j, 9, __func__);
    ut_equals_int(ut, *(int*)k, 5, __func__);
    ut_equals_int(ut, *(int*)l, 4, __func__);
    ut_equals_int(ut, *(int*)m, 0, __func__);

    // Tests on odd list size
    void *t = new_int32_t(11);
    st += dll_insert_tail(list, t);
    st += dll_get(list, &j, 5);

    if (st != DS_OK)
        goto error;

    ut_equals_int(ut, *(int*)j, 5, __func__);

    dll_free(&list);

    return DS_OK;

    error:
    printf("Error at %s\n", __func__);
    dll_free(&list);
    return st;
}

// Tests limit functionality
Status dll_test_limit(UnitTest ut)
{
    DoublyLinkedList list;

    Status st = dll_create(&list, compare_int32_t, copy_int32_t, display_int32_t, free);

    if (st != DS_OK)
        return st;

    st = dll_set_limit(list, 10);

    if (st != DS_OK)
        goto error;

    void *elem;
    for (int i = 0; i < 20; i++)
    {
        elem = new_int32_t(i);

        st = dll_insert_tail(list, elem);

        if (st == DS_ERR_FULL)
        {
            free(elem);
        }
    }

    ut_equals_int(ut, st, DS_ERR_FULL, __func__);
    ut_equals_integer_t(ut, dll_length(list), dll_limit(list), __func__);
    ut_equals_int(ut, dll_set_limit(list, 9), DS_ERR_INVALID_OPERATION, __func__);

    int *t = new_int32_t(1);
    ut_equals_int(ut, dll_insert_head(list, t), DS_ERR_FULL, __func__);
    ut_equals_int(ut, dll_insert_at(list, t, 1), DS_ERR_FULL, __func__);
    ut_equals_int(ut, dll_insert_tail(list, t), DS_ERR_FULL, __func__);

    // Removes the limit
    ut_equals_int(ut, dll_set_limit(list, 0), DS_OK, __func__);
    ut_equals_integer_t(ut, dll_limit(list), 0, __func__);
    ut_equals_int(ut, dll_insert_tail(list, t), DS_OK, __func__);

    dll_free(&list);

    return DS_OK;

    error:
    printf("Error at %s\n", __func__);
    dll_free(&list);
    return st;
}

// Tests indexof functions
Status dll_test_indexof(UnitTest ut)
{
    DoublyLinkedList list;

    Status st = dll_create(&list, compare_int32_t, copy_int32_t, display_int32_t, free);

    if (st != DS_OK)
        return st;

    void *elem;

    // 0, 1, 2, 0, 1, 2, 0, 1, 2
    for (int i = 0; i < 9; i++)
    {
        elem = new_int32_t(i % 3);

        st = dll_insert_tail(list, elem);

        if (st != DS_OK)
        {
            free(elem);
            goto error;
        }
    }

    void *r0, *r1, *r2;
    void *d0, *d1, *d2;
    integer_t f1, f2;

    void *n0 = new_int32_t(0), *n1 = new_int32_t(1), *n2 = new_int32_t(2);

    st += dll_get(list, &r0, 0);
    st += dll_get(list, &d0, dll_index_first(list, n0));
    st += dll_get(list, &r1, 1);
    st += dll_get(list, &d1, dll_index_first(list, n1));
    st += dll_get(list, &r2, 2);
    st += dll_get(list, &d2, dll_index_first(list, n2));

    if (st != DS_OK)
        goto free_all;

    ut_equals_int(ut, *(int*)r0, *(int*)d0, __func__);
    ut_equals_int(ut, *(int*)r1, *(int*)d1, __func__);
    ut_equals_int(ut, *(int*)r2, *(int*)d2, __func__);

    st += dll_get(list, &r0, dll_length(list) - 3);
    st += dll_get(list, &d0, dll_index_last(list, n0));
    st += dll_get(list, &r1, dll_length(list) - 2);
    st += dll_get(list, &d1, dll_index_last(list, n1));
    st += dll_get(list, &r2, dll_length(list) - 1);
    st += dll_get(list, &d2, dll_index_last(list, n2));

    if (st != DS_OK)
        goto free_all;

    ut_equals_int(ut, *(int*)r0, *(int*)d0, __func__);
    ut_equals_int(ut, *(int*)r1, *(int*)d1, __func__);
    ut_equals_int(ut, *(int*)r2, *(int*)d2, __func__);

    void *n3 = new_int32_t(3);

    f1 = dll_index_first(list, n3);
    f2 = dll_index_last(list, n3);

    ut_equals_integer_t(ut, f1, -1, __func__);
    ut_equals_integer_t(ut, f2, -1, __func__);

    free(n0);free(n1);free(n2);free(n3);

    dll_free(&list);

    return DS_OK;

    free_all:
    free(n0);free(n1);free(n2);
    goto error;

    error:
    printf("Error at %s\n", __func__);
    dll_free(&list);
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
    printf("Error at %s\n", __func__);
    ut_report(ut, "DoublyLinkedList");
    ut_delete(&ut);
    return st;
}