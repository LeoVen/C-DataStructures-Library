/**
 * @file DoublyLinkedList.c
 * 
 * @author Leonardo Vencovsky (https://github.com/LeoVen)
 *
 * @date 25/09/2018
 */

#include "DoublyLinkedList.h"

// NOT EXPOSED API

typedef struct DoublyLinkedNode
{
    int data;                       /*!< Node's data */
    struct DoublyLinkedNode *next; /*!< Pointer to the next node on the list */
    struct DoublyLinkedNode *prev; /*!< Pointer to the previous node on the list */
} DoublyLinkedNode;

Status dll_make_node(DoublyLinkedNode **node, int value);

Status dll_delete_node(DoublyLinkedNode **node);

Status dll_get_node_at(DoublyLinkedList *dll, DoublyLinkedNode **result, size_t position);

Status dll_init(DoublyLinkedList **dll)
{
    (*dll) = malloc(sizeof(DoublyLinkedList));

    if (!(*dll))
        return DS_ERR_ALLOC;

    (*dll)->length = 0;
    (*dll)->head = NULL;
    (*dll)->tail = NULL;

    return DS_OK;
}

Status dll_insert_head(DoublyLinkedList *dll, int value)
{
    if (dll == NULL)
        return DS_ERR_NULL_POINTER;

    DoublyLinkedNode *node;

    Status st = dll_make_node(&node, value);

    if (st != DS_OK)
        return st;

    if (!node)
        return DS_ERR_ALLOC;

    if (dll_empty(dll))
    {

        dll->head = node;
        dll->tail = node;
    } else
    {

        node->next = dll->head;

        dll->head->prev = node;

        dll->head = node;
    }

    (dll->length)++;

    return DS_OK;
}

