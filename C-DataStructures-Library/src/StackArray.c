/**
 * @file StackArray.c
 * 
 * @author Leonardo Vencovsky (https://github.com/LeoVen)
 *
 * @date 03/10/2018
 */

#include "StackArray.h"

Status sta_init(StackArray **sta)
{
    (*sta) = malloc(sizeof(StackArray));

    if (!(*sta))
        return DS_ERR_ALLOC;

    (*sta)->buffer = malloc(sizeof(int) * STACK_ARRAY_INIT_SIZE);

    if (!((*sta)->buffer))
        return DS_ERR_ALLOC;

    (*sta)->capacity = STACK_ARRAY_INIT_SIZE;
    (*sta)->growth_rate = STACK_ARRAY_GROW_RATE;

    (*sta)->height = 0;

    return DS_OK;
}

Status sta_push(StackArray *sta, int value)
{
    if (sta == NULL)
        return DS_ERR_NULL_POINTER;

    if (sta_full(sta))
    {
        Status st = sta_realloc(sta);

        if (st != DS_OK)
            return st;
    }

    sta->buffer[sta->height] = value;

    (sta->height)++;

    return DS_OK;
}

Status sta_pop(StackArray *sta, int *result)
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

Status sta_display(StackArray *sta)
{
    if (sta == NULL)
        return DS_ERR_NULL_POINTER;

    printf("\nStackArray\n");

    if (sta_empty(sta))
    {
        printf("[ empty ]");

        return DS_OK;
    }

    size_t i;
    for (i = 0; i < sta->height; i++)
    {
        printf("\n|%10d |", sta->buffer[i]);
    }

    printf("\n");

    return DS_OK;
}

Status sta_display_array(StackArray *sta)
{
    if (sta == NULL)
        return DS_ERR_NULL_POINTER;

    if (sta_empty(sta))
    {
        printf("\n[ empty ] \n");

        return DS_OK;
    }

    printf("\n[ ");

    for (size_t i = 0; i < sta->height - 1; i++)
    {
        printf("%d, ", sta->buffer[i]);
    }

    printf("%d ]\n", sta->buffer[sta->height - 1]);

    return DS_OK;
}

Status sta_display_raw(StackArray *sta)
{
    if (sta == NULL)
        return DS_ERR_NULL_POINTER;

    printf("\n");

    if (sta_empty(sta))
        return DS_OK;

    size_t i;
    for (i = 0; i < sta->height; i++)
        printf(" %d", sta->buffer[i]);

    printf("\n");

    return DS_OK;
}

Status sta_delete(StackArray **sta)
{
    if ((*sta) == NULL)
        return DS_ERR_INVALID_OPERATION;

    free((*sta)->buffer);

    free(*sta);

    *sta = NULL;

    return DS_OK;
}

Status sta_erase(StackArray **sta)
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

int sta_peek(StackArray *sta)
{
    return sta->buffer[sta->height - 1];
}

size_t sta_height(StackArray *sta)
{
    if (sta == NULL)
        return 0;

    return sta->height;
}

size_t sta_capacity(StackArray *sta)
{
    if (sta == NULL)
        return 0;

    return sta->capacity;
}

bool sta_empty(StackArray *sta)
{
    return (sta->height == 0);
}

bool sta_full(StackArray *sta)
{
    return (sta->height == sta->capacity);
}

bool sta_fits(StackArray *sta, size_t size)
{
    return (sta->height + size) <= sta->capacity;
}

Status sta_copy(StackArray *sta, StackArray **result)
{
    if (sta == NULL)
        return DS_ERR_NULL_POINTER;

    Status st = sta_init(result);

    if (st != DS_OK)
        return st;

    if (sta_empty(sta))
        return DS_OK;

    while (!sta_fits(*result, sta->height))
    {
        st = sta_realloc(*result);

        if (st != DS_OK)
            return st;
    }

    for (size_t i = 0; i < sta->height; i++)
    {
        (*result)->buffer[i] = sta->buffer[i];
    }

    (*result)->height = sta->height;

    return DS_OK;
}

Status sta_realloc(StackArray *sta)
{
    if (sta == NULL)
        return DS_ERR_NULL_POINTER;

    sta->capacity *= sta->growth_rate;

    int *new_buffer = realloc(sta->buffer, sizeof(int) * sta->capacity);

    if (!new_buffer)
    {
        sta->capacity /= sta->growth_rate;

        return DS_ERR_ALLOC;
    }

    sta->buffer = new_buffer;

    return DS_OK;
}
