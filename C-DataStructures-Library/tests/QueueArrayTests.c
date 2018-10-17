/**
 * @file QueueArrayTests.c
 * 
 * @author Leonardo Vencovsky (https://github.com/LeoVen)
 *
 * @date 15/10/2018
 */

#include "QueueArray.h"
#include "UnitTest.h"

// Tests qua_grow() in linear insertions
// Tests while (!empty) loop
// Tests front == rear on empty queue
// Sum of dequeued contents should equal to the known result (500500)
Status qua_test_linear_insertion(UnitTest ut)
{
    QueueArray queue;

    Status st = qua_init(&queue);

    if (st != DS_OK)
        return st;

    for (int i = 1; i <= 1000; i++)
    {
        st = qua_enqueue(queue, i);

        if (st != DS_OK)
            goto error;
    }

    // size after all insertions
    size_t size = queue->size;

    int j, sum = 0;
    while (!qua_empty(queue))
    {
        st = qua_dequeue(queue, &j);

        if (st != DS_OK)
            goto error;

        sum += j;
    }

    ut_equals_int(ut, sum, 500500, "qua_tests_linear_insertion");
    ut_equals_size_t(ut, queue->front, queue->rear, "qua_tests_linear_insertion");
    ut_equals_size_t(ut, size, queue->rear, "qua_tests_linear_insertion");

    qua_delete(&queue);

    return DS_OK;

    error:
    qua_delete(&queue);
    return st;
}

// Tests locked capacity
Status qua_test_locked(UnitTest ut)
{
    QueueArray queue;

    // in case I ever change the default initial capacity
    Status st = qua_create(&queue, 16, 200);

    if (st != DS_OK)
        return st;

    qua_cap_lock(queue);

    for (int i = 0; i < 17; i++)
    {
        st = qua_enqueue(queue, i);
    }

    Status saved_st = st;
    size_t size = queue->size;

    int j, sum = 0;
    while (!qua_empty(queue))
    {
        st = qua_dequeue(queue, &j);

        if (st != DS_OK)
            goto error;

        sum += j; // sum from 0 to 15
    }

    ut_equals_int(ut, sum, 120, "qua_test_locked");
    ut_equals_int(ut, saved_st, DS_ERR_FULL, "qua_test_locked");
    ut_equals_size_t(ut, size, 16, "qua_test_locked");

    qua_delete(&queue);

    return DS_OK;

    error:
    qua_delete(&queue);
    return st;
}

// Intensive test. Checks if all elements are preserved.
Status qua_test_intensive(UnitTest ut)
{
    QueueArray queue;

    Status st = qua_init(&queue);

    if (st != DS_OK)
        return st;

    int j, sum = 0, numbers = 0;

    // The total sum must be from 1 to 10000 that result in 50005000
    for (int i = 0; numbers < 10000; i = rand())
    {
        if (i % 2 == 0 || qua_empty(queue))
            st = qua_enqueue(queue, ++numbers);
        else
        {
            st = qua_dequeue(queue, &j);

            sum += j;
        }

        if (st != DS_OK)
            goto error;
    }

    // Emptying the queue
    while (!qua_empty(queue))
    {
        st = qua_dequeue(queue, &j);

        if (st != DS_OK)
            goto error;

        sum += j; // sum from 0 to 15
    }

    ut_equals_int(ut, sum, 50005000, "qua_test_intensive");
    ut_equals_size_t(ut, queue->front, queue->rear, "qua_test_intensive");

    qua_delete(&queue);

    return DS_OK;

    error:
    qua_delete(&queue);
    return st;
}

// Runs all QueueArray tests
Status QueueArrayTests(void)
{
    UnitTest ut;

    Status st = ut_init(&ut);

    if (st != DS_OK)
        goto error;

    st += qua_test_linear_insertion(ut);
    st += qua_test_locked(ut);
    st += qua_test_intensive(ut);

    if (st != DS_OK)
        goto error;

    ut_report(ut, "QueueArray");

    ut_delete(&ut);

    return DS_OK;

    error:
    ut_report(ut, "QueueArray");
    ut_delete(&ut);
    return st;
}