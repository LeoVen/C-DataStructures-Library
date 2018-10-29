/**
 * @file DequeArray.c
 * 
 * @author Leonardo Vencovsky (https://github.com/LeoVen)
 *
 * @date 29/10/2018
 */

#include "DequeArray.h"

// NOT EXPOSED API

Status dqa_grow(DequeArray dqa);

// END OF NOT EXPOSED API

Status dqa_init(DequeArray *dqa)
{
    (*dqa) = malloc(sizeof(DequeArray_t));

    if (!(*dqa))
        return DS_ERR_ALLOC;

    (*dqa)->buffer = malloc(sizeof(int) * 32);

    if (!((*dqa)->buffer))
        return DS_ERR_ALLOC;

    (*dqa)->capacity = 32;
    (*dqa)->growth_rate = 200;

    (*dqa)->size = 0;

    (*dqa)->front = 0;
    (*dqa)->rear = 0;

    (*dqa)->locked = false;

    return DS_OK;
}

Status dqa_create(DequeArray *dqa, index_t initial_capacity, index_t growth_rate)
{
    if (initial_capacity < 8 || growth_rate <= 100)
        return DS_ERR_INVALID_ARGUMENT;

    (*dqa) = malloc(sizeof(DequeArray_t));

    if (!(*dqa))
        return DS_ERR_ALLOC;

    (*dqa)->buffer = malloc(sizeof(int) * initial_capacity);

    if (!((*dqa)->buffer))
        return DS_ERR_ALLOC;

    (*dqa)->capacity = initial_capacity;
    (*dqa)->growth_rate = growth_rate;

    (*dqa)->size = 0;

    (*dqa)->front = 0;
    (*dqa)->rear = 0;

    (*dqa)->locked = false;

    return DS_OK;
}

Status dqa_enqueue_front(DequeArray dqa, int value)
{
    if (dqa == NULL)
        return DS_ERR_NULL_POINTER;

    if (dqa_full(dqa))
    {
        Status st = dqa_grow(dqa);

        if (st != DS_OK)
            return st;
    }

    dqa->front = (dqa->front == 0) ? dqa->capacity - 1 : dqa->front - 1;

    dqa->buffer[dqa->front] = value;

    (dqa->size)++;

    return DS_OK;
}

Status dqa_enqueue_rear(DequeArray dqa, int value)
{
    if (dqa == NULL)
        return DS_ERR_NULL_POINTER;

    if (dqa_full(dqa))
    {
        Status st = dqa_grow(dqa);

        if (st != DS_OK)
            return st;
    }

    dqa->buffer[dqa->rear] = value;

    dqa->rear = (dqa->rear == dqa->capacity - 1) ? 0 : dqa->rear + 1;

    (dqa->size)++;

    return DS_OK;
}

Status dqa_dequeue_front(DequeArray dqa, int *value)
{
    *value = 0;

    if (dqa == NULL)
        return DS_ERR_NULL_POINTER;

    if (dqa_empty(dqa))
        return DS_ERR_INVALID_OPERATION;

    *value = dqa->buffer[dqa->front];

    dqa->front = (dqa->front == dqa->capacity - 1) ? 0 : dqa->front + 1;

    (dqa->size)--;

    return DS_OK;
}

Status dqa_dequeue_rear(DequeArray dqa, int *value)
{
    *value = 0;

    if (dqa == NULL)
        return DS_ERR_NULL_POINTER;

    if (dqa_empty(dqa))
        return DS_ERR_INVALID_OPERATION;

    dqa->rear = (dqa->rear == 0) ? dqa->capacity - 1 : dqa->rear - 1;

    *value = dqa->buffer[dqa->rear];

    (dqa->size)--;

    return DS_OK;
}

Status dqa_display(DequeArray dqa)
{
    if (dqa == NULL)
        return DS_ERR_NULL_POINTER;

    if (dqa_empty(dqa))
    {
        printf("\nDequeArray\n[ empty ]\n");

        return DS_OK;
    }

    printf("\nDequeArray\nfront <->");

    for (index_t i = dqa->front, j = 0; j < dqa->size; i = (i + 1) % dqa->capacity, j++)
        printf(" %d <->", dqa->buffer[i]);

    printf(" rear\n");

    return DS_OK;
}

Status dqa_display_array(DequeArray dqa)
{
    if (dqa == NULL)
        return DS_ERR_NULL_POINTER;

    if (dqa_empty(dqa))
    {
        printf("\n[ empty ]\n");

        return DS_OK;
    }

    printf("\nDequeArray\n[ ");

    for (index_t i = dqa->front, j = 0; j < dqa->size - 1; i = (i + 1) % dqa->capacity, j++)
        printf("%d, ", dqa->buffer[i]);

    printf("%d ]\n", dqa->buffer[(dqa->rear == 0) ? dqa->capacity - 1 : dqa->rear - 1]);

    return DS_OK;
}

Status dqa_display_raw(DequeArray dqa)
{
    if (dqa == NULL)
        return DS_ERR_NULL_POINTER;

    printf("\n");

    if (dqa_empty(dqa))
        return DS_OK;

    for (index_t i = dqa->front, j = 0; j < dqa->size; i = (i + 1) % dqa->capacity, j++)
        printf("%d ", dqa->buffer[i]);

    printf("\n");

    return DS_OK;
}

