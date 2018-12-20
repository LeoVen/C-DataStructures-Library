/**
 * @file AVLTreeTests.c
 * 
 * @author Leonardo Vencovsky (https://github.com/LeoVen)
 *
 * @date 17/12/2018
 */

#include "AVLTree.h"
#include "UnitTest.h"
#include "Utility.h"

void avl_test_IO0(UnitTest ut)
{
    // Set the amount of elements to be added to the red-black tree
    integer_t T = 20000;

    Interface_t *interface = interface_new(compare_int64_t, copy_int64_t,
                                           display_int64_t, free, NULL, NULL);

    AVLTree_t *tree = avl_new(interface);

    void *element;
    bool success;
    for (integer_t i = 1; i <= T; i++)
    {
        element = new_int64_t(i);

        success = avl_insert(tree, element);

        if (!success)
            free(element);
    }

    ut_equals_integer_t(ut, avl_size(tree), T, __func__);

    void *key;
    for (integer_t i = 1; i <= T; i++)
    {
        key = new_int64_t(i);

        success = avl_remove(tree, key);

        free(key);

        if (!success)
            goto error;
    }

    ut_equals_integer_t(ut, avl_size(tree), 0, __func__);

    avl_free(tree);
    interface_free(interface);

    return;

    error:
    printf("Error at %s\n", __func__);
    avl_free(tree);
    interface_free(interface);
}

void avl_test_IO1(UnitTest ut)
{
    integer_t T = 20000;

    Interface_t *interface = interface_new(compare_int64_t, copy_int64_t,
                                           display_int64_t, free, NULL, NULL);

    AVLTree_t *tree = avl_new(interface);

    void *element;
    bool success;

    while (avl_size(tree) < T)
    {
        element = new_int64_t(random_int64_t(T * (-1), T));

        success = avl_insert(tree, element);

        if (!success)
            free(element);
    }

    ut_equals_integer_t(ut, avl_size(tree), T, __func__);

    avl_erase(tree);

    ut_equals_integer_t(ut, avl_size(tree), 0, __func__);

    avl_free(tree);
    interface_free(interface);
}

void avl_test_IO2(UnitTest ut)
{
    integer_t T = 20000;

    Interface_t *interface = interface_new(compare_int64_t, copy_int64_t,
                                           display_int64_t, free, NULL, NULL);

    AVLTree_t *tree = avl_new(interface);

    void *element;
    bool success;

    while (avl_size(tree) < T)
    {
        element = new_int64_t(random_int64_t(T * (-1), T));

        success = avl_insert(tree, element);

        if (!success)
            free(element);
    }

    ut_equals_integer_t(ut, avl_size(tree), T, __func__);

    while (!avl_empty(tree))
    {
        success = avl_pop(tree);

        if (!success)
            goto error;
    }

    ut_equals_integer_t(ut, avl_size(tree), 0, __func__);

    avl_free(tree);
    interface_free(interface);
    return;

    error:
    printf("Error at %s\n", __func__);
    avl_free(tree);
    interface_free(interface);
    ut_error();
}

void avl_test_IO3(UnitTest ut)
{
    // Lets have one tree with all the elements inside it. Divide it into two
    // other trees. The sum of the elements of bot trees must add up to the
    // original sum of the first tree.

    // Total elements of the first tree.
    integer_t T = 20000;

    Interface_t *interface = interface_new(compare_int64_t, copy_int64_t,
                                           display_int64_t, free, NULL, NULL);

    AVLTree_t *tree_original = avl_new(interface);
    AVLTree_t *tree_copy1 = avl_new(interface);
    AVLTree_t *tree_copy2 = avl_new(interface);

    int64_t total_sum = 0, copy1_sum = 0, copy2_sum = 0;
    void *element, *copy;
    bool success;

    while (avl_size(tree_original) < T)
    {
        element = new_int64_t(random_int64_t(T * (-1), T));

        success = avl_insert(tree_original, element);

        if (!success)
            free(element);
    }

    ut_equals_integer_t(ut, avl_size(tree_original), T, __func__);

    while (!avl_empty(tree_original))
    {
        // Remove from the original tree and make a copy of it
        element = avl_peek(tree_original);

        if (!element)
            goto error;

        copy = copy_int64_t(element);

        if (!copy)
            goto error;

        // Removing
        success = avl_pop(tree_original);

        if (!success)
        {
            free(copy);
            goto error;
        }

        total_sum += *(int64_t*)copy;

        // Decide to which tree to add this element
        if ((*(int64_t*)copy) % 2 == 0)
            success = avl_insert(tree_copy1, copy);
        else
            success = avl_insert(tree_copy2, copy);

        if (!success)
        {
            free(copy);
            goto error;
        }
    }

    // Now empty both trees and check their sum
    while (!avl_empty(tree_copy1))
    {
        element = avl_peek(tree_copy1);

        if (!element)
            goto error;

        copy1_sum += *(int64_t*)element;

        success = avl_pop(tree_copy1);

        if (!success)
            goto error;
    }

    while (!avl_empty(tree_copy2))
    {
        element = avl_peek(tree_copy2);

        if (!element)
            goto error;

        copy2_sum += *(int64_t*)element;

        success = avl_pop(tree_copy2);

        if (!success)
            goto error;
    }

    ut_equals_integer_t(ut, avl_size(tree_original), 0, __func__);
    ut_equals_integer_t(ut, avl_size(tree_copy1), 0, __func__);
    ut_equals_integer_t(ut, avl_size(tree_copy2), 0, __func__);
    ut_equals_integer_t(ut, total_sum, copy1_sum + copy2_sum, __func__);

    avl_free(tree_original);
    avl_free(tree_copy1);
    avl_free(tree_copy2);
    interface_free(interface);
    return;

    error:
    printf("Error at %s\n", __func__);
    avl_free(tree_original);
    avl_free(tree_copy1);
    avl_free(tree_copy2);
    interface_free(interface);
    ut_error();
}

// Runs all AVLTree tests
Status AVLTreeTests(void)
{
    UnitTest ut;

    Status st = ut_init(&ut);

    if (st != DS_OK)
        goto error;

    avl_test_IO0(ut);
    avl_test_IO1(ut);
    avl_test_IO2(ut);
    avl_test_IO3(ut);

    ut_report(ut, "AVLTree");

    ut_delete(&ut);

    return DS_OK;

    error:
    printf("Error at %s\n", __func__);
    ut_report(ut, "AVLTree");
    ut_delete(&ut);
    return st;
}