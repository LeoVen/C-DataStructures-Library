/**
 * @file PriorityQueue.c
 * 
 * @author Leonardo Vencovsky (https://github.com/LeoVen)
 *
 * @date 28/09/2018
 */

#include "PriorityQueue.h"

// NOT EXPOSED API

typedef struct PriorityQueueNode
{
    int data;						/*!< Node's data */
    int priority;					/*!< Node's priority */
    struct PriorityQueueNode *prev; /*!< Pointer to the previous node in the @c PriorityQueue */
} PriorityQueueNode;

Status prq_make_node(PriorityQueueNode **node, int value, int priority);
Status prq_delete_node(PriorityQueueNode **node);

// END OF NOT EXPOSED API

Status prq_init(PriorityQueue **prq)
{
    (*prq) = malloc(sizeof(PriorityQueue));

    if (!(*prq))
        return DS_ERR_ALLOC;

    (*prq)->front = NULL;
    (*prq)->rear = NULL;

    (*prq)->length = 0;

    return DS_OK;
}

Status prq_enqueue(PriorityQueue *prq, int value, int priority)
{
    if (prq == NULL)
        return DS_ERR_NULL_POINTER;

    PriorityQueueNode *node;

    prq_make_node(&node, value, priority);

    if (!node)
        return DS_ERR_ALLOC;

    if (prq_empty(prq))
    {

        prq->rear = node;
        prq->front = node;
    }
    else
    {

        PriorityQueueNode *before = NULL;
        PriorityQueueNode *curr = prq->front;

        while (curr != NULL && curr->priority >= node->priority)
        {
            before = curr;

            curr = curr->prev;
        }

        if (before == NULL)
        {

            node->prev = prq->front;

            prq->front = node;
        }
        else
        {

            node->prev = curr;

            before->prev = node;
        }
    }

    (prq->length)++;

    return DS_OK;
}

Status prq_dequeue(PriorityQueue *prq, int *result)
{
    if (prq == NULL)
        return DS_ERR_NULL_POINTER;

    if (prq_empty(prq))
        return DS_ERR_INVALID_OPERATION;

    PriorityQueueNode *node = prq->front;

    *result = node->data;

    prq->front = prq->front->prev;

    Status st = prq_delete_node(&node);

    if (st != DS_OK)
        return st;

    (prq->length)--;

    if (prq->length == 0)
        prq->rear = NULL;

    return DS_OK;
}

Status prq_display(PriorityQueue *prq)
{
    if (prq == NULL)
        return DS_ERR_NULL_POINTER;

    if (prq_empty(prq))
    {
        printf("\nPriority Queue\n[ empty ]\n");

        return DS_OK;
    }

    PriorityQueueNode *scan = prq->front;

    printf("\nPriority Queue\nfront <-");

    while (scan != NULL)
    {
        printf(" %d <-", scan->data);
        scan = scan->prev;
    }

    printf(" rear\n");

    return DS_OK;
}

Status prq_display_array(PriorityQueue *prq)
{
    if (prq == NULL)
        return DS_ERR_NULL_POINTER;

    if (prq_empty(prq))
    {
        printf("\n[ empty ]\n");

        return DS_OK;
    }

    PriorityQueueNode *scan = prq->front;

    printf("\n[ ");

    while (scan->prev != NULL)
    {
        printf("%d, ", scan->data);

        scan = scan->prev;
    }

    printf("%d ]\n", scan->data);

    return DS_OK;
}

Status prq_display_priority(PriorityQueue *prq)
{
    if (prq == NULL)
        return DS_ERR_NULL_POINTER;

    if (prq_empty(prq))
    {
        printf("\n[ empty ]\n");

        return DS_OK;
    }

    PriorityQueueNode *scan = prq->front;

    printf("\n[ ");

    while (scan->prev != NULL)
    {
        printf("%d, ", scan->priority);

        scan = scan->prev;
    }

    printf("%d ]\n", scan->priority);

    return DS_OK;
}

Status prq_display_raw(PriorityQueue *prq)
{
    if (prq == NULL)
        return DS_ERR_NULL_POINTER;

    printf("\n");

    if (prq_empty(prq))
        return DS_OK;

    PriorityQueueNode *scan = prq->front;

    while (scan != NULL)
    {
        printf("%d ", scan->data);

        scan = scan->prev;
    }

    printf("\n");

    return DS_OK;
}

Status prq_delete(PriorityQueue **prq)
{
    if ((*prq) == NULL)
        return DS_ERR_NULL_POINTER;

    PriorityQueueNode *prev = (*prq)->front;

    Status st;

    while ((*prq)->front != NULL)
    {
        (*prq)->front = (*prq)->front->prev;

        st = prq_delete_node(&prev);

        if (st != DS_OK)
            return st;

        prev = (*prq)->front;
    }

    free((*prq));

    (*prq) = NULL;

    return DS_OK;
}

Status prq_erase(PriorityQueue **prq)
{
    if ((*prq) == NULL)
        return DS_ERR_NULL_POINTER;

    Status st = prq_delete(prq);

    if (st != DS_OK)
        return st;

    st = prq_init(prq);

    if (st != DS_OK)
        return st;

    return DS_OK;
}

int prq_peek_front(PriorityQueue *prq)
{
    if (prq == NULL)
        return 0;

    if (prq_empty(prq))
        return 0;

    return prq->front->data;
}

int prq_peek_rear(PriorityQueue *prq)
{
    if (prq == NULL)
        return 0;

    if (prq_empty(prq))
        return 0;

    return prq->rear->data;
}

int prq_priority_highest(PriorityQueue *prq)
{
    if (prq == NULL)
        return 0;

    if (prq_empty(prq))
        return 0;

    PriorityQueueNode *scan = prq->front;

    int result = scan->priority;

    while (scan != NULL)
    {
        if (scan->priority > result)
            result = scan->priority;

        scan = scan->prev;
    }

    return result;
}

int prq_priority_lowest(PriorityQueue *prq)
{
    if (prq == NULL)
        return 0;

    if (prq_empty(prq))
        return 0;

    PriorityQueueNode *scan = prq->front;

    int result = scan->priority;

    while (scan != NULL)
    {
        if (scan->priority < result)
            result = scan->priority;

        scan = scan->prev;
    }

    return result;
}

size_t prq_length(PriorityQueue *prq)
{
    if (prq == NULL)
        return 0;

    return prq->length;
}

bool prq_empty(PriorityQueue *prq)
{
    return (prq->length == 0 || prq->rear == NULL);
}

Status prq_copy(PriorityQueue *prq, PriorityQueue **result)
{
    *result = NULL;

    if (prq == NULL)
        return DS_ERR_NULL_POINTER;

    Status st = prq_init(result);

    if (st != DS_OK)
        return st;

    if (prq_empty(prq))
    {
        return DS_OK;
    }

    PriorityQueueNode *scan = prq->front;

    while (scan != NULL)
    {
        st = prq_enqueue(*result, scan->data, scan->priority);

        if (st != DS_OK)
            return st;

        scan = scan->prev;
    }

    return DS_OK;
}

// NOT EXPOSED API

Status prq_make_node(PriorityQueueNode **node, int value, int priority)
{
    (*node) = malloc(sizeof(PriorityQueueNode));

    if (!(*node))
        return DS_ERR_ALLOC;

    (*node)->prev = NULL;

    (*node)->data = value;
    (*node)->priority = priority;

    return DS_OK;
}

Status prq_delete_node(PriorityQueueNode **node)
{
    free(*node);

    (*node) = NULL;

    return DS_OK;
}

// END OF NOT EXPOSED API