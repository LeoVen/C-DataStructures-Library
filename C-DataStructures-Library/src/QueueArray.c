/**
 * @file QueueArray.c
 * 
 * @author Leonardo Vencovsky (https://github.com/LeoVen)
 *
 * @date 04/10/2018
 */

#include "QueueArray.h"

// NOT EXPOSED API

Status qua_grow(QueueArray qua);

// END OF NOT EXPOSED API

Status qua_init(QueueArray *qua)
{
    (*qua) = malloc(sizeof(QueueArray_t));

    if (!(*qua))
        return DS_ERR_ALLOC;

    (*qua)->buffer = malloc(sizeof(int) * 32);

    if (!((*qua)->buffer))
        return DS_ERR_ALLOC;

    (*qua)->capacity = 32;
    (*qua)->growth_rate = 200;

    (*qua)->size = 0;

    (*qua)->front = 0;
    (*qua)->rear = 0;

    (*qua)->locked = false;

    return DS_OK;
}

Status qua_create(QueueArray *qua, index_t initial_capacity, index_t growth_rate)
{
    if (initial_capacity < 8 || growth_rate <= 100)
        return DS_ERR_INVALID_ARGUMENT;

    (*qua) = malloc(sizeof(QueueArray_t));

    if (!(*qua))
        return DS_ERR_ALLOC;

    (*qua)->buffer = malloc(sizeof(int) * initial_capacity);

    if (!((*qua)->buffer))
        return DS_ERR_ALLOC;

    (*qua)->capacity = initial_capacity;
    (*qua)->growth_rate = growth_rate;

    (*qua)->size = 0;

    (*qua)->front = 0;
    (*qua)->rear = 0;

    (*qua)->locked = false;

    return DS_OK;
}

Status qua_enqueue(QueueArray qua, int element)
{
    if (qua == NULL)
        return DS_ERR_NULL_POINTER;

    if (qua_full(qua))
    {
        Status st = qua_grow(qua);

        if (st != DS_OK)
            return st;
    }

    qua->buffer[qua->rear] = element;

    qua->rear = (qua->rear == qua->capacity - 1) ? 0 : qua->rear + 1;

    (qua->size)++;

    return DS_OK;
}

Status qua_dequeue(QueueArray qua, int *result)
{
    *result = 0;

    if (qua == NULL)
        return DS_ERR_NULL_POINTER;

    if (qua_empty(qua))
        return DS_ERR_INVALID_OPERATION;

    *result = qua->buffer[qua->front];

    qua->front = (qua->front == qua->capacity - 1) ? 0 : qua->front + 1;

    (qua->size)--;

    return DS_OK;
}

Status qua_display(QueueArray qua)
{
    if (qua == NULL)
        return DS_ERR_NULL_POINTER;

    if (qua_empty(qua))
    {
        printf("\nQueueArray\n[ empty ]\n");

        return DS_OK;
    }

    printf("\nQueueArray\nfront <-");

    index_t i;
    for (i = qua->front; i < qua->rear; i++)
        printf(" %d <-", qua->buffer[i]);

    printf(" rear\n");

    return DS_OK;
}

Status qua_display_array(QueueArray qua)
{
    if (qua == NULL)
        return DS_ERR_NULL_POINTER;

    if (qua_empty(qua))
    {
        printf("\n[ empty ] \n");

        return DS_OK;
    }

    printf("\n[ ");

    index_t i, j;
    for (i = qua->front, j = 0; j < qua->size - 1; i = (i + 1) % qua->capacity, j++)
    {
        printf("%d, ", qua->buffer[i]);
    }

    index_t real_rear = (qua->rear == 0) ? qua->capacity - 1 : qua->rear - 1;

    printf("%d ]\n", qua->buffer[real_rear]);

    return DS_OK;
}

Status qua_display_raw(QueueArray qua)
{
    if (qua == NULL)
        return DS_ERR_NULL_POINTER;

    printf("\n");

    if (qua_empty(qua))
        return DS_OK;

    for (index_t i = qua->front; i < qua->rear; i++)
        printf("%d ", qua->buffer[i]);

    printf("\n");

    return DS_OK;
}

Status qua_delete(QueueArray *qua)
{
    if ((*qua) == NULL)
        return DS_ERR_NULL_POINTER;

    free((*qua)->buffer);

    free(*qua);

    *qua = NULL;

    return DS_OK;
}

Status qua_erase(QueueArray *qua)
{
    if ((*qua) == NULL)
        return DS_ERR_NULL_POINTER;

    Status st = qua_delete(qua);

    if (st != DS_OK)
        return st;

    st = qua_init(qua);

    if (st != DS_OK)
        return st;

    return DS_OK;
}

