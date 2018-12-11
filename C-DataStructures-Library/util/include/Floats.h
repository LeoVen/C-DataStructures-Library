/**
 * @file Floats.h
 * 
 * @author Leonardo Vencovsky (https://github.com/LeoVen)
 *
 * @date 05/11/2018
 */

#ifndef C_DATASTRUCTURES_LIBRARY_FLOATS_H
#define C_DATASTRUCTURES_LIBRARY_FLOATS_H

#include "Core.h"

#ifdef __cplusplus
extern "C" {
#endif

void display_float(const void *element);
void display_double(const void *element);
void display_long_double(const void *element);

int compare_float(const void *element1, const void *element2);
int compare_double(const void *element1, const void *element2);
int compare_long_double(const void *element1, const void *element2);

void *copy_float(const void *element);
void *copy_double(const void *element);
void *copy_long_double(const void *element);

void *new_float(float element);
void *new_double(double element);
void *new_long_double(long double element);

#ifdef __cplusplus
}
#endif

#endif //C_DATASTRUCTURES_LIBRARY_FLOATS_H
