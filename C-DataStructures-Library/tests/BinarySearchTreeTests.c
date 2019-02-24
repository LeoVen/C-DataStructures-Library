/**
 * @file BinarySearchTreeTests.c
 * 
 * @author Leonardo Vencovsky (https://github.com/LeoVen)
 *
 * @date 24/02/2019
 */
 
#include "BinarySearchTree.h"
#include "UnitTest.h"
#include "Utility.h"

void bst_test_IO0(UnitTest ut)
{
    // Set the amount of elements to be added to the red-black tree
    integer_t T = 10000;

    Interface_t *interface = interface_new(compare_int64_t, copy_int64_t,
                                           display_int64_t, free, NULL, NULL);

    BinarySearchTree_t *tree = bst_new(interface);

    void *element;
    bool success;
    for (integer_t i = 1; i <= T; i++)
    {
        element = new_int64_t(i);

        success = bst_insert(tree, element);

        if (!success)
            free(element);
    }

    ut_equals_integer_t(ut, bst_count(tree), T, __func__);

    void *key;
    for (integer_t i = 1; i <= T; i++)
    {
        key = new_int64_t(i);

        success = bst_remove(tree, key);

        free(key);

        if (!success)
            goto error;
    }

    ut_equals_integer_t(ut, bst_count(tree), 0, __func__);

    bst_free(tree);
    interface_free(interface);

    return;

    error:
    printf("Error at %s\n", __func__);
    bst_free(tree);
    interface_free(interface);
}

void bst_test_IO1(UnitTest ut)
{
    integer_t T = 10000;

    Interface_t *interface = interface_new(compare_int64_t, copy_int64_t,
                                           display_int64_t, free, NULL, NULL);

    BinarySearchTree_t *tree = bst_new(interface);

    void *element;
    bool success;

    while (bst_count(tree) < T)
    {
        element = new_int64_t(random_int64_t(T * (-1), T));

        success = bst_insert(tree, element);

        if (!success)
            free(element);
    }

    ut_equals_integer_t(ut, bst_count(tree), T, __func__);

    bst_erase(tree);

    ut_equals_integer_t(ut, bst_count(tree), 0, __func__);

    bst_free(tree);
    interface_free(interface);
}

void bst_test_IO2(UnitTest ut)
{
    integer_t T = 10000;

    Interface_t *interface = interface_new(compare_int64_t, copy_int64_t,
                                           display_int64_t, free, NULL, NULL);

    BinarySearchTree_t *tree = bst_new(interface);

    void *element;
    bool success;

    while (bst_count(tree) < T)
    {
        element = new_int64_t(random_int64_t(T * (-1), T));

        success = bst_insert(tree, element);

        if (!success)
            free(element);
    }

    ut_equals_integer_t(ut, bst_count(tree), T, __func__);

    while (!bst_empty(tree))
    {
        success = bst_pop(tree);

        if (!success)
            goto error;
    }

    ut_equals_integer_t(ut, bst_count(tree), 0, __func__);

    bst_free(tree);
    interface_free(interface);
    return;

    error:
    printf("Error at %s\n", __func__);
    bst_free(tree);
    interface_free(interface);
    ut_error();
}

void bst_test_IO3(UnitTest ut)
{
    // Lets have one tree with all the elements inside it. Divide it into two
    // other trees. The sum of the elements of bot trees must add up to the
    // original sum of the first tree.

    // Total elements of the first tree.
    integer_t T = 10000;

    Interface_t *interface = interface_new(compare_int64_t, copy_int64_t,
                                           display_int64_t, free, NULL, NULL);

    BinarySearchTree_t *tree_original = bst_new(interface);
    BinarySearchTree_t *tree_copy1 = bst_new(interface);
    BinarySearchTree_t *tree_copy2 = bst_new(interface);

    int64_t total_sum = 0, copy1_sum = 0, copy2_sum = 0;
    void *element, *copy;
    bool success;

    while (bst_count(tree_original) < T)
    {
        element = new_int64_t(random_int64_t(T * (-1), T));

        success = bst_insert(tree_original, element);

        if (!success)
            free(element);
    }

    ut_equals_integer_t(ut, bst_count(tree_original), T, __func__);

    while (!bst_empty(tree_original))
    {
        // Remove from the original tree and make a copy of it
        element = bst_peek(tree_original);

        if (!element)
            goto error;

        copy = copy_int64_t(element);

        if (!copy)
            goto error;

        // Removing
        success = bst_pop(tree_original);

        if (!success)
        {
            free(copy);
            goto error;
        }

        total_sum += *(int64_t*)copy;

        // Decide to which tree to add this element
        if ((*(int64_t*)copy) % 2 == 0)
            success = bst_insert(tree_copy1, copy);
        else
            success = bst_insert(tree_copy2, copy);

        if (!success)
        {
            free(copy);
            goto error;
        }
    }

    // Now empty both trees and check their sum
    while (!bst_empty(tree_copy1))
    {
        element = bst_peek(tree_copy1);

        if (!element)
            goto error;

        copy1_sum += *(int64_t*)element;

        success = bst_pop(tree_copy1);

        if (!success)
            goto error;
    }

    while (!bst_empty(tree_copy2))
    {
        element = bst_peek(tree_copy2);

        if (!element)
            goto error;

        copy2_sum += *(int64_t*)element;

        success = bst_pop(tree_copy2);

        if (!success)
            goto error;
    }

    ut_equals_integer_t(ut, bst_count(tree_original), 0, __func__);
    ut_equals_integer_t(ut, bst_count(tree_copy1), 0, __func__);
    ut_equals_integer_t(ut, bst_count(tree_copy2), 0, __func__);
    ut_equals_integer_t(ut, total_sum, copy1_sum + copy2_sum, __func__);

    bst_free(tree_original);
    bst_free(tree_copy1);
    bst_free(tree_copy2);
    interface_free(interface);
    return;

    error:
    printf("Error at %s\n", __func__);
    bst_free(tree_original);
    bst_free(tree_copy1);
    bst_free(tree_copy2);
    interface_free(interface);
    ut_error();
}

// Runs all BinarySearchTree tests
Status BinarySearchTreeTests(void)
{
    UnitTest ut;

    Status st = ut_init(&ut);

    if (st != DS_OK)
        goto error;

    bst_test_IO0(ut);
    bst_test_IO1(ut);
    bst_test_IO2(ut);
    bst_test_IO3(ut);

    ut_report(ut, "BinarySearchTree");

    ut_delete(&ut);

    return DS_OK;

    error:
    printf("Error at %s\n", __func__);
    ut_report(ut, "BinarySearchTree");
    ut_delete(&ut);
    return st;
}