/**
 * @file GQueue.c
 * 
 * @author Leonardo Vencovsky (https://github.com/LeoVen)
 *
 * @date 01/11/2018
 */

#include "GQueue.h"

// NOT EXPOSED API

typedef struct GQueueNode_s
{
    void *data;                /*!< Node's data */
    signed char flag;          /*!< Node's data type flag */
    struct GQueueNode_s *prev; /*!< Pointer to the previous node in the @c GQueue */
} GQueueNode_t, *GQueueNode;

Status gque_make_node(GQueueNode *node, void *element, signed char flag);

void *gque_free_node(GQueueNode *node);

Status gque_delete_node(GQueueNode *node, void(free_func)(void*));

// END OF NOT EXPOSED API

Status gque_init(GQueue *que)
{
    (*que) = malloc(sizeof(GQueue_t));

    if (!(*que))
        return DS_ERR_ALLOC;

    (*que)->length = 0;

    (*que)->front = NULL;
    (*que)->rear = NULL;

    (*que)->d_free = NULL;
    (*que)->d_copy = NULL;
    (*que)->d_compare = NULL;
    (*que)->d_display = NULL;

    return DS_OK;
}

Status gque_create(GQueue *que, free_f free_func, copy_f copy_func, compare_f compare_func, display_f display_func)
{
    (*que) = malloc(sizeof(GQueue_t));

    if (!(*que))
        return DS_ERR_ALLOC;

    (*que)->length = 0;

    (*que)->front = NULL;
    (*que)->rear = NULL;

    (*que)->d_free = free_func;
    (*que)->d_copy = copy_func;
    (*que)->d_compare = compare_func;
    (*que)->d_display = display_func;

    return DS_OK;
}

