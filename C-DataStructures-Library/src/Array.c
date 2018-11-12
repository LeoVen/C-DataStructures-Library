/**
 * @file Array.c
 * 
 * @author Leonardo Vencovsky (https://github.com/LeoVen)
 *
 * @date 07/10/2018
 */

#include "Array.h"

struct Array_s
{

    void **buffer;

    /// \brief Array length.
    ///
    /// Current buffer length.
    index_t length;

    arr_compare_f d_compare;
    arr_copy_f d_copy;
    arr_display_f d_display;
    arr_free_f d_free;
};

///////////////////////////////////////////////////// NOT EXPOSED FUNCTIONS ///

static void arr_quicksort(Array array, void **buffer, index_t length);

////////////////////////////////////////////// END OF NOT EXPOSED FUNCTIONS ///

Status arr_init(Array *array, index_t length)
{
    if (length <= 0)
        return DS_ERR_INVALID_ARGUMENT;

    (*array) = malloc(sizeof(Array_t));

    if (!(*array))
        return DS_ERR_ALLOC;

    (*array)->buffer = malloc(sizeof(int) * length);

    if (!(*array)->buffer)
        return DS_ERR_ALLOC;

    (*array)->d_compare = NULL;
    (*array)->d_copy = NULL;
    (*array)->d_display = NULL;
    (*array)->d_free = NULL;

    for (index_t i = 0; i < length; i++)
        (*array)->buffer[i] = NULL;

    (*array)->length = length;

    return DS_OK;
}

Status arr_create(Array *array, index_t length, arr_compare_f compare_f,
                  arr_copy_f copy_f, arr_display_f display_f, arr_free_f free_f)
{
    if (length <= 0)
        return DS_ERR_INVALID_ARGUMENT;

    (*array) = malloc(sizeof(Array_t));

    if (!(*array))
        return DS_ERR_ALLOC;

    (*array)->buffer = malloc(sizeof(int) * length);

    if (!(*array)->buffer)
        return DS_ERR_ALLOC;

    (*array)->d_compare = compare_f;
    (*array)->d_copy = copy_f;
    (*array)->d_display = display_f;
    (*array)->d_free = free_f;

    for (index_t i = 0; i < length; i++)
        (*array)->buffer[i] = NULL;

    (*array)->length = length;

    return DS_OK;
}

Status arr_free(Array *array)
{
    if (*array == NULL)
        return DS_ERR_NULL_POINTER;

    if ((*array)->d_free == NULL)
        return DS_ERR_INCOMPLETE_TYPE;

    for (index_t i = 0; i < (*array)->length; i++)
    {
        (*array)->d_free((*array)->buffer[i]);
    }

    free((*array)->buffer);
    free((*array));

    (*array) = NULL;

    return DS_OK;
}

Status arr_erase(Array *array)
{
    if (*array == NULL)
        return DS_ERR_NULL_POINTER;

    Array new_array;

    Status st = arr_create(&new_array, (*array)->length, (*array)->d_compare,
            (*array)->d_copy, (*array)->d_display, (*array)->d_free);

    if (st != DS_OK)
        return st;

    st = arr_free(array);

    // Probably didn't set the free function...
    if (st != DS_OK)
    {
        free(new_array->buffer);
        free(new_array);

        return st;
    }

    *array = new_array;

    return DS_OK;
}

Status arr_set_func_compare(Array array, arr_compare_f function)
{
    if (array == NULL)
        return DS_ERR_NULL_POINTER;

    array->d_compare = function;

    return DS_OK;
}

Status arr_set_func_copy(Array array, arr_copy_f function)
{
    if (array == NULL)
        return DS_ERR_NULL_POINTER;

    array->d_copy = function;

    return DS_OK;
}

Status arr_set_func_display(Array array, arr_display_f function)
{
    if (array == NULL)
        return DS_ERR_NULL_POINTER;

    array->d_display = function;

    return DS_OK;
}

Status arr_set_func_free(Array array, arr_free_f function)
{
    if (array == NULL)
        return DS_ERR_NULL_POINTER;

    array->d_free = function;

    return DS_OK;
}

index_t arr_length(Array array)
{
    if (array == NULL)
        return -1;

    return array->length;
}

Status arr_set(Array array, index_t index, void *element)
{
    if (array == NULL)
        return DS_ERR_NULL_POINTER;

    if (index >= array->length)
        return DS_ERR_OUT_OF_RANGE;

    if (index < 0)
        return DS_ERR_NEGATIVE_VALUE;

    if (array->buffer[index] == NULL)
    {
        array->buffer[index] = element;

        return DS_OK;
    }

    return DS_ERR_INVALID_OPERATION;
}

index_t arr_set_next(Array array, void *element)
{
    if (array == NULL)
        return -2;

    for (index_t i = 0; i < array->length; i++)
    {
        if (array->buffer[i] != NULL)
        {
            array->buffer[i] = element;

            return i;
        }
    }

    return -1;
}

index_t arr_set_last(Array array, void *element)
{
    if (array == NULL)
        return -2;

    for (index_t i = array->length - 1; i >= 0; i--)
    {
        if (array->buffer[i] != NULL)
        {
            array->buffer[i] = element;

            return i;
        }
    }

    return -1;
}

void *arr_get(Array array, index_t index)
{
    if (array == NULL)
        return NULL;

    if (index < 0 || index >= array->length)
        return NULL;

    return array->buffer[index];
}

