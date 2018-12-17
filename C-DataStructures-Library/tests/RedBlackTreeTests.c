/**
 * @file RedBlackTreeTests.c
 * 
 * @author Leonardo Vencovsky (https://github.com/LeoVen)
 *
 * @date 14/12/2018
 */

#include "RedBlackTree.h"
#include "UnitTest.h"
#include "Utility.h"

void rbt_test_IO0(UnitTest ut)
{
    // Set the amount of elements to be added to the red-black tree
    integer_t T = 20000;

    Interface_t *interface = interface_new(compare_int64_t, copy_int64_t,
                                           display_int64_t, free, NULL, NULL);

    RedBlackTree_t *tree = rbt_new(interface);

    void *element;
    bool success;
    for (integer_t i = 1; i <= T; i++)
    {
        element = new_int64_t(i);

        success = rbt_insert(tree, element);

        if (!success)
            free(element);
    }

    ut_equals_integer_t(ut, rbt_size(tree), T, __func__);

    void *key;
    for (integer_t i = 1; i <= T; i++)
    {
        key = new_int64_t(i);

        success = rbt_remove(tree, key);

        free(key);

        if (!success)
            goto error;
    }

    ut_equals_integer_t(ut, rbt_size(tree), 0, __func__);

    rbt_free(tree);
    interface_free(interface);

    return;

    error:
    printf("Error at %s\n", __func__);
    rbt_free(tree);
    interface_free(interface);
}

void rbt_test_IO1(UnitTest ut)
{
    integer_t T = 20000;

    Interface_t *interface = interface_new(compare_int64_t, copy_int64_t,
                                           display_int64_t, free, NULL, NULL);

    RedBlackTree_t *tree = rbt_new(interface);

    void *element;
    bool success;

    while (rbt_size(tree) < T)
    {
        element = new_int64_t(random_int64_t(T * (-1), T));

        success = rbt_insert(tree, element);

        if (!success)
            free(element);
    }

    ut_equals_integer_t(ut, rbt_size(tree), T, __func__);

    rbt_erase(tree);

    ut_equals_integer_t(ut, rbt_size(tree), 0, __func__);

    rbt_free(tree);
    interface_free(interface);
}

// Runs all RedBlackTree tests
Status RedBlackTreeTests(void)
{
    UnitTest ut;

    Status st = ut_init(&ut);

    if (st != DS_OK)
        goto error;

    rbt_test_IO0(ut);
    rbt_test_IO1(ut);

    ut_report(ut, "RedBlackTree");

    ut_delete(&ut);

    return DS_OK;

    error:
    printf("Error at %s\n", __func__);
    ut_report(ut, "RedBlackTree");
    ut_delete(&ut);
    return st;
}