/**
 * @file DynamicArray.c
 * 
 * @author Leonardo Vencovsky (https://github.com/LeoVen)
 *
 * @date 01/10/2018
 */

#include "DynamicArray.h"

// NOT EXPOSED API

Status dar_grow(DynamicArray dar);

// END OF NOT EXPOSED API

Status dar_init(DynamicArray *dar)
{
    (*dar) = malloc(sizeof(DynamicArray_t));

    if (!(*dar))
        return DS_ERR_ALLOC;

    (*dar)->buffer = calloc(32, sizeof(int));

    if (!((*dar)->buffer))
        return DS_ERR_ALLOC;

    (*dar)->capacity = 32;
    (*dar)->growth_rate = 200;

    (*dar)->size = 0;

    (*dar)->locked = false;

    return DS_OK;
}

Status dar_create(DynamicArray *dar, index_t initial_capacity, index_t growth_rate)
{
    if (initial_capacity < 8 || growth_rate <= 100)
        return DS_ERR_INVALID_ARGUMENT;

    (*dar) = malloc(sizeof(DynamicArray_t));

    if (!(*dar))
        return DS_ERR_ALLOC;

    (*dar)->buffer = calloc(initial_capacity, sizeof(int));

    if (!((*dar)->buffer))
        return DS_ERR_ALLOC;

    (*dar)->capacity = initial_capacity;
    (*dar)->growth_rate = growth_rate;

    (*dar)->size = 0;

    (*dar)->locked = false;

    return DS_OK;
}

Status dar_make(DynamicArray *dar, int *array, index_t arr_size)
{
    Status st = dar_init(dar);

    if (st != DS_OK)
        return st;

    for (index_t i = 0; i < arr_size; i++)
    {
        st = dar_insert_back((*dar), array[i]);

        if (st != DS_OK)
            return st;
    }

    return DS_OK;
}

Status dar_insert(DynamicArray dar, int *array, index_t arr_size, index_t index)
{
    if (dar == NULL)
        return DS_ERR_NULL_POINTER;

    if (index > dar->size)
        return DS_ERR_OUT_OF_RANGE;

    if (index < 0)
        return DS_ERR_NEGATIVE_VALUE;

    Status st;

    while (!dar_fits(dar, arr_size))
    {
        st = dar_grow(dar);

        if (st != DS_OK)
            return st;
    }

    for (index_t i = dar->size; i > index; i--)
    {
        dar->buffer[i + arr_size - 1] = dar->buffer[i - 1];
    }

    for (index_t i = index, j = 0; j < arr_size; i++, j++)
    {
        dar->buffer[i] = array[j];
    }

    dar->size += arr_size;

    return DS_OK;
}

Status dar_insert_front(DynamicArray dar, int value)
{
    if (dar == NULL)
        return DS_ERR_NULL_POINTER;

    if (dar_full(dar))
    {
        Status st = dar_grow(dar);

        if (st != DS_OK)
            return st;
    }

    for (index_t i = dar->size; i > 0; i--)
    {
        dar->buffer[i] = dar->buffer[i - 1];
    }

    dar->buffer[0] = value;

    (dar->size)++;

    return DS_OK;
}

Status dar_insert_at(DynamicArray dar, int value, index_t index)
{
    if (dar == NULL)
        return DS_ERR_NULL_POINTER;

    if (index > dar->size)
        return DS_ERR_OUT_OF_RANGE;

    if (index < 0)
        return DS_ERR_NEGATIVE_VALUE;

    Status st;

    if (index == 0)
    {
        st = dar_insert_front(dar, value);

        if (st != DS_OK)
            return st;
    }
    else if (index == dar->size)
    {
        st = dar_insert_back(dar, value);

        if (st != DS_OK)
            return st;
    }
    else
    {
        if (dar_full(dar))
        {
            st = dar_grow(dar);

            if (st != DS_OK)
                return st;
        }

        for (index_t i = dar->size; i > index; i--)
        {
            dar->buffer[i] = dar->buffer[i - 1];
        }

        dar->buffer[index] = value;

        (dar->size)++;
    }

    return DS_OK;
}

Status dar_insert_back(DynamicArray dar, int value)
{
    if (dar == NULL)
        return DS_ERR_NULL_POINTER;

    if (dar_full(dar))
    {
        Status st = dar_grow(dar);

        if (st != DS_OK)
            return st;
    }

    dar->buffer[dar->size] = value;

    (dar->size)++;

    return DS_OK;
}

Status dar_remove(DynamicArray dar, index_t from, index_t to)
{
    if (dar == NULL)
        return DS_ERR_NULL_POINTER;

    if (from > to)
        return DS_ERR_INVALID_ARGUMENT;

    if (to >= dar->size)
        return DS_ERR_OUT_OF_RANGE;

    if (dar_empty(dar))
        return DS_ERR_INVALID_OPERATION;

    if (from < 0 || to < 0)
        return DS_ERR_NEGATIVE_VALUE;

    Status st;

    // 'from' and 'to' are inclusive
    index_t distance = to - from + 1;

    if (from == to)
    {
        int result;

        st = dar_remove_at(dar, &result, from);

        if (st != DS_OK)
            return st;

        return DS_OK;
    }
    else if (to == dar->size - 1)
    {
        dar->size -= distance;

        return DS_OK;
    }

    for (index_t i = from, j = to + 1; j <= dar->size - 1; i++, j++)
    {
        dar->buffer[i] = dar->buffer[j];
    }

    dar->size -= distance;

    return DS_OK;
}

