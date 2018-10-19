/**
 * @file StackArrayTests.c
 * 
 * @author Leonardo Vencovsky (https://github.com/LeoVen)
 *
 * @date 18/10/2018
 */

#include "StackArray.h"
#include "UnitTest.h"

// Tests locked capacity
Status sta_test_locked(UnitTest ut)
{
    StackArray stack;

    // in case I ever change the default initial capacity
    Status st = sta_create(&stack, 16, 200);

    if (st != DS_OK)
        return st;

    sta_cap_lock(stack);

    for (int i = 0; i < 17; i++)
    {
        st = sta_push(stack, i);
    }

    Status saved_st = st;
    size_t size = stack->height;

    int j, sum = 0;
    while (!sta_empty(stack))
    {
        st = sta_pop(stack, &j);

        if (st != DS_OK)
            goto error;

        sum += j; // sum from 0 to 15
    }

    ut_equals_int(ut, sum, 120, __func__);
    ut_equals_int(ut, saved_st, DS_ERR_FULL, __func__);
    ut_equals_size_t(ut, size, 16, __func__);

    sta_delete(&stack);

    return DS_OK;

    error:
    sta_delete(&stack);
    return st;
}

// Tests capacity multiplication
Status sta_test_growth(UnitTest ut)
{
    StackArray stack;

    Status st = sta_create(&stack, 60, 250);

    if (st != DS_OK)
        return st;;

    for (int i = 0; i < 100; i++)
    {
        st = sta_insert(stack, i);

        if (st != DS_OK)
            goto error;
    }

    // 60 * (250 / 100)
    ut_equals_size_t(ut, stack->capacity, 150, __func__);

    sta_delete(&stack);

    return DS_OK;

    error:
    sta_delete(&stack);
    return st;
}

// Runs all StackArray tests
Status StackArrayTests(void)
{
    UnitTest ut;

    Status st = ut_init(&ut);

    if (st != DS_OK)
        goto error;

    st += sta_test_locked(ut);
    st += sta_test_growth(ut);

    if (st != DS_OK)
        goto error;

    ut_report(ut, "StackArray");

    ut_delete(&ut);

    return DS_OK;

    error:
    ut_report(ut, "StackArray");
    ut_delete(&ut);
    return st;
}