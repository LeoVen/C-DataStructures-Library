/**
 * @file StackArray.c
 * 
 * @author Leonardo Vencovsky (https://github.com/LeoVen)
 *
 * @date 03/10/2018
 */

#include "StackArray.h"

// NOT EXPOSED API

Status sta_grow(StackArray sta);

// END OF NOT EXPOSED API

Status sta_init(StackArray *sta)
{
    (*sta) = malloc(sizeof(StackArray_t));

    if (!(*sta))
        return DS_ERR_ALLOC;

    (*sta)->buffer = malloc(sizeof(int) * 32);

    if (!((*sta)->buffer))
    {
        free(*sta);

        *sta = NULL;

        return DS_ERR_ALLOC;
    }

    (*sta)->capacity = 32;
    (*sta)->growth_rate = 200;

    (*sta)->height = 0;

    (*sta)->locked = false;

    return DS_OK;
}

Status sta_create(StackArray *sta, integer_t initial_capacity, integer_t growth_rate)
{
    if (initial_capacity < 8 || growth_rate <= 100)
        return DS_ERR_INVALID_ARGUMENT;

    (*sta) = malloc(sizeof(StackArray_t));

    if (!(*sta))
        return DS_ERR_ALLOC;

    (*sta)->buffer = malloc(sizeof(int) * initial_capacity);

    if (!((*sta)->buffer))
    {
        free(*sta);

        *sta = NULL;

        return DS_ERR_ALLOC;
    }

    (*sta)->capacity = initial_capacity;
    (*sta)->growth_rate = growth_rate;

    (*sta)->height = 0;

    (*sta)->locked = false;

    return DS_OK;
}

Status sta_push(StackArray sta, int element)
{
    if (sta == NULL)
        return DS_ERR_NULL_POINTER;

    if (sta_full(sta))
    {
        Status st = sta_grow(sta);

        if (st != DS_OK)
            return st;
    }

    sta->buffer[sta->height] = element;

    (sta->height)++;

    return DS_OK;
}

Status sta_pop(StackArray sta, int *result)
{
    *result = 0;

    if (sta == NULL)
        return DS_ERR_NULL_POINTER;

    if (sta_empty(sta))
        return DS_ERR_INVALID_OPERATION;

    *result = sta->buffer[sta->height - 1];

    (sta->height)--;

    return DS_OK;
}

Status sta_insert(StackArray sta, int element)
{
    return sta_push(sta, element);
}

Status sta_remove(StackArray sta, int *result)
{
    return sta_pop(sta, result);
}

Status sta_display(StackArray sta)
{
    if (sta == NULL)
        return DS_ERR_NULL_POINTER;

    if (sta_empty(sta))
    {
        printf("\nStack\n[ empty]\n");

        return DS_OK;
    }

    printf("\nStack");

    for (integer_t i = 0; i < sta->height; i++)
    {
        printf("\n|%10d |", sta->buffer[i]);
    }

    printf("\n");

    return DS_OK;
}

Status sta_display_array(StackArray sta)
{
    if (sta == NULL)
        return DS_ERR_NULL_POINTER;

    if (sta_empty(sta))
    {
        printf("\n[ empty ] \n");

        return DS_OK;
    }

    printf("\n[ ");

    for (integer_t i = 0; i < sta->height - 1; i++)
    {
        printf("%d, ", sta->buffer[i]);
    }

    printf("%d ]\n", sta->buffer[sta->height - 1]);

    return DS_OK;
}

Status sta_display_raw(StackArray sta)
{
    if (sta == NULL)
        return DS_ERR_NULL_POINTER;

    printf("\n");

    if (sta_empty(sta))
        return DS_OK;

    for (integer_t i = 0; i < sta->height; i++)
        printf("%d ", sta->buffer[i]);

    printf("\n");

    return DS_OK;
}

Status sta_delete(StackArray *sta)
{
    if ((*sta) == NULL)
        return DS_ERR_INVALID_OPERATION;

    free((*sta)->buffer);

    free(*sta);

    *sta = NULL;

    return DS_OK;
}

Status sta_erase(StackArray *sta)
{
    if ((*sta) == NULL)
        return DS_ERR_INVALID_OPERATION;

    Status st = sta_delete(sta);

    if (st != DS_OK)
        return st;

    st = sta_init(sta);

    if (st != DS_OK)
        return st;

    return DS_OK;
}

int sta_peek(StackArray sta)
{
    return sta->buffer[sta->height - 1];
}

integer_t sta_height(StackArray sta)
{
    if (sta == NULL)
        return -1;

    return sta->height;
}

integer_t sta_capacity(StackArray sta)
{
    if (sta == NULL)
        return -1;

    return sta->capacity;
}

bool sta_empty(StackArray sta)
{
    return (sta->height == 0);
}

bool sta_full(StackArray sta)
{
    return (sta->height == sta->capacity);
}

bool sta_fits(StackArray sta, integer_t size)
{
    return (sta->height + size) <= sta->capacity;
}

Status sta_copy(StackArray sta, StackArray *result)
{
    if (sta == NULL)
        return DS_ERR_NULL_POINTER;

    Status st = sta_create(result, sta->capacity, sta->growth_rate);

    if (st != DS_OK)
        return st;

    if (sta_empty(sta))
        return DS_OK;

    for (integer_t i = 0; i < sta->height; i++)
    {
        (*result)->buffer[i] = sta->buffer[i];
    }

    (*result)->height = sta->height;

    (*result)->locked = sta->locked;

    return DS_OK;
}

Status sta_cap_lock(StackArray sta)
{
    if (sta == NULL)
        return DS_ERR_NULL_POINTER;

    sta->locked = true;

    return DS_OK;
}

Status sta_cap_unlock(StackArray sta)
{
    if (sta == NULL)
        return DS_ERR_NULL_POINTER;

    sta->locked = false;

    return DS_OK;
}

// NOT EXPOSED API

// This function reallocates the data buffer increasing its capacity
Status sta_grow(StackArray sta)
{
    if (sta == NULL)
        return DS_ERR_NULL_POINTER;

    if (sta->locked)
        return DS_ERR_FULL;

    integer_t old_capacity = sta->capacity;

    // capacity = capacity * (growth_rate / 100)
    sta->capacity = (integer_t) ((double) (sta->capacity) * ((double) (sta->growth_rate) / 100.0));

    // 4 is the minimum growth
    if (sta->capacity - old_capacity < 4)
        sta->capacity = old_capacity + 4;

    int *new_buffer = realloc(sta->buffer, sizeof(int) * sta->capacity);

    // Reallocation failed
    if (!new_buffer)
    {
        sta->capacity = old_capacity;

        return DS_ERR_ALLOC;
    }

    sta->buffer = new_buffer;

    return DS_OK;
}

// END OF NOT EXPOSED API