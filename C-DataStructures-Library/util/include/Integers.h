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

int *copy_int(void *element);
long *copy_long(void *element);
long long *copy_long_long(void *element);
unsigned *copy_unsigned(void *element);
unsigned long *copy_unsigned_long(void *element);
unsigned long long *copy_unsigned_long_long(void *element);

int *new_int(int element);
long *new_long(long element);
long long *new_long_long(long long element);
unsigned *new_unsigned(unsigned element);
unsigned long *new_unsigned_long(unsigned long element);
unsigned long long *new_unsigned_long_long(unsigned long long element);

#endif //C_DATASTRUCTURES_LIBRARY_INTEGERS_H
