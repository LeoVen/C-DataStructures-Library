/**
 * @file UnitTest.c
 * 
 * @author Leonardo Vencovsky (https://github.com/LeoVen)
 *
 * @date 15/10/2018
 */

#include "UnitTest.h"

Status ut_init(UnitTest *ut)
{
    *ut = malloc(sizeof(UnitTest_t));

    if (!(*ut))
        return DS_ERR_ALLOC;

    (*ut)->total = 0;
    (*ut)->passed = 0;

    return DS_OK;
}

Status ut_delete(UnitTest *ut)
{
    if (*ut == NULL)
        return DS_ERR_NULL_POINTER;

    free(*ut);

    *ut = NULL;

    return DS_OK;
}

void ut_report(UnitTest ut, const char *struct_name)
{
    printf("\n+--------------------------------------------------+");
    printf("\n|                  UnitTest Report                 |");
    printf("\n+--------------------------------------------------+");
    printf("\n  Name   : %s", struct_name);
    printf("\n  Total  : %u", ut->total);
    printf("\n  Passed : %u\n", ut->passed);
}

void ut_equals_int(UnitTest ut, int param1, int param2, const char *test_name)
{
    if (param1 == param2)
    {
        ut->passed++;
        printf("\n%-30s %s", test_name, "PASSED");
    }
    else
    {
        printf("\n%-30s %s", test_name, "FAILED");
    }

    ut->total++;
}

void ut_equals_long_long(UnitTest ut, long long param1, long long param2, const char *test_name)
{
    if (param1 == param2)
    {
        ut->passed++;
        printf("\n%-30s %s", test_name, "PASSED");
    }
    else
    {
        printf("\n%-30s %s", test_name, "FAILED");
    }

    ut->total++;
}

void ut_equals_size_t(UnitTest ut, size_t param1, size_t param2, const char *test_name)
{
    if (param1 == param2)
    {
        ut->passed++;
        printf("\n%-30s %s", test_name, "PASSED");
    }
    else
    {
        printf("\n%-30s %s", test_name, "FAILED");
    }

    ut->total++;
}