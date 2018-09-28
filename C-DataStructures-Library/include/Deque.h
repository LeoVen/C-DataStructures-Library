/**
 * @file Deque.h
 * 
 * @author Leonardo Vencovsky (https://github.com/LeoVen)
 *
 * @date 27/09/2018
 */

#ifndef C_DATASTRUCTURES_LIBRARY_DEQUE_H
#define C_DATASTRUCTURES_LIBRARY_DEQUE_H

#include "Core.h"

typedef struct Deque
{
    size_t length;			 /*!< Total @c Deque length */
    struct DequeNode *front; /*!< Where @c QueueNode are removed */
    struct DequeNode *rear;  /*!< Where @c QueueNode are inserted */
} Deque;

Status deq_init(Deque **deq);

Status deq_enqueue_front(Deque *deq, int value);

Status deq_enqueue_rear(Deque *deq, int value);

Status deq_dequeue_front(Deque *deq, int *result);

Status deq_dequeue_rear(Deque *deq, int *result);

Status deq_display(Deque *deq);

Status deq_display_array(Deque *deq);

Status deq_display_raw(Deque *deq);

Status deq_delete(Deque **deq);

Status deq_erase(Deque **deq);

int deq_peek_front(Deque *deq);

int deq_peek_rear(Deque *deq);

size_t deq_length(Deque *deq);

bool deq_empty(Deque *deq);

Status deq_copy(Deque *que, Deque **result);

#endif //C_DATASTRUCTURES_LIBRARY_DEQUE_H
