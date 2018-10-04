/**
 * @file DynamicArray.h
 * 
 * @author Leonardo Vencovsky (https://github.com/LeoVen)
 *
 * @date 01/10/2018
 */

#ifndef C_DATASTRUCTURES_LIBRARY_DYNAMICARRAY_H
#define C_DATASTRUCTURES_LIBRARY_DYNAMICARRAY_H

#define DYNAMIC_ARRAY_INIT_SIZE 8
#define DYNAMIC_ARRAY_GROW_RATE 2

#include "Core.h"

typedef struct DynamicArray
{
    int *buffer;        /*!< @c DynamicArray data buffer */
    size_t size;        /*!< @c DynamicArray size */
    size_t capacity;    /*!< @c DynamicArray total capacity */
    size_t growth_rate; /*!< @c DynamicArray buffer growth rate */
} DynamicArray;

Status dar_init(DynamicArray **dar);

Status dar_make(DynamicArray **dar, int *array, size_t arr_size);

Status dar_insert(DynamicArray *dar, int *array, size_t arr_size, size_t index);

Status dar_insert_front(DynamicArray *dar, int value);

Status dar_insert_at(DynamicArray *dar, int value, size_t index);

Status dar_insert_back(DynamicArray *dar, int value);

Status dar_remove(DynamicArray *dar, size_t from, size_t to);

Status dar_remove_front(DynamicArray *dar);

Status dar_remove_at(DynamicArray *dar, size_t index);

Status dar_remove_back(DynamicArray *dar);

Status dar_update(DynamicArray *dar, int value, size_t index);

Status dar_get(DynamicArray *dar, size_t index, int *result);

Status dar_display(DynamicArray *dar);

Status dar_display_array(DynamicArray *dar);

Status dar_display_raw(DynamicArray *dar);

Status dar_delete(DynamicArray **dar);

Status dar_erase(DynamicArray **dar);

size_t dar_cap(DynamicArray *dar);

size_t dar_size(DynamicArray *dar);

bool dar_empty(DynamicArray *dar);

bool dar_full(DynamicArray *dar);

bool dar_fits(DynamicArray *dar, size_t size);

bool dar_contains(DynamicArray *dar, int value);

Status dar_copy(DynamicArray *dar, DynamicArray **result);

Status dar_prepend(DynamicArray *dar1, DynamicArray *dar2);

Status dar_add(DynamicArray *dar1, DynamicArray *dar2, size_t index);

Status dar_append(DynamicArray *dar1, DynamicArray *dar2);

Status dar_realloc(DynamicArray *dar);

#endif //C_DATASTRUCTURES_LIBRARY_DYNAMICARRAY_H
