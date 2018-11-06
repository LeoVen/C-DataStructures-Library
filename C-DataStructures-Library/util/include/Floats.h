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

void display_float(void *element);
void display_double(void *element);
void display_long_double(void *element);

int compare_float(void *element1, void *element2);
int compare_double(void *element1, void *element2);
int compare_long_double(void *element1, void *element2);

float *copy_float(void *element);
double *copy_double(void *element);
long double *copy_long_double(void *element);

float *new_float(float element);
double *new_double(double element);
long double *new_long_double(long double element);

#ifdef __cplusplus
}
#endif

#endif //C_DATASTRUCTURES_LIBRARY_FLOATS_H
