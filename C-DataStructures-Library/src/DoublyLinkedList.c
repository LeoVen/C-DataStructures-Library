/**
 * @file DoublyLinkedList.c
 * 
 * @author Leonardo Vencovsky (https://github.com/LeoVen)
 *
 * @date 25/09/2018
 */

#include "DoublyLinkedList.h"

// NOT EXPOSED API

typedef struct DoublyLinkedNode_s
{
    int data;                        /*!< Node's data */
    struct DoublyLinkedNode_s *next; /*!< Pointer to the next node on the list */
    struct DoublyLinkedNode_s *prev; /*!< Pointer to the previous node on the list */
} DoublyLinkedNode_t, *DoublyLinkedNode;

Status dll_make_node(DoublyLinkedNode *node, int value);

Status dll_delete_node(DoublyLinkedNode *node);

Status dll_get_node_at(DoublyLinkedList dll, DoublyLinkedNode *result, index_t position);

// END OF NOT EXPOSED API

Status dll_init(DoublyLinkedList *dll)
{
    (*dll) = malloc(sizeof(DoublyLinkedList_t));

    if (!(*dll))
        return DS_ERR_ALLOC;

    (*dll)->length = 0;
    (*dll)->limit = 0;

    (*dll)->head = NULL;
    (*dll)->tail = NULL;

    return DS_OK;
}

Status dll_insert_head(DoublyLinkedList dll, int value)
{
    if (dll == NULL)
        return DS_ERR_NULL_POINTER;

    if (dll->limit > 0 && dll->length >= dll->limit)
        return DS_ERR_FULL;

    DoublyLinkedNode node;

    Status st = dll_make_node(&node, value);

    if (st != DS_OK)
        return st;

    if (dll_empty(dll))
    {
        dll->head = node;
        dll->tail = node;
    }
    else
    {
        node->next = dll->head;

        dll->head->prev = node;

        dll->head = node;
    }

    (dll->length)++;

    return DS_OK;
}

Status dll_insert_at(DoublyLinkedList dll, int value, index_t position)
{
    if (dll == NULL)
        return DS_ERR_NULL_POINTER;

    if (dll->limit > 0 && dll->length >= dll->limit)
        return DS_ERR_FULL;

    if (position > dll->length)
        return DS_ERR_OUT_OF_RANGE;

    if (position < 0)
        return DS_ERR_NEGATIVE_VALUE;

    Status st;

    if (position == 0)
    {
        st = dll_insert_head(dll, value);

        if (st != DS_OK)
            return st;

        return DS_OK;
    }
    else if (position == dll->length)
    {
        st = dll_insert_tail(dll, value);

        if (st != DS_OK)
            return st;

        return DS_OK;
    }
    else
    {
        DoublyLinkedNode curr = NULL;

        st = dll_get_node_at(dll, &curr, position);

        if (st != DS_OK)
            return st;

        DoublyLinkedNode node = NULL;

        st = dll_make_node(&node, value);

        if (st != DS_OK)
            return st;

        node->prev = curr->prev;
        node->next = curr;

        curr->prev->next = node;
        curr->prev = node;

        (dll->length)++;

        return DS_OK;
    }
}

Status dll_insert_tail(DoublyLinkedList dll, int value)
{
    if (dll == NULL)
        return DS_ERR_NULL_POINTER;

    if (dll->limit > 0 && dll->length >= dll->limit)
        return DS_ERR_FULL;

    DoublyLinkedNode node;

    Status st = dll_make_node(&node, value);

    if (st != DS_OK)
        return st;

    if (dll_empty(dll))
    {
        dll->head = node;
        dll->tail = node;
    }
    else
    {
        dll->tail->next = node;

        node->prev = dll->tail;

        dll->tail = node;
    }

    (dll->length)++;

    return DS_OK;
}

