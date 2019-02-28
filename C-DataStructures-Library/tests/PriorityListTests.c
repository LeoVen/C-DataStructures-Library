/**
 * @file PriorityListTests.c
 * 
 * @author Leonardo Vencovsky (https://github.com/LeoVen)
 *
 * @date 26/02/2019
 */

#include "PriorityList.h"
#include "UnitTest.h"
#include "Utility.h"

// Quick function to test if a number is prime
static bool
pli_test_isprime(int32_t n)
{
    if (n <= 1)
        return false;
    if (n <= 3)
        return true;

    if (n % 2 == 0 || n % 3 == 0)
        return false;

    for (int32_t i = 5; i * i <= n; i += 6)
        if (n % i == 0 || n % (i + 2) == 0)
            return false;

    return true;
}

// Prime > odd > even
static int
pli_test_pri1(const void *e1, const void *e2)
{
    int32_t *i1 = (int32_t*)e1;
    int32_t *i2 = (int32_t*)e2;

    bool isp1 = pli_test_isprime(*i1);
    bool isp2 = pli_test_isprime(*i2);

    if (isp1 && !isp2)
        return 1;
    else if (!isp1 && isp2)
        return -1;
    else
    {
        isp1 = *i1 % 2 == 0;
        isp2 = *i2 % 2 == 0;

        if (!isp1 && isp2)
            return 1;
        else if (isp1 && !isp2)
            return -1;

        return *i1 - *i2;
    }
}

void pli_test_IO0(UnitTest ut)
{
    Interface int_interface = interface_new(compare_int32_t, copy_int32_t,
                                            display_int32_t, free, NULL,
                                            pli_test_pri1);

    PriorityList_t *list = pli_new(int_interface);

    if (!int_interface || !list)
        goto error;

    int numbers[50];

    for (int i = 0; i < 50; i++)
        numbers[i] = (i + 1) % 31;

    for (int i = 0; i < 50; i++)
    {
        if (!pli_insert(list, &numbers[i]))
            goto error;
    }

    ut_equals_integer_t(ut, 50, pli_count(list), __func__);

    // Check if every item comes out according to the priority function
    void *prev, *curr;
    bool failed = false;
    if (!pli_remove(list, &prev))
        goto error;

    while (!pli_empty(list))
    {
        if (!pli_remove(list, &curr))
            goto error;

        // An element removed first should not have a priority less than the
        // next element removed
        if (int_interface->priority(prev, curr) < 0)
        {
            failed = true;
            break;
        }
    }

    ut_equals_bool(ut, false, failed, __func__);

    pli_free_shallow(list);
    interface_free(int_interface);

    return;

    error:
    printf("Error at %s\n", __func__);
    ut_error();
    pli_free(list);
    interface_free(int_interface);
}

void pli_test_limit(UnitTest ut)
{
    Interface int_interface = interface_new(compare_int32_t, copy_int32_t,
                                            display_int32_t, free, NULL,
                                            pli_test_pri1);

    PriorityList_t *list = pli_new(int_interface);

    if (!list || !int_interface)
        goto error;

    if (!pli_set_limit(list, 10))
        goto error;

    int *elem = NULL;
    for (int i = 0; i < 20; i++)
    {
        elem = new_int32_t(i);

        if (!pli_insert(list, elem))
        {
            free(elem);
        }
    }

    ut_equals_integer_t(ut, pli_count(list), pli_limit(list), __func__);
    ut_equals_bool(ut, pli_set_limit(list, 9), false, __func__);

    // This gets freed later
    int *t = new_int32_t(1);

    ut_equals_int(ut, pli_insert(list, t), false, __func__);

    // Removes the limit
    ut_equals_int(ut, pli_set_limit(list, 0), true, __func__);
    ut_equals_integer_t(ut, pli_limit(list), 0, __func__);
    ut_equals_int(ut, pli_insert(list, t), true, __func__);

    pli_free(list);
    interface_free(int_interface);

    return;

    error:
    printf("Error at %s\n", __func__);
    ut_error();
    pli_free(list);
    interface_free(int_interface);
}

// Runs all PriorityList tests
Status PriorityListTests(void)
{
    UnitTest ut;

    Status st = ut_init(&ut);

    if (st != DS_OK)
        goto error;

    pli_test_IO0(ut);
    pli_test_limit(ut);

    ut_report(ut, "PriorityList");

    ut_delete(&ut);

    return DS_OK;

    error:
    printf("Error at %s\n", __func__);
    ut_report(ut, "PriorityList");
    ut_delete(&ut);
    return st;
}