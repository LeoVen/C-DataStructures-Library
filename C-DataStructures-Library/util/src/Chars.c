/**
 * @file Chars.c
 * 
 * @author Leonardo Vencovsky (https://github.com/LeoVen)
 *
 * @date 05/11/2018
 */

#include "Chars.h"

void display_char(const void *element)
{
    const char *e = (const char*)element;

    printf("%c", *e);
}

void display_string(const void *element)
{
    const char *e = (const char*)element;

    printf("%s", e);
}

int compare_char(const void *element1, const void *element2)
{
    const char *e1 = (const char*)element1;
    const char *e2 = (const char*)element2;

    if (*e1 > *e2)
        return 1;
    else if (*e1 < *e2)
        return -1;

    return 0;
}

int compare_string(const void *element1, const void *element2)
{
    const char *e1 = (const char*)element1;
    const char *e2 = (const char*)element2;

    return strcmp(e1, e2);
}

void *copy_char(const void *element)
{
    const char *e = (const char*)element;

    return new_char(*e);
}

void *copy_string(const void *element)
{
    return strdup(element);
}

void *new_char(char element)
{
    char *e = malloc(sizeof(char));

    *e = element;

    return e;
}

void *new_string(const char *element)
{
    return strdup(element);
}