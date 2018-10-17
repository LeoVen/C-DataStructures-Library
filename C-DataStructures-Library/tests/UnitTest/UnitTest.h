/**
 * @file UnitTest.h
 * 
 * @author Leonardo Vencovsky (https://github.com/LeoVen)
 *
 * @date 15/10/2018
 */

#ifndef C_DATASTRUCTURES_LIBRARY_UNITTEST_H
#define C_DATASTRUCTURES_LIBRARY_UNITTEST_H

#include "Core.h"

typedef struct UnitTest_s
{
    size_t passed, total;
} UnitTest_t, *UnitTest;

Status ut_init(UnitTest *ut);

Status ut_delete(UnitTest *ut);

void ut_equals_int(UnitTest ut, int param1, int param2, const char *test_name);

void ut_equals_size_t(UnitTest ut, size_t param1, size_t param2, const char *test_name);

#endif //C_DATASTRUCTURES_LIBRARY_UNITTEST_H