Status dar_remove_front(DynamicArray dar, int *result)
{
    if (dar == NULL)
        return DS_ERR_NULL_POINTER;

    if (dar_empty(dar))
        return DS_ERR_INVALID_OPERATION;

    *result = dar->buffer[0];

    for (index_t i = 0; i < dar->size; i++)
    {
        dar->buffer[i] = dar->buffer[i + 1];
    }

    (dar->size)--;

    return DS_OK;
}

Status dar_remove_at(DynamicArray dar, int *result, index_t index)
{
    if (dar == NULL)
        return DS_ERR_NULL_POINTER;

    if (dar_empty(dar))
        return DS_ERR_INVALID_OPERATION;

    if (index >= dar->size)
        return DS_ERR_OUT_OF_RANGE;

    if (index < 0)
        return DS_ERR_NEGATIVE_VALUE;

    Status st;

    if (index == 0)
    {
        st = dar_remove_front(dar, result);

        if (st != DS_OK)
            return st;
    }
    else if (index == dar->size - 1)
    {
        st = dar_remove_back(dar, result);

        if (st != DS_OK)
            return st;
    }
    else
    {
        *result = dar->buffer[index];

        for (index_t i = index; i < dar->size; i++)
        {
            dar->buffer[i] = dar->buffer[i + 1];
        }

        (dar->size)--;
    }

    return DS_OK;
}

Status dar_remove_back(DynamicArray dar, int *result)
{
    if (dar == NULL)
        return DS_ERR_NULL_POINTER;

    if (dar_empty(dar))
        return DS_ERR_INVALID_OPERATION;

    *result = dar->buffer[dar->size - 1];

    (dar->size)--;

    return DS_OK;
}

Status dar_update(DynamicArray dar, int value, index_t index)
{
    if (dar == NULL)
        return DS_ERR_NULL_POINTER;

    if (dar_empty(dar))
        return DS_ERR_INVALID_OPERATION;

    if (index >= dar->size)
        return DS_ERR_OUT_OF_RANGE;

    if (index < 0)
        return DS_ERR_NEGATIVE_VALUE;

    dar->buffer[index] = value;

    return DS_OK;
}

Status dar_get(DynamicArray dar, int *result, index_t index)
{
    *result = 0;

    if (dar == NULL)
        return DS_ERR_NULL_POINTER;

    if (dar_empty(dar))
        return DS_ERR_INVALID_OPERATION;

    if (index >= dar->size)
        return DS_ERR_OUT_OF_RANGE;

    if (index < 0)
        return DS_ERR_NEGATIVE_VALUE;

    *result = dar->buffer[index];

    return DS_OK;
}

Status dar_display(DynamicArray dar)
{
    if (dar == NULL)
        return DS_ERR_NULL_POINTER;

    if (dar_empty(dar))
    {
        printf("\n[ Empty ] \n");

        return DS_OK;
    }

    printf("\nDynamicArray\n[ ");

    for (index_t i = 0; i < dar->size - 1; i++)
    {
        printf("%d, ", dar->buffer[i]);
    }

    printf("%d", dar->buffer[dar->size - 1]);

    printf(" ]\n");

    return DS_OK;
}

Status dar_display_array(DynamicArray dar)
{
    if (dar == NULL)
        return DS_ERR_NULL_POINTER;

    if (dar_empty(dar))
    {
        printf("\n[ empty ] \n");

        return DS_OK;
    }

    printf("\n[ ");

    for (index_t i = 0; i < dar->size - 1; i++)
    {
        printf("%d, ", dar->buffer[i]);
    }

    printf("%d ]\n", dar->buffer[dar->size - 1]);

    return DS_OK;
}

Status dar_display_raw(DynamicArray dar)
{
    if (dar == NULL)
        return DS_ERR_NULL_POINTER;

    printf("\n");

    if (dar_empty(dar))
        return DS_OK;

    for (index_t i = 0; i < dar->size; i++)
    {
        printf("%d ", dar->buffer[i]);
    }

    printf("\n");

    return DS_OK;
}

Status dar_delete(DynamicArray *dar)
{
    if (*dar == NULL)
        return DS_ERR_NULL_POINTER;

    free((*dar)->buffer);
    free((*dar));

    (*dar) = NULL;

    return DS_OK;
}

Status dar_erase(DynamicArray *dar)
{
    if (dar == NULL)
        return DS_ERR_NULL_POINTER;

    Status st = dar_delete(dar);

    if (st != DS_OK)
        return st;

    st = dar_init(dar);

    if (st != DS_OK)
        return st;

    return DS_OK;
}

