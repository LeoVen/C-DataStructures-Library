/**
 * @file DynamicArrayTests.c
 * 
 * @author Leonardo Vencovsky (https://github.com/LeoVen)
 *
 * @date 23/10/2018
 */

#include "DynamicArray.h"
#include "UnitTest.h"

// Tests locked capacity
Status dar_test_locked(UnitTest ut)
{
    DynamicArray array;

    // in case I ever change the default initial capacity
    Status st = dar_create(&array, 16, 200);

    if (st != DS_OK)
        return st;

    dar_cap_lock(array);

    for (int i = 0; i < 17; i++)
    {
        st = dar_insert_back(array, i);
    }

    ut_equals_int(ut, dar_insert_front(array, 1), DS_ERR_FULL, __func__);
    ut_equals_int(ut, dar_insert_at(array, 1, 0), DS_ERR_FULL, __func__);
    ut_equals_int(ut, dar_insert_back(array, 1), DS_ERR_FULL, __func__);

    index_t size = array->size;

    int j, sum = 0;
    while (!dar_empty(array))
    {
        st = dar_remove_back(array, &j);

        if (st != DS_OK)
            goto error;

        sum += j; // sum from 0 to 15
    }

    ut_equals_int(ut, sum, 120, __func__);
    ut_equals_index_t(ut, size, 16, __func__);

    dar_delete(&array);

    return DS_OK;

    error:
    dar_delete(&array);
    return st;
}


// Tests capacity multiplication
Status dar_test_growth(UnitTest ut)
{
    DynamicArray array;

    Status st = dar_create(&array, 60, 250);

    if (st != DS_OK)
        return st;;

    for (int i = 0; i < 100; i++)
    {
        st = dar_insert_back(array, i);

        if (st != DS_OK)
            goto error;
    }

    // 60 * (250 / 100)
    ut_equals_index_t(ut, array->capacity, 150, __func__);

    dar_delete(&array);

    return DS_OK;

    error:
    dar_delete(&array);
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