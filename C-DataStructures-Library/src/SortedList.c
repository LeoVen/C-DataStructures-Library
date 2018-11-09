/**
 * @file SortedList.c
 * 
 * @author Leonardo Vencovsky (https://github.com/LeoVen)
 *
 * @date 08/11/2018
 */

#include "SortedList.h"

/// This is a generic sorted doubly-linked list. Its elements can be added in
/// ASCENDING or DESCENDING order. This property can be set when creating a new
/// list or using sli_set_order(). You can also limit its length using
/// sli_set_limit(). To remove this limitation simply set the limit to a value
/// less than or equal to 0.
///
/// To initialize a list use sli_init(). This only initializes the structure.
/// If you don't set the proper functions later you won't be able to insert
/// elements, copy the list, display the list or even delete it. If you want to
/// initialize it completely, use instead sli_create(). Here you must pass in
/// default functions (compare, copy, display and free), making a complete
/// list.
///
/// To add an element to the list use sli_insert(). To remove, you have three
/// options: sli_remove() that removes an element at a given position;
/// sli_remove_max() that removes the biggest element; and sli_remove_min()
/// that removes the smallest element.
///
/// To delete a list use sli_free(). This completely frees all elements and
/// sets the list pointer to NULL. Note that if you haven't set a delete
/// function you won't be able to delete the list or its elements. You must set
/// a delete function that will be responsible for freeing from memory all
/// elements.
struct SortedList_s
{
    /// \brief List length.
    ///
    /// List current amount of elements linked between the \c head and \c tail
    /// pointers.
    index_t length;

    /// \brief List length limit.
    ///
    /// If it is set to 0 or a negative value then the list has no limit to its
    /// length. Otherwise it won't be able to have more elements than the
    /// specified value. The list is always initialized with no restrictions to
    /// its length, that is, \c limit equals 0. The user won't be able to limit
    /// the list length if it already has more elements than the specified
    /// limit.
    index_t limit;

    /// \brief Points to the first Node on the list.
    ///
    /// Points to the first Node on the list or \c NULL if the list is empty.
    struct SortedListNode_s *head;

    /// \brief Points to the last Node on the list.
    ///
    /// Points to the first Node on the list or \c NULL if the list is empty.
    struct SortedListNode_s *tail;

    /// \brief Defines the order of elements.
    ///
    /// The order of elements can either be \c ASCENDING or \c DESCENDING.
    SORT_ORDER order;

    /// \brief Comparator function.
    ///
    /// A function that compares one element with another that returns an int
    /// with the following rules:
    ///
    /// - [ > 0 ] if first element is greater than the second;
    /// - [ < 0 ] if second element is greater than the first;
    /// - [  0  ] if elements are equal.
    sli_compare_f d_compare;

    /// \brief Copy function.
    ///
    /// A function that returns an exact copy of an element.
    sli_copy_f d_copy;

    /// \brief Display function.
    ///
    /// A function that displays an element in the console. Useful for
    /// debugging.
    sli_display_f d_display;

    /// \brief Deallocator function.
    ///
    /// A function that completely frees an element from memory.
    sli_free_f d_free;
};

/// This node is an implementation detail and should never be used by the user.
struct SortedListNode_s
{
    /// Data pointer.
    void *data;

    /// Next node on the list.
    struct SortedListNode_s *next;

    /// Previous node on the list.
    struct SortedListNode_s *prev;
};

/// A type for a sorted list node.
typedef struct SortedListNode_s SortedListNode_t;

/// A pointer type for a sorted list node.
typedef struct SortedListNode_s *SortedListNode;

///////////////////////////////////////////////////// NOT EXPOSED FUNCTIONS ///

Status sli_make_node(SortedListNode *node, void *data);

Status sli_free_node(SortedListNode *node, sli_free_f free_f);

////////////////////////////////////////////// END OF NOT EXPOSED FUNCTIONS ///

