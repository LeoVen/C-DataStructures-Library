/**
 * @file CircularLinkedList.c
 * 
 * @author Leonardo Vencovsky (https://github.com/LeoVen)
 *
 * @date 09/10/2018
 */

#include "CircularLinkedList.h"

// NOT EXPOSED API

typedef struct CircularLinkedNode_s
{
    int data;                          /*!< Pointer to node's content */
    struct CircularLinkedNode_s *next; /*!< Pointer to the next node on the list */
    struct CircularLinkedNode_s *prev; /*!< Pointer to the previous node on the list */
} CircularLinkedNode_t, *CircularLinkedNode;

Status cll_make_node(CircularLinkedNode *node, int value);

Status cll_delete_node(CircularLinkedNode *node);

// END OF NOT EXPOSED API

Status cll_init(CircularLinkedList *cll)
{
    (*cll) = malloc(sizeof(CircularLinkedList_t));

    if (!(*cll))
        return DS_ERR_ALLOC;

    (*cll)->length = 0;
    (*cll)->limit = 0;

    (*cll)->cursor = NULL;

    return DS_OK;
}

Status cll_insert_after(CircularLinkedList cll, int element)
{
    if (cll == NULL)
        return DS_ERR_NULL_POINTER;

    CircularLinkedNode node;

    if (cll->limit != 0 && cll->length >= cll->limit)
        return DS_ERR_FULL;

    Status st = cll_make_node(&node, element);

    if (st != DS_OK)
        return st;

    if (cll_empty(cll))
    {
        cll->cursor = node;

        node->next = node;
        node->prev = node;
    }
    else
    {
        node->next = cll->cursor->next;
        node->prev = cll->cursor;

        node->next->prev = node;

        cll->cursor->next = node;
    }

    (cll->length)++;

    return DS_OK;
}

Status cll_insert_before(CircularLinkedList cll, int element)
{
    if (cll == NULL)
        return DS_ERR_NULL_POINTER;

    if (cll->limit != 0 && cll->length >= cll->limit)
        return DS_ERR_FULL;

    CircularLinkedNode node;

    Status st = cll_make_node(&node, element);

    if (st != DS_OK)
        return st;

    if (cll_empty(cll))
    {
        cll->cursor = node;

        node->next = node;
        node->prev = node;
    }
    else
    {
        node->next = cll->cursor;
        node->prev = cll->cursor->prev;

        node->prev->next = node;

        cll->cursor->prev = node;
    }

    (cll->length)++;

    return DS_OK;
}

Status cll_remove_after(CircularLinkedList cll, int *result)
{
    *result = 0;

    if (cll == NULL)
        return DS_ERR_NULL_POINTER;

    if (cll_empty(cll))
        return DS_ERR_INVALID_OPERATION;

    Status st;

    if (cll->length == 1)
    {
        *result = cll->cursor->data;

        st = cll_delete_node(&(cll->cursor));

        if (st != DS_OK)
            return st;
    }
    else
    {
        CircularLinkedNode node = cll->cursor->next;

        *result = node->data;

        node->prev->next = node->next;
        node->next->prev = node->prev;

        st = cll_delete_node(&node);

        if (st != DS_OK)
            return st;
    }

    (cll->length)--;

    return DS_OK;
}

Status cll_remove_current(CircularLinkedList cll, int *result)
{
    *result = 0;

    if (cll == NULL)
        return DS_ERR_NULL_POINTER;

    if (cll_empty(cll))
        return DS_ERR_INVALID_OPERATION;

    Status st;

    if (cll->length == 1)
    {
        *result = cll->cursor->data;

        st = cll_delete_node(&(cll->cursor));

        if (st != DS_OK)
            return st;
    }
    else
    {
        CircularLinkedNode node = cll->cursor;

        *result = node->data;

        //cll->cursor = cll->cursor->prev;
        cll->cursor = cll->cursor->next;

        node->prev->next = node->next;
        node->next->prev = node->prev;

        st = cll_delete_node(&node);

        if (st != DS_OK)
            return st;
    }

    (cll->length)--;

    return DS_OK;
}

Status cll_remove_before(CircularLinkedList cll, int *result)
{
    *result = 0;

    if (cll == NULL)
        return DS_ERR_NULL_POINTER;

    if (cll_empty(cll))
        return DS_ERR_INVALID_OPERATION;

    Status st;

    if (cll->length == 1)
    {
        *result = cll->cursor->data;

        st = cll_delete_node(&(cll->cursor));

        if (st != DS_OK)
            return st;
    }
    else
    {
        CircularLinkedNode node = cll->cursor->prev;

        *result = node->data;

        node->prev->next = node->next;
        node->next->prev = node->prev;

        st = cll_delete_node(&node);

        if (st != DS_OK)
            return st;
    }

    (cll->length)--;

    return DS_OK;
}

Status cll_iter_next(CircularLinkedList cll, size_t positions)
{
    if (cll == NULL)
        return DS_ERR_NULL_POINTER;

    if (cll_empty(cll))
        return DS_ERR_INVALID_OPERATION;

    for (size_t i = 0; i < positions; i++)
        cll->cursor = cll->cursor->next;

    return DS_OK;
}

Status cll_iter_prev(CircularLinkedList cll, size_t positions)
{
    if (cll == NULL)
        return DS_ERR_NULL_POINTER;

    if (cll_empty(cll))
        return DS_ERR_INVALID_OPERATION;

    for (size_t i = 0; i < positions; i++)
        cll->cursor = cll->cursor->prev;

    return DS_OK;
}

