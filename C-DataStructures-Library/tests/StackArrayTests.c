/**
 * @file StackArrayTests.c
 * 
 * @author Leonardo Vencovsky (https://github.com/LeoVen)
 *
 * @date 18/10/2018
 */

#include "StackArray.h"
#include "UnitTest.h"
#include "Utility.h"

// Tests locked capacity
void sta_test_locked(UnitTest ut)
{
    Interface int_interface = interface_new(compare_int32_t, copy_int32_t, display_int32_t,
            free, NULL, NULL);

    if (!int_interface)
        goto error;

    // in case I ever change the default initial capacity
    StackArray stack = sta_create(16, 200, int_interface);

    if (!stack)
        goto error;

    sta_capacity_lock(stack);

    bool success = false;

    void *element;
    for (int i = 0; i < 17; i++)
    {
        element = new_int32_t(i);

        success = sta_push(stack, element);

        if (!success) /* Reached the stack maximum size */
            free(element);
    }

    integer_t size = sta_size(stack);

    ut_equals_bool(ut, success, false, __func__);
    ut_equals_integer_t(ut, size, 16, __func__);

    sta_capacity_unlock(stack);

    element = new_int32_t(1);
    success = sta_push(stack, element);
    if (!success)
    {
        free(element);
        goto error;
    }

    ut_equals_integer_t(ut, sta_size(stack), 17, __func__);

    success = sta_pop(stack, &element);

    if (success)
        free(element);
    else
        goto error;

    int sum = 0;
    while (!sta_empty(stack))
    {
        sta_pop(stack, &element);

        sum += *(int*)element; /* sum from 0 to 15 */

        free(element);
    }

    ut_equals_int(ut, sum, 120, __func__);

    sta_free(stack);

    return;

    error:
    printf("Error at %s\n", __func__);
    sta_free(stack);
    interface_free(int_interface);
}

// Tests capacity multiplication
void sta_test_growth(UnitTest ut)
{
    Interface int_interface = interface_new(compare_int32_t, copy_int32_t, display_int32_t,
            free, NULL, NULL);

    if (!int_interface)
        goto error;

    StackArray stack = sta_create(60, 250, int_interface);

    if (!stack)
        goto error;

    bool success = false;

    void *element;
    for (int i = 0; i < 100; i++)
    {
        element = new_int32_t(i);

        success = sta_push(stack, element);

        if (!success)
        {
            free(element);
            goto error;
        }
    }

    // 60 * (250 / 100)
    ut_equals_integer_t(ut, sta_capacity(stack), 150, __func__);

    sta_free(stack);

    return;

    error:
    printf("Error at %s\n", __func__);
    sta_free(stack);
    interface_free(int_interface);
}

// Runs all StackArray tests
Status StackArrayTests(void)
{
    UnitTest ut;

    Status st = ut_init(&ut);

    if (st != DS_OK)
        goto error;

    sta_test_locked(ut);
    sta_test_growth(ut);

    ut_report(ut, "StackArray");

    ut_delete(&ut);

    return DS_OK;

    error:
    printf("Error at %s\n", __func__);
    ut_report(ut, "StackArray");
    ut_delete(&ut);
    return st;
}