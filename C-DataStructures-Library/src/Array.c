/**
 * @file Array.c
 * 
 * @author Leonardo Vencovsky (https://github.com/LeoVen)
 *
 * @date 07/10/2018
 */

#include "Array.h"

Status arr_init(Array *arr, size_t size)
{
    if (size == 0)
        return DS_ERR_INVALID_ARGUMENT;

    (*arr) = malloc(sizeof(Array_t));

    if (!(*arr))
        return DS_ERR_ALLOC;

    (*arr)->buffer = calloc(size, sizeof(int));

    if (!(*arr)->buffer)
        return DS_ERR_ALLOC;

    (*arr)->size = size;

    return DS_OK;
}

Status arr_insert(Array arr, size_t index, int value)
{
    if (arr == NULL)
        return DS_ERR_NULL_POINTER;

    if (index >= arr->size)
        return DS_ERR_INVALID_POSITION;

    if (arr->buffer[index] == 0)
    {
        arr->buffer[index] = value;

        return DS_OK;
    }

    return DS_ERR_INVALID_POSITION;
}

Status arr_display(Array arr)
{
    if (arr == NULL)
        return DS_ERR_NULL_POINTER;

    printf("\nArray\n[ ");

    for (size_t i = 0; i < arr->size - 1; i++)
        printf("%d, ", arr->buffer[i]);

    printf("%d ]\n", arr->buffer[arr->size - 1]);

    return DS_OK;
}

Status arr_display_raw(Array arr)
{
    if (arr == NULL)
        return DS_ERR_NULL_POINTER;

    printf("\n");

    for (size_t i = 0; i < arr->size; i++)
        printf("%d ", arr->buffer[i]);

    printf("\n");

    return DS_OK;
}

Status arr_delete(Array *arr)
{
    if (*arr == NULL)
        return DS_ERR_NULL_POINTER;

    free((*arr)->buffer);
    free((*arr));

    (*arr) = NULL;

    return DS_OK;
}

Status arr_erase(Array arr)
{
    if (arr == NULL)
        return DS_ERR_NULL_POINTER;

    for (size_t i = 0; i < arr->size; i++)
        arr->buffer[i] = 0;

    return DS_OK;
}

Status arr_copy(Array arr, Array *result)
{
    if (arr == NULL)
        return DS_ERR_NULL_POINTER;

    Status st = arr_init(result, arr->size);

    if (st != DS_OK)
        return st;

    for (size_t i = 0; i < arr->size; i++)
        (*result)->buffer[i] = arr->buffer[i];

    return DS_OK;
}

Status arr_switch(Array arr, size_t pos1, size_t pos2)
{
    if (arr == NULL)
        return DS_ERR_NULL_POINTER;

    if (pos1 >= arr->size || pos2 >= arr->size)
        return DS_ERR_INVALID_POSITION;

    int temp = arr->buffer[pos1];
    arr->buffer[pos1] = arr->buffer[pos2];
    arr->buffer[pos2] = temp;

    return DS_OK;
}

Status arr_reverse(Array arr)
{
    if (arr == NULL)
        return DS_ERR_NULL_POINTER;

    Status st;

    for (size_t i = 0; i < arr->size / 2; i++)
    {
        st = arr_switch(arr, i, arr->size - i - 1);

        if (st != DS_OK)
            return st;
    }

    return DS_OK;
}
