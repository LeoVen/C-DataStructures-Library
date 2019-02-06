/**
 * @file Array.c
 *
 * @author Leonardo Vencovsky (https://github.com/LeoVen)
 *
 * @date 06/02/2019
 */

#include "Array.h"
#include "UnitTest.h"
#include "Utility.h"

void arr_test_IO1(UnitTest ut)
{
    Interface_t *interface = interface_new(compare_int64_t, copy_int64_t,
            display_int64_t, free, hash_int64_t, NULL);

    Array_t *array = arr_new(interface, 200);

    if (!array || !interface)
        goto error;

    void *elem = NULL;
    for (int i = 0; i < 100; i++)
    {
        elem = new_int64_t(i);
        if (arr_set_first(array, elem) < 0)
        {
            free(elem);
            goto error;
        }
    }

    void *R0;
    if (arr_get(array, &R0, 30) < 0)
        goto error;

    ut_equals_integer_t(ut, 200, arr_length(array), __func__);
    ut_equals_integer_t(ut, 100, arr_count(array), __func__);
    ut_equals_integer_t(ut, 30, *(integer_t*)R0, __func__);

    void *result;
    integer_t sum = 0;
    while (!arr_empty(array))
    {
        if (arr_remove_first(array, &result) < 0)
            goto error;

        sum += *(integer_t*)result;

        free(result);
    }

    ut_equals_integer_t(ut, 0, arr_count(array), __func__);
    ut_equals_integer_t(ut, 4950, sum, __func__);

    if (arr_set_first(array, NULL) < 0)
        goto error;
    if (arr_set(array, NULL, 0) < 0)
        goto error;
    if (arr_set_last(array, NULL) < 0)
        goto error;

    ut_equals_integer_t(ut, 0, arr_count(array), __func__);

    arr_free(array);
    interface_free(interface);

    return;

    error:
    printf("Error at %s\n", __func__);
    arr_free(array);
    interface_free(interface);
    ut_error();
}

void arr_test_IO2(UnitTest ut)
{
    Interface_t *interface = interface_new(compare_int64_t, copy_int64_t,
                                           display_int64_t, free, hash_int64_t, NULL);

    Array_t *array = arr_new(interface, 200);

    if (!array || !interface)
        goto error;

    void *elem = NULL;
    for (int i = 0, j = 75; i < 100; i++)
    {
        elem = new_int64_t(i);
        if (i % 2 == 0)
        {
            if (arr_set_first(array, elem) < 0)
            {
                free(elem);
                goto error;
            }
        }
        else if (i % 3 == 0)
        {
            if (arr_set_last(array, elem) < 0)
            {
                free(elem);
                goto error;
            }
        }
        else
        {
            if (arr_set(array, elem, j++) < 0)
            {
                free(elem);
                goto error;
            }
        }
    }

    void *R0, *R1, *R2;

    if (arr_remove_first(array, &R0) < 0)
        goto error;
    if (arr_remove(array, &R1, 75) < 0)
        goto error;
    if (arr_remove_last(array, &R2) < 0)
        goto error;

    ut_equals_integer_t(ut, 97, arr_count(array), __func__);
    ut_equals_integer_t(ut, 0, *(integer_t*)R0, __func__);
    ut_equals_integer_t(ut, 1, *(integer_t*)R1, __func__);
    ut_equals_integer_t(ut, 3, *(integer_t*)R2, __func__);

    free(R0);
    free(R1);
    free(R2);

    if (arr_get_first(array, &R0) < 0)
        goto error;
    if (arr_get(array, &R1, 76) < 0)
        goto error;
    if (arr_get_last(array, &R2) < 0)
        goto error;

    ut_equals_integer_t(ut, 2, *(integer_t*)R0, __func__);
    ut_equals_integer_t(ut, 5, *(integer_t*)R1, __func__);
    ut_equals_integer_t(ut, 9, *(integer_t*)R2, __func__);

    integer_t result = arr_set(array, NULL, 10);

    ut_equals_integer_t(ut, -3, result, __func__);

    arr_free(array);
    interface_free(interface);

    return;

    error:
    printf("Error at %s\n", __func__);
    arr_free(array);
    interface_free(interface);
    ut_error();
}

// Runs all Array tests
Status ArrayTests(void)
{
    UnitTest ut;

    Status st = ut_init(&ut);

    if (st != DS_OK)
        goto error;

    arr_test_IO1(ut);
    arr_test_IO2(ut);

    ut_report(ut, "Array");

    ut_delete(&ut);

    return DS_OK;

    error:
    printf("Error at %s\n", __func__);
    ut_report(ut, "AssociativeList");
    ut_delete(&ut);
    return st;
}