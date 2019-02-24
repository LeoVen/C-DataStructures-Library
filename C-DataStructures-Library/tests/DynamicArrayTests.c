/**
 * @file DynamicArrayTests.c
 *
 * @author Leonardo Vencovsky (https://github.com/LeoVen)
 *
 * @date 23/10/2018
 */

#include "DynamicArray.h"
#include "UnitTest.h"
#include "Utility.h"

// Tests locked capacity
void dar_test_locked(UnitTest ut)
{
    Interface_t *interface = interface_new(compare_int32_t, copy_int32_t,
            display_int32_t, free, NULL, NULL);

    DynamicArray_t *array = dar_create(interface, 16, 200);

    if (!array)
        return;

    dar_capacity_lock(array);

    void *elem;
    for (int i = 0; i < 17; i++)
    {
        elem = new_int32_t(i);

        if (!dar_insert_back(array, elem))
            free(elem);
    }

    int *t = new_int32_t(1);
    ut_equals_bool(ut, dar_insert_front(array, t), false, __func__);
    ut_equals_bool(ut, dar_insert_at(array, t, 0), false, __func__);
    ut_equals_bool(ut, dar_insert_back(array, t), false, __func__);
    free(t);

    integer_t size = dar_size(array);

    int sum = 0;
    void *R;
    while (!dar_empty(array))
    {
        if (!dar_remove_back(array, &R))
            goto error;

        sum += *(int*)R; // sum from 0 to 15
        free(R);
    }

    ut_equals_int(ut, sum, 120, __func__);
    ut_equals_integer_t(ut, size, 16, __func__);

    dar_free(array);
    interface_free(interface);

    return;

    error:
    printf("Error at %s\n", __func__);
    ut_error();
    dar_free(array);
    interface_free(interface);
}

// Tests capacity multiplication
void dar_test_growth(UnitTest ut)
{
    Interface_t *interface = interface_new(compare_int32_t, copy_int32_t,
                                           display_int32_t, free, NULL, NULL);

    DynamicArray_t *array = dar_create(interface, 60, 250);

    if (!array)
        return;

    void *elem;
    for (int i = 0; i < 100; i++)
    {
        elem = new_int32_t(i);

        if (!dar_insert_back(array, elem))
        {
            free(elem);

            goto error;
        }
    }

    // 60 * (250 / 100)
    ut_equals_integer_t(ut, dar_capacity(array), 150, __func__);

    dar_free(array);
    interface_free(interface);

    return;

    error:
    printf("Error at %s\n", __func__);
    ut_error();
    dar_free(array);
    interface_free(interface);
}

// Runs all DynamicArray tests
Status DynamicArrayTests(void)
{
    UnitTest ut;

    Status st = ut_init(&ut);

    if (st != DS_OK)
        goto error;

    dar_test_locked(ut);
    dar_test_growth(ut);

    ut_report(ut, "DynamicArray");

    ut_delete(&ut);

    return DS_OK;

    error:
    printf("Error at %s\n", __func__);
    ut_report(ut, "DynamicArray");
    ut_delete(&ut);
    return st;
}