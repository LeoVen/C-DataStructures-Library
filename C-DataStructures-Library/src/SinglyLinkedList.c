/**
 * @file SinglyLinkedList.c
 *
 * @author Leonardo Vencovsky (https://github.com/LeoVen)
 *
 * @date 25/09/2018
 */

#include "SinglyLinkedList.h"

// NOT EXPOSED API

typedef struct SinglyLinkedNode
{
    int data;					   /*!< Node's data */
    struct SinglyLinkedNode *next; /*!< Pointer to the next node on the list */
} SinglyLinkedNode;

Status sll_make_node(SinglyLinkedNode **node, int value);
Status sll_delete_node(SinglyLinkedNode **node);
Status sll_get_node_at(SinglyLinkedList *sll, SinglyLinkedNode **result, size_t position);

// END OF NOT EXPOSED API

Status sll_init(SinglyLinkedList **sll)
{
    (*sll) = malloc(sizeof(SinglyLinkedList));

    if (!(*sll))
        return DS_ERR_ALLOC;

    (*sll)->length = 0;
    (*sll)->head = NULL;
    (*sll)->tail = NULL;

    return DS_OK;
}

Status sll_insert_head(SinglyLinkedList *sll, int value)
{
    if (sll == NULL)
        return DS_ERR_NULL_POINTER;

    SinglyLinkedNode *node;

    Status st = sll_make_node(&node, value);

    if (st != DS_OK)
        return st;

    if (!node)
        return DS_ERR_ALLOC;

    if (sll_empty(sll))
    {
        sll->head = node;
        sll->tail = node;
    }
    else
    {
        node->next = sll->head;
        sll->head = node;
    }

    (sll->length)++;

    return DS_OK;
}

Status sll_insert_at(SinglyLinkedList *sll, int value, size_t position)
{
    if (sll == NULL)
        return DS_ERR_NULL_POINTER;

    if (position > sll->length)
        return DS_ERR_INVALID_POSITION;

    Status st;

    if (position == 0)
    {
        st = sll_insert_head(sll, value);

        if (st != DS_OK)
            return st;

        return DS_OK;
    }
    else if (position == sll->length)
    {
        st = sll_insert_tail(sll, value);

        if (st != DS_OK)
            return st;

        return DS_OK;
    }
    else
    {
        SinglyLinkedNode *node = NULL;

        st = sll_make_node(&node, value);

        if (st != DS_OK)
            return st;

        if (!node)
            return DS_ERR_ALLOC;

        SinglyLinkedNode *curr = NULL;

        st = sll_get_node_at(sll, &curr, position - 1);

        if (st != DS_OK)
            return st;

        node->next = curr->next;
        curr->next = node;

        (sll->length)++;

        return DS_OK;
    }
}

Status sll_insert_tail(SinglyLinkedList *sll, int value)
{
    if (sll == NULL)
        return DS_ERR_NULL_POINTER;

    SinglyLinkedNode *node;

    Status st = sll_make_node(&node, value);

    if (st != DS_OK)
        return st;

    if (!node)
        return DS_ERR_ALLOC;

    if (sll_empty(sll))
    {
        sll->head = node;
        sll->tail = node;
    }
    else
    {
        sll->tail->next = node;
        sll->tail = node;
    }

    (sll->length)++;

    return DS_OK;
}

Status sll_remove_head(SinglyLinkedList *sll, int *result)
{
    if (sll == NULL)
        return DS_ERR_NULL_POINTER;

    if (sll_empty(sll))
        return DS_ERR_INVALID_OPERATION;

    SinglyLinkedNode *node = sll->head;

    *result = node->data;

    sll->head = sll->head->next;

    sll_delete_node(&node);

    (sll->length)--;

    if (sll_empty(sll))
    {
        sll->head = NULL;
        sll->tail = NULL;
    }

    return DS_OK;
}

Status sll_remove_at(SinglyLinkedList *sll, int *result, size_t position)
{
    if (sll == NULL)
        return DS_ERR_NULL_POINTER;

    if (sll_empty(sll))
        return DS_ERR_INVALID_OPERATION;

    if (position >= sll->length)
        return DS_ERR_INVALID_POSITION;

    Status st;

    if (position == 0)
    {
        st = sll_remove_head(sll, result);

        if (st != DS_OK)
            return st;

        return DS_OK;
    }
    else if (position == sll->length - 1)
    {
        st = sll_remove_tail(sll, result);

        if (st != DS_OK)
            return st;

        return DS_OK;
    }
    else
    {
        SinglyLinkedNode *prev = NULL;
        SinglyLinkedNode *curr = NULL;

        st = sll_get_node_at(sll, &prev, position - 1);

        if (st != DS_OK)
            return st;

        st = sll_get_node_at(sll, &curr, position);

        if (st != DS_OK)
            return st;

        prev->next = curr->next;

        *result = curr->data;

        sll_delete_node(&curr);

        (sll->length)--;

        if (sll_empty(sll))
        {
            sll->head = NULL;
            sll->tail = NULL;
        }

        return DS_OK;
    }
}