void *arr_get_next(Array array, index_t *index)
{
    *index = -1;

    if (array == NULL)
        return NULL;

    for (index_t i = array->length - 1; i >= 0; i--)
    {
        if (array->buffer[i] != NULL)
        {
            *index = i;

            return array->buffer[i];
        }
    }

    return NULL;
}

void *arr_get_last(Array array, index_t *index)
{
    *index = -1;

    if (array == NULL)
        return NULL;

    for (index_t i = array->length - 1; i >= 0; i--)
    {
        if (array->buffer[i] != NULL)
        {
            *index = i;

            return array->buffer[i];
        }
    }

    return NULL;
}

void *arr_pop(Array array, index_t index)
{
    if (array == NULL)
        return NULL;

    if (index < 0 || index >= array->length)
        return NULL;

    void *element = array->buffer[index];

    array->buffer[index] = NULL;

    return element;
}

void *arr_pop_next(Array array, index_t *index)
{
    *index = -1;

    if (array == NULL)
        return NULL;

    for (index_t i = array->length - 1; i >= 0; i--)
    {
        if (array->buffer[i] != NULL)
        {
            void *element = array->buffer[i];

            array->buffer[i] = NULL;

            *index = i;

            return element;
        }
    }

    return NULL;
}

void *arr_pop_last(Array array, index_t *index)
{
    *index = -1;

    if (array == NULL)
        return NULL;

    for (index_t i = array->length - 1; i >= 0; i--)
    {
        if (array->buffer[i] != NULL)
        {
            void *element = array->buffer[i];

            array->buffer[i] = NULL;

            *index = i;

            return element;
        }
    }

    return NULL;
}

bool arr_full(Array array)
{
    for (index_t i = 0; i < array->length; i++)
    {
        if (array->buffer[i] == NULL)
            return false;
    }

    return true;
}

bool arr_empty(Array array)
{
    for (index_t i = 0; i < array->length; i++)
    {
        if (array->buffer[i] != NULL)
            return false;
    }

    return true;
}

Status arr_sort(Array array)
{
    if (array == NULL)
        return DS_ERR_NULL_POINTER;

    if (array->d_compare == NULL)
        return DS_ERR_INCOMPLETE_TYPE;

    arr_quicksort(array, array->buffer, array->length);

    return DS_OK;
}

Status arr_copy(Array array, Array *result)
{
    if (array == NULL)
        return DS_ERR_NULL_POINTER;

    if (array->d_copy == NULL)
        return DS_ERR_INCOMPLETE_TYPE;

    Status st = arr_create(result, array->length, array->d_compare,
                           array->d_copy, array->d_display, array->d_free);

    if (st != DS_OK)
        return st;

    for (index_t i = 0; i < array->length; i++)
        (*result)->buffer[i] = array->buffer[i] == NULL ? NULL :
                               array->d_copy(array->buffer[i]);

    return DS_OK;
}

Status arr_switch(Array array, index_t pos1, index_t pos2)
{
    if (array == NULL)
        return DS_ERR_NULL_POINTER;

    if (pos1 >= array->length || pos2 >= array->length)
        return DS_ERR_OUT_OF_RANGE;

    if (pos1 < 0 || pos2 < 0)
        return DS_ERR_NEGATIVE_VALUE;

    void *temp = array->buffer[pos1];
    array->buffer[pos1] = array->buffer[pos2];
    array->buffer[pos2] = temp;

    return DS_OK;
}

Status arr_reverse(Array arr)
{
    if (arr == NULL)
        return DS_ERR_NULL_POINTER;

    Status st;

    for (index_t i = 0; i < arr->length / 2; i++)
    {
        st = arr_switch(arr, i, arr->length - i - 1);

        if (st != DS_OK)
            return st;
    }

    return DS_OK;
}

Status arr_display(Array array)
{
    if (array == NULL)
        return DS_ERR_NULL_POINTER;

    if (array->d_display == NULL)
        return DS_ERR_INCOMPLETE_TYPE;

    if (arr_empty(array))
    {
        printf("\nArray\n[ empty ]\n");

        return DS_OK;
    }

    printf("\nArray\n[ ");

    for (index_t i = 0; i < array->length - 1; i++)
    {
        array->d_display(array->buffer[i]);

        printf(", ");
    }

    array->d_display(array->buffer[array->length - 1]);

    printf(" ]\n");

    return DS_OK;
}

Status arr_display_raw(Array array)
{
    if (array == NULL)
        return DS_ERR_NULL_POINTER;

    if (array->d_display == NULL)
        return DS_ERR_INCOMPLETE_TYPE;

    printf("\n");

    for (index_t i = 0; i < array->length - 1; i++)
    {
        array->d_display(array->buffer[i]);

        printf(" ");
    }

    printf("\n");

    return DS_OK;
}

///////////////////////////////////////////////////// NOT EXPOSED FUNCTIONS ///

static void arr_quicksort(Array array, void **buffer, index_t length)
{
    if (length < 2)
        return;

    void *pivot = buffer[length / 2];

    index_t i, j;
    for (i = 0, j = length - 1; ; i++, j--)
    {
        while (array->d_compare(buffer[i], pivot) < 0)
            i++;

        while (array->d_compare(buffer[j], pivot) > 0)
            j--;

        if (i >= j)
            break;

        void *temp = buffer[i];
        buffer[i] = buffer[j];
        buffer[j] = temp;
    }

    arr_quicksort(array, buffer, i);
    arr_quicksort(array, buffer + i, length - i);
}

////////////////////////////////////////////// END OF NOT EXPOSED FUNCTIONS ///

///////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////// Iterator ///
///////////////////////////////////////////////////////////////////////////////