Status dll_remove_head(DoublyLinkedList dll, int *result)
{
    if (dll == NULL)
        return DS_ERR_NULL_POINTER;

    if (dll_empty(dll))
        return DS_ERR_INVALID_OPERATION;

    DoublyLinkedNode node = dll->head;

    *result = node->data;

    dll->head = dll->head->next;

    if (dll->head == NULL)
        dll->tail = NULL;
    else
        dll->head->prev = NULL;

    dll_delete_node(&node);

    (dll->length)--;

    if (dll_empty(dll))
    {
        dll->head = NULL;
        dll->tail = NULL;
    }

    return DS_OK;
}

Status dll_remove_at(DoublyLinkedList dll, int *result, index_t position)
{
    if (dll == NULL)
        return DS_ERR_NULL_POINTER;

    if (dll_empty(dll))
        return DS_ERR_INVALID_OPERATION;

    if (position >= dll->length)
        return DS_ERR_OUT_OF_RANGE;

    if (position < 0)
        return DS_ERR_NEGATIVE_VALUE;

    Status st;

    if (position == 0)
    {
        st = dll_remove_head(dll, result);

        if (st != DS_OK)
            return st;

        return DS_OK;
    }
    else if (position == dll->length - 1)
    {
        st = dll_remove_tail(dll, result);

        if (st != DS_OK)
            return st;

        return DS_OK;
    }
    else
    {
        DoublyLinkedNode curr = NULL;

        st = dll_get_node_at(dll, &curr, position);

        if (st != DS_OK)
            return st;

        curr->prev->next = curr->next;
        curr->next->prev = curr->prev;

        *result = curr->data;

        dll_delete_node(&curr);

        (dll->length)--;

        if (dll_empty(dll))
        {
            dll->head = NULL;
            dll->tail = NULL;
        }

        return DS_OK;
    }
}

Status dll_remove_tail(DoublyLinkedList dll, int *result)
{
    if (dll == NULL)
        return DS_ERR_NULL_POINTER;

    if (dll_empty(dll))
        return DS_ERR_INVALID_OPERATION;

    DoublyLinkedNode node = dll->tail;

    *result = node->data;

    dll->tail = dll->tail->prev;

    if (dll->tail == NULL)
        dll->head = NULL;
    else
        dll->tail->next = NULL;

    dll_delete_node(&node);

    (dll->length)--;

    if (dll_empty(dll))
    {
        dll->head = NULL;
        dll->tail = NULL;
    }

    return DS_OK;
}

Status dll_update(DoublyLinkedList dll, int value, index_t position)
{
    if (dll == NULL)
        return DS_ERR_NULL_POINTER;

    if (dll_empty(dll))
        return DS_ERR_INVALID_OPERATION;

    if (position >= dll->length)
        return DS_ERR_OUT_OF_RANGE;

    if (position < 0)
        return DS_ERR_NEGATIVE_VALUE;

    DoublyLinkedNode curr = NULL;

    Status st = dll_get_node_at(dll, &curr, position);

    if (st != DS_OK)
        return st;

    curr->data = value;

    return DS_OK;
}

Status dll_get(DoublyLinkedList dll, int *result, index_t position)
{
    *result = 0;

    if (dll == NULL)
        return DS_ERR_NULL_POINTER;

    if (dll_empty(dll))
        return DS_ERR_INVALID_OPERATION;

    if (position >= dll->length)
        return DS_ERR_OUT_OF_RANGE;

    if (position < 0)
        return DS_ERR_NEGATIVE_VALUE;

    DoublyLinkedNode curr = NULL;

    Status st = dll_get_node_at(dll, &curr, position);

    if (st != DS_OK)
        return st;

    *result = curr->data;

    return DS_OK;
}

