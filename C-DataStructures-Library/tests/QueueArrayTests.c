/**
 * @file QueueArrayTests.c
 * 
 * @author Leonardo Vencovsky (https://github.com/LeoVen)
 *
 * @date 15/10/2018
 */

#include "QueueArray.h"
#include "UnitTest.h"
#include "Utility.h"

// Tests qar_grow() in linear insertions
// Tests while (!empty) loop
// Sum of dequeued contents should equal to the known result (500500)
void qar_test_linear_insertion(UnitTest ut)
{
    Interface int_interface = interface_new(compare_int32_t, copy_int32_t,
            display_int32_t, free, NULL, NULL);

    if (!int_interface)
        goto error;

    // in case I ever change the default initial capacity
    QueueArray queue = qar_create(int_interface, 16, 200);

    if (!queue)
        goto error;

    bool success = false;

    void *element;
    for (int i = 1; i <= 1000; i++)
    {
        element = new_int32_t(i);

        success = qar_enqueue(queue, element);

        if (!success)
        {
            free(element);
            goto error;
        }
    }

    int sum = 0;
    while (!qar_empty(queue))
    {
        success = qar_dequeue(queue, &element);

        if (!success)
        {
            free(element);
            goto error;
        }

        sum += *(int*)element;
        free(element);
    }

    // see if all elements are preserved
    ut_equals_int(ut, sum, 500500, __func__);

    qar_free(queue);
    interface_free(int_interface);

    return;

    error:
    printf("Error at %s\n", __func__);
    ut_error();
    qar_free(queue);
    interface_free(int_interface);
}

// Tests locked capacity
void qar_test_locked(UnitTest ut)
{
    Interface int_interface = interface_new(compare_int32_t, copy_int32_t,
            display_int32_t,free, NULL, NULL);

    if (!int_interface)
        goto error;

    // in case I ever change the default initial capacity
    QueueArray queue = qar_create(int_interface, 16, 200);

    if (!queue)
        goto error;

    qar_capacity_lock(queue);

    bool success = false;

    void *element;
    for (int i = 1; i < 18; i++)
    {
        element = new_int32_t(i);

        success = qar_enqueue(queue, element);

        if (!success) /* Reached the stack maximum count */
            free(element);
    }

    integer_t count = qar_count(queue);

    ut_equals_bool(ut, success, false, __func__);
    ut_equals_integer_t(ut, count, 16, __func__);

    qar_capacity_unlock(queue);

    element = new_int32_t(17);
    success = qar_enqueue(queue, element);
    if (!success)
    {
        free(element);
        goto error;
    }

    ut_equals_integer_t(ut, qar_count(queue), 17, __func__);

    success = qar_dequeue(queue, &element);

    if (success)
        free(element);
    else
        goto error;

    int sum = 0;
    while (!qar_empty(queue))
    {
        qar_dequeue(queue, &element);

        sum += *(int*)element; /* sum from 2 to 17  -> 152 */

        free(element);
    }

    ut_equals_int(ut, sum, 152, __func__);

    qar_free(queue);
    interface_free(int_interface);

    return;

    error:
    printf("Error at %s\n", __func__);
    ut_error();
    qar_free(queue);
    interface_free(int_interface);
}

// Intensive test. Checks if all elements are preserved.
void qar_test_intensive(UnitTest ut)
{
    Interface int_interface = interface_new(compare_int32_t, copy_int32_t,
            display_int32_t, free, NULL, NULL);

    if (!int_interface)
        goto error;

    // in case I ever change the default initial capacity
    QueueArray queue = qar_create(int_interface, 16, 200);

    if (!queue)
        goto error;

    bool success = false;
    void *element = NULL;
    int sum = 0, numbers = 0;

    // The total sum must be from 1 to 10000 that result in 50005000
    for (int i = 0; numbers < 10000; i = rand())
    {
        if (i % 2 == 0 || qar_empty(queue))
        {
            element = new_int32_t(++numbers);
            success = qar_enqueue(queue, element);

            if (!success)
            {
                free(element);
                goto error;
            }
        }
        else
        {
            success = qar_dequeue(queue, &element);

            if (!success)
                goto error;

            sum += *(int*)element;

            free(element);
        }
    }

    // Emptying the queue
    while (!qar_empty(queue))
    {
        success = qar_dequeue(queue, &element);

        if (!success)
            goto error;

        sum += *(int*)element;

        free(element);
    }

    ut_equals_int(ut, sum, 50005000, __func__);

    qar_free(queue);
    interface_free(int_interface);

    return;

    error:
    printf("Error at %s\n", __func__);
    ut_error();
    qar_free(queue);
    interface_free(int_interface);
}

// Tests capacity multiplication
void qar_test_growth(UnitTest ut)
{
    Interface int_interface = interface_new(compare_int32_t, copy_int32_t,
            display_int32_t, free, NULL, NULL);

    if (!int_interface)
        goto error;

    QueueArray queue = qar_create(int_interface, 60, 250);

    if (!queue)
        goto error;

    bool success = false;

    void *element;
    for (int i = 0; i < 100; i++)
    {
        element = new_int32_t(i);

        success = qar_enqueue(queue, element);

        if (!success)
        {
            free(element);
            goto error;
        }
    }

    // 60 * (250 / 100)
    ut_equals_integer_t(ut, qar_capacity(queue), 150, __func__);

    qar_free(queue);
    interface_free(int_interface);

    return;

    error:
    printf("Error at %s\n", __func__);
    ut_error();
    qar_free(queue);
    interface_free(int_interface);
}

// Runs all QueueArray tests
Status QueueArrayTests(void)
{
    UnitTest ut;

    Status st = ut_init(&ut);

    if (st != DS_OK)
        goto error;

    qar_test_linear_insertion(ut);
    qar_test_locked(ut);
    qar_test_intensive(ut);
    qar_test_growth(ut);

    ut_report(ut, "QueueArray");

    ut_delete(&ut);

    return DS_OK;

    error:
    printf("Error at %s\n", __func__);
    ut_report(ut, "QueueArray");
    ut_delete(&ut);
    return st;
}