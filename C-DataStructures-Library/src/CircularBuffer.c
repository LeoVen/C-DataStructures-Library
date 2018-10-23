/**
 * @file CircularBuffer.c
 * 
 * @author Leonardo Vencovsky (https://github.com/LeoVen)
 *
 * @date 08/10/2018
 */

#include "CircularBuffer.h"

// NOT EXPOSED API

Status cbf_wrap(CircularBuffer cbf);

// END OF NOT EXPOSED API

Status cbf_init(CircularBuffer *cbf, index_t size)
{
    if (size < 0)
        return DS_ERR_NEGATIVE_VALUE;

    (*cbf) = malloc(sizeof(CircularBuffer_t));

    if (!(*cbf))
        return DS_ERR_ALLOC;

    (*cbf)->buffer = calloc(size, sizeof(int));

    if (!((*cbf)->buffer))
        return DS_ERR_ALLOC;

    (*cbf)->capacity = size;
    (*cbf)->size = 0;

    (*cbf)->start = 0;
    (*cbf)->end = 0;

    return DS_OK;
}

Status cbf_insert(CircularBuffer cbf, int value)
{
    if (cbf == NULL)
        return DS_ERR_NULL_POINTER;

    if (cbf_full(cbf))
        return DS_ERR_FULL;

    cbf->buffer[cbf->end] = value;

    (cbf->end)++;

    Status st = cbf_wrap(cbf);

    if (st != DS_OK)
        return st;

    (cbf->size)++;

    return DS_OK;
}

Status cbf_remove(CircularBuffer cbf, int *value)
{
    if (cbf == NULL)
        return DS_ERR_NULL_POINTER;

    if (cbf_empty(cbf))
        return DS_ERR_INVALID_OPERATION;

    *value = cbf->buffer[cbf->start];

    cbf->buffer[cbf->start] = 0;

    (cbf->start)++;

    Status st = cbf_wrap(cbf);

    if (st != DS_OK)
        return st;

    (cbf->size)--;

    return DS_OK;
}

Status cbf_display(CircularBuffer cbf)
{
    if (cbf == NULL)
        return DS_ERR_NULL_POINTER;

    if (cbf_empty(cbf))
    {
        printf("\nCircular Buffer\n[ empty ]\n");

        return DS_OK;
    }

    printf("\nCircular Buffer\n[ ");

    for (index_t i = 0; i < cbf->capacity - 1; i++)
        printf("%d, ", cbf->buffer[i]);

    printf("%d ]\n", cbf->buffer[cbf->capacity - 1]);

    return DS_OK;
}

Status cbf_display_array(CircularBuffer cbf)
{
    if (cbf == NULL)
        return DS_ERR_NULL_POINTER;

    if (cbf_empty(cbf))
    {
        printf("\n[ empty ]\n");

        return DS_OK;
    }

    printf("\n[ ");

    for (index_t i = cbf->start, j = 0; j < cbf->size - 1; i++, j++)
    {
        if (i >= cbf->capacity)
            i = 0;

        printf("%d, ", cbf->buffer[i]);
    }

    printf("%d ]\n", cbf->buffer[(cbf->end == 0) ? cbf->capacity - 1 : cbf->end - 1]);

    return DS_OK;
}

Status cbf_display_raw(CircularBuffer cbf)
{
    if (cbf == NULL)
        return DS_ERR_NULL_POINTER;

    printf("\n");

    for (index_t i = 0; i < cbf->capacity; i++)
        printf(" %d", cbf->buffer[i]);

    printf("\n");

    return DS_OK;
}

Status cbf_delete(CircularBuffer *cbf)
{
    if ((*cbf) == NULL)
        return DS_ERR_NULL_POINTER;

    free((*cbf)->buffer);
    free(*cbf);

    *cbf = NULL;

    return DS_OK;
}

Status cbf_erase(CircularBuffer *cbf)
{
    if (cbf == NULL)
        return DS_ERR_NULL_POINTER;

    index_t size = (*cbf)->capacity;

    Status st = cbf_delete(cbf);

    if (st != DS_OK)
        return st;

    st = cbf_init(cbf, size);

    if (st != DS_OK)
        return st;

    return DS_OK;
}

index_t cbf_size(CircularBuffer cbf)
{
    if (cbf == NULL)
        return -1;

    return cbf->size;
}

index_t cbf_capacity(CircularBuffer cbf)
{
    if (cbf == NULL)
        return -1;

    return cbf->capacity;
}

bool cbf_empty(CircularBuffer cbf)
{
    return cbf->size == 0;
}

bool cbf_full(CircularBuffer cbf)
{
    return cbf->size == cbf->capacity;
}

Status cbf_copy(CircularBuffer cbf, CircularBuffer *result)
{
    if (cbf == NULL)
        return DS_ERR_NULL_POINTER;

    Status st = cbf_init(result, cbf->capacity);

    if (st != DS_OK)
        return st;

    for (index_t i = 0; i < cbf->capacity; i++)
        (*result)->buffer[i] = cbf->buffer[i];

    (*result)->size = cbf->size;

    (*result)->start = cbf->start;
    (*result)->end = cbf->end;

    return DS_OK;
}

// NOT EXPOSED API

Status cbf_wrap(CircularBuffer cbf)
{
    if (cbf == NULL)
        return DS_ERR_NULL_POINTER;

    if (cbf->start >= cbf->capacity)
        cbf->start = 0;

    if (cbf->end >= cbf->capacity)
        cbf->end = 0;

    return DS_OK;
}

// END OF NOT EXPOSED API