Status dll_display(DoublyLinkedList dll)
{
    if (dll == NULL)
        return DS_ERR_NULL_POINTER;

    if (dll_empty(dll))
    {
        printf("\nDoubly Linked List\n[ empty ]\n");

        return DS_OK;
    }

    DoublyLinkedNode scan = dll->head;

    printf("\nDoubly Linked List\nNULL <->");

    while (scan != NULL)
    {
        printf(" %d <->", scan->data);

        scan = scan->next;
    }

    printf(" NULL\n");

    return DS_OK;
}

Status dll_display_array(DoublyLinkedList dll)
{
    if (dll == NULL)
        return DS_ERR_NULL_POINTER;

    if (dll_empty(dll))
    {
        printf("\n[ empty ]\n");

        return DS_OK;
    }

    DoublyLinkedNode scan = dll->head;

    printf("\n[ ");

    while (scan->next != NULL)
    {
        printf("%d, ", scan->data);

        scan = scan->next;
    }

    printf("%d ]\n", scan->data);

    return DS_OK;
}

Status dll_display_raw(DoublyLinkedList dll)
{
    if (dll == NULL)
        return DS_ERR_NULL_POINTER;

    printf("\n");

    if (dll_empty(dll))
        return DS_OK;

    DoublyLinkedNode scan = dll->head;

    while (scan != NULL)
    {
        printf("%d ", scan->data);

        scan = scan->next;
    }

    printf("\n");

    return DS_OK;
}

Status dll_delete(DoublyLinkedList *dll)
{
    if ((*dll) == NULL)
        return DS_ERR_NULL_POINTER;

    DoublyLinkedNode prev = (*dll)->head;

    Status st;

    while ((*dll)->head != NULL)
    {
        (*dll)->head = (*dll)->head->next;

        st = dll_delete_node(&prev);

        if (st != DS_OK)
            return st;

        prev = (*dll)->head;
    }

    free((*dll));

    (*dll) = NULL;

    return DS_OK;
}

Status dll_erase(DoublyLinkedList *dll)
{
    if ((*dll) == NULL)
        return DS_ERR_NULL_POINTER;

    Status st = dll_delete(dll);

    if (st != DS_OK)
        return st;

    st = dll_init(dll);

    if (st != DS_OK)
        return st;

    return DS_OK;
}

bool dll_contains(DoublyLinkedList dll, int key)
{
    if (dll == NULL)
        return false;

    if (dll_empty(dll))
        return false;

    DoublyLinkedNode scan = dll->head;

    while (scan != NULL)
    {
        if (scan->data == key)
            return true;

        scan = scan->next;
    }

    return false;
}

bool dll_empty(DoublyLinkedList dll)
{
    return dll->length == 0;
}

index_t dll_length(DoublyLinkedList dll)
{
    if (dll == NULL)
        return -1;

    return dll->length;
}

Status dll_limit(DoublyLinkedList dll, index_t limit)
{
    if (dll == NULL)
        return DS_ERR_NULL_POINTER;

    if (dll->length > limit && limit > 0)
        return DS_ERR_INVALID_OPERATION;

    dll->limit = limit;

    return DS_OK;
}

int dll_max(DoublyLinkedList dll)
{
    if (dll == NULL)
        return 0;

    if (dll_empty(dll))
        return 0;

    DoublyLinkedNode scan = dll->head;

    int result = scan->data;

    while (scan != NULL)
    {
        if (scan->data > result)
            result = scan->data;

        scan = scan->next;
    }

    return result;
}

int dll_min(DoublyLinkedList dll)
{
    if (dll == NULL)
        return 0;

    if (dll_empty(dll))
        return 0;

    DoublyLinkedNode scan = dll->head;

    int result = scan->data;

    while (scan != NULL)
    {
        if (scan->data < result)
            result = scan->data;

        scan = scan->next;
    }

    return result;
}

index_t dll_index_first(DoublyLinkedList dll, int key)
{
    if (dll == NULL)
        return -1;

    if (dll_empty(dll))
        return -1;

    DoublyLinkedNode scan = dll->head;

    index_t index = 0;

    while (scan != NULL)
    {
        if (scan->data == key)
            return index;

        index++;

        scan = scan->next;
    }

    return -1;
}

