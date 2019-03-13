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
void qli_test_limit(UnitTest ut)
{
    Interface_t int_interface;
    interface_init(&int_interface, compare_int32_t, copy_int32_t,
                   display_int32_t, free, NULL, NULL);

    uint8_t qli_storage[qli_size];
    QueueList_t *queue = (QueueList_t*)&qli_storage[0];
    qli_init(queue, &int_interface);

    if (!qli_set_limit(queue, 10))
        goto error;

    void *elem = NULL;
    for (int i = 0; i < 20; i++)
    {
        elem = new_int32_t(i);

        if (!qli_enqueue(queue, elem))
        {
            free(elem);
        }
    }

    ut_equals_bool(ut, qli_enqueue(queue, elem), false, __func__);
    ut_equals_integer_t(ut, qli_count(queue), qli_limit(queue), __func__);
    ut_equals_bool(ut, qli_set_limit(queue, 9), false, __func__);

    int *t = new_int32_t(1);
    ut_equals_int(ut, qli_enqueue(queue, t), false, __func__);

    // Removes the limit
    ut_equals_int(ut, qli_set_limit(queue, 0), true, __func__);
    ut_equals_integer_t(ut, qli_limit(queue), 0, __func__);
    ut_equals_int(ut, qli_enqueue(queue, t), true, __func__);

    qli_erase(queue);

    return;

    error:
    printf("Error at %s\n", __func__);
    ut_error();
    qli_erase(queue);
}

// Runs all QueueList tests
Status QueueListTests(void)
{
    UnitTest ut;

    Status st = ut_init(&ut);

    if (st != DS_OK)
        goto error;

    qli_test_limit(ut);

    ut_report(ut, "QueueList");

    ut_delete(&ut);

    return DS_OK;

    error:
    printf("Error at %s\n", __func__);
    ut_report(ut, "QueueList");
    ut_delete(&ut);
    return st;
}