Status dll_insert_at(DoublyLinkedList *dll, int value, size_t position)
{
    if (dll == NULL)
        return DS_ERR_NULL_POINTER;

    if (position > dll->length)
        return DS_ERR_INVALID_POSITION;

    Status st;

    if (position == 0)
    {

        st = dll_insert_head(dll, value);

        if (st != DS_OK)
            return st;

        return DS_OK;
    } else if (position == dll->length)
    {

        st = dll_insert_tail(dll, value);

        if (st != DS_OK)
            return st;

        return DS_OK;
    } else
    {

        DoublyLinkedNode *node = NULL;

        st = dll_make_node(&node, value);

        if (st != DS_OK)
            return st;

        if (!node)
            return DS_ERR_ALLOC;

        DoublyLinkedNode *curr = NULL;

        st = dll_get_node_at(dll, &curr, position);

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

Status dll_insert_tail(DoublyLinkedList *dll, int value)
{
    if (dll == NULL)
        return DS_ERR_NULL_POINTER;

    DoublyLinkedNode *node;

    Status st = dll_make_node(&node, value);

    if (st != DS_OK)
        return st;

    if (!node)
        return DS_ERR_ALLOC;

    if (dll_empty(dll))
    {

        dll->head = node;
        dll->tail = node;
    } else
    {

        dll->tail->next = node;

        node->prev = dll->tail;

        dll->tail = node;
    }

    (dll->length)++;

    return DS_OK;
}

Status dll_remove_head(DoublyLinkedList *dll, int *result)
{
    if (dll == NULL)
        return DS_ERR_NULL_POINTER;

    if (dll_empty(dll))
        return DS_ERR_INVALID_OPERATION;

    DoublyLinkedNode *node = dll->head;

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

Status dll_remove_at(DoublyLinkedList *dll, int *result, size_t position)
{
    if (dll == NULL)
        return DS_ERR_NULL_POINTER;

    if (dll_empty(dll))
        return DS_ERR_INVALID_OPERATION;

    if (position >= dll->length)
        return DS_ERR_INVALID_POSITION;

    Status st;

    if (position == 0)
    {

        st = dll_remove_head(dll, result);

        if (st != DS_OK)
            return st;

        return DS_OK;
    } else if (position == dll->length - 1)
    {

        st = dll_remove_tail(dll, result);

        if (st != DS_OK)
            return st;

        return DS_OK;
    } else
    {
        DoublyLinkedNode *curr = NULL;

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

Status dll_remove_tail(DoublyLinkedList *dll, int *result)
{
    if (dll == NULL)
        return DS_ERR_NULL_POINTER;

    if (dll_empty(dll))
        return DS_ERR_INVALID_OPERATION;

    DoublyLinkedNode *node = dll->tail;

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

Status dll_update(DoublyLinkedList *dll, int value, size_t position)
{
    if (dll == NULL)
        return DS_ERR_NULL_POINTER;

    if (dll_empty(dll))
        return DS_ERR_INVALID_OPERATION;

    if (position >= dll->length)
        return DS_ERR_INVALID_POSITION;

    DoublyLinkedNode *curr = NULL;

    Status st = dll_get_node_at(dll, &curr, position);

    if (st != DS_OK)
        return st;

    curr->data = value;

    return DS_OK;
}

Status dll_get(DoublyLinkedList *dll, int *result, size_t position)
{
    if (dll == NULL)
        return DS_ERR_NULL_POINTER;

    if (dll_empty(dll))
        return DS_ERR_INVALID_OPERATION;

    if (position >= dll->length)
        return DS_ERR_INVALID_POSITION;

    *result = 0;

    DoublyLinkedNode *curr = NULL;

    Status st = dll_get_node_at(dll, &curr, position);

    if (st != DS_OK)
        return st;

    *result = curr->data;

    return DS_OK;
}

Status dll_display(DoublyLinkedList *dll)
{
    if (dll == NULL)
        return DS_ERR_NULL_POINTER;

    if (dll_empty(dll))
    {
        printf("\nDoubly Linked List\n[ empty ]\n");

        return DS_OK;
    }

    DoublyLinkedNode *scan = dll->head;

    printf("\nDoubly Linked List\nNULL <->");

    while (scan != NULL)
    {
        printf(" %d <->", scan->data);

        scan = scan->next;
    }

    printf(" NULL\n");

    return DS_OK;
}

Status dll_display_array(DoublyLinkedList *dll)
{
    if (dll == NULL)
        return DS_ERR_NULL_POINTER;

    if (dll_empty(dll))
    {
        printf("\n[ empty ]\n");

        return DS_OK;
    }

    DoublyLinkedNode *scan = dll->head;

    printf("\n[ ");

    while (scan->next != NULL)
    {
        printf("%d, ", scan->data);

        scan = scan->next;
    }

    printf("%d ]\n", scan->data);

    return DS_OK;
}

Status dll_display_raw(DoublyLinkedList *dll)
{
    if (dll == NULL)
        return DS_ERR_NULL_POINTER;

    if (dll_empty(dll))
        return DS_ERR_INVALID_OPERATION;

    DoublyLinkedNode *scan = dll->head;

    printf("\n");

    while (scan != NULL)
    {
        printf("%d ", scan->data);

        scan = scan->next;
    }

    printf("\n");

    return DS_OK;
}

Status dll_delete(DoublyLinkedList **dll)
{
    if ((*dll) == NULL)
        return DS_ERR_NULL_POINTER;

    DoublyLinkedNode *prev = (*dll)->head;

    while ((*dll)->head != NULL)
    {
        (*dll)->head = (*dll)->head->next;
        free(prev);
        prev = (*dll)->head;
    }

    free((*dll));

    (*dll) = NULL;

    return DS_OK;
}

Status dll_erase(DoublyLinkedList **dll)
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

bool dll_contains(DoublyLinkedList *dll, int key)
{
    if (dll == NULL)
        return false;

    if (dll_empty(dll))
        return false;

    DoublyLinkedNode *scan = dll->head;

    while (scan != NULL)
    {
        if (scan->data == key)
            return true;

        scan = scan->next;
    }

    return false;
}

size_t dll_length(DoublyLinkedList *dll)
{
    if (dll == NULL)
        return 0;

    return dll->length;
}

bool dll_empty(DoublyLinkedList *dll)
{
    return (dll->length == 0 || dll->head == NULL);
}

//Status dll_link(DoublyLinkedList *dll1, DoublyLinkedList *dll2)

//Status dll_link_at(DoublyLinkedList *dll1, DoublyLinkedList *dll2, size_t position)

//Status dll_unlink(DoublyLinkedList *dll, DoublyLinkedList *result, size_t position)

//Status dll_unlink_at(DoublyLinkedList *dll, DoublyLinkedList *result, size_t position1, size_t position2)

Status dll_copy(DoublyLinkedList *dll, DoublyLinkedList **result)
{
    *result = NULL;

    if (dll == NULL)
        return DS_ERR_NULL_POINTER;

    if (dll_empty(dll))
        return DS_ERR_INVALID_OPERATION;

    Status st = dll_init(result);

    if (st != DS_OK)
        return st;

    DoublyLinkedNode *scan = dll->head;

    while (scan != NULL)
    {
        st = dll_insert_tail(*result, scan->data);

        if (st != DS_OK)
            return st;

        scan = scan->next;
    }

    return DS_OK;
}

Status dll_reverse(DoublyLinkedList *dll)
{
    if (dll == NULL)
        return DS_ERR_NULL_POINTER;

    if (dll->length < 2)
        return DS_ERR_INVALID_OPERATION;

    DoublyLinkedNode *prev = NULL;
    DoublyLinkedNode *curr = dll->head;
    DoublyLinkedNode *next = NULL;

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

Status dll_make_node(DoublyLinkedNode **node, int value)
{
    (*node) = malloc(sizeof(DoublyLinkedNode));

    if (!(*node))
        return DS_ERR_ALLOC;

    (*node)->next = NULL;
    (*node)->prev = NULL;
    (*node)->data = value;

    return DS_OK;
}

Status dll_delete_node(DoublyLinkedNode **node)
{
    free(*node);

    (*node) = NULL;

    return DS_OK;
}

Status dll_get_node_at(DoublyLinkedList *dll, DoublyLinkedNode **result, size_t position)
{
    if (dll == NULL)
        return DS_ERR_NULL_POINTER;

    if (dll_empty(dll))
        return DS_ERR_INVALID_OPERATION;

    if (position >= dll->length)
        return DS_ERR_INVALID_POSITION;

    (*result) = dll->head;

    size_t i;
    for (i = 0; i < position; i++)
    {

        if ((*result) == NULL)
            return DS_ERR_ITER;

        (*result) = (*result)->next;
    }

    return DS_OK;
}

// END OF NOT EXPOSED API