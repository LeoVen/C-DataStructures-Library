/**
 * @file StackTests.c
 * 
 * @author Leonardo Vencovsky (https://github.com/LeoVen)
 *
 * @date 23/10/2018
 */


#include "Stack.h"
#include "UnitTest.h"
#include "Util.h"

// Tests locked capacity
Status stk_test_limit(UnitTest ut)
{
    Stack stack;

    Status st = stk_create(&stack, compare_int, copy_int, display_int, free);

    if (st != DS_OK)
        return st;

    st = stk_set_limit(stack, 10);

    if (st != DS_OK)
        goto error;

    int *elem;
    for (int i = 0; i < 20; i++)
    {
        elem = new_int(i);

        st = stk_insert(stack, elem);

        if (st == DS_ERR_FULL)
        {
            free(elem);
        }
    }

    ut_equals_int(ut, st, DS_ERR_FULL, __func__);
    ut_equals_index_t(ut, stk_height(stack), stk_limit(stack), __func__);
    ut_equals_int(ut, stk_set_limit(stack, 9), DS_ERR_INVALID_OPERATION, __func__);

    // This gets freed later
    int *t = new_int(1);

    ut_equals_int(ut, stk_push(stack, t), DS_ERR_FULL, __func__);
    ut_equals_int(ut, stk_insert(stack, t), DS_ERR_FULL, __func__);

    // Removes the limit
    ut_equals_int(ut, stk_set_limit(stack, 0), DS_OK, __func__);
    ut_equals_index_t(ut, stk_limit(stack), 0, __func__);
    ut_equals_int(ut, stk_insert(stack, t), DS_OK, __func__);

    stk_free(&stack);

    return DS_OK;

    error:
    stk_free(&stack);
    return st;
}

// Runs all Stack tests
Status StackTests(void)
{
    UnitTest ut;

    Status st = ut_init(&ut);

    if (st != DS_OK)
        goto error;

    st += stk_test_limit(ut);

    if (st != DS_OK)
        goto error;

    ut_report(ut, "Stack");

    ut_delete(&ut);

    return DS_OK;

    error:
    ut_report(ut, "Stack");
    ut_delete(&ut);
    return st;
}