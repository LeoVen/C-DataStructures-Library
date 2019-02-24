/**
 * @file RedBlackTreeTests.c
 *
 * @author Leonardo Vencovsky (https://github.com/LeoVen)
 *
 * @date 19/02/2019
 */

#include "Heap.h"
#include "UnitTest.h"
#include "Utility.h"

// Checks if no elements are lost
void hep_test_IO0(UnitTest ut)
{
    const int elements = 1000;

    Interface int_interface = interface_new(compare_int32_t, copy_int32_t,
                                            display_int32_t, free, NULL, NULL);

    Heap_t *heap = hep_new(int_interface, MinHeap);

    if (!int_interface || !heap)
        goto error;

    int sum0 = 0, sum1 = 0;
    void *elem;
    for (int i = 0; i < elements; i++)
    {
        elem = new_int32_t(random_int32_t(-elements, elements));
        sum0 += *(int *)elem;
        if (!hep_insert(heap, elem))
            free(elem);
    }

    ut_equals_integer_t(ut, elements, hep_count(heap), __func__);

    while (!hep_empty(heap))
    {
        if (!hep_remove(heap, &elem))
            goto error;
        sum1 += *(int *)elem;
        free(elem);
    }

    ut_equals_integer_t(ut, 0, hep_count(heap), __func__);
    ut_equals_int(ut, sum0, sum1, __func__);

    hep_free(heap);
    interface_free(int_interface);

    return;

    error:
    printf("Error at %s\n", __func__);
    ut_error();
    hep_free(heap);
    interface_free(int_interface);
}

// Checks if when removed, the elements are sorted for both MinHeap and MaxHeap
void hep_test_IO1(UnitTest ut)
{
    const int elements = 10000;

    enum HeapKind_e K[2] = {MaxHeap, MinHeap};

    for (int k = 0; k < 2; k++)
    {
        Interface int_interface = interface_new(compare_int32_t, copy_int32_t,
                                                display_int32_t, free, NULL, NULL);

        Heap_t *heap = hep_new(int_interface, K[k]);

        if (!int_interface || !heap)
            goto error;

        void *elem;
        for (int i = 0; i < elements; i++)
        {
            elem = new_int32_t(random_int32_t(-elements, elements));
            if (!hep_insert(heap, elem))
                free(elem);
        }

        void **buffer = malloc(sizeof(void*) * (size_t)(elements));
        memset(buffer, 0, elements);

        int j = 0;
        while (!hep_empty(heap))
        {
            if (!hep_remove(heap, &elem))
                goto error;

            buffer[j++] = elem;
        }

        bool sorted = true;
        for (int i = 0; i < elements - 1; i++)
        {
            // The element before should always have higher "priority"
            if (compare_int32_t(buffer[i], buffer[i + 1]) * hep_kind(heap) < 0)
            {
                sorted = false;
                break;
            }
        }

        ut_equals_bool(ut, true, sorted, __func__);

        for (int i = 0; i < elements; i++)
        {
            free(buffer[i]);
        }
        free(buffer);

        hep_free(heap);
        interface_free(int_interface);

        continue;

        error:
        printf("Error at %s\n", __func__);
        ut_error();
        hep_free(heap);
        interface_free(int_interface);
    }
}

// Runs all Heap tests
Status HeapTests(void)
{
    UnitTest ut;

    Status st = ut_init(&ut);

    if (st != DS_OK)
        goto error;

    hep_test_IO0(ut);
    hep_test_IO1(ut);

    ut_report(ut, "Heap");

    ut_delete(&ut);

    return DS_OK;

    error:
    printf("Error at %s\n", __func__);
    ut_report(ut, "Heap");
    ut_delete(&ut);
    return st;
}