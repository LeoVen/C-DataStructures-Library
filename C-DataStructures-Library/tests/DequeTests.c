/**
 * @file DequeTests.c
 * 
 * @author Leonardo Vencovsky (https://github.com/LeoVen)
 *
 * @date 20/10/2018
 */

#include "Deque.h"
#include "UnitTest.h"

// Tests limit functionality
Status deq_test_limit(UnitTest ut)
{
    Deque queue;

    Status st = deq_init(&queue);

    if (st != DS_OK)
        return st;

    st = deq_limit(queue, 10);

    if (st != DS_OK)
        goto error;

    for (int i = 0; i < 20; i++)
    {
        st = deq_enqueue_rear(queue, i);
    }

    ut_equals_int(ut, st, DS_ERR_FULL, __func__);
    ut_equals_size_t(ut, queue->length, queue->limit, __func__);
    ut_equals_int(ut, deq_limit(queue, 9), DS_ERR_INVALID_OPERATION, __func__);

    ut_equals_int(ut, deq_enqueue_front(queue, 1), DS_ERR_FULL, __func__);
    ut_equals_int(ut, deq_enqueue_rear(queue, 1), DS_ERR_FULL, __func__);

    // Removes the limit
    ut_equals_int(ut, deq_limit(queue, 0), DS_OK, __func__);
    ut_equals_size_t(ut, queue->limit, 0, __func__);
    ut_equals_int(ut, deq_enqueue_front(queue, 10), DS_OK, __func__);

    deq_delete(&queue);

    return DS_OK;

    error:
    deq_delete(&queue);
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
    ut_report(ut, "Deque");
    ut_delete(&ut);
    return st;
}
