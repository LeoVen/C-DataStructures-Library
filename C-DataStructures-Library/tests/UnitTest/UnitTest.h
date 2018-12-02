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

#ifdef __cplusplus
extern "C" {
#endif

typedef struct UnitTest_s
{
    integer_t passed, total;
} UnitTest_t, *UnitTest;

Status ut_init(UnitTest *ut);

Status ut_delete(UnitTest *ut);

void ut_report(UnitTest ut, const char *struct_name);

void ut_equals_int(UnitTest ut, int param1, int param2, const char *test_name);

void ut_equals_integer_t(UnitTest ut, integer_t param1, integer_t param2, const char *test_name);

#ifdef __cplusplus
}
#endif

#endif //C_DATASTRUCTURES_LIBRARY_UNITTEST_H
