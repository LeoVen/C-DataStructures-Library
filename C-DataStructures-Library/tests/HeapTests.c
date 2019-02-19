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

// Runs all Heap tests
Status HeapTests(void)
{
    UnitTest ut;

    Status st = ut_init(&ut);

    if (st != DS_OK)
        goto error;

    // Tests go here

    ut_report(ut, "Heap");

    ut_delete(&ut);

    return DS_OK;

    error:
    printf("Error at %s\n", __func__);
    ut_report(ut, "Heap");
    ut_delete(&ut);
    return st;
}