index_t dar_cap(DynamicArray dar)
{
    if (dar == NULL)
        return -1;

    return dar->capacity;
}

index_t dar_size(DynamicArray dar)
{
    if (dar == NULL)
        return -1;

    return dar->size;
}

bool dar_empty(DynamicArray dar)
{
    return dar->size == 0;
}

bool dar_full(DynamicArray dar)
{
    return dar->size == dar->capacity;
}

bool dar_fits(DynamicArray dar, index_t size)
{
    return (dar->size + size) <= dar->capacity;
}

bool dar_contains(DynamicArray dar, int value)
{
    if (dar == NULL)
        return false;

    if (dar_empty(dar))
        return false;

    for (index_t i = 0; i < dar->size; i++)
    {
        if (dar->buffer[i] == value)
            return true;
    }

    return false;
}

Status dar_copy(DynamicArray dar, DynamicArray *result)
{
    if (dar == NULL)
        return DS_ERR_NULL_POINTER;

    Status st = dar_create(result, dar->capacity, dar->growth_rate);

    if (st != DS_OK)
        return st;

    if (dar_empty(dar))
        return DS_OK;

    for (index_t i = 0; i < dar->size; i++)
    {
        (*result)->buffer[i] = dar->buffer[i];
    }

    (*result)->size = dar->size;
    (*result)->locked = dar->locked;

    return DS_OK;
}

Status dar_prepend(DynamicArray dar1, DynamicArray dar2)
{
    if (dar1 == NULL || dar2 == NULL)
        return DS_ERR_NULL_POINTER;

    if (dar_empty(dar2))
        return DS_OK;

    Status st;

    while (!dar_fits(dar1, dar2->size))
    {
        st = dar_grow(dar1);

        if (st != DS_OK)
            return st;
    }

    for (index_t i = dar1->size; i > 0; i--)
    {
        dar1->buffer[i + dar2->size - 1] = dar1->buffer[i - 1];
    }

    for (index_t i = 0; i < dar2->size; i++)
    {
        dar1->buffer[i] = dar2->buffer[i];
    }

    dar1->size += dar2->size;

    return DS_OK;
}

Status dar_add(DynamicArray dar1, DynamicArray dar2, index_t index)
{
    if (dar1 == NULL || dar2 == NULL)
        return DS_ERR_NULL_POINTER;

    if (index < 0)
        return DS_ERR_NEGATIVE_VALUE;

    if (dar_empty(dar2))
        return DS_OK;

    Status st;

    if (index == dar1->size)
    {
        st = dar_append(dar1, dar2);

        if (st != DS_OK)
            return st;
    }
    else if (index == 0)
    {
        st = dar_prepend(dar1, dar2);

        if (st != DS_OK)
            return st;
    }
    else
    {
        while (!dar_fits(dar1, dar2->size))
        {
            st = dar_grow(dar1);

            if (st != DS_OK)
                return st;
        }

        for (index_t i = dar1->size; i >= index; i--)
        {
            dar1->buffer[i + dar2->size] = dar1->buffer[i];
        }

        for (index_t i = index, j = 0; i < index + dar2->size; i++, j++)
        {
            dar1->buffer[i] = dar2->buffer[j];
        }

        dar1->size += dar2->size;
    }

    return DS_OK;
}

Status dar_append(DynamicArray dar1, DynamicArray dar2)
{
    if (dar1 == NULL || dar2 == NULL)
        return DS_ERR_NULL_POINTER;

    if (dar_empty(dar2))
        return DS_OK;

    Status st;

    while (!dar_fits(dar1, dar2->size))
    {
        st = dar_grow(dar1);

        if (st != DS_OK)
            return st;
    }

    for (index_t i = dar1->size, j = 0; i < dar1->size + dar2->size - 1; i++, j++)
    {
        dar1->buffer[i] = dar2->buffer[j];
    }

    dar1->size += dar2->size;

    return DS_OK;
}

Status dar_cap_lock(DynamicArray dar)
{
    if (dar == NULL)
        return DS_ERR_NULL_POINTER;

    dar->locked = true;

    return DS_OK;
}

Status dar_cap_unlock(DynamicArray dar)
{
    if (dar == NULL)
        return DS_ERR_NULL_POINTER;

    dar->locked = false;

    return DS_OK;
}

// NOT EXPOSED API

Status dar_grow(DynamicArray dar)
{
    if (dar == NULL)
        return DS_ERR_NULL_POINTER;

    if (dar->locked)
        return DS_ERR_FULL;

    index_t old_capacity = dar->capacity;

    dar->capacity = (index_t) ((double) (dar->capacity) * ((double) (dar->growth_rate) / 100.0));

    // 4 is the minimum growth
    if (dar->capacity - old_capacity < 4)
        dar->capacity = old_capacity + 4;

    int *new_buffer = realloc(dar->buffer, sizeof(int) * dar->capacity);

    if (!new_buffer)
    {
        dar->capacity = old_capacity;

        return DS_ERR_ALLOC;
    }

    dar->buffer = new_buffer;

    return DS_OK;
}

// END OF NOT EXPOSED API