Status cll_display(CircularLinkedList cll)
{
    if (cll == NULL)
        return DS_ERR_NULL_POINTER;

    if (cll_empty(cll))
    {
        printf("\nCircular Linked List\n[ empty ]\n");

        return DS_OK;
    }

    CircularLinkedNode scan = cll->cursor;

    printf("\nCircular Linked List\n <->");

    for (size_t i = 0; i < cll->length; i++)
    {
        printf(" %d <->", scan->data);

        scan = scan->next;
    }

    printf("\n");

    return DS_OK;
}

Status cll_display_array(CircularLinkedList cll)
{
    if (cll == NULL)
        return DS_ERR_NULL_POINTER;

    if (cll_empty(cll))
    {
        printf("\n[ empty ]\n");

        return DS_OK;
    }

    CircularLinkedNode scan = cll->cursor;

    printf("\n[ ");

    for (size_t i = 0; i < cll->length - 1; i++)
    {
        printf("%d, ", scan->data);

        scan = scan->next;
    }

    printf("%d ]\n", scan->data);

    return DS_OK;
}

Status cll_display_raw(CircularLinkedList cll)
{
    if (cll == NULL)
        return DS_ERR_NULL_POINTER;

    CircularLinkedNode scan = cll->cursor;

    printf("\n");

    for (size_t i = 0; i < cll->length; i++)
    {
        printf("%d ", scan->data);

        scan = scan->next;
    }

    printf("\n");

    return DS_OK;
}

Status cll_delete(CircularLinkedList *cll)
{
    if ((*cll) == NULL)
        return DS_ERR_NULL_POINTER;

    Status st;

    CircularLinkedNode prev = (*cll)->cursor;

    for (size_t i = 0; i < (*cll)->length; i++)
    {
        (*cll)->cursor = (*cll)->cursor->next;

        st = cll_delete_node(&prev);

        if (st != DS_OK)
            return st;

        prev = (*cll)->cursor;
    }

    free(*cll);

    *cll = NULL;

    return DS_OK;
}

Status cll_erase(CircularLinkedList *cll)
{
    if ((*cll) == NULL)
        return DS_ERR_NULL_POINTER;

    Status st = cll_delete(cll);

    if (st != DS_OK)
        return st;

    st = cll_init(cll);

    if (st != DS_OK)
        return st;

    return DS_OK;
}

bool cll_contains(CircularLinkedList cll, int key)
{
    if (cll == NULL)
        return false;

    if (cll_empty(cll))
        return false;

    CircularLinkedNode scan = cll->cursor;

    for (size_t i = 0; i < cll->length; i++)
    {
        if (scan->data == key)
            return true;

        scan = scan->next;
    }

    return false;
}

bool cll_empty(CircularLinkedList cll)
{
    return cll->length == 0;
}

size_t cll_length(CircularLinkedList cll)
{
    if (cll == NULL)
        return 0;

    return cll->length;
}

Status cll_limit(CircularLinkedList cll, size_t limit)
{
    if (cll == NULL)
        return DS_ERR_NULL_POINTER;

    if (cll->length > limit && limit != 0)
        return DS_ERR_INVALID_OPERATION;

    cll->limit = limit;

    return DS_OK;
}

int cll_max(CircularLinkedList cll)
{
    int result;

    if (cll == NULL)
        return 0;

    if (cll_empty(cll))
        return 0;

    CircularLinkedNode scan = cll->cursor;

    result = scan->data;

    for (size_t i = 0; i < cll->length; i++)
    {
        if (scan->data > result)
            result = scan->data;

        scan = scan->next;
    }

    return result;
}

int cll_min(CircularLinkedList cll)
{
    int result;

    if (cll == NULL)
        return 0;

    if (cll_empty(cll))
        return 0;

    CircularLinkedNode scan = cll->cursor;

    result = scan->data;

    for (size_t i = 0; i < cll->length; i++)
    {
        if (scan->data < result)
            result = scan->data;

        scan = scan->next;
    }

    return result;
}

int cll_peek_next(CircularLinkedList cll)
{
    if (cll == NULL)
        return 0;

    if (cll_empty(cll))
        return 0;

    return cll->cursor->next->data;
}

int cll_peek(CircularLinkedList cll)
{
    if (cll == NULL)
        return 0;

    if (cll_empty(cll))
        return 0;

    return cll->cursor->data;
}

int cll_peek_prev(CircularLinkedList cll)
{
    if (cll == NULL)
        return 0;

    if (cll_empty(cll))
        return 0;

    return cll->cursor->prev->data;
}

Status cll_copy(CircularLinkedList cll, CircularLinkedList *result)
{
    if (cll == NULL)
        return DS_ERR_NULL_POINTER;

    Status st = cll_init(result);

    if (st != DS_OK)
        return st;

    if (cll_empty(cll))
        return DS_OK;

    CircularLinkedNode scan = cll->cursor;

    for (size_t i = 0; i < cll->length; i++)
    {
        st = cll_insert_before(*result, scan->data);

        if (st != DS_OK)
            return st;

        scan = scan->next;
    }

    return DS_OK;
}

// NOT EXPOSED API

Status cll_make_node(CircularLinkedNode *node, int value)
{
    *node = malloc(sizeof(CircularLinkedNode_t));

    if (!(*node))
        return DS_ERR_ALLOC;

    (*node)->next = NULL;
    (*node)->prev = NULL;

    (*node)->data = value;

    return DS_OK;
}

Status cll_delete_node(CircularLinkedNode *node)
{
    if ((*node) == NULL)
        return DS_ERR_NULL_POINTER;

    free(*node);

    *node = NULL;

    return DS_OK;
}

// END OF NOT EXPOSED API