/**
 * @file PriorityQueue.h
 * 
 * @author Leonardo Vencovsky (https://github.com/LeoVen)
 *
 * @date 28/09/2018
 */

#ifndef C_DATASTRUCTURES_LIBRARY_PRIORITYQUEUE_H
#define C_DATASTRUCTURES_LIBRARY_PRIORITYQUEUE_H

#include "Core.h"

typedef struct PriorityQueue_s
{
    size_t length;                     /*!< Total @c Queue length */
    struct PriorityQueueNode_s *front; /*!< Where @c PriorityQueueNode are removed */
    struct PriorityQueueNode_s *rear;  /*!< Where @c PriorityQueueNode are inserted */
} PriorityQueue_t, *PriorityQueue;

Status prq_init(PriorityQueue *prq);

Status prq_enqueue(PriorityQueue prq, int value, int priority);

Status prq_dequeue(PriorityQueue prq, int *result);

Status prq_display(PriorityQueue prq);

Status prq_display_array(PriorityQueue prq);

Status prq_display_priority(PriorityQueue prq);

Status prq_display_raw(PriorityQueue prq);

Status prq_delete(PriorityQueue *prq);

Status prq_erase(PriorityQueue *prq);

int prq_peek_front(PriorityQueue que);

int prq_peek_rear(PriorityQueue que);

int prq_priority_highest(PriorityQueue prq);

int prq_priority_lowest(PriorityQueue prq);

size_t prq_length(PriorityQueue prq);

bool prq_empty(PriorityQueue prq);

Status prq_copy(PriorityQueue prq, PriorityQueue *result);

#endif //C_DATASTRUCTURES_LIBRARY_PRIORITYQUEUE_H