Status sli_init(SortedList *list, SORT_ORDER order)
{
    *list = malloc(sizeof(SortedList_t));

    if (!(*list))
        return DS_ERR_ALLOC;

    (*list)->length = 0;
    (*list)->limit = 0;

    (*list)->order = order;

    (*list)->head = NULL;
    (*list)->tail = NULL;

    (*list)->d_compare = NULL;
    (*list)->d_copy = NULL;
    (*list)->d_display = NULL;
    (*list)->d_free = NULL;

    return DS_OK;
}

Status sli_create(SortedList *list, SORT_ORDER order, sli_compare_f compare_f, sli_copy_f copy_f,
        sli_display_f display_f, sli_free_f free_f)
{
    *list = malloc(sizeof(SortedList_t));

    if (!(*list))
        return DS_ERR_ALLOC;

    (*list)->length = 0;
    (*list)->limit = 0;

    (*list)->order = order;

    (*list)->head = NULL;
    (*list)->tail = NULL;

    (*list)->d_compare = compare_f;
    (*list)->d_copy = copy_f;
    (*list)->d_display = display_f;
    (*list)->d_free = free_f;

    return DS_OK;
}

Status sli_free(SortedList *list)
{
    if (*list == NULL)
        return DS_ERR_NULL_POINTER;

    if ((*list)->d_free == NULL)
        return DS_ERR_INCOMPLETE_TYPE;

    SortedListNode prev = (*list)->head;

    Status st;

    while ((*list)->head != NULL)
    {
        (*list)->head = (*list)->head->next;

        st = sli_free_node(&prev, (*list)->d_free);

        if (st != DS_OK)
            return st;

        prev = (*list)->head;
    }

    free((*list));

    (*list) = NULL;

    return DS_OK;
}

Status sli_erase(SortedList *list)
{
    if (*list == NULL)
        return DS_OK;

    SortedList new_list;

    Status st = sli_create(&new_list, (*list)->order, (*list)->d_compare,
                           (*list)->d_copy, (*list)->d_display, (*list)->d_free);

    if (st !=  DS_OK)
        return st;

    st = sli_free(list);

    // Probably didn't set the delete function...
    if (st !=  DS_OK)
    {
        free(new_list);

        return st;
    }

    *list = new_list;

    return DS_OK;
}

Status sli_set_func_compare(SortedList list, sli_compare_f function)
{
    if (list == NULL)
        return DS_ERR_NULL_POINTER;

    list->d_compare = function;

    return DS_OK;
}

Status sli_set_func_copy(SortedList list, sli_copy_f function)
{
    if (list == NULL)
        return DS_ERR_NULL_POINTER;

    list->d_copy = function;

    return DS_OK;
}

Status sli_set_func_display(SortedList list, sli_display_f function)
{
    if (list == NULL)
        return DS_ERR_NULL_POINTER;

    list->d_display = function;

    return DS_OK;
}

Status sli_set_func_free(SortedList list, sli_free_f function)
{
    if (list == NULL)
        return DS_ERR_NULL_POINTER;

    list->d_free = function;

    return DS_OK;
}

Status sli_set_order(SortedList list, SORT_ORDER order)
{
    if (list == NULL)
        return DS_ERR_NULL_POINTER;

    if (!sli_empty(list))
        return DS_ERR_INVALID_OPERATION;

    list->order = order;

    return DS_OK;
}

Status sli_set_limit(SortedList list, index_t limit)
{
    if (list == NULL)
        return DS_ERR_NULL_POINTER;

    // The new limit can't be lower than the list's current length.
    if (list->length > limit && limit > 0)
        return DS_ERR_INVALID_OPERATION;

    list->limit = limit;

    return DS_OK;
}

index_t sli_length(SortedList list)
{
    if (list == NULL)
        return -1;

    return list->length;
}

index_t sli_limit(SortedList list)
{
    if (list == NULL)
        return -1;

    return list->limit;
}