int qua_peek_front(QueueArray qua)
{
    if (qua == NULL)
        return 0;

    if (qua_empty(qua))
        return 0;

    return qua->buffer[qua->front];
}

int qua_peek_rear(QueueArray qua)
{
    if (qua == NULL)
        return 0;

    if (qua_empty(qua))
        return 0;

    return qua->buffer[(qua->rear == 0) ? qua->capacity - 1 : qua->rear - 1];
}

index_t qua_size(QueueArray qua)
{
    if (qua == NULL)
        return -1;

    return qua->size;
}

index_t qua_capacity(QueueArray qua)
{
    if (qua == NULL)
        return -1;

    return qua->capacity;
}

bool qua_empty(QueueArray qua)
{
    return qua->size == 0;
}

bool qua_full(QueueArray qua)
{
    return qua->size == qua->capacity;
}

bool qua_fits(QueueArray qua, index_t size)
{
    return (qua->size + size) <= qua->capacity;
}

Status qua_copy(QueueArray qua, QueueArray *result)
{
    if (qua == NULL)
        return DS_ERR_NULL_POINTER;

    Status st = qua_create(result, qua->capacity, qua->growth_rate);

    if (st != DS_OK)
        return st;

    if (qua_empty(qua))
        return DS_OK;

    for (index_t i = 0; i < qua->capacity; i++)
    {
        (*result)->buffer[i] = qua->buffer[i];
    }

    (*result)->front = qua->front;
    (*result)->rear = qua->rear;

    (*result)->size = qua->size;

    (*result)->locked = qua->locked;

    return DS_OK;
}

Status qua_cap_lock(QueueArray qua)
{
    if (qua == NULL)
        return DS_ERR_NULL_POINTER;

    qua->locked = true;

    return DS_OK;
}

Status qua_cap_unlock(QueueArray qua)
{
    if (qua == NULL)
        return DS_ERR_NULL_POINTER;

    qua->locked = false;

    return DS_OK;
}

// NOT EXPOSED API

// This function reallocates the data buffer effectively increasing its capacity
Status qua_grow(QueueArray qua)
{
    if (qua == NULL)
        return DS_ERR_NULL_POINTER;

    if (qua->locked)
        return DS_ERR_FULL;

    index_t old_capacity = qua->capacity;

    qua->capacity = (index_t)((double)(qua->capacity) * ((double)(qua->growth_rate) / 100.0));

    // 4 is the minimum growth
    if (qua->capacity - old_capacity < 4)
        qua->capacity = old_capacity + 4;

    int *new_buffer = realloc(qua->buffer, sizeof(int) * qua->capacity);

    // Reallocation failed
    if (!new_buffer)
    {
        qua->capacity = old_capacity;

        return DS_ERR_ALLOC;
    }

    qua->buffer = new_buffer;

    index_t real_rear = (qua->rear == 0) ? old_capacity - 1 : qua->rear - 1;

    // Shift elements if the rear index wrapped around the buffer
    if (real_rear < qua->front)
    {
        // When the buffer is full the rear index equals the front index. In
        // order to shift elements effectively this calculates which portion of
        // the buffer has the least amount of elements.
        // Shift the right portion to the end of the buffer if the total
        // elements are less than the left portion; otherwise append the left
        // portion to the right portion.
        if (old_capacity - qua->front < qua->rear)
        {
            index_t distance = old_capacity - qua->front;

            for (index_t i = old_capacity - 1, j = qua->capacity - 1; i >= qua->front; i--, j--)
            {
                qua->buffer[j] = qua->buffer[i];
            }

            qua->front = qua->capacity - distance;
        }
        // If the growth rate is less than 150 the rear index might wrap around the buffer again
        else
        {
            for (index_t i = 0, j = old_capacity; i < qua->rear; i++, j = (j + 1) % qua->capacity)
            {
                qua->buffer[j] = qua->buffer[i];
            }

            qua->rear = (old_capacity + qua->rear) % qua->capacity;
        }
    }
    // This case only happens when qua->front == 0 and qua->rear == 0
    // The rear index has wrapped around but the buffer increased in size
    // allowing the rear index to keep increasing instead of wrapping around
    else if (qua->rear == 0)
    {
        qua->rear = old_capacity;
    }
    // This should never happen. This function should never be called when the
    // buffer is not full (front == rear and size == capacity).
    else
        return DS_ERR_UNEXPECTED_RESULT;

    return DS_OK;
}

// END OF NOT EXPOSED API