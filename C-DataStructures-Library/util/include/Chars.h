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

void display_char(void *element);
void display_string(void *element);

int compare_char(void *element1, void *element2);
int compare_string(void *element1, void *element2);

void *copy_char(void *element);
void *copy_string(void *element);

void *new_char(char element);
void *new_string(const char *element);

#ifdef __cplusplus
}
#endif

#endif //C_DATASTRUCTURES_LIBRARY_CHAR_H