SORT_ORDER sli_order(SortedList list)
{
    if (list == NULL)
        return 0;

    return list->order;
}

Status sli_insert(SortedList list, void *element)
{
    if (list == NULL)
        return DS_ERR_NULL_POINTER;

    if (list->d_compare == NULL)
        return DS_ERR_INCOMPLETE_TYPE;

    if (sli_full(list))
        return DS_ERR_FULL;

    SortedListNode node;

    Status st = sli_make_node(&node, element);

    if (st != DS_OK)
        return st;

    // First node
    if (sli_empty(list))
    {
        list->head = node;
        list->tail = node;
    }
    // Insert node in its position
    else
    {
        SortedListNode scan = list->head;
        SortedListNode before = NULL;

        if (list->order == ASCENDING)
        {
            // Insert 'head'. Change list->head.
            if (list->d_compare(node->data, list->head->data) <= 0)
            {
                // Current smallest element
                node->next = list->head;

                list->head->prev = node;

                list->head = node;
            }
            else
            {
                while (scan != NULL && list->d_compare(node->data, scan->data) > 0)
                {
                    before = scan;

                    scan = scan->next;
                }

                // Insert 'tail'. Change list->tail.
                if (scan == NULL)
                {
                    // Current biggest element
                    node->prev = before;

                    before->next = node;

                    list->tail = node;
                }
                // Insert at the middle of the list.
                else
                {
                    before->next = node;
                    scan->prev = node;

                    node->next = scan;
                    node->prev = before;
                }
            }
        }
        // Defaults to DESCENDING
        else
        {
            // Insert 'head'. Change list->head.
            if (list->d_compare(node->data, list->head->data) >= 0)
            {
                // Current biggest element
                node->next = list->head;

                list->head->prev = node;

                list->head = node;
            }
            else
            {
                while (scan != NULL && list->d_compare(node->data, scan->data) < 0)
                {
                    before = scan;

                    scan = scan->next;
                }

                // Insert 'tail'. Change list->tail.
                if (scan == NULL)
                {
                    // Current smallest element
                    node->prev = before;

                    before->next = node;

                    list->tail = node;
                }
                // Insert at the middle of the list.
                else
                {
                    before->next = node;

                    scan->prev = node;

                    node->next = scan;
                    node->prev = before;
                }
            }
        }
    }

    list->length++;

    return DS_OK;
}

//Status sli_remove(SortedList list, void **result, index_t position);
//
//Status sli_remove_max(SortedList list, void **result);
//
//Status sli_remove_min(SortedList list, void **result);

bool sli_full(SortedList list)
{
    return list->limit > 0 && list->length >= list->limit;
}

bool sli_empty(SortedList list)
{
    return list->length == 0;
}

void *sli_max(SortedList list)
{
    if (list == NULL)
        return NULL;

    if (sli_empty(list))
        return NULL;

    if (list->order == ASCENDING)
        return list->tail->data;

    return list->head->data;
}

void *sli_min(SortedList list)
{
    if (list == NULL)
        return NULL;

    if (sli_empty(list))
        return NULL;

    if (list->order == ASCENDING)
        return list->head->data;

    return list->tail->data;
}

index_t sli_index_first(SortedList list, void *key)
{
    if (list == NULL)
        return -1;

    if (sli_empty(list))
        return -1;

    SortedListNode scan = list->head;

    index_t index = 0;

    while (scan != NULL)
    {
        if (list->d_compare(scan->data, key) == 0)
            return index;

        index++;

        scan = scan->next;
    }

    return -1;
}

index_t sli_index_last(SortedList list, void *key)
{
    if (list == NULL)
        return -1;

    if (sli_empty(list))
        return -1;

    SortedListNode scan = list->tail;

    index_t index = 0;

    while (scan != NULL)
    {
        if (list->d_compare(scan->data, key) == 0)
            return list->length - 1 - index;

        index++;

        scan = scan->prev;
    }

    return -1;
}

//Status sli_reverse(SortedList list);

