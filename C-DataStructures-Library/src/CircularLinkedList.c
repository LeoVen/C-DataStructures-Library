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

Status cll_make_node(CircularLinkedNode *cln, int value);

Status cll_delete_node(CircularLinkedNode *cln);

Status cll_insert_first(CircularLinkedList cll, int value);

Status cll_remove_last(CircularLinkedList cll, int *result);

// END OF NOT EXPOSED API

Status cll_init(CircularLinkedList *cll)
{
    (*cll) = malloc(sizeof(CircularLinkedList_t));

    if (!(*cll))
        return DS_ERR_ALLOC;

    (*cll)->curr = NULL;

    (*cll)->length = 0;

    return DS_OK;
}

Status cll_insert_after(CircularLinkedList cll, int value)
{
    if (cll == NULL)
        return DS_ERR_NULL_POINTER;

    Status st;

    if (cll_empty(cll))
    {
        st = cll_insert_first(cll, value);

        if (st != DS_OK)
            return st;
    }
    else
    {
        CircularLinkedNode cln;

        st = cll_make_node(&cln, value);

        if (st != DS_OK)
            return st;

        cln->next = cll->curr->next;

        cll->curr->next = cln;

        (cll->length)++;
    }

    return DS_OK;
}

Status cll_insert_before(CircularLinkedList cll, int value)
{
    if (cll == NULL)
        return DS_ERR_NULL_POINTER;

    Status st;

    if (cll_empty(cll))
    {
        st = cll_insert_first(cll, value);

        if (st != DS_OK)
            return st;
    }
    else
    {
        CircularLinkedNode cln;

        st = cll_make_node(&cln, value);

        if (st != DS_OK)
            return st;

        st = cll_iter_before(cll);

        if (st != DS_OK)
            return st;

        cln->next = cll->curr->next;

        cll->curr->next = cln;

        cll->curr = cln->next;

        (cll->length)++;
    }

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
        st = cll_remove_last(cll, result);

        if (st != DS_OK)
            return st;
    }
    else
    {
        CircularLinkedNode node = cll->curr->next;

        *result = node->data;

        cll->curr->next = node->next;

        st = cll_delete_node(&node);

        if (st != DS_OK)
            return st;

        (cll->length)--;
    }

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
        st = cll_remove_last(cll, result);

        if (st != DS_OK)
            return st;
    }
    else
    {
        st = cll_iter_before(cll);

        if (st != DS_OK)
            return st;

        CircularLinkedNode node = cll->curr->next;

        *result = node->data;

        cll->curr->next = node->next;

        st = cll_delete_node(&node);

        if (st != DS_OK)
            return st;

        (cll->length)--;
    }

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
        st = cll_remove_last(cll, result);

        if (st != DS_OK)
            return st;
    }
    else
    {
        st = cll_iter(cll, cll->length - 2);

        if (st != DS_OK)
            return st;

        CircularLinkedNode node = cll->curr->next;

        *result = node->data;

        cll->curr->next = node->next;

        st = cll_delete_node(&node);

        if (st != DS_OK)
            return st;

        cll->curr = cll->curr->next;

        (cll->length)--;
    }

    return DS_OK;
}

Status cll_iter_next(CircularLinkedList cll)
{
    if (cll == NULL)
        return DS_ERR_NULL_POINTER;

    if (cll_empty(cll))
        return DS_ERR_INVALID_OPERATION;

    cll->curr = cll->curr->next;

    return DS_OK;
}

Status cll_iter(CircularLinkedList cll, size_t positions)
{
    if (cll == NULL)
        return DS_ERR_NULL_POINTER;

    if (cll_empty(cll))
        return DS_ERR_INVALID_OPERATION;

    for (size_t i = 0; i < positions; i++)
        cll->curr = cll->curr->next;

    return DS_OK;
}

Status cll_iter_before(CircularLinkedList cll)
{
    if (cll == NULL)
        return DS_ERR_NULL_POINTER;

    if (cll_empty(cll))
        return DS_ERR_INVALID_OPERATION;

    for (size_t i = 0; i < cll->length - 1; i++)
        cll->curr = cll->curr->next;

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

    CircularLinkedNode scan = cll->curr;

    printf("\nCircular Linked List\n ->");

    for (size_t i = 0; i < cll->length; i++)
    {
        printf(" %d ->", scan->data);

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

    CircularLinkedNode scan = cll->curr;

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

    CircularLinkedNode scan = cll->curr;

    printf("\n");

    for (size_t i = 0; i < cll->length; i++)
    {
        printf(" %d", scan->data);

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

    CircularLinkedNode prev = (*cll)->curr;

    for (size_t i = 0; i < (*cll)->length; i++)
    {
        (*cll)->curr = (*cll)->curr->next;

        st = cll_delete_node(&prev);

        if (st != DS_OK)
            return st;

        prev = (*cll)->curr;
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

size_t cll_length(CircularLinkedList cll)
{
    if (cll == NULL)
        return 0;

    return cll->length;
}

bool cll_contains(CircularLinkedList cll, int key)
{
    if (cll == NULL)
        return false;

    if (cll_empty(cll))
        return false;

    CircularLinkedNode scan = cll->curr;

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

int cll_max(CircularLinkedList cll)
{
    int result;

    if (cll == NULL)
        return 0;

    if (cll_empty(cll))
        return 0;

    CircularLinkedNode scan = cll->curr;

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

    CircularLinkedNode scan = cll->curr;

    result = scan->data;

    for (size_t i = 0; i < cll->length; i++)
    {
        if (scan->data < result)
            result = scan->data;

        scan = scan->next;
    }

    return result;
}

int cll_current(CircularLinkedList cll)
{
    if (cll == NULL)
        return 0;

    if (cll_empty(cll))
        return 0;

    return cll->curr->data;
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

    CircularLinkedNode scan = cll->curr;

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

Status cll_make_node(CircularLinkedNode *cln, int value)
{
    *cln = malloc(sizeof(CircularLinkedNode_t));

    if (!(*cln))
        return DS_ERR_ALLOC;

    (*cln)->next = NULL;
    (*cln)->prev = NULL;

    (*cln)->data = value;

    return DS_OK;
}

Status cll_delete_node(CircularLinkedNode *cln)
{
    if ((*cln) == NULL)
        return DS_ERR_NULL_POINTER;

    free(*cln);

    return DS_OK;
}

Status cll_insert_first(CircularLinkedList cll, int value)
{
    if (cll == NULL)
        return DS_ERR_NULL_POINTER;

    if (cll->length != 0 || cll->curr != NULL)
        return DS_ERR_INVALID_OPERATION;

    CircularLinkedNode cln;

    Status st = cll_make_node(&cln, value);

    if (st != DS_OK)
        return st;

    cll->curr = cln;

    cln->next = cln;

    (cll->length)++;

    return DS_OK;
}

Status cll_remove_last(CircularLinkedList cll, int *result)
{
    *result = 0;

    if (cll == NULL)
        return DS_ERR_NULL_POINTER;

    if (cll->length != 1)
        return DS_ERR_INVALID_OPERATION;

    *result = cll->curr->data;

    Status st = cll_delete_node(&(cll->curr));

    if (st != DS_OK)
        return st;

    cll->curr = NULL;

    (cll->length)--;

    return DS_OK;
}

// END OF NOT EXPOSED API