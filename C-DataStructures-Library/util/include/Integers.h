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

void display_int(void *element);
void display_long(void *element);
void display_long_long(void *element);
void display_unsigned(void *element);
void display_unsigned_long(void *element);
void display_unsigned_long_long(void *element);

int compare_int(void *element1, void *element2);
int compare_long(void *element1, void *element2);
int compare_long_long(void *element1, void *element2);
int compare_unsigned(void *element1, void *element2);
int compare_unsigned_long(void *element1, void *element2);
int compare_unsigned_long_long(void *element1, void *element2);

void *copy_int(void *element);
void *copy_long(void *element);
void *copy_long_long(void *element);
void *copy_unsigned(void *element);
void *copy_unsigned_long(void *element);
void *copy_unsigned_long_long(void *element);

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
