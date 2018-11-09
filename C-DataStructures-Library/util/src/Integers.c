/**
 * @file Integers.c
 * 
 * @author Leonardo Vencovsky (https://github.com/LeoVen)
 *
 * @date 01/11/2018
 */

#include "Integers.h"

void display_int(void *element)
{
    int *e = (int*)element;

    printf("%d", *e);
}

void display_long(void *element)
{
    long *e = (long*)element;

    printf("%ld", *e);
}

void display_long_long(void *element)
{
    long long *e = (long long*)(element);

    printf("%lld", *e);
}

void display_unsigned(void *element)
{
    unsigned *e = (unsigned*)element;

    printf("%u", *e);
}


void display_unsigned_long(void *element)
{
    unsigned long *e = (unsigned long*)element;

    printf("%lu", *e);
}

void display_unsigned_long_long(void *element)
{
    unsigned long long *e = (unsigned long long*)element;

    printf("%llu", *e);
}

int compare_int(void *element1, void *element2)
{
    int *e1 = (int*)element1;
    int *e2 = (int*)element2;

    if (*e1 > *e2)
        return 1;
    else if (*e1 < *e2)
        return -1;

    return 0;
}

int compare_long(void *element1, void *element2)
{
    long *e1 = (long*)element1;
    long *e2 = (long*)element2;

    if (*e1 > *e2)
        return 1;
    else if (*e1 < *e2)
        return -1;

    return 0;
}

int compare_long_long(void *element1, void *element2)
{
    long long *e1 = (long long*)element1;
    long long *e2 = (long long*)element2;

    if (*e1 > *e2)
        return 1;
    else if (*e1 < *e2)
        return -1;

    return 0;
}

int compare_unsigned(void *element1, void *element2)
{
    unsigned *e1 = (unsigned*)element1;
    unsigned *e2 = (unsigned*)element2;

    if (*e1 > *e2)
        return 1;
    else if (*e1 < *e2)
        return -1;

    return 0;
}

int compare_unsigned_long(void *element1, void *element2)
{
    unsigned long *e1 = (unsigned long*)element1;
    unsigned long *e2 = (unsigned long*)element2;

    if (*e1 > *e2)
        return 1;
    else if (*e1 < *e2)
        return -1;

    return 0;
}

int compare_unsigned_long_long(void *element1, void *element2)
{
    unsigned long long *e1 = (unsigned long long*)element1;
    unsigned long long *e2 = (unsigned long long*)element2;

    if (*e1 > *e2)
        return 1;
    else if (*e1 < *e2)
        return -1;

    return 0;
}

void *copy_int(void *element)
{
    int *e = (int*)element;

    return new_int(*e);
}

void *copy_long(void *element)
{
    long *e = (long*)element;

    return new_long(*e);
}

void *copy_long_long(void *element)
{
    long long *e = (long long*)element;

    return new_long_long(*e);
}

void *copy_unsigned(void *element)
{
    unsigned *e = (unsigned*)element;

    return new_unsigned(*e);
}

void *copy_unsigned_long(void *element)
{
    unsigned long *e = (unsigned long*)element;

    return new_unsigned_long(*e);
}

void *copy_unsigned_long_long(void *element)
{
    unsigned long long *e = (unsigned long long*)element;

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