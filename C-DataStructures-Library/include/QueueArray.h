/**
 * @file QueueArray.h
 * 
 * @author Leonardo Vencovsky (https://github.com/LeoVen)
 *
 * @date 04/10/2018
 */

#ifndef C_DATASTRUCTURES_LIBRARY_QUEUEARRAY_H
#define C_DATASTRUCTURES_LIBRARY_QUEUEARRAY_H

#include "Core.h"

#define QUEUE_ARRAY_INIT_SIZE 8
#define QUEUE_ARRAY_GROW_RATE 2

typedef struct QueueArray
{
    int *buffer;        /*!< @c QueueArray data buffer */
    size_t front;       /*!< Front of the queue where elements are removed */
    size_t rear;        /*!< Back of the queue where elements are inserted */
    size_t length;      /*!< @c QueueArray length */
    size_t capacity;    /*!< @c QueueArray total capacity */
    size_t growth_rate; /*!< @c QueueArray buffer growth rate */
} QueueArray;

Status qua_init(QueueArray **qua);

Status qua_enqueue(QueueArray *qua, int value);

Status qua_dequeue(QueueArray *qua, int *value);

Status qua_display(QueueArray *qua);

Status qua_display_array(QueueArray *qua);

Status qua_display_raw(QueueArray *qua);

Status qua_delete(QueueArray **qua);

Status qua_erase(QueueArray **qua);

int qua_peek_front(QueueArray *qua);

int qua_peek_rear(QueueArray *qua);

size_t qua_length(QueueArray *qua);

size_t qua_capacity(QueueArray *qua);

bool qua_empty(QueueArray *qua);

bool qua_full(QueueArray *qua);

bool qua_fits(QueueArray *qua, size_t size);

Status qua_copy(QueueArray *qua, QueueArray **result);

Status qua_realloc(QueueArray *qua);

#endif //C_DATASTRUCTURES_LIBRARY_QUEUEARRAY_H
