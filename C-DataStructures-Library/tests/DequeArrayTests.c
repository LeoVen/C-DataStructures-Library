/**
 * @file DequeArrayTests.c
 * 
 * @author Leonardo Vencovsky (https://github.com/LeoVen)
 *
 * @date 29/10/2018
 */

#include "DequeArray.h"
#include "UnitTest.h"
#include "Utility.h"

// Tests dqa_grow() in linear insertions
// Tests while (!empty) loop
// Tests front == rear on empty queue
// Sum of dequeued contents should equal to the known result (500500)
Status dqa_test_linear_insertion(UnitTest ut)
{
    DequeArray queue;

    Status st = dqa_init(&queue);

    if (st != DS_OK)
        return st;

    for (int i = 1; i <= 1000; i++)
    {
        st = dqa_enqueue_rear(queue, i);

        if (st != DS_OK)
            goto error;
    }

    integer_t size1 = queue->size;

    int j, sum1 = 0;
    while (!dqa_empty(queue))
    {
        st = dqa_dequeue_rear(queue, &j);

        if (st != DS_OK)
            goto error;

        sum1 += j;
    }

    ut_equals_int(ut, sum1, 500500, __func__);
    ut_equals_integer_t(ut, queue->front, queue->rear, __func__);

    st = dqa_erase(&queue);

    if (st != DS_OK)
        goto error;

    for (int i = 1; i <= 1000; i++)
    {
        st = dqa_enqueue_front(queue, i);

        if (st != DS_OK)
            goto error;
    }

    integer_t size2 = queue->size;

    int sum2 = 0;
    while (!dqa_empty(queue))
    {
        st = dqa_dequeue_front(queue, &j);

        if (st != DS_OK)
            goto error;

        sum2 += j;
    }

    ut_equals_int(ut, sum2, 500500, __func__);
    ut_equals_integer_t(ut, queue->front, queue->rear, __func__);

    ut_equals_integer_t(ut, size1, size2, __func__);

    dqa_delete(&queue);

    return DS_OK;

    error:
    printf("Error at %s\n", __func__);
    dqa_delete(&queue);
    return st;
}

// Tests locked capacity
Status dqa_test_locked(UnitTest ut)
{
    DequeArray queue;

    // in case I ever change the default initial capacity
    Status st = dqa_create(&queue, 16, 200);

    if (st != DS_OK)
        return st;

    dqa_cap_lock(queue);

    for (int i = 0; i < 17; i++)
    {
        st = dqa_enqueue_front(queue, i);
    }

    Status saved_st = st;
    integer_t size = queue->size;

    int j, sum = 0;
    while (!dqa_empty(queue))
    {
        st = dqa_dequeue_rear(queue, &j);

        if (st != DS_OK)
            goto error;

        sum += j; // sum from 0 to 15
    }

    ut_equals_int(ut, sum, 120, __func__);
    ut_equals_int(ut, saved_st, DS_ERR_FULL, __func__);
    ut_equals_integer_t(ut, size, 16, __func__);

    dqa_delete(&queue);

    return DS_OK;

    error:
    printf("Error at %s\n", __func__);
    dqa_delete(&queue);
    return st;
}

// Intensive test. Checks if all elements are preserved.
Status dqa_test_intensive(UnitTest ut)
{
    DequeArray queue;

    Status st = dqa_init(&queue);

    if (st != DS_OK)
        return st;

    int j = 0, k = 0, sum = 0, numbers = 0;

    // The total sum must be from 1 to 10000 that result in 50005000
    for (int i = 0; numbers < 10000; i = rand(), j = rand())
    {
        if (i % 2 == 0 || dqa_empty(queue))
        {
            if (j % 2 == 0)
                st = dqa_enqueue_rear(queue, ++numbers);
            else
                st = dqa_enqueue_front(queue, ++numbers);
        }
        else
        {
            if (j % 2 == 0)
                st = dqa_dequeue_rear(queue, &k);
            else
                st = dqa_dequeue_front(queue, &k);

            sum += k;
        }

        if (st != DS_OK)
            goto error;
    }

    // Emptying the queue
    while (!dqa_empty(queue))
    {
        st = dqa_dequeue_rear(queue, &j);

        if (st != DS_OK)
            goto error;

        sum += j;
    }

    ut_equals_int(ut, sum, 50005000, __func__);
    ut_equals_integer_t(ut, queue->front, queue->rear, __func__);

    dqa_delete(&queue);

    return DS_OK;

    error:
    printf("Error at %s\n", __func__);
    dqa_delete(&queue);
    return st;
}

// Tests capacity multiplication
Status dqa_test_growth(UnitTest ut)
{
    DequeArray queue;

    Status st = dqa_create(&queue, 60, 250);

    if (st != DS_OK)
        return st;;

    for (int i = 0; i < 100; i++)
    {
        st = dqa_enqueue_front(queue, i);

        if (st != DS_OK)
            goto error;
    }

    // 60 * (250 / 100)
    ut_equals_integer_t(ut, queue->capacity, 150, __func__);

    dqa_delete(&queue);

    return DS_OK;

    error:
    printf("Error at %s\n", __func__);
    dqa_delete(&queue);
    return st;
}

// Runs all DequeArray tests
Status DequeArrayTests(void)
{
    UnitTest ut;

    Status st = ut_init(&ut);

    if (st != DS_OK)
        goto error;

    st += dqa_test_linear_insertion(ut);
    st += dqa_test_locked(ut);
    st += dqa_test_intensive(ut);
    st += dqa_test_growth(ut);

    if (st != DS_OK)
        goto error;

    ut_report(ut, "DequeArray");

    ut_delete(&ut);

    return DS_OK;

    error:
    printf("Error at %s\n", __func__);
    ut_report(ut, "DequeArray");
    ut_delete(&ut);
    return st;
}