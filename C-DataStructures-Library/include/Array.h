/**
 * @file Array.h
 * 
 * @author Leonardo Vencovsky (https://github.com/LeoVen)
 *
 * @date 07/10/2018
 */

#ifndef C_DATASTRUCTURES_LIBRARY_ARRAY_H
#define C_DATASTRUCTURES_LIBRARY_ARRAY_H

#include "Core.h"

typedef struct Array_s
{
    int *buffer;    /*!< Buffer */
    size_t size;    /*!< Buffer size */
} Array_t, *Array;

Status arr_init(Array *arr, size_t size);

Status arr_insert(Array arr, size_t index, int value);

Status arr_display(Array arr);

Status arr_display_raw(Array arr);

Status arr_delete(Array *arr);

Status arr_erase(Array arr);

Status arr_copy(Array arr, Array *result);

Status arr_switch(Array arr, size_t pos1, size_t pos2);

Status arr_reverse(Array arr);

#endif //C_DATASTRUCTURES_LIBRARY_ARRAY_H