Status gque_enqueue(GQueue que, void *element, signed char flag)
{
    if (que == NULL)
        return DS_ERR_NULL_POINTER;

    if (gque_full(que))
        return DS_ERR_FULL;

    GQueueNode node;

    Status st = gque_make_node(&node, element, flag);

    if (st != DS_OK)
        return st;

    if (gque_empty(que))
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

Status gque_dequeue(GQueue que, void **result, signed char *flag)
{
    if (que == NULL)
        return DS_ERR_NULL_POINTER;

    if (gque_empty(que))
        return DS_ERR_INVALID_OPERATION;

    GQueueNode node = que->front;

    que->front = que->front->prev;

    *flag = node->flag;
    *result = gque_free_node(&node);


    (que->length)--;

    if (que->length == 0)
        que->rear = NULL;

    return DS_OK;
}

Status gque_display(GQueue que)
{
    if (que == NULL)
        return DS_ERR_NULL_POINTER;

    if (gque_empty(que))
    {
        printf("\nGQueue\n[ empty ]\n");

        return DS_OK;
    }

    GQueueNode scan = que->front;

    printf("\nGQueue\nfront <-");

    while (scan != NULL)
    {
        que->d_display(scan->data);

        printf(" <- ");

        scan = scan->prev;
    }

    printf(" rear\n");

    return DS_OK;
}

Status gque_display_array(GQueue que)
{
    if (que == NULL)
        return DS_ERR_NULL_POINTER;

    if (gque_empty(que))
    {
        printf("\n[ empty ]\n");

        return DS_OK;
    }

    GQueueNode scan = que->front;

    printf("\n[ ");

    while (scan->prev != NULL)
    {
        que->d_display(scan->data);

        printf(", ");

        scan = scan->prev;
    }

    que->d_display(scan->data);

    printf(" ]\n");

    return DS_OK;
}

Status gque_display_raw(GQueue que)
{
    if (que == NULL)
        return DS_ERR_NULL_POINTER;

    printf("\n");

    if (gque_empty(que))
        return DS_OK;

    GQueueNode scan = que->front;

    while (scan != NULL)
    {
        que->d_display(scan->data);

        printf(" ");

        scan = scan->prev;
    }

    printf("\n");

    return DS_OK;
}

Status gque_delete(GQueue *que)
{
    if ((*que) == NULL)
        return DS_ERR_NULL_POINTER;

    if ((*que)->d_free == NULL)
        return DS_ERR_INCOMPLETE_TYPE;

    GQueueNode prev = (*que)->front;

    Status st;

    while ((*que)->front != NULL)
    {
        (*que)->front = (*que)->front->prev;

        st = gque_delete_node(&prev, (*que)->d_free);

        if (st != DS_OK)
            return st;

        prev = (*que)->front;
    }

    free((*que));

    (*que) = NULL;

    return DS_OK;
}

Status gque_erase(GQueue *que)
{
    if ((*que) == NULL)
        return DS_ERR_NULL_POINTER;

    free_f g_free = (*que)->d_free;
    copy_f g_copy = (*que)->d_copy;
    compare_f g_compare = (*que)->d_compare;
    display_f g_display = (*que)->d_display;

    Status st = gque_delete(que);

    if (st != DS_OK)
        return st;

    st = gque_create(que, g_free, g_copy, g_compare, g_display);

    if (st != DS_OK)
        return st;

    return DS_OK;
}

void* gque_peek_front(GQueue que)
{
    if (que == NULL)
        return 0;

    if (gque_empty(que))
        return 0;

    return que->front->data;
}

void* gque_peek_rear(GQueue que)
{
    if (que == NULL)
        return 0;

    if (gque_empty(que))
        return 0;

    return que->rear->data;
}

bool gque_complete(GQueue que)
{
    return (que->d_free != NULL) &&
            (que->d_copy != NULL) &&
            (que->d_compare != NULL) &&
            (que->d_display) != NULL;
}

bool gque_full(GQueue que)
{
    return que->limit > 0 && que->length >= que->limit;
}

bool gque_empty(GQueue que)
{
    return que->length == 0;
}

index_t gque_length(GQueue que)
{
    if (que == NULL)
        return -1;

    return que->length;
}

Status gque_limit(GQueue que, index_t limit)
{
    if (que == NULL)
        return DS_ERR_NULL_POINTER;

    if (que->length > limit && limit > 0)
        return DS_ERR_INVALID_OPERATION;

    que->limit = limit;

    return DS_OK;
}

Status gque_copy(GQueue que, GQueue *result)
{
    *result = NULL;

    if (que == NULL)
        return DS_ERR_NULL_POINTER;

    if (que->d_copy == NULL)
        return DS_ERR_INCOMPLETE_TYPE;

    Status st = gque_create(result, que->d_free, que->d_copy, que->d_compare, que->d_display);

    if (st != DS_OK)
        return st;

    if (gque_empty(que))
        return DS_OK;

    GQueueNode scan = que->front;

    while (scan != NULL)
    {
        st = gque_enqueue(*result, que->d_copy(scan->data), scan->flag);

        if (st != DS_OK)
            return st;

        scan = scan->prev;
    }

    return DS_OK;
}

Status gque_copy_shallow(GQueue que, GQueue *result)
{
    *result = NULL;

    if (que == NULL)
        return DS_ERR_NULL_POINTER;

    Status st = gque_create(result, que->d_free, que->d_copy, que->d_compare, que->d_display);

    if (st != DS_OK)
        return st;

    if (gque_empty(que))
        return DS_OK;

    GQueueNode scan = que->front;

    while (scan != NULL)
    {
        st = gque_enqueue(*result, scan->data, scan->flag);

        if (st != DS_OK)
            return st;

        scan = scan->prev;
    }

    return DS_OK;
}

// NOT EXPOSED API

Status gque_make_node(GQueueNode *node, void *element, signed char flag)
{
    (*node) = malloc(sizeof(GQueueNode_t));

    if (!(*node))
        return DS_ERR_ALLOC;

    (*node)->data = element;

    (*node)->flag = flag;

    (*node)->prev = NULL;

    return DS_OK;
}

void *gque_free_node(GQueueNode *node)
{
    if ((*node) == NULL)
        return NULL;

    void *element = (*node)->data;

    free(*node);

    *node = NULL;

    return element;
}

Status gque_delete_node(GQueueNode *node, free_f free_func)
{
    if ((*node) == NULL)
        return DS_ERR_NULL_POINTER;

    free_func((*node)->data);

    free(*node);

    (*node) = NULL;

    return DS_OK;
}

// END OF NOT EXPOSED API