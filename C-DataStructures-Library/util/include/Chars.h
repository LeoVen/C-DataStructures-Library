/**
 * @file Chars.h
 * 
 * @author Leonardo Vencovsky (https://github.com/LeoVen)
 *
 * @date 05/11/2018
 */

#ifndef C_DATASTRUCTURES_LIBRARY_CHAR_H
#define C_DATASTRUCTURES_LIBRARY_CHAR_H

#include "Core.h"
#include "string.h"

#ifdef __cplusplus
extern "C" {
#endif

void display_char(const void *element);
void display_string(const void *element);

int compare_char(const void *element1, const void *element2);
int compare_string(const void *element1, const void *element2);

void *copy_char(const void *element);
void *copy_string(const void *element);

void *new_char(char element);
void *new_string(const char *element);

#ifdef __cplusplus
}
#endif

#endif //C_DATASTRUCTURES_LIBRARY_CHAR_H
