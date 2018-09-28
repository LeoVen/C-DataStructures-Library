/**
 * @file Deque.c
 * 
 * @author Leonardo Vencovsky (https://github.com/LeoVen)
 *
 * @date 27/09/2018
 */

#include "Deque.h"

// NOT EXPOSED API

typedef struct DequeNode
{
    int data;				/*!< Node's data */
    struct DequeNode *next; /*!< Pointer to the next node in the @c Deque */
    struct DequeNode *prev; /*!< Pointer to the previous node in the @c Deque */
} DequeNode;

Status deq_make_node(DequeNode **node, int value);
Status deq_delete_node(DequeNode **node);

// END OF NOT EXPOSED API

Status deq_init(Deque **deq)
{
    (*deq) = malloc(sizeof(Deque));

    if (!(*deq))
        return DS_ERR_ALLOC;

    (*deq)->length = 0;

    (*deq)->front = NULL;
    (*deq)->rear = NULL;

    return DS_OK;
}

Status deq_enqueue_front(Deque *deq, int value)
{
    if (deq == NULL)
        return DS_ERR_NULL_POINTER;

    DequeNode *node;

    deq_make_node(&node, value);

    if (!node)
        return DS_ERR_ALLOC;

    if (deq_empty(deq))
    {

        deq->front = node;
        deq->rear = node;
    }
    else
    {

        node->prev = deq->front;

        deq->front->next = node;
        deq->front = node;
    }

    (deq->length)++;

    return DS_OK;
}

Status deq_enqueue_rear(Deque *deq, int value)
{

    if (deq == NULL)
        return DS_ERR_NULL_POINTER;

    DequeNode *node;

    deq_make_node(&node, value);

    if (!node)
        return DS_ERR_ALLOC;

    if (deq_empty(deq))
    {

        deq->front = node;
        deq->rear = node;
    }
    else
    {

        node->next = deq->rear;

        deq->rear->prev = node;
        deq->rear = node;
    }

    (deq->length)++;

    return DS_OK;
}

Status deq_dequeue_front(Deque *deq, int *result)
{
    if (deq == NULL)
        return DS_ERR_NULL_POINTER;

    if (deq_empty(deq))
        return DS_ERR_INVALID_OPERATION;

    DequeNode *node = deq->front;

    *result = node->data;

    deq->front = deq->front->prev;

    if (deq->front == NULL)
        deq->rear = NULL;
    else
        deq->front->next = NULL;

    Status st = deq_delete_node(&node);

    if (st != DS_OK)
        return st;

    (deq->length)--;

    return DS_OK;
}

Status deq_dequeue_rear(Deque *deq, int *result)
{
    if (deq == NULL)
        return DS_ERR_NULL_POINTER;

    if (deq_empty(deq))
        return DS_ERR_INVALID_OPERATION;

    DequeNode *node = deq->rear;

    *result = node->data;

    deq->rear = deq->rear->next;

    if (deq->rear == NULL)
        deq->front = NULL;
    else
        deq->rear->prev = NULL;

    Status st = deq_delete_node(&node);

    if (st != DS_OK)
        return st;

    (deq->length)--;

    return DS_OK;
}

Status deq_display(Deque *deq)
{
    if (deq == NULL)
        return DS_ERR_NULL_POINTER;

    if (deq_empty(deq))
    {

        printf("\nDeque\n[ empty ]\n");
        return DS_OK;
    }

    DequeNode *scan = deq->front;

    printf("\nDeque\nfront <->");

    while (scan != NULL)
    {
        printf(" %d <->", scan->data);
        scan = scan->prev;
    }

    printf(" rear\n");

    return DS_OK;
}

Status deq_display_array(Deque *deq)
{
    if (deq == NULL)
        return DS_ERR_NULL_POINTER;

    if (deq_empty(deq))
    {
        printf("\n[ empty ]\n");

        return DS_OK;
    }

    DequeNode *scan = deq->front;

    printf("\n[ ");

    while (scan->prev != NULL)
    {
        printf("%d, ", scan->data);

        scan = scan->prev;
    }

    printf("%d ]\n", scan->data);

    return DS_OK;
}

Status deq_display_raw(Deque *deq)
{
    if (deq == NULL)
        return DS_ERR_NULL_POINTER;

    printf("\n");

    if (deq_empty(deq))
        return DS_OK;

    DequeNode *scan = deq->front;

    while (scan != NULL)
    {
        printf("%d ", scan->data);
        scan = scan->prev;
    }

    printf("\n");

    return DS_OK;
}

Status deq_delete(Deque **deq)
{
    if ((*deq) == NULL)
        return DS_ERR_NULL_POINTER;

    DequeNode *prev = (*deq)->front;

    Status st;

    while ((*deq)->front != NULL)
    {
        (*deq)->front = (*deq)->front->prev;

        st = deq_delete_node(&prev);

        if (st != DS_OK)
            return st;

        prev = (*deq)->front;
    }

    free((*deq));

    (*deq) = NULL;

    return DS_OK;
}

Status deq_erase(Deque **deq)
{
    if ((*deq) == NULL)
        return DS_ERR_NULL_POINTER;

    Status st = deq_delete(deq);

    if (st != DS_OK)
        return st;

    st = deq_init(deq);

    if (st != DS_OK)
        return st;

    return DS_OK;
}

int deq_peek_front(Deque *deq)
{
    if (deq == NULL)
        return 0;

    if (deq_empty(deq))
        return 0;

    return deq->front->data;
}

int deq_peek_rear(Deque *deq)
{
    if (deq == NULL)
        return 0;

    if (deq_empty(deq))
        return 0;

    return deq->rear->data;
}

size_t deq_length(Deque *deq)
{
    if (deq == NULL)
        return 0;

    return deq->length;
}

bool deq_empty(Deque *deq)
{
    return (deq->length == 0 || deq->rear == NULL);
}

Status deq_copy(Deque *deq, Deque **result)
{
    *result = NULL;

    if (deq == NULL)
        return DS_ERR_NULL_POINTER;

    Status st = deq_init(result);

    if (st != DS_OK)
        return st;

    if (deq_empty(deq))
        return DS_OK;

    DequeNode *scan = deq->front;

    while (scan != NULL)
    {
        st = deq_enqueue_rear(*result, scan->data);

        if (st != DS_OK)
            return st;

        scan = scan->prev;
    }

    return DS_OK;
}

// NOT EXPOSED API

Status deq_make_node(DequeNode **node, int value)
{
    (*node) = malloc(sizeof(DequeNode));

    if (!(*node))
        return DS_ERR_ALLOC;

    (*node)->data = value;

    (*node)->prev = NULL;
    (*node)->next = NULL;

    return DS_OK;
}

Status deq_delete_node(DequeNode **node)
{
    free(*node);

    (*node) = NULL;

    return DS_OK;
}

// END OF NOT EXPOSED API