/**
 * @file SortedList.c
 * 
 * @author Leonardo Vencovsky (https://github.com/LeoVen)
 *
 * @date 08/11/2018
 */

#include "SortedList.h"

/// \brief A generic sorted doubly-linked list.
///
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
    SortOrder order;

    /// \brief Comparator function.
    ///
    /// A function that compares one element with another that returns an int
    /// with the following rules:
    ///
    /// - <code>[ > 0 ]</code> if first element is greater than the second;
    /// - <code>[ < 0 ]</code> if second element is greater than the first;
    /// - <code>[ 0 ]</code> if elements are equal.
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
    /// \brief Data pointer.
    ///
    /// Points to node's data. The data needs to be dynamically allocated.
    void *data;

    /// \brief Next node on the list.
    ///
    /// Next node on the list or \c NULL if this is the tail node.
    struct SortedListNode_s *next;

    /// \brief Previous node on the list.
    ///
    /// Previous node on the list or \c NULL if this is the head node.
    struct SortedListNode_s *prev;
};

/// A type for a sorted list node.
typedef struct SortedListNode_s SortedListNode_t;

/// A pointer type for a sorted list node.
typedef struct SortedListNode_s *SortedListNode;

///////////////////////////////////////////////////// NOT EXPOSED FUNCTIONS ///

Status sli_make_node(SortedListNode *node, void *data);

Status sli_free_node(SortedListNode *node, sli_free_f free_f);

Status sli_get_node_at(SortedList list, SortedListNode *result,
        index_t position);

Status sli_insert_tail(SortedList list, void *element);

////////////////////////////////////////////// END OF NOT EXPOSED FUNCTIONS ///

Status sli_init(SortedList *list, SortOrder order)
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

