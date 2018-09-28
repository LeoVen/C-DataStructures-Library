/**
 * @file Queue.c
 * 
 * @author Leonardo Vencovsky (https://github.com/LeoVen)
 *
 * @date 26/09/2018
 */

#include "Queue.h"

// NOT EXPOSED API

typedef struct QueueNode
{
    int data;				/*!< Node's data */
    struct QueueNode *prev; /*!< Pointer to the previous node in the @c Queue */
} QueueNode;

Status que_make_node(QueueNode **node, int value);
Status que_delete_node(QueueNode **node);

// END OF NOT EXPOSED API

Status que_init(Queue **que)
{
    (*que) = malloc(sizeof(Queue));

    if (!(*que))
        return DS_ERR_ALLOC;

    (*que)->length = 0;

    (*que)->front = NULL;
    (*que)->rear = NULL;

    return DS_OK;
}

Status que_enqueue(Queue *que, int value)
{
    if (que == NULL)
        return DS_ERR_NULL_POINTER;

    QueueNode *node;

    Status st = que_make_node(&node, value);

    if (st != DS_OK)
        return st;

    if (que_empty(que))
    {
        que->rear = node;
        que->front = node;
    }
    else
    {
        que->rear->prev = node;
        que->rear = node;
    }

    (que->length)++;

    return DS_OK;
}

Status que_dequeue(Queue *que, int *result)
{
    if (que == NULL)
        return DS_ERR_NULL_POINTER;

    if (que_empty(que))
        return DS_ERR_INVALID_OPERATION;

    QueueNode *node = que->front;

    *result = node->data;

    que->front = que->front->prev;

    Status st = que_delete_node(&node);

    if (st != DS_OK)
        return st;

    (que->length)--;

    if (que->length == 0)
        que->rear = NULL;

    return DS_OK;
}

Status que_display(Queue *que)
{
    if (que == NULL)
        return DS_ERR_NULL_POINTER;

    if (que_empty(que))
    {
        printf("\nQueue\n[ empty ]\n");

        return DS_OK;
    }

    QueueNode *scan = que->front;

    printf("\nQueue\nfront <-");

    while (scan != NULL)
    {
        printf(" %d <-", scan->data);
        scan = scan->prev;
    }

    printf(" rear\n");

    return DS_OK;
}

Status que_display_array(Queue *que)
{
    if (que == NULL)
        return DS_ERR_NULL_POINTER;

    if (que_empty(que))
    {
        printf("\n[ empty ]\n");

        return DS_OK;
    }

    QueueNode *scan = que->front;

    printf("\n[ ");

    while (scan->prev != NULL)
    {
        printf("%d, ", scan->data);

        scan = scan->prev;
    }

    printf("%d ]\n", scan->data);

    return DS_OK;
}

Status que_display_raw(Queue *que)
{
    if (que == NULL)
        return DS_ERR_NULL_POINTER;

    printf("\n");

    if (que_empty(que))
        return DS_OK;

    QueueNode *scan = que->front;

    while (scan != NULL)
    {
        printf("%d ", scan->data);

        scan = scan->prev;
    }

    printf("\n");

    return DS_OK;
}

Status que_delete(Queue **que)
{
    if ((*que) == NULL)
        return DS_ERR_NULL_POINTER;

    QueueNode *prev = (*que)->front;

    Status st;

    while ((*que)->front != NULL)
    {
        (*que)->front = (*que)->front->prev;

        st = que_delete_node(&prev);

        if (st != DS_OK)
            return st;

        prev = (*que)->front;
    }

    free((*que));

    (*que) = NULL;

    return DS_OK;
}

Status que_erase(Queue **que)
{
    if ((*que) == NULL)
        return DS_ERR_NULL_POINTER;

    Status st = que_delete(que);

    if (st != DS_OK)
        return st;

    st = que_init(que);

    if (st != DS_OK)
        return st;

    return DS_OK;
}

int que_peek_front(Queue *que)
{
    if (que == NULL)
        return 0;

    if (que_empty(que))
        return 0;

    return que->front->data;
}

int que_peek_rear(Queue *que)
{
    if (que == NULL)
        return 0;

    if (que_empty(que))
        return 0;

    return que->rear->data;
}

size_t que_length(Queue *que)
{
    if (que == NULL)
        return 0;

    return que->length;
}

bool que_empty(Queue *que)
{
    return (que->length == 0 || que->rear == NULL);
}

Status que_copy(Queue *que, Queue **result)
{
    *result = NULL;

    if (que == NULL)
        return DS_ERR_NULL_POINTER;

    Status st = que_init(result);

    if (st != DS_OK)
        return st;

    if (que_empty(que))
        return DS_OK;

    QueueNode *scan = que->front;

    while (scan != NULL)
    {
        st = que_enqueue(*result, scan->data);

        if (st != DS_OK)
            return st;

        scan = scan->prev;
    }

    return DS_OK;
}

// NOT EXPOSED API

Status que_make_node(QueueNode **node, int value)
{
    (*node) = malloc(sizeof(QueueNode));

    if (!(*node))
        return DS_ERR_ALLOC;

    (*node)->data = value;

    (*node)->prev = NULL;

    return DS_OK;
}

Status que_delete_node(QueueNode **node)
{
    free(*node);

    (*node) = NULL;

    return DS_OK;
}

// END OF NOT EXPOSED API