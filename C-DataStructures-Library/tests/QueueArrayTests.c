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
Status qua_tests_linear_insertion(UnitTest ut)
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

    int sum = 0;

    for (size_t t = 0; t < queue->size; t++)
    {
        sum += queue->buffer[t];
    }

    qua_delete(&queue);

    ut_equals_int(ut, sum, 500500, "qua_tests_linear_insertion");

    return DS_OK;

    error:
    qua_delete(&queue);
    return st;
}

// Removes from queue until it is empty and tests front == rear on empty queue
Status qua_tests_empty(UnitTest ut)
{
    QueueArray queue;

    Status st = qua_init(&queue);

    if (st != DS_OK)
        goto error;

    for (int i = 1; i <= 1000; i++)
    {
        st = qua_enqueue(queue, i);

        if (st != DS_OK)
            goto error;
    }

    int j;
    while (!qua_empty(queue))
    {
        st = qua_dequeue(queue, &j);

        if (st != DS_OK)
            goto error;
    }

    ut_equals_size_t(ut, queue->front, queue->rear, "qua_tests_empty");

    qua_delete(&queue);

    return DS_OK;

    error:
    qua_delete(&queue);
    return st;
}

Status QueueArrayTests(void)
{
    UnitTest ut;

    Status st = ut_init(&ut);

    if (st != DS_OK)
        goto error;

    st = qua_tests_linear_insertion(ut);

    if (st != DS_OK)
        goto error;

    st = qua_tests_empty(ut);

    if (st != DS_OK)
        goto error;

    ut_delete(&ut);

    return DS_OK;

    error:
    ut_delete(&ut);
    return st;
}