Status sll_remove_tail(SinglyLinkedList *sll, int *result)
{
    if (sll == NULL)
        return DS_ERR_NULL_POINTER;

    if (sll_empty(sll))
        return DS_ERR_INVALID_OPERATION;

    SinglyLinkedNode *prev = NULL;
    SinglyLinkedNode *curr = sll->head;

    while (curr->next != NULL)
    {
        prev = curr;
        curr = curr->next;
    }

    *result = curr->data;

    if (prev == NULL)
    {
        sll_delete_node(&curr);

        sll->head = NULL;
        sll->tail = NULL;
    }
    else
    {
        prev->next = NULL;
        sll->tail = prev;

        sll_delete_node(&curr);
    }

    (sll->length)--;

    if (sll_empty(sll))
    {
        sll->head = NULL;
        sll->tail = NULL;
    }

    return DS_OK;
}

// Only for primitive types
Status sll_update(SinglyLinkedList *sll, int value, size_t position)
{
    if (sll == NULL)
        return DS_ERR_NULL_POINTER;

    if (sll_empty(sll))
        return DS_ERR_INVALID_OPERATION;

    if (position >= sll->length)
        return DS_ERR_INVALID_POSITION;

    SinglyLinkedNode *curr = NULL;

    Status st = sll_get_node_at(sll, &curr, position);

    if (st != DS_OK)
        return st;

    curr->data = value;

    return DS_OK;
}

Status sll_get(SinglyLinkedList *sll, int *result, size_t position)
{
    if (sll == NULL)
        return DS_ERR_NULL_POINTER;

    if (sll_empty(sll))
        return DS_ERR_INVALID_OPERATION;

    if (position >= sll->length)
        return DS_ERR_INVALID_POSITION;

    *result = 0;

    SinglyLinkedNode *curr = NULL;

    Status st = sll_get_node_at(sll, &curr, position);

    if (st != DS_OK)
        return st;

    *result = curr->data;

    return DS_OK;
}

Status sll_display(SinglyLinkedList *sll)
{
    if (sll == NULL)
        return DS_ERR_NULL_POINTER;

    if (sll_empty(sll))
    {
        printf("\nSingly Linked List\n[ empty ]\n");

        return DS_OK;
    }

    SinglyLinkedNode *scan = sll->head;

    printf("\nSingly Linked List\n");

    while (scan != NULL)
    {
        printf(" %d ->", scan->data);

        scan = scan->next;
    }

    printf(" NULL\n");

    return DS_OK;
}

Status sll_display_array(SinglyLinkedList *sll)
{
    if (sll == NULL)
        return DS_ERR_NULL_POINTER;

    if (sll_empty(sll))
    {
        printf("\n[ empty ]\n");

        return DS_OK;
    }

    SinglyLinkedNode *scan = sll->head;

    printf("\n[ ");

    while (scan->next != NULL)
    {
        printf("%d, ", scan->data);

        scan = scan->next;
    }

    printf("%d ]\n", scan->data);

    return DS_OK;
}

Status sll_display_raw(SinglyLinkedList *sll)
{
    if (sll == NULL)
        return DS_ERR_NULL_POINTER;

    if (sll_empty(sll))
        return DS_ERR_INVALID_OPERATION;

    SinglyLinkedNode *scan = sll->head;

    printf("\n");

    while (scan != NULL)
    {
        printf("%d ", scan->data);

        scan = scan->next;
    }

    printf("\n");

    return DS_OK;
}

Status sll_delete(SinglyLinkedList **sll)
{
    if ((*sll) == NULL)
        return DS_ERR_NULL_POINTER;

    SinglyLinkedNode *prev = (*sll)->head;

    Status st;

    while ((*sll)->head != NULL)
    {
        (*sll)->head = (*sll)->head->next;

        st = sll_delete_node(&prev);

        if (st != DS_OK)
            return st;

        prev = (*sll)->head;
    }

    free((*sll));

    (*sll) = NULL;

    return DS_OK;
}

Status sll_erase(SinglyLinkedList **sll)
{
    if ((*sll) == NULL)
        return DS_ERR_NULL_POINTER;

    Status st = sll_delete(sll);

    if (st != DS_OK)
        return st;

    st = sll_init(sll);

    if (st != DS_OK)
        return st;

    return DS_OK;
}

bool sll_contains(SinglyLinkedList *sll, int key)
{
    if (sll == NULL)
        return false;

    if (sll_empty(sll))
        return false;

    SinglyLinkedNode *scan = sll->head;

    while (scan != NULL)
    {
        if (scan->data == key)
            return true;

        scan = scan->next;
    }

    return false;
}

bool sll_empty(SinglyLinkedList *sll)
{
    return (sll->length == 0 || sll->head == NULL);
}

size_t sll_length(SinglyLinkedList *sll)
{
    if (sll == NULL)
        return 0;

    return sll->length;
}

