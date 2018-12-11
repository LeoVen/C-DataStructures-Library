/**
 * @file Integers.c
 * 
 * @author Leonardo Vencovsky (https://github.com/LeoVen)
 *
 * @date 01/11/2018
 */

#include "Integers.h"

void display_int(const void *element)
{
    const int *e = (const int*)element;

    printf("%d", *e);
}

void display_long(const void *element)
{
    const long *e = (const long*)element;

    printf("%ld", *e);
}

void display_long_long(const void *element)
{
    const long long *e = (const long long*)(element);

    printf("%lld", *e);
}

void display_unsigned(const void *element)
{
    const unsigned *e = (const unsigned*)element;

    printf("%u", *e);
}


void display_unsigned_long(const void *element)
{
    const unsigned long *e = (const unsigned long*)element;

    printf("%lu", *e);
}

void display_unsigned_long_long(const void *element)
{
    const unsigned long long *e = (const unsigned long long*)element;

    printf("%llu", *e);
}

int compare_int(const void *element1,const  void *element2)
{
    const int *e1 = (int*)element1;
    const int *e2 = (int*)element2;

    if (*e1 > *e2)
        return 1;
    else if (*e1 < *e2)
        return -1;

    return 0;
}

int compare_long(const void *element1, const void *element2)
{
    const long *e1 = (const long*)element1;
    const long *e2 = (const long*)element2;

    if (*e1 > *e2)
        return 1;
    else if (*e1 < *e2)
        return -1;

    return 0;
}

int compare_long_long(const void *element1, const void *element2)
{
    const long long *e1 = (const long long*)element1;
    const long long *e2 = (const long long*)element2;

    if (*e1 > *e2)
        return 1;
    else if (*e1 < *e2)
        return -1;

    return 0;
}

int compare_unsigned(const void *element1, const void *element2)
{
    const unsigned *e1 = (const unsigned*)element1;
    const unsigned *e2 = (const unsigned*)element2;

    if (*e1 > *e2)
        return 1;
    else if (*e1 < *e2)
        return -1;

    return 0;
}

int compare_unsigned_long(const void *element1, const void *element2)
{
    const unsigned long *e1 = (const unsigned long*)element1;
    const unsigned long *e2 = (const unsigned long*)element2;

    if (*e1 > *e2)
        return 1;
    else if (*e1 < *e2)
        return -1;

    return 0;
}

int compare_unsigned_long_long(const void *element1, const void *element2)
{
    const unsigned long long *e1 = (const unsigned long long*)element1;
    const unsigned long long *e2 = (const unsigned long long*)element2;

    if (*e1 > *e2)
        return 1;
    else if (*e1 < *e2)
        return -1;

    return 0;
}

void *copy_int(const void *element)
{
    const int *e = (const int*)element;

    return new_int(*e);
}

void *copy_long(const void *element)
{
    const long *e = (const long*)element;

    return new_long(*e);
}

void *copy_long_long(const void *element)
{
    const long long *e = (const long long*)element;

    return new_long_long(*e);
}

void *copy_unsigned(const void *element)
{
    const unsigned *e = (const unsigned*)element;

    return new_unsigned(*e);
}

void *copy_unsigned_long(const void *element)
{
    const unsigned long *e = (const unsigned long*)element;

    return new_unsigned_long(*e);
}

void *copy_unsigned_long_long(const void *element)
{
    const unsigned long long *e = (const unsigned long long*)element;

    return new_unsigned_long_long(*e);
}

void *new_int(int element)
{
    int *e = malloc(sizeof(int));

    *e = element;

    return e;
}

void *new_long(long element)
{
    long *e = malloc(sizeof(long));

    *e = element;

    return e;
}

void *new_long_long(long long element)
{
    long long *e = malloc(sizeof(long long));

    *e = element;

    return e;
}

void *new_unsigned(unsigned element)
{
    unsigned *e = malloc(sizeof(unsigned));

    *e = element;

    return e;
}

void *new_unsigned_long(unsigned long element)
{
    unsigned long *e = malloc(sizeof(unsigned long));

    *e = element;

    return e;
}

void *new_unsigned_long_long(unsigned long long element)
{
    unsigned long long *e = malloc(sizeof(unsigned long long));

    *e = element;

    return e;
}