Status dqa_delete(DequeArray *dqa)
{
    if ((*dqa) == NULL)
        return DS_ERR_NULL_POINTER;

    free((*dqa)->buffer);

    free(*dqa);

    *dqa = NULL;

    return DS_OK;
}

Status dqa_erase(DequeArray *dqa)
{
    if ((*dqa) == NULL)
        return DS_ERR_NULL_POINTER;

    Status st = dqa_delete(dqa);

    if (st != DS_OK)
        return st;

    st = dqa_init(dqa);

    if (st != DS_OK)
        return st;

    return DS_OK;
}

int dqa_peek_front(DequeArray dqa)
{
    if (dqa == NULL)
        return 0;

    if (dqa_empty(dqa))
        return 0;

    return dqa->buffer[dqa->front];
}

int dqa_peek_rear(DequeArray dqa)
{
    if (dqa == NULL)
        return 0;

    if (dqa_empty(dqa))
        return 0;

    return dqa->buffer[(dqa->rear == 0) ? dqa->capacity - 1 : dqa->rear - 1];
}

index_t dqa_size(DequeArray dqa)
{
    if (dqa == NULL)
        return -1;

    return dqa->size;
}

index_t dqa_capacity(DequeArray dqa)
{
    if (dqa == NULL)
        return -1;

    return dqa->capacity;
}

bool dqa_empty(DequeArray dqa)
{
    return dqa->size == 0;
}

bool dqa_full(DequeArray dqa)
{
    return dqa->size == dqa->capacity;
}

bool dqa_fits(DequeArray dqa, index_t size)
{
    return (dqa->size + size) <= dqa->capacity;
}

Status dqa_copy(DequeArray dqa, DequeArray *result)
{
    if (dqa == NULL)
        return DS_ERR_NULL_POINTER;

    Status st = dqa_create(result, dqa->capacity, dqa->growth_rate);

    if (st != DS_OK)
        return st;

    if (dqa_empty(dqa))
        return DS_OK;

    for (index_t i = 0; i < dqa->capacity; i++)
        (*result)->buffer[i] = dqa->buffer[i];

    (*result)->front = dqa->front;
    (*result)->rear = dqa->rear;

    (*result)->size = dqa->size;

    (*result)->locked = dqa->locked;

    return DS_OK;
}

Status dqa_cap_lock(DequeArray dqa)
{
    if (dqa == NULL)
        return DS_ERR_NULL_POINTER;

    dqa->locked = true;

    return DS_OK;
}

Status dqa_cap_unlock(DequeArray dqa)
{
    if (dqa == NULL)
        return DS_ERR_NULL_POINTER;

    dqa->locked = false;

    return DS_OK;
}

// NOT EXPOSED API

Status dqa_grow(DequeArray dqa)
{
    if (dqa == NULL)
        return DS_ERR_NULL_POINTER;

    if (dqa->locked)
        return DS_ERR_FULL;

    index_t old_capacity = dqa->capacity;

    // capacity = capacity * (growth_rate / 100)
    dqa->capacity = (index_t) ((double) (dqa->capacity) * ((double) (dqa->growth_rate) / 100.0));

    // 4 is the minimum growth
    if (dqa->capacity - old_capacity < 4)
        dqa->capacity = old_capacity + 4;

    int *new_buffer = realloc(dqa->buffer, sizeof(int) * dqa->capacity);

    if (!new_buffer)
    {
        dqa->capacity = old_capacity;

        return DS_ERR_ALLOC;
    }

    dqa->buffer = new_buffer;

    index_t real_rear = (dqa->rear == 0) ? old_capacity - 1 : dqa->rear - 1;

    // Shift elements if the rear index or the front index wrapped around the
    // buffer.
    if (real_rear < dqa->front)
    {
        // When the buffer is full the rear index equals the front index. In
        // order to shift elements effectively this calculates which portion of
        // the buffer has the least amount of elements.
        // Shift the right portion to the end of the buffer if the total
        // elements are less than the left portion; otherwise append the left
        // portion to the right portion.
        if (old_capacity - dqa->front < dqa->rear)
        {
            index_t distance = old_capacity - dqa->front;

            for (index_t i = old_capacity - 1, j = dqa->capacity - 1; i >= dqa->front; i--, j--)
            {
                dqa->buffer[j] = dqa->buffer[i];
            }

            dqa->front = dqa->capacity - distance;
        }
        // If the growth rate is less than 150 the rear index might wrap around the buffer again
        else
        {
            for (index_t i = 0, j = old_capacity; i < dqa->rear; i++, j = (j + 1) % dqa->capacity)
            {
                dqa->buffer[j] = dqa->buffer[i];
            }

            dqa->rear = (old_capacity + dqa->rear) % dqa->capacity;
        }
    }
    // This case only happens when dqa->front == 0 and dqa->rear == 0
    // The rear index has wrapped around but the buffer increased in size
    // allowing the rear index to keep increasing instead of wrapping around.
    // This can be achieved by inserting elements only at one end and never
    // removing elements from the other end.
    else if (dqa->rear == 0)
    {
        dqa->rear = old_capacity;
    }
    // This should never happen. This function should never be called when the
    // buffer is not full (front == rear and size == capacity).
    else
        return DS_ERR_UNEXPECTED_RESULT;

    return DS_OK;
}

// END OF NOT EXPOSED API