Status sli_create(SortedList *list, SortOrder order, sli_compare_f compare_f, sli_copy_f copy_f,
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

    // Can only set a new compare function if the list is empty, otherwise you
    // would be adding new elements in the list with a different logic than the
    // elements already in the list.
    if (!sli_empty(list))
        return DS_ERR_INVALID_OPERATION;

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

Status sli_set_order(SortedList list, SortOrder order)
{
    if (list == NULL)
        return DS_ERR_NULL_POINTER;

    if (!sli_empty(list))
        return DS_ERR_INVALID_OPERATION;

    list->order = order;

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

SortOrder sli_order(SortedList list)
{
    if (list == NULL)
        return 0;

    return list->order;
}

/// \brief Array indexing wrapper.
///
/// This function is zero-based and returns a copy of the element located at
/// the specified position.
///
/// \param list Reference list.
/// \param result Resulting copy of the element.
/// \param index Element position.
///
/// \return
Status sli_get(SortedList list, void **result, index_t index)
{
    if (list == NULL)
        return DS_ERR_NULL_POINTER;

    if (sli_empty(list))
        return DS_ERR_INVALID_OPERATION;

    if (index < 0)
        return DS_ERR_NEGATIVE_VALUE;

    if (index >= list->length)
        return DS_ERR_OUT_OF_RANGE;

    if (list->d_copy == NULL)
        return DS_ERR_INCOMPLETE_TYPE;

    SortedListNode node;

    Status st = sli_get_node_at(list, &node, index);

    if (st != DS_OK)
        return st;

    *result = list->d_copy(node->data);

    return DS_OK;
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
                // The new element will be the new smallest element
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
                    // The new element will be the new biggest element
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
                // The new element will be the new biggest element
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
                    // The new element will be the new smallest element
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

Status sli_remove(SortedList list, void **result, index_t position)
{
    *result = NULL;

    if (list == NULL)
        return DS_ERR_NULL_POINTER;

    if (sli_empty(list))
        return DS_ERR_INVALID_OPERATION;

    if (position < 0)
        return DS_ERR_NEGATIVE_VALUE;

    if (position >= list->length)
        return DS_ERR_OUT_OF_RANGE;

    SortedListNode node;

    // Remove head
    if (position == 0)
    {
        node = list->head;

        *result = node->data;

        list->head = list->head->next;

        if (list->head != NULL)
            list->head->prev = NULL;
    }
    // Remove tail
    else if (position == list->length - 1)
    {
        node = list->tail;

        *result = node->data;

        list->tail = list->tail->prev;

        if (list->tail != NULL)
            list->tail->next = NULL;
    }
    // Remove somewhere in the middle
    else
    {
        Status st = sli_get_node_at(list, &node, position);

        if (st != DS_OK)
            return st;

        // Unlink the current node
        // Behold the power of doubly-linked lists!
        node->prev->next = node->next;
        node->next->prev = node->prev;

        *result = node->data;
    }

    list->length--;

    if (sli_empty(list))
    {
        list->head = NULL;
        list->tail = NULL;
    }

    return DS_OK;
}

Status sli_remove_max(SortedList list, void **result)
{
    *result = NULL;

    if (list == NULL)
        return DS_ERR_NULL_POINTER;

    if (sli_empty(list))
        return DS_ERR_INVALID_OPERATION;

    SortedListNode node;

    // Remove from tail.
    if (list->order == ASCENDING)
    {
        node = list->tail;

        *result = node->data;

        list->tail = list->tail->prev;
        list->tail->next = NULL;

        free(node);
    }
    // Remove from head.
    else
    {
        node = list->head;

        *result = node->data;

        list->head = list->head->next;
        list->head->prev = NULL;

        free(node);
    }

    list->length--;

    if (sli_empty(list))
    {
        list->head = NULL;
        list->tail = NULL;
    }

    return DS_OK;
}

Status sli_remove_min(SortedList list, void **result)
{
    *result = NULL;

    if (list == NULL)
        return DS_ERR_NULL_POINTER;

    if (sli_empty(list))
        return DS_ERR_INVALID_OPERATION;

    SortedListNode node;

    // Remove from head.
    if (list->order == ASCENDING)
    {
        node = list->head;

        *result = node->data;

        list->head = list->head->next;
        list->head->prev = NULL;

        free(node);
    }
    // Remove from tail.
    else
    {
        node = list->tail;

        *result = node->data;

        list->tail = list->tail->prev;
        list->tail->next = NULL;

        free(node);
    }

    list->length--;

    if (sli_empty(list))
    {
        list->head = NULL;
        list->tail = NULL;
    }

    return DS_OK;
}

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

bool sli_contains(SortedList list, void *key)
{
    if (list == NULL)
        return false;

    SortedListNode scan = list->head;

    while (scan != NULL)
    {
        if (list->d_compare(scan->data, key) == 0)
            return true;

        scan = scan->next;
    }

    return false;
}

Status sli_reverse(SortedList list)
{
    // Reverse just like a doubly-linked list and change the list order
    // ASCENDING -> DESCENDING
    // DESCENDING -> ASCENDING

    if (list == NULL)
        return DS_ERR_NULL_POINTER;

    if (list->length != 1)
    {
        SortedListNode prev = NULL;
        SortedListNode curr = list->head;
        SortedListNode next = NULL;

        list->tail = list->head;

        while (curr != NULL)
        {
            next = curr->next;

            curr->next = prev;
            curr->prev = next;

            prev = curr;

            curr = next;
        }

        list->head = prev;
    }

    // If list length is 1 then just by doing this will do the trick
    list->order = (list->order == ASCENDING) ? DESCENDING : ASCENDING;

    return DS_OK;
}

Status sli_copy(SortedList list, SortedList *result)
{
    *result = NULL;

    if (list == NULL)
        return DS_ERR_NULL_POINTER;

    Status st = sli_create(result, list->order, list->d_compare, list->d_copy,
            list->d_display, list->d_free);

    if (st != DS_OK)
        return st;

    (*result)->limit = list->limit;

    SortedListNode node, scan = list->head;

    void *elem;

    while (scan != NULL)
    {
        // Create a node with a copy of scan's data
        elem = list->d_copy(scan->data);

        st = sli_insert_tail(*result, elem);

        if (st != DS_OK)
        {
            list->d_free(elem);

            return st;
        }

        scan = scan->next;
    }

    (*result)->length = list->length;

    return DS_OK;
}

Status sli_to_array(SortedList list, void ***result, index_t *length)
{
    *result = NULL;
    *length = -1;

    if (list == NULL)
        return DS_ERR_NULL_POINTER;

    if (sli_empty(list))
        return DS_ERR_INVALID_OPERATION;

    *length = list->length;

    *result = malloc(sizeof(void*) * (*length));

    if (!(*result))
        return DS_ERR_NULL_POINTER;

    SortedListNode scan = list->head;

    for (index_t i = 0; i < *length; i++)
    {
        (*result)[i] = list->d_copy(scan->data);

        scan = scan->next;
    }

    return DS_OK;
}

/// \brief Merge two SortedLists.
///
/// Removes all elements from list2 and inserts into list1.
///
/// \param list1 List where elements are added to.
/// \param list2 List where elements are removed from.
///
/// \return DS_ERR_NULL_POINTER if either list1 or list2 references to \c NULL.
Status sli_merge(SortedList list1, SortedList list2)
{
    if (list1 == NULL || list2 == NULL)
        return DS_ERR_NULL_POINTER;

    Status st;

    void *result;

    while (!sli_empty(list2))
    {
        st = sli_remove(list2, &result, 0);

        if (st != DS_OK)
            return st;

        st = sli_insert(list1, result);

        if (st != DS_OK)
            return st;
    }

    return DS_OK;
}

/// \brief Unlinks elements from the list.
///
/// Unlinks all elements starting from \c position all the way to the end of
/// the list. The \c result list must not have been initialized.
///
/// \param list
/// \param result
/// \param position
/// \return
Status sli_unlink(SortedList list, SortedList *result, index_t position)
{
    *result = NULL;

    if (list == NULL)
        return DS_ERR_NULL_POINTER;

    if (sli_empty(list))
        return DS_ERR_INVALID_OPERATION;

    if (position < 0)
        return DS_ERR_NEGATIVE_VALUE;

    if (position >= list->length)
        return DS_ERR_OUT_OF_RANGE;

    Status st = sli_create(result, list->order, list->d_compare, list->d_copy,
            list->d_display, list->d_free);

    if (st != DS_OK)
        return st;

    (*result)->limit = list->limit;

    SortedListNode node, new_tail;

    // Special case
    if (position == 0)
    {
        // Simply transfer everything from one list to another.
        (*result)->length = list->length;
        (*result)->head = list->head;
        (*result)->tail = list->tail;

        list->length = 0;
        list->head = NULL;
        list->tail = NULL;
    }
    else
    {
        st = sli_get_node_at(list, &node, position);

        if (st != DS_OK)
            return st;

        // New list tail. The position parameter is inclusive so go one node
        // back.
        new_tail = node->prev;

        // Separating chains.
        node->prev = NULL;
        new_tail->next = NULL;

        // Resulting list head is node and tail is the old list tail.
        (*result)->head = node;
        (*result)->tail = list->tail;

        list->tail = new_tail;

        // Recalculate lengths
        (*result)->length = list->length - position;

        list->length = position;
    }

    return DS_OK;
}

/// \brief Extracts a sublist.
///
/// Extracts a sublist from the specified list. The sublist is stored in the
/// \c result SortedList.
///
/// \param list Reference list where the sublist is to be removed from.
/// \param result An uninitialized SortedList to receive the resulting sublist.
/// \param start Start of the sublist.
/// \param end End of the sublist.
///
/// \return
Status sli_sublist(SortedList list, SortedList *result, index_t start,
        index_t end)
{
    *result = NULL;

    if (list == NULL)
        return DS_ERR_NULL_POINTER;

    if (start < 0 || end < 0)
        return DS_ERR_NEGATIVE_VALUE;

    if (end < start)
        return DS_ERR_INVALID_ARGUMENT;

    if (sli_empty(list))
        return DS_ERR_INVALID_OPERATION;

    if (end >= list->length)
        return DS_ERR_OUT_OF_RANGE;

    Status st = sli_create(result, list->order, list->d_compare, list->d_copy,
            list->d_display, list->d_free);

    if (st != DS_OK)
        return st;

    (*result)->limit = list->limit;

    SortedListNode node;

    // Remove only one node
    if (start == end)
    {
        st = sli_get_node_at(list, &node, start);

        if (st != DS_OK)
            return st;

        if (node->next != NULL)
            node->next->prev = node->prev;
        if (node->prev != NULL)
            node->prev->next = node->next;

        node->next = NULL, node->prev = NULL;

        (*result)->head = node;
        (*result)->tail = node;

    }
    // Simply transfer everything
    else if (start == 0 && end == list->length - 1)
    {
        (*result)->head = list->head;
        (*result)->tail = list->tail;

        list->head = NULL;
        list->tail = NULL;
    }
    // Remove from head to end
    else if (start == 0)
    {
        st = sli_get_node_at(list, &node, end);

        if (st != DS_OK)
            return st;

        // New list head. The position parameters are inclusive so go one node
        // forward.
        SortedListNode new_head = node->next;

        // Separating chains.
        node->next = NULL;
        new_head->prev = NULL;

        (*result)->head = list->head;
        (*result)->tail = node;

        list->head = new_head;
    }
    // Remove from start to tail
    else if (end == list->length - 1)
    {
        st = sli_get_node_at(list, &node, start);

        if (st != DS_OK)
            return st;

        // New list tail. The position parameters are inclusive so go one node
        // back.
        SortedListNode new_tail = node->prev;

        // Separating chains.
        node->prev = NULL;
        new_tail->next = NULL;

        // Resulting list head is node and tail is the old list tail.
        (*result)->head = node;
        (*result)->tail = list->tail;

        list->tail = new_tail;
    }
    // Start and end are inner nodes
    else
    {
        // 'before' the 'start' node and 'after' the 'end' node
        SortedListNode before, after;

        st += sli_get_node_at(list, &before, start - 1);
        st += sli_get_node_at(list, &after, end + 1);

        if (st != DS_OK)
            return st;

        (*result)->head = before->next;
        (*result)->tail = after->prev;

        before->next = after;
        after->prev = before;

        (*result)->head->prev = NULL;
        (*result)->tail->next = NULL;
    }

    (*result)->length = end - start + 1;

    list->length -= (*result)->length;

    return DS_OK;
}

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

// This function effectively searches for a given node. If the position is
// greater than the list length the search will begin at the end of the list,
// reducing the amount of iterations needed. This effectively reduces searches
// to O(n / 2) iterations.
Status sli_get_node_at(SortedList list, SortedListNode *result,
        index_t position)
{
    *result = NULL;

    if (list == NULL)
        return DS_ERR_NULL_POINTER;

    if (sli_empty(list))
        return DS_ERR_INVALID_OPERATION;

    if (position < 0)
        return DS_ERR_NEGATIVE_VALUE;

    if (position >= list->length)
        return DS_ERR_OUT_OF_RANGE;

    // Start looking for the node at the start of the list
    if (position <= list->length / 2)
    {
        (*result) = list->head;

        for (index_t i = 0; i < position; i++)
        {
            // Bad iteration :(
            if ((*result) == NULL)
                return DS_ERR_ITER;

            (*result) = (*result)->next;
        }
    }
    // Start looking for the node at the end of the list
    else
    {
        (*result) = list->tail;

        for (index_t i = list->length - 1; i > position; i--)
        {
            // Bad iteration :(
            if ((*result) == NULL)
                return DS_ERR_ITER;

            (*result) = (*result)->prev;
        }
    }

    return DS_OK;
}

Status sli_insert_tail(SortedList list, void *element)
{
    if (list == NULL)
        return DS_ERR_NULL_POINTER;

    if (sli_full(list))
        return DS_ERR_FULL;

    SortedListNode node;

    Status st = sli_make_node(&node, element);

    if (st != DS_OK)
        return st;

    if (sli_empty(list))
    {
        list->head = node;
        list->tail = node;
    }
    else
    {
        list->tail->next = node;

        node->prev = list->tail;

        list->tail = node;
    }

    (list->length)++;

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
    /// Target SortedList. The iterator might need to use some information
    /// provided by the list.
    struct SortedList_s *target;

    /// \brief Current element.
    ///
    /// Points to the current node. The iterator is always initialized with the
    /// cursor pointing to the start (head) of the list.
    struct SortedListNode_s *cursor;
};

///////////////////////////////////////////////////// NOT EXPOSED FUNCTIONS ///

// VOID

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

Status sli_iter_next(SortedListIterator iter)
{
    if (iter == NULL)
        return DS_ERR_NULL_POINTER;

    if (iter->cursor == NULL || iter->target == NULL)
        return DS_ERR_ITER;

    if (!sli_iter_has_next(iter))
        return DS_ERR_ITER;

    iter->cursor = iter->cursor->next;

    return DS_OK;
}

Status sli_iter_prev(SortedListIterator iter)
{
    if (iter == NULL)
        return DS_ERR_NULL_POINTER;

    if (iter->cursor == NULL || iter->target == NULL)
        return DS_ERR_ITER;

    if (!sli_iter_has_prev(iter))
        return DS_ERR_ITER;

    iter->cursor = iter->cursor->prev;

    return DS_OK;
}

Status sli_iter_to_head(SortedListIterator iter)
{
    if (iter == NULL)
        return DS_ERR_NULL_POINTER;

    if (iter->target == NULL)
        return DS_ERR_ITER;

    iter->cursor = iter->target->head;

    return DS_OK;
}

Status sli_iter_to_tail(SortedListIterator iter)
{
    if (iter == NULL)
        return DS_ERR_NULL_POINTER;

    if (iter->target == NULL)
        return DS_ERR_ITER;

    iter->cursor = iter->target->tail;

    return DS_OK;
}

bool sli_iter_has_next(SortedListIterator iter)
{
    return iter->cursor->next != NULL;
}

bool sli_iter_has_prev(SortedListIterator iter)
{
    return iter->cursor->prev != NULL;
}

Status sli_iter_get(SortedListIterator iter, void **result)
{
    *result = NULL;

    if (iter == NULL)
        return DS_ERR_NULL_POINTER;

    if (iter->cursor == NULL || iter->target == NULL)
        return DS_ERR_ITER;

    if (iter->target->d_copy == NULL)
        return DS_ERR_INCOMPLETE_TYPE;

    // Makes a copy of the element to preserve the list integrity.
    *result = iter->target->d_copy(iter->cursor->data);

    return DS_OK;
}

Status sli_iter_remove_next(SortedListIterator iter, void **result)
{
    if (iter == NULL)
        return DS_ERR_NULL_POINTER;

    if (iter->cursor == NULL || iter->target == NULL)
        return DS_ERR_ITER;

    if (iter->cursor->next == NULL)
        return DS_ERR_INVALID_OPERATION;

    SortedListNode node = iter->cursor->next;

    *result = node->data;

    if (node->next == NULL)
    {
        // The 'next' node is the tail node. Change tail from target list.
        iter->target->tail = iter->cursor;

        iter->cursor->next = NULL;
    }
    else
    {
        // The 'next' node is somewhere in the middle of the list.
        // Skip it...
        iter->cursor->next = node->next;

        node->next->prev = iter->cursor;
    }

    free(node);

    iter->target->length--;

    return DS_OK;
}

Status sli_iter_remove_curr(SortedListIterator iter, void **result)
{
    if (iter == NULL)
        return DS_ERR_NULL_POINTER;

    if (iter->cursor == NULL || iter->target == NULL)
        return DS_ERR_ITER;

    if (iter->cursor->next == NULL)
        return DS_ERR_INVALID_OPERATION;

    SortedListNode node = iter->cursor;

    *result = node->data;

    // Special case
    if (iter->target->length == 1)
    {
        iter->target->head = NULL;
        iter->target->tail = NULL;
    }
    else if (node->next == NULL)
    {
        // The current node is the tail node. Change the cursor to the previous
        // node and change the list tail pointer.
        iter->cursor = iter->cursor->prev;

        iter->cursor->next = NULL;

        iter->target->tail = iter->cursor;
    }
    else if (node->prev == NULL)
    {
        // The current node is the head node. Change the cursor to the next
        // node and change the list head pointer.
        iter->cursor = iter->cursor->next;

        iter->cursor->prev = NULL;

        iter->target->head = iter->cursor;
    }
    else
    {
        // Go one node behind if possible
        if (iter->cursor->prev != NULL)
            iter->cursor = iter->cursor->prev;
        else
            iter->cursor = iter->cursor->next;

        // Remove inner node
        // Use doubly-linked list dark magic
        node->prev->next = node->next;
        node->next->prev = node->prev;

        // WHOA...
    }

    free(node);

    iter->target->length--;

    return DS_OK;
}

Status sli_iter_remove_prev(SortedListIterator iter, void **result)
{
    if (iter == NULL)
        return DS_ERR_NULL_POINTER;

    if (iter->cursor == NULL || iter->target == NULL)
        return DS_ERR_ITER;

    if (iter->cursor->prev == NULL)
        return DS_ERR_INVALID_OPERATION;

    SortedListNode node = iter->cursor->prev;

    *result = node->data;

    if (node->prev == NULL)
    {
        // The 'prev' node is the head node. Change head from target list.
        iter->target->head = iter->cursor;

        iter->cursor->prev = NULL;
    }
    else
    {
        // The 'prev' node is somewhere in the middle of the list.
        // Skip it...
        iter->cursor->prev = node->prev;

        node->prev->next = iter->cursor;
    }

    free(node);

    iter->target->length--;

    return DS_OK;
}

void *sli_iter_peek_next(SortedListIterator iter)
{
    if (iter == NULL)
        return NULL;

    if (iter->cursor == NULL || iter->target == NULL)
        return NULL;

    if (iter->cursor->next == NULL)
        return NULL;

    return iter->cursor->next->data;
}

void *sli_iter_peek(SortedListIterator iter)
{
    if (iter == NULL)
        return NULL;

    if (iter->cursor == NULL || iter->target == NULL)
        return NULL;

    return iter->cursor->data;
}

void *sli_iter_peek_prev(SortedListIterator iter)
{
    if (iter == NULL)
        return NULL;

    if (iter->cursor == NULL || iter->target == NULL)
        return NULL;

    if (iter->cursor->prev == NULL)
        return NULL;

    return iter->cursor->prev->data;
}
