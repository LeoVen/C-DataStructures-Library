/**
 * @file QueueListListTests.c
 * 
 * @author Leonardo Vencovsky (https://github.com/LeoVen)
 *
 * @date 20/10/2018
 */

#include "QueueList.h"
#include "UnitTest.h"
#include "Utility.h"

// Tests limit functionality
Status qli_test_limit(UnitTest ut)
{
    QueueList queue;

    Status st = qli_create(&queue, compare_int32_t, copy_int32_t,
            display_int32_t, free);

    if (st != DS_OK)
        return st;

    st = qli_set_limit(queue, 10);

    if (st != DS_OK)
        goto error;

    void *elem;
    for (int i = 0; i < 20; i++)
    {
        elem = new_int32_t(i);

        st = qli_enqueue(queue, elem);

        if (st == DS_ERR_FULL)
        {
            free(elem);
        }
    }

    ut_equals_int(ut, st, DS_ERR_FULL, __func__);
    ut_equals_integer_t(ut, qli_length(queue), qli_limit(queue), __func__);
    ut_equals_int(ut, qli_set_limit(queue, 9), DS_ERR_INVALID_OPERATION, __func__);

    int *t = new_int32_t(1);
    ut_equals_int(ut, qli_enqueue(queue, t), DS_ERR_FULL, __func__);

    // Removes the limit
    ut_equals_int(ut, qli_set_limit(queue, 0), DS_OK, __func__);
    ut_equals_integer_t(ut, qli_limit(queue), 0, __func__);
    ut_equals_int(ut, qli_enqueue(queue, t), DS_OK, __func__);

    qli_free(&queue);

    return DS_OK;

    error:
    printf("Error at %s\n", __func__);
    qli_free(&queue);
    return st;
}

// Runs all QueueList tests
Status QueueListTests(void)
{
    UnitTest ut;

    Status st = ut_init(&ut);

    if (st != DS_OK)
        goto error;

    st += qli_test_limit(ut);

    if (st != DS_OK)
        goto error;

    ut_report(ut, "QueueList");

    ut_delete(&ut);

    return DS_OK;

    error:
    printf("Error at %s\n", __func__);
    ut_report(ut, "QueueList");
    ut_delete(&ut);
    return st;
}
