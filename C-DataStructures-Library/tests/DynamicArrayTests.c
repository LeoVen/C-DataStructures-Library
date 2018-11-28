/**
 * @file DynamicArrayTests.c
 * 
 * @author Leonardo Vencovsky (https://github.com/LeoVen)
 *
 * @date 23/10/2018
 */

#include "DynamicArray.h"
#include "UnitTest.h"
#include "Util.h"

// Tests locked capacity
Status dar_test_locked(UnitTest ut)
{
    DynamicArray array;

    // in case I ever change the default initial capacity
    Status st = dar_create(&array, 16, 200,
                           compare_int, copy_int, display_int, free);

    if (st != DS_OK)
        return st;

    dar_capacity_lock(array);

    void *elem;
    for (int i = 0; i < 17; i++)
    {
        elem = new_int(i);

        st = dar_insert_back(array, elem);

        if (st != DS_OK)
            free(elem);
    }

    int *t = new_int(1);
    ut_equals_int(ut, dar_insert_front(array, t), DS_ERR_FULL, __func__);
    ut_equals_int(ut, dar_insert_at(array, t, 0), DS_ERR_FULL, __func__);
    ut_equals_int(ut, dar_insert_back(array, t), DS_ERR_FULL, __func__);

    free(t);

    index_t size = dar_size(array);

    int sum = 0;
    void *R;
    while (!dar_empty(array))
    {
        st = dar_remove_back(array, &R);

        if (st != DS_OK)
            goto error;

        sum += *(int*)R; // sum from 0 to 15
    }

    ut_equals_int(ut, sum, 120, __func__);
    ut_equals_index_t(ut, size, 16, __func__);

    dar_free(&array);

    return DS_OK;

    error:
    dar_free(&array);
    return st;
}

// Tests capacity multiplication
Status dar_test_growth(UnitTest ut)
{
    DynamicArray array;

    Status st = dar_create(&array, 16, 200,
                           compare_int, copy_int, display_int, free);

    if (st != DS_OK)
        return st;

    void *elem;
    for (int i = 0; i < 100; i++)
    {
        elem = new_int(i);

        st = dar_insert_back(array, elem);

        if (st != DS_OK)
        {
            free(elem);

            goto error;
        }
    }

    // 60 * (250 / 100)
    ut_equals_index_t(ut, dar_capacity(array), 150, __func__);

    dar_free(&array);

    return DS_OK;

    error:
    dar_free(&array);
    return st;
}

// Runs all DynamicArray tests
Status DynamicArrayTests(void)
{
    UnitTest ut;

    Status st = ut_init(&ut);

    if (st != DS_OK)
        goto error;

    st += dar_test_locked(ut);
    st += dar_test_growth(ut);

    if (st != DS_OK)
        goto error;

    ut_report(ut, "DynamicArray");

    ut_delete(&ut);

    return DS_OK;

    error:
    ut_report(ut, "DynamicArray");
    ut_delete(&ut);
    return st;
}