/**
 * @file Queue.h
 * 
 * @author Leonardo Vencovsky (https://github.com/LeoVen)
 *
 * @date 26/09/2018
 */

#ifndef C_DATASTRUCTURES_LIBRARY_QUEUE_H
#define C_DATASTRUCTURES_LIBRARY_QUEUE_H

#include "Core.h"

typedef struct Queue
{
    size_t length;			 /*!< Total @c Queue length */
    struct QueueNode *front; /*!< Where @c QueueNode are removed */
    struct QueueNode *rear;  /*!< Where @c QueueNode are inserted */
} Queue;

Status que_init(Queue **que);

Status que_enqueue(Queue *que, int value);

Status que_dequeue(Queue *que, int *result);

Status que_display(Queue *que);

Status que_display_array(Queue *que);

Status que_display_raw(Queue *que);

Status que_delete(Queue **que);

Status que_erase(Queue **que);

int que_peek_front(Queue *que);

int que_peek_rear(Queue *que);

size_t que_length(Queue *que);

bool que_empty(Queue *que);

Status que_copy(Queue *que, Queue **result);

#endif //C_DATASTRUCTURES_LIBRARY_QUEUE_H
