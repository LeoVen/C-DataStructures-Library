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
    Interface int_interface = interface_new(compare_int32_t, copy_int32_t,
            display_int32_t, free, NULL, NULL);

    StackArray_t *stack = sta_create(int_interface, 16, 200);

    if (!stack || !int_interface)
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

    integer_t size = sta_count(stack);

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

    ut_equals_integer_t(ut, sta_count(stack), 17, __func__);

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
    interface_free(int_interface);

    return;

    error:
    printf("Error at %s\n", __func__);
    ut_error();
    sta_free(stack);
    interface_free(int_interface);
}

// Tests capacity multiplication
void sta_test_growth(UnitTest ut)
{
    Interface_t int_interface;
    interface_init(&int_interface, compare_int32_t, copy_int32_t,
                   display_int32_t, free, NULL, NULL);

    StackArray_t *stack = sta_create(&int_interface, 60, 250);

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
    ut_error();
    sta_free(stack);
}

void sta_test_foreach(UnitTest ut)
{
    Interface_t int_interface;
    interface_init(&int_interface, compare_int32_t, copy_int32_t,
                   display_int32_t, free, NULL, NULL);

    StackArray_t *stack = sta_create(&int_interface, 32, 200);

    if (!stack)
        goto error;

    int *elem = NULL;
    for (int i = 0; i < 1001; i++)
    {
        elem = new_int32_t(i);

        if (!sta_push(stack, elem))
        {
            free(elem);
        }
    }

    int32_t sum = 0;

    STA_FOR_EACH(stack, {
        sum += *(int*)var;
    })

    ut_equals_int(ut, 500500, sum, __func__);

    sum = 0;

    STA_FOR_EACH(stack, {
        int32_t i = *(int*)var;
        sum += i % 2 == 0 ? i : 0;
    })

    ut_equals_int(ut, 250500, sum, __func__);

    sta_free(stack);

    return;

    error:
    printf("Error at %s\n", __func__);
    ut_error();
    sta_free(stack);
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
    sta_test_foreach(ut);

    ut_report(ut, "StackArray");

    ut_delete(&ut);

    return DS_OK;

    error:
    printf("Error at %s\n", __func__);
    ut_report(ut, "StackArray");
    ut_delete(&ut);
    return st;
}