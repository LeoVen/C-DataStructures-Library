/**
 * @file Floats.c
 * 
 * @author Leonardo Vencovsky (https://github.com/LeoVen)
 *
 * @date 05/11/2018
 */

#include "Floats.h"

void display_float(const void *element)
{
    const float *e = (const float*)element;

    printf("%f", *e);
}

void display_double(const void *element)
{
    const double *e = (const double*)element;

    printf("%lf", *e);
}

void display_long_double(const void *element)
{
    const long double *e = (const long double*)element;

    printf("%Lf", *e);
}

int compare_float(const void *element1, const void *element2)
{
    const float *e1 = (const float*)element1;
    const float *e2 = (const float*)element2;

    if (*e1 > *e2)
        return 1;
    else if (*e1 < *e2)
        return -1;

    return 0;
}

int compare_double(const void *element1, const void *element2)
{
    const double *e1 = (const double*)element1;
    const double *e2 = (const double*)element2;

    if (*e1 > *e2)
        return 1;
    else if (*e1 < *e2)
        return -1;

    return 0;
}

int compare_long_double(const void *element1, const void *element2)
{
    const long double *e1 = (const long double*)element1;
    const long double *e2 = (const long double*)element2;

    if (*e1 > *e2)
        return 1;
    else if (*e1 < *e2)
        return -1;

    return 0;
}

void *copy_float(const void *element)
{
    const float *e = (const float*)element;

    return new_float(*e);
}

void *copy_double(const void *element)
{
    const double *e = (const double*)element;

    return new_double(*e);
}

void *copy_long_double(const void *element)
{
    const long double *e = (const long double*)element;

    return new_long_double(*e);
}

void *new_float(const float element)
{
    float *e = malloc(sizeof(float));

    *e = element;

    return e;
}

void *new_double(double element)
{
    double *e = malloc(sizeof(double));

    *e = element;

    return e;
}

void *new_long_double(long double element)
{
    long double *e = malloc(sizeof(long double));

    *e = element;

    return e;
}
