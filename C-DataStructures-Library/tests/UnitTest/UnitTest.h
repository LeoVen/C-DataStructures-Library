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

void FinalReport();

Status ut_init(UnitTest *ut);

Status ut_delete(UnitTest *ut);

void ut_report(UnitTest ut, const char *struct_name);

void ut_equals_bool(UnitTest ut, bool param1, bool param2,
        const char *test_name);

void ut_equals_int(UnitTest ut, int param1, int param2, const char *test_name);

void ut_equals_integer_t(UnitTest ut, integer_t param1, integer_t param2,
        const char *test_name);

void ut_equals_unsigned_t(UnitTest ut, unsigned_t param1, unsigned_t param2,
                          const char *test_name);

void ut_equals_double(UnitTest ut, double param1, double param2,
                      const char *test_name);

void ut_error();

#ifdef __cplusplus
}
#endif

#endif //C_DATASTRUCTURES_LIBRARY_UNITTEST_H
