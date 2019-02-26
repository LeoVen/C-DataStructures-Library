/**
 * @file StackTests.c
 * 
 * @author Leonardo Vencovsky (https://github.com/LeoVen)
 *
 * @date 23/10/2018
 */


#include "Stack.h"
#include "UnitTest.h"
#include "Utility.h"

// Tests locked capacity
void stk_test_limit(UnitTest ut)
{
    Interface int_interface = interface_new(compare_int32_t, copy_int32_t,
            display_int32_t, free, NULL, NULL);

    Stack_t *stack = stk_new(int_interface);

    if (!stack || !int_interface)
        goto error;

    if (!stk_set_limit(stack, 10))
        goto error;

    int *elem = NULL;
    for (int i = 0; i < 20; i++)
    {
        elem = new_int32_t(i);

        if (!stk_push(stack, elem))
        {
            free(elem);
        }
    }

    ut_equals_bool(ut, stk_push(stack, elem), false, __func__);
    ut_equals_integer_t(ut, stk_count(stack), stk_limit(stack), __func__);
    ut_equals_bool(ut, stk_set_limit(stack, 9), false, __func__);

    // This gets freed later
    int *t = new_int32_t(1);

    ut_equals_int(ut, stk_push(stack, t), false, __func__);
    ut_equals_int(ut, stk_push(stack, t), false, __func__);

    // Removes the limit
    ut_equals_int(ut, stk_set_limit(stack, 0), true, __func__);
    ut_equals_integer_t(ut, stk_limit(stack), 0, __func__);
    ut_equals_int(ut, stk_push(stack, t), true, __func__);

    stk_free(stack);
    interface_free(int_interface);

    return;

    error:
    printf("Error at %s\n", __func__);
    ut_error();
    stk_free(stack);
    interface_free(int_interface);
}

// Runs all Stack tests
Status StackTests(void)
{
    UnitTest ut;

    Status st = ut_init(&ut);

    if (st != DS_OK)
        goto error;

    stk_test_limit(ut);

    ut_report(ut, "Stack");

    ut_delete(&ut);

    return DS_OK;

    error:
    printf("Error at %s\n", __func__);
    ut_report(ut, "Stack");
    ut_delete(&ut);
    return st;
}