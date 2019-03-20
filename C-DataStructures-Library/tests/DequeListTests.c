/**
 * @file DequeListTests.c
 * 
 * @author Leonardo Vencovsky (https://github.com/LeoVen)
 *
 * @date 20/10/2018
 */

#include "DequeList.h"
#include "UnitTest.h"
#include "Utility.h"

// Tests limit functionality
void dql_test_limit(UnitTest ut)
{
    Interface_t int_interface;
    interface_init(&int_interface, compare_int32_t, copy_int32_t,
                   display_int32_t, free, NULL, NULL);

    uint8_t dql_storage[dql_size];
    DequeList_t *deque = (DequeList_t*)&dql_storage[0];
    dql_init(deque, &int_interface);

    if (!dql_set_limit(deque, 10))
        goto error;

    void *elem;
    for (int i = 0; i < 20; i++)
    {
        elem = new_int32_t(i);

        if (!dql_enqueue_rear(deque, elem))
        {
            free(elem);
        }
    }

    ut_equals_integer_t(ut, dql_count(deque), dql_limit(deque), __func__);
    ut_equals_bool(ut, dql_set_limit(deque, 9), false, __func__);

    int *t = new_int32_t(1);
    ut_equals_bool(ut, dql_enqueue_front(deque, t), false, __func__);
    ut_equals_bool(ut, dql_enqueue_rear(deque, t), false, __func__);

    // Removes the limit
    ut_equals_bool(ut, dql_set_limit(deque, 0), true, __func__);
    ut_equals_integer_t(ut, dql_limit(deque), 0, __func__);
    ut_equals_bool(ut, dql_enqueue_front(deque, t), true, __func__);

    dql_erase(deque);

    return;

    error:
    printf("Error at %s\n", __func__);
    dql_erase(deque);
}

void dql_test_foreach(UnitTest ut)
{
    Interface_t int_interface;
    interface_init(&int_interface, compare_int32_t, copy_int32_t,
                   display_int32_t, free, NULL, NULL);

    DQL_DECL(deque)
    dql_init(deque, &int_interface);

    int *elem = NULL;
    for (int i = 0; i < 1001; i++)
    {
        elem = new_int32_t(i);

        if (!dql_enqueue_rear(deque, elem))
        {
            free(elem);
        }
    }

    int32_t sum = 0;

    DQL_FOR_EACH(deque, {
            sum += *(int*)var;
    })

    ut_equals_int(ut, 500500, sum, __func__);

    sum = 0;

    DQL_FOR_EACH(deque, {
            int32_t i = *(int*)var;
            sum += i % 2 == 0 ? i : 0;
    })

    ut_equals_int(ut, 250500, sum, __func__);

    dql_erase(deque);
}

// Runs all DequeList tests
Status DequeListTests(void)
{
    UnitTest ut;

    Status st = ut_init(&ut);

    if (st != DS_OK)
        goto error;

    dql_test_limit(ut);
    dql_test_foreach(ut);

    ut_report(ut, "DequeList");

    ut_delete(&ut);

    return DS_OK;

    error:
    printf("Error at %s\n", __func__);
    ut_report(ut, "DequeList");
    ut_delete(&ut);
    return st;
}
