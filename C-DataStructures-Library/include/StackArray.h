/**
 * @file StackArray.h
 * 
 * @author Leonardo Vencovsky (https://github.com/LeoVen)
 *
 * @date 03/10/2018
 */

#ifndef C_DATASTRUCTURES_LIBRARY_STACKARRAY_H
#define C_DATASTRUCTURES_LIBRARY_STACKARRAY_H

#include "Core.h"

#define STACK_ARRAY_INIT_SIZE 8
#define STACK_ARRAY_GROW_RATE 2

typedef struct StackArray_s
{
    int *buffer;        /*!< @c StackArray data buffer */
    size_t height;      /*!< @c StackArray buffer size */
    size_t capacity;    /*!< @c StackArray total capacity */
    size_t growth_rate; /*!< @c StackArray buffer growth rate */
} StackArray_t, *StackArray;

Status sta_init(StackArray *sta);

Status sta_push(StackArray sta, int value);

Status sta_pop(StackArray sta, int *result);

Status sta_display(StackArray sta);

Status sta_display_array(StackArray sta);

Status sta_display_raw(StackArray sta);

Status sta_delete(StackArray *sta);

Status sta_erase(StackArray *sta);

int sta_peek(StackArray sta);

size_t sta_height(StackArray sta);

size_t sta_capacity(StackArray sta);

bool sta_empty(StackArray sta);

bool sta_full(StackArray sta);

bool sta_fits(StackArray sta, size_t size);

Status sta_copy(StackArray sta, StackArray *result);

Status sta_realloc(StackArray sta);

#endif //C_DATASTRUCTURES_LIBRARY_STACKARRAY_H