Status sll_link(SinglyLinkedList *sll1, SinglyLinkedList *sll2)
{
    if (sll1 == NULL || sll2 == NULL)
        return DS_ERR_NULL_POINTER;

    if (sll_empty(sll2))
        return DS_ERR_INVALID_OPERATION;

    if (sll_empty(sll1))
    {
        sll1->head = sll2->head;
        sll1->tail = sll2->tail;

        sll1->length = sll2->length;
    }
    else
    {
        sll1->tail->next = sll2->head;
        sll1->tail = sll2->tail;

        sll1->length += sll2->length;
    }

    sll2->head = NULL;
    sll2->tail = NULL;

    sll2->length = 0;

    return DS_OK;
}

Status sll_link_at(SinglyLinkedList *sll1, SinglyLinkedList *sll2, size_t position)
{
    if (sll1 == NULL || sll2 == NULL)
        return DS_ERR_NULL_POINTER;

    if (position > sll1->length)
        return DS_ERR_INVALID_POSITION;

    if (sll_empty(sll1) || sll_empty(sll2))
        return DS_ERR_INVALID_OPERATION;

    if (position == 0)
    {
        sll2->tail->next = sll1->head;

        sll1->head = sll2->head;
    }
    else if (position == sll1->length)
    {
        sll1->tail->next = sll2->head;
        sll1->tail = sll2->tail;
    }
    else
    {
        SinglyLinkedNode *prev;

        sll_get_node_at(sll1, &prev, position - 1);

        sll2->tail->next = prev->next;

        prev->next = sll2->head;
    }

    sll2->head = NULL;
    sll2->tail = NULL;

    sll1->length += sll2->length;
    sll2->length = 0;

    return DS_OK;
}

Status sll_unlink(SinglyLinkedList *sll, SinglyLinkedList *result, size_t position)
{
    if (sll == NULL)
        return DS_ERR_NULL_POINTER;

    if (!sll_empty(result))
        return DS_ERR_INVALID_OPERATION;

    if (position >= sll->length)
        return DS_ERR_INVALID_POSITION;

    size_t len = sll_length(sll);

    if (position == 0)
    {
        result->head = sll->head;
        result->tail = sll->tail;

        result->length = sll->length;

        result->tail = NULL;
        result->head = NULL;
    }
    else
    {
        SinglyLinkedNode *prev;

        Status st = sll_get_node_at(sll, &prev, position - 1);

        if (st != DS_OK)
            return st;

        result->head = prev->next;
        result->tail = sll->tail;

        sll->tail = prev;

        prev->next = NULL;
    }

    sll->length = position;

    result->length = len - position;

    return DS_OK;
}

//Status sll_unlink_at(SinglyLinkedList *sll, SinglyLinkedList *result, size_t position1, size_t position2);

Status sll_copy(SinglyLinkedList *sll, SinglyLinkedList **result)
{
    *result = NULL;

    if (sll == NULL)
        return DS_ERR_NULL_POINTER;

    if (sll_empty(sll))
        return DS_ERR_INVALID_OPERATION;

    Status st = sll_init(result);

    if (st != DS_OK)
        return st;

    SinglyLinkedNode *scan = sll->head;

    while (scan != NULL)
    {
        st = sll_insert_tail(*result, scan->data);

        if (st != DS_OK)
            return st;

        scan = scan->next;
    }

    return DS_OK;
}

Status sll_reverse(SinglyLinkedList *sll)
{
    if (sll == NULL)
        return DS_ERR_NULL_POINTER;

    if (sll->length < 2)
        return DS_ERR_INVALID_OPERATION;

    SinglyLinkedNode *prev = NULL;
    SinglyLinkedNode *curr = sll->head;
    SinglyLinkedNode *next = NULL;

    sll->tail = sll->head;

    while (curr != NULL)
    {
        next = curr->next;

        curr->next = prev;

        prev = curr;

        curr = next;
    }

    sll->head = prev;

    return DS_OK;
}

// NOT EXPOSED API

Status sll_make_node(SinglyLinkedNode **node, int value)
{
    (*node) = malloc(sizeof(SinglyLinkedNode));

    if (!(*node))
        return DS_ERR_ALLOC;

    (*node)->next = NULL;
    (*node)->data = value;

    return DS_OK;
}

Status sll_delete_node(SinglyLinkedNode **node)
{
    if (*node == NULL)
        return DS_ERR_NULL_POINTER;

    free(*node);

    (*node) = NULL;

    return DS_OK;
}

Status sll_get_node_at(SinglyLinkedList *sll, SinglyLinkedNode **result, size_t position)
{
    if (sll == NULL)
        return DS_ERR_NULL_POINTER;

    if (sll_empty(sll))
        return DS_ERR_INVALID_OPERATION;

    if (position >= sll->length)
        return DS_ERR_INVALID_POSITION;

    (*result) = sll->head;

    for (size_t i = 0; i < position; i++)
    {

        if ((*result) == NULL)
            return DS_ERR_ITER;

        (*result) = (*result)->next;
    }

    return DS_OK;
}

// END OF NOT EXPOSED API