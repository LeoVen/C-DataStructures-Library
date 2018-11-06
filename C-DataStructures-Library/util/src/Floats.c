/**
 * @file Floats.c
 * 
 * @author Leonardo Vencovsky (https://github.com/LeoVen)
 *
 * @date 05/11/2018
 */

#include "Floats.h"

void display_float(void *element)
{
    float *e = (float*)element;

    printf("%f", *e);
}

void display_double(void *element)
{
    double *e = (double*)element;

    printf("%lf", *e);
}

void display_long_double(void *element)
{
    long double *e = (long double*)element;

    printf("%Lf", *e);
}

int compare_float(void *element1, void *element2)
{
    float *e1 = (float*)element1;
    float *e2 = (float*)element2;

    if (*e1 > *e2)
        return 1;
    else if (*e1 < *e2)
        return -1;

    return 0;
}

int compare_double(void *element1, void *element2)
{
    double *e1 = (double*)element1;
    double *e2 = (double*)element2;

    if (*e1 > *e2)
        return 1;
    else if (*e1 < *e2)
        return -1;

    return 0;
}

int compare_long_double(void *element1, void *element2)
{
    long double *e1 = (long double*)element1;
    long double *e2 = (long double*)element2;

    if (*e1 > *e2)
        return 1;
    else if (*e1 < *e2)
        return -1;

    return 0;
}

float *copy_float(void *element)
{
    float *e = (float*)element;

    return new_float(*e);
}

double *copy_double(void *element)
{
    double *e = (double*)element;

    return new_double(*e);
}

long double *copy_long_double(void *element)
{
    long double *e = (long double*)element;

    return new_long_double(*e);
}

float *new_float(float element)
{
    float *e = malloc(sizeof(float));

    *e = element;

    return e;
}

double *new_double(double element)
{
    double *e = malloc(sizeof(double));

    *e = element;

    return e;
}

long double *new_long_double(long double element)
{
    long double *e = malloc(sizeof(long double));

    *e = element;

    return e;
}
