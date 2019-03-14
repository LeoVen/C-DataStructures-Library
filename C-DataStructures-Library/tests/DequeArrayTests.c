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
// Sum of dequeued contents should equal to the known result (500500)
void dqa_test_linear_insertion_rear(UnitTest ut)
{
    Interface int_interface = interface_new(compare_int32_t, copy_int32_t,
                                            display_int32_t, free, NULL, NULL);

    if (!int_interface)
        goto error;

    // in case I ever change the default initial capacity
    DequeArray_t *deque = dqa_create(int_interface, 16, 200);

    if (!deque)
        goto error;

    bool success = false;

    void *element;
    for (int i = 1; i <= 1000; i++)
    {
        element = new_int32_t(i);

        success = dqa_enqueue_rear(deque, element);

        if (!success)
        {
            free(element);
            goto error;
        }
    }

    int sum = 0;
    while (!dqa_empty(deque))
    {
        success = dqa_dequeue_rear(deque, &element);

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

    dqa_free(deque);
    interface_free(int_interface);

    return;

    error:
    printf("Error at %s\n", __func__);
    ut_error();
    dqa_free(deque);
    interface_free(int_interface);
}

// Tests dqa_grow() in linear insertions
// Tests while (!empty) loop
// Sum of dequeued contents should equal to the known result (500500)
void dqa_test_linear_insertion_front(UnitTest ut)
{
    Interface int_interface = interface_new(compare_int32_t, copy_int32_t,
                                            display_int32_t, free, NULL, NULL);

    if (!int_interface)
        goto error;

    // in case I ever change the default initial capacity
    DequeArray_t *deque = dqa_create(int_interface, 16, 200);

    if (!deque)
        goto error;

    bool success = false;

    void *element;
    for (int i = 1; i <= 1000; i++)
    {
        element = new_int32_t(i);

        success = dqa_enqueue_front(deque, element);

        if (!success)
        {
            free(element);
            goto error;
        }
    }

    int sum = 0;
    while (!dqa_empty(deque))
    {
        success = dqa_dequeue_front(deque, &element);

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

    dqa_free(deque);
    interface_free(int_interface);

    return;

    error:
    printf("Error at %s\n", __func__);
    ut_error();
    dqa_free(deque);
    interface_free(int_interface);
}

// Tests locked capacity
void dqa_test_locked(UnitTest ut)
{
    Interface int_interface = interface_new(compare_int32_t, copy_int32_t,
                                            display_int32_t,free, NULL, NULL);

    if (!int_interface)
        goto error;

    // in case I ever change the default initial capacity
    DequeArray_t *deque = dqa_create(int_interface, 16, 200);

    if (!deque)
        goto error;

    dqa_capacity_lock(deque);

    bool success = false;

    // [ 16, 14, 12, 10, 8, 6, 4, 2, 1, 3, 5, 7, 9, 11, 13, 15 ]
    void *element;
    for (int i = 1; i < 18; i++)
    {
        element = new_int32_t(i);

        if (i % 2 == 0)
        {
            success = dqa_enqueue_front(deque, element);
        }
        else
        {
            success = dqa_enqueue_rear(deque, element);
        }

        if (!success) /* Reached the stack maximum size */
            free(element);
    }

    integer_t size = dqa_count(deque);

    ut_equals_bool(ut, success, false, __func__);
    ut_equals_integer_t(ut, size, 16, __func__);

    dqa_capacity_unlock(deque);

    // [ 16, 14, 12, 10, 8, 6, 4, 2, 1, 3, 5, 7, 9, 11, 13, 15, 17 ]
    element = new_int32_t(17);
    success = dqa_enqueue_rear(deque, element);
    if (!success)
    {
        free(element);
        goto error;
    }

    ut_equals_integer_t(ut, dqa_count(deque), 17, __func__);

    // 16 is removed
    success = dqa_dequeue_front(deque, &element);

    if (success)
        free(element);
    else
        goto error;

    // [ 14, 12, 10, 8, 6, 4, 2, 1, 3, 5, 7, 9, 11, 13, 15, 17 ]
    int sum = 0;
    while (!dqa_empty(deque))
    {
        dqa_dequeue_rear(deque, &element);

        sum += *(int*)element; /* sum from 1 to 17 excluding 16 -> 137 */

        free(element);
    }

    ut_equals_int(ut, sum, 137, __func__);

    dqa_free(deque);
    interface_free(int_interface);

    return;

    error:
    printf("Error at %s\n", __func__);
    ut_error();
    dqa_free(deque);
    interface_free(int_interface);
}

// Intensive test. Checks if all elements are preserved.
void dqa_test_intensive(UnitTest ut)
{
    Interface int_interface = interface_new(compare_int32_t, copy_int32_t,
                                            display_int32_t, free, NULL, NULL);

    if (!int_interface)
        goto error;

    // in case I ever change the default initial capacity
    DequeArray_t *deque = dqa_create(int_interface, 16, 200);

    if (!deque)
        goto error;

    bool success = false;
    void *element = NULL;
    int sum = 0, numbers = 0;

    // The total sum must be from 1 to 10000 that result in 50005000
    for (int i = 0; numbers < 10000; i = rand())
    {
        if (i % 2 == 0 || dqa_empty(deque))
        {
            element = new_int32_t(++numbers);

            if (i % 4 == 0)
            {
                success = dqa_enqueue_front(deque, element);
            }
            else
            {
                success = dqa_enqueue_rear(deque, element);
            }

            if (!success)
            {
                free(element);
                goto error;
            }
        }
        else
        {
            if (i % 3 == 0)
            {
                success = dqa_dequeue_front(deque, &element);
            }
            else
            {
                success = dqa_dequeue_rear(deque, &element);
            }

            if (!success)
                goto error;

            sum += *(int*)element;

            free(element);
        }
    }

    // Emptying the queue
    while (!dqa_empty(deque))
    {
        success = dqa_dequeue_front(deque, &element);

        if (!success)
            goto error;

        sum += *(int*)element;

        free(element);
    }

    ut_equals_int(ut, sum, 50005000, __func__);

    dqa_free(deque);
    interface_free(int_interface);

    return;

    error:
    printf("Error at %s\n", __func__);
    ut_error();
    dqa_free(deque);
    interface_free(int_interface);
}

// Tests capacity multiplication
void dqa_test_growth(UnitTest ut)
{
    Interface int_interface = interface_new(compare_int32_t, copy_int32_t,
                                            display_int32_t, free, NULL, NULL);

    if (!int_interface)
        goto error;

    DequeArray_t *deque = dqa_create(int_interface, 60, 250);

    if (!deque)
        goto error;

    bool success = false;

    void *element;
    for (int i = 0; i < 100; i++)
    {
        element = new_int32_t(i);

        success = dqa_enqueue_rear(deque, element);

        if (!success)
        {
            free(element);
            goto error;
        }
    }

    // 60 * (250 / 100)
    ut_equals_integer_t(ut, dqa_capacity(deque), 150, __func__);

    dqa_free(deque);
    interface_free(int_interface);

    return;

    error:
    printf("Error at %s\n", __func__);
    ut_error();
    dqa_free(deque);
    interface_free(int_interface);
}

// Runs all DequeArray tests
Status DequeArrayTests(void)
{
    UnitTest ut;

    Status st = ut_init(&ut);

    if (st != DS_OK)
        goto error;

    dqa_test_linear_insertion_rear(ut);
    dqa_test_linear_insertion_front(ut);
    dqa_test_locked(ut);
    dqa_test_intensive(ut);
    dqa_test_growth(ut);

    ut_report(ut, "DequeArray");

    ut_delete(&ut);

    return DS_OK;

    error:
    printf("Error at %s\n", __func__);
    ut_report(ut, "DequeArray");
    ut_delete(&ut);
    return st;
}
