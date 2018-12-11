/**
 * @file Integers.h
 * 
 * @author Leonardo Vencovsky (https://github.com/LeoVen)
 *
 * @date 01/11/2018
 */

#ifndef C_DATASTRUCTURES_LIBRARY_INTEGERS_H
#define C_DATASTRUCTURES_LIBRARY_INTEGERS_H

#include "Core.h"

#ifdef __cplusplus
extern "C" {
#endif

void display_int(const void *element);
void display_long(const void *element);
void display_long_long(const void *element);
void display_unsigned(const void *element);
void display_unsigned_long(const void *element);
void display_unsigned_long_long(const void *element);

int compare_int(const void *element1, const void *element2);
int compare_long(const void *element1, const void *element2);
int compare_long_long(const void *element1, const void *element2);
int compare_unsigned(const void *element1, const void *element2);
int compare_unsigned_long(const void *element1, const void *element2);
int compare_unsigned_long_long(const void *element1, const void *element2);

void *copy_int(const void *element);
void *copy_long(const void *element);
void *copy_long_long(const void *element);
void *copy_unsigned(const void *element);
void *copy_unsigned_long(const void *element);
void *copy_unsigned_long_long(const void *element);

void *new_int(int element);
void *new_long(long element);
void *new_long_long(long long element);
void *new_unsigned(unsigned element);
void *new_unsigned_long(unsigned long element);
void *new_unsigned_long_long(unsigned long long element);

#ifdef __cplusplus
}
#endif

#endif //C_DATASTRUCTURES_LIBRARY_INTEGERS_H
