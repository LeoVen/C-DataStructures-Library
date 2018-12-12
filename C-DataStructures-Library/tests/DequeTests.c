/**
 * @file DequeTests.c
 * 
 * @author Leonardo Vencovsky (https://github.com/LeoVen)
 *
 * @date 20/10/2018
 */

#include "Deque.h"
#include "UnitTest.h"
#include "Utility.h"

// Tests limit functionality
Status deq_test_limit(UnitTest ut)
{
    Deque deque;

    Status st = deq_create(&deque, compare_int32_t, copy_int32_t, display_int32_t, free);

    if (st != DS_OK)
        return st;

    st = deq_set_limit(deque, 10);

    if (st != DS_OK)
        goto error;

    void *elem;
    for (int i = 0; i < 20; i++)
    {
        elem = new_int32_t(i);

        st = deq_enqueue_rear(deque, elem);

        if (st == DS_ERR_FULL)
        {
            free(elem);
        }
    }

    ut_equals_int(ut, st, DS_ERR_FULL, __func__);
    ut_equals_integer_t(ut, deq_length(deque), deq_limit(deque), __func__);
    ut_equals_int(ut, deq_set_limit(deque, 9), DS_ERR_INVALID_OPERATION, __func__);

    int *t = new_int32_t(1);
    ut_equals_int(ut, deq_enqueue_front(deque, t), DS_ERR_FULL, __func__);
    ut_equals_int(ut, deq_enqueue_rear(deque, t), DS_ERR_FULL, __func__);

    // Removes the limit
    ut_equals_int(ut, deq_set_limit(deque, 0), DS_OK, __func__);
    ut_equals_integer_t(ut, deq_limit(deque), 0, __func__);
    ut_equals_int(ut, deq_enqueue_front(deque, t), DS_OK, __func__);

    deq_free(&deque);

    return DS_OK;

    error:
    printf("Error at %s\n", __func__);
    deq_free(&deque);
    return st;
}

// Runs all Deque tests
Status DequeTests(void)
{
    UnitTest ut;

    Status st = ut_init(&ut);

    if (st != DS_OK)
        goto error;

    st += deq_test_limit(ut);

    if (st != DS_OK)
        goto error;

    ut_report(ut, "Deque");

    ut_delete(&ut);

    return DS_OK;

    error:
    printf("Error at %s\n", __func__);
    ut_report(ut, "Deque");
    ut_delete(&ut);
    return st;
}