index_t dll_index_last(DoublyLinkedList dll, int key)
{
    if (dll == NULL)
        return -1;

    if (dll_empty(dll))
        return -1;

    DoublyLinkedNode scan = dll->tail;

    index_t index = 0;

    while (scan != NULL)
    {
        if (scan->data == key)
            return dll->length - 1 - index;

        index++;

        scan = scan->prev;
    }

    return -1;
}

//Status dll_link(DoublyLinkedList dll1, DoublyLinkedList dll2)

//Status dll_link_at(DoublyLinkedList dll1, DoublyLinkedList dll2, index_t position)

//Status dll_unlink(DoublyLinkedList dll, DoublyLinkedList result, index_t position)

//Status dll_unlink_at(DoublyLinkedList dll, DoublyLinkedList result, index_t position1, index_t position2)

Status dll_copy(DoublyLinkedList dll, DoublyLinkedList *result)
{
    *result = NULL;

    if (dll == NULL)
        return DS_ERR_NULL_POINTER;

    Status st = dll_init(result);

    if (st != DS_OK)
        return st;

    if (dll_empty(dll))
        return DS_OK;

    DoublyLinkedNode scan = dll->head;

    while (scan != NULL)
    {
        st = dll_insert_tail(*result, scan->data);

        if (st != DS_OK)
            return st;

        scan = scan->next;
    }

    (*result)->limit = dll->limit;

    return DS_OK;
}

Status dll_reverse(DoublyLinkedList dll)
{
    if (dll == NULL)
        return DS_ERR_NULL_POINTER;

    if (dll->length < 2)
        return DS_ERR_INVALID_OPERATION;

    DoublyLinkedNode prev = NULL;
    DoublyLinkedNode curr = dll->head;
    DoublyLinkedNode next = NULL;

    dll->tail = dll->head;

    while (curr != NULL)
    {
        next = curr->next;

        curr->next = prev;
        curr->prev = next;

        prev = curr;

        curr = next;
    }

    dll->head = prev;

    return DS_OK;
}

// NOT EXPOSED API

Status dll_make_node(DoublyLinkedNode *node, int value)
{
    (*node) = malloc(sizeof(DoublyLinkedNode_t));

    if (!(*node))
        return DS_ERR_ALLOC;

    (*node)->next = NULL;
    (*node)->prev = NULL;
    (*node)->data = value;

    return DS_OK;
}

// This function effectively searches for a given node. If the position is
// greater than the list length the search will begin at the end of the list,
// reducing the amount of iterations needed. This effectively reduces searches
// to O(n / 2) iterations.
Status dll_get_node_at(DoublyLinkedList dll, DoublyLinkedNode *result, index_t position)
{
    *result = NULL;

    if (dll == NULL)
        return DS_ERR_NULL_POINTER;

    if (dll_empty(dll))
        return DS_ERR_INVALID_OPERATION;

    if (position < 0)
        return DS_ERR_NEGATIVE_VALUE;

    if (position >= dll->length)
        return DS_ERR_OUT_OF_RANGE;

    if (position <= dll->length / 2)
    {
        (*result) = dll->head;

        for (index_t i = 0; i < position; i++)
        {
            if ((*result) == NULL)
                return DS_ERR_ITER;

            (*result) = (*result)->next;
        }
    }
    else
    {
        (*result) = dll->tail;

        for (index_t i = dll->length - 1; i > position; i--)
        {
            if ((*result) == NULL)
                return DS_ERR_ITER;

            (*result) = (*result)->prev;
        }
    }

    return DS_OK;
}

Status dll_delete_node(DoublyLinkedNode *node)
{
    if ((*node) == NULL)
        return DS_ERR_NULL_POINTER;

    free(*node);

    (*node) = NULL;

    return DS_OK;
}

// END OF NOT EXPOSED API