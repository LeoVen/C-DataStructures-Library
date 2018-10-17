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