//Status sli_merge(SortedList list1, SortedList list2);
//
//Status sli_unlink(SortedList list, SortedList *result, index_t position);
//
//Status sli_unlink_at(SortedList list, SortedList *result, index_t start, index_t end);

Status sli_display(SortedList list)
{
    if (list == NULL)
        return DS_ERR_NULL_POINTER;

    if (list->d_display == NULL)
        return DS_ERR_INCOMPLETE_TYPE;

    if (sli_empty(list))
    {
        printf("\nSorted List\n[ empty ]\n");

        return DS_OK;
    }

    SortedListNode scan = list->head;

    printf("\nSorted List\nNULL <-> ");

    while (scan != NULL)
    {
        list->d_display(scan->data);

        printf(" <-> ");

        scan = scan->next;
    }

    printf(" NULL\n");

    return DS_OK;
}

Status sli_display_array(SortedList list)
{
    if (list == NULL)
        return DS_ERR_NULL_POINTER;

    if (list->d_display == NULL)
        return DS_ERR_INCOMPLETE_TYPE;

    if (sli_empty(list))
    {
        printf("\n[ empty ]\n");

        return DS_OK;
    }

    SortedListNode scan = list->head;

    printf("\nSorted List\n[ ");

    while (scan->next != NULL)
    {
        list->d_display(scan->data);

        printf(", ");

        scan = scan->next;
    }

    list->d_display(scan->data);

    printf(" ]\n");

    return DS_OK;
}

Status sli_display_raw(SortedList list)
{
    if (list == NULL)
        return DS_ERR_NULL_POINTER;

    if (list->d_display == NULL)
        return DS_ERR_INCOMPLETE_TYPE;

    if (sli_empty(list))
    {
        printf("\n");

        return DS_OK;
    }

    SortedListNode scan = list->head;

    printf("\n");

    while (scan->next != NULL)
    {
        list->d_display(scan->data);

        printf(" ");

        scan = scan->next;
    }

    list->d_display(scan->data);

    printf("\n");

    return DS_OK;
}

///////////////////////////////////////////////////// NOT EXPOSED FUNCTIONS ///

Status sli_make_node(SortedListNode *node, void *data)
{
    *node = malloc(sizeof(SortedListNode_t));

    if (!(*node))
        return DS_ERR_ALLOC;

    (*node)->data = data;

    (*node)->next = NULL;
    (*node)->prev = NULL;

    return DS_OK;
}

Status sli_free_node(SortedListNode *node, sli_free_f free_f)
{
    if (*node == NULL)
        return DS_ERR_NULL_POINTER;

    free_f((*node)->data);

    free(*node);

    *node = NULL;

    return DS_OK;
}

////////////////////////////////////////////// END OF NOT EXPOSED FUNCTIONS ///

///////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////// Iterator ///
///////////////////////////////////////////////////////////////////////////////

struct SortedListIterator_s
{
    /// \brief Target SortedList.
    ///
    /// Target SortedList. The iterator might need to use some informations
    /// provided by the list.
    struct SortedList_s *target;

    /// \brief Current element.
    ///
    /// Points to the current node. The iterator is always initialized with the
    /// cursor pointing to the start (head) of the list.
    struct SortedListNode_s *cursor;
};

///////////////////////////////////////////////////// NOT EXPOSED FUNCTIONS ///



////////////////////////////////////////////// END OF NOT EXPOSED FUNCTIONS ///

Status sli_iter_init(SortedListIterator *iter, SortedList target)
{
    *iter = malloc(sizeof(SortedListIterator_t));

    if (!(*iter))
        return DS_ERR_ALLOC;

    (*iter)->target = target;
    (*iter)->cursor = target->head;

    return DS_OK;
}

Status sli_iter_free(SortedListIterator *iter)
{
    if (*iter == NULL)
        return DS_ERR_NULL_POINTER;

    free(*iter);

    *iter = NULL;

    return DS_OK;
}