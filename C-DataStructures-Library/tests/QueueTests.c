/**
 * @file QueueTests.c
 * 
 * @author Leonardo Vencovsky (https://github.com/LeoVen)
 *
 * @date 20/10/2018
 */


#include "Queue.h"
#include "UnitTest.h"
#include "Util.h"

// Tests limit functionality
Status que_test_limit(UnitTest ut)
{
    Queue queue;

    Status st = que_create(&queue, compare_int, copy_int, display_int, free);

    if (st != DS_OK)
        return st;

    st = que_set_limit(queue, 10);

    if (st != DS_OK)
        goto error;

    void *elem;
    for (int i = 0; i < 20; i++)
    {
        elem = new_int(i);

        st = que_enqueue(queue, elem);

        if (st == DS_ERR_FULL)
        {
            free(elem);
        }
    }

    ut_equals_int(ut, st, DS_ERR_FULL, __func__);
    ut_equals_integer_t(ut, que_length(queue), que_limit(queue), __func__);
    ut_equals_int(ut, que_set_limit(queue, 9), DS_ERR_INVALID_OPERATION, __func__);

    int *t = new_int(1);
    ut_equals_int(ut, que_enqueue(queue, t), DS_ERR_FULL, __func__);

    // Removes the limit
    ut_equals_int(ut, que_set_limit(queue, 0), DS_OK, __func__);
    ut_equals_integer_t(ut, que_limit(queue), 0, __func__);
    ut_equals_int(ut, que_enqueue(queue, t), DS_OK, __func__);

    que_free(&queue);

    return DS_OK;

    error:
    que_free(&queue);
    return st;
}

// Runs all Queue tests
Status QueueTests(void)
{
    UnitTest ut;

    Status st = ut_init(&ut);

    if (st != DS_OK)
        goto error;

    st += que_test_limit(ut);

    if (st != DS_OK)
        goto error;

    ut_report(ut, "Queue");

    ut_delete(&ut);

    return DS_OK;

    error:
    ut_report(ut, "Queue");
    ut_delete(&ut);
    return st;
}
