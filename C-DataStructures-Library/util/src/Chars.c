/**
 * @file Chars.c
 * 
 * @author Leonardo Vencovsky (https://github.com/LeoVen)
 *
 * @date 05/11/2018
 */

#include "Chars.h"

void display_char(void *element)
{
    char *e = (char*)element;

    printf("%c", *e);
}

void display_string(void *element)
{
    char *e = (char*)element;

    printf("%s", e);
}

int compare_char(void *element1, void *element2)
{
    char *e1 = (char*)element1;
    char *e2 = (char*)element2;

    if (*e1 > *e2)
        return 1;
    else if (*e1 < *e2)
        return -1;

    return 0;
}

int compare_string(void *element1, void *element2)
{
    const char *e1 = (const char*)element1;
    const char *e2 = (const char*)element2;

    return strcmp(e1, e2);
}

char *copy_char(void *element)
{
    char *e = (char*)element;

    return new_char(*e);
}

char *copy_string(void *element);

char *new_char(char element)
{
    char *e = malloc(sizeof(char));

    *e = element;

    return e;
}

char *new_string(const char *element)
{
    return strdup(element);
}