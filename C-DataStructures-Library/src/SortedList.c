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
/// If you don't set the default functions later you won't be able to insert
/// elements, copy the list, display the list or even delete it. If you want to
/// initialize it completely, use instead sli_create(). Here you must pass in
/// default functions (compare, copy, display and free) according with the
/// specifications of each type of function.
///
/// To add an element to the list use sli_insert(). To remove, you have three
/// options: sli_remove() that removes an element at a given position;
/// sli_remove_max() that removes the biggest element; and sli_remove_min()
/// that removes the smallest element.
///
/// To delete a list use sli_free(). This completely frees all elements and
/// sets the list pointer to \c NULL. Note that if you haven't set a default
/// free function you won't be able to delete the list or its elements. You
/// must set a free function that will be responsible for freeing from memory
/// all elements. You can also use sli_free_shallow() that will only free the
/// list structure. If you simply want to erase all its contents use
/// sli_erase(). This will keep all default functions and all elements will be
/// removed from the list and freed from memory.
///
/// The list maintains a version id that keeps track of structural changes done
/// to the list. This prevents any iterators from working the moment the list
/// structure is changed. It works to prevent any undefined behaviour or
/// run-time errors.
///
/// \b Functions \b List
/// - sli_init()
/// - sli_create()
/// - sli_free()
/// - sli_free_shallow()
/// - sli_erase()
/// - sli_set_v_compare()
/// - sli_set_v_copy()
/// - sli_set_v_display()
/// - sli_set_v_free()
/// - sli_set_limit()
/// - sli_set_order()
/// - sli_length()
/// - sli_limit()
/// - sli_order()
/// - sli_get()
/// - sli_insert()
/// - sli_insert_all()
/// - sli_remove()
/// - sli_remove_max()
/// - sli_remove_min()
/// - sli_full()
/// - sli_empty()
/// - sli_max()
/// - sli_min()
/// - sli_index_first()
/// - sli_index_last()
/// - sli_contains()
/// - sli_reverse()
/// - sli_copy()
/// - sli_to_array()
/// - sli_merge()
/// - sli_unlink()
/// - sli_sublist()
/// - sli_display()
/// - sli_display_array()
/// - sli_display_raw()
struct SortedList_s
{
    /// \brief List length.
    ///
    /// List current amount of elements linked between the \c head and \c tail
    /// pointers.
    integer_t length;

    /// \brief List length limit.
    ///
    /// If it is set to 0 or a negative value then the list has no limit to its
    /// length. Otherwise it won't be able to have more elements than the
    /// specified value. The list is always initialized with no restrictions to
    /// its length, that is, \c limit equals 0. The user won't be able to limit
    /// the list length if it already has more elements than the specified
    /// limit.
    integer_t limit;

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
    sli_compare_f v_compare;

    /// \brief Copy function.
    ///
    /// A function that returns an exact copy of an element.
    sli_copy_f v_copy;

    /// \brief Display function.
    ///
    /// A function that displays an element in the console. Useful for
    /// debugging.
    sli_display_f v_display;

    /// \brief Deallocator function.
    ///
    /// A function that completely frees an element from memory.
    sli_free_f v_free;

    /// \brief A version id to keep track of modifications.
    ///
    /// This version id is used by the iterator to check if the structure was
    /// modified. The iterator can only function if its version_id is the same
    /// as the structure's version id, that is, there have been no structural
    /// modifications (except for those done by the iterator itself).
    integer_t version_id;
};

/// \brief A SortedList_s node.
///
/// Implementation detail. This is a doubly-linked node with a pointer to the
/// previous node (or \c NULL if it is the head node) and another pointer to
/// the next node (or \c NULL if it is the tail node).
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

/// \brief A type for a sorted list node.
///
/// Defines a type to a <code> struct SortedListNode_s </code>.
typedef struct SortedListNode_s SortedListNode_t;

/// \brief A pointer type for a sorted list node.
///
/// Defines a pointer type to a <code> struct SortedListNode_s </code>.
typedef struct SortedListNode_s *SortedListNode;

///////////////////////////////////////////////////// NOT EXPOSED FUNCTIONS ///

static Status sli_make_node(SortedListNode *node, void *data);

static Status sli_free_node(SortedListNode *node, sli_free_f free_f);

static Status sli_free_node_shallow(SortedListNode *node);

static Status sli_get_node_at(SortedList list, SortedListNode *result,
        integer_t position);

static Status sli_insert_tail(SortedList list, void *element);

////////////////////////////////////////////// END OF NOT EXPOSED FUNCTIONS ///

/// \brief Initializes a SortedList_s structure.
///
/// This function initializes the SortedList_s structure but does not sets any
/// default functions. If you don't set them later you won't be able to add
/// elements, copy the list, free the list or display it. It also sets a
/// default order of \c DESCENDING.
///
/// \param[in,out] list SortedList_s to be allocated.
///
/// \return DS_ERR_ALLOC if list allocation failed.
/// \return DS_OK if all operations are successful.
Status sli_init(SortedList *list)
{
    *list = malloc(sizeof(SortedList_t));

    if (!(*list))
        return DS_ERR_ALLOC;

    (*list)->length = 0;
    (*list)->limit = 0;
    (*list)->version_id = 0;

    (*list)->order = DESCENDING;

    (*list)->head = NULL;
    (*list)->tail = NULL;

    (*list)->v_compare = NULL;
    (*list)->v_copy = NULL;
    (*list)->v_display = NULL;
    (*list)->v_free = NULL;

    return DS_OK;
}

/// \brief Creates a SortedList_s.
///
/// This function completely creates a SortedList_s. This sets the list order
/// of elements and all of its default functions.
///
/// \param[in,out] list SortedList_s to be allocated.
/// \param[in] order The sorting order of the list's elements.
/// \param[in] compare_f A function that compares two elements.
/// \param[in] copy_f A function that makes an exact copy of an element.
/// \param[in] display_f A function that displays in the console an element.
/// \param[in] free_f A function that completely frees from memory an element.
///
/// \return DS_ERR_ALLOC if list allocation failed.
/// \return DS_OK if all operations are successful.
Status sli_create(SortedList *list, SortOrder order, sli_compare_f compare_f,
        sli_copy_f copy_f, sli_display_f display_f, sli_free_f free_f)
{
    *list = malloc(sizeof(SortedList_t));

    if (!(*list))
        return DS_ERR_ALLOC;

    (*list)->length = 0;
    (*list)->limit = 0;
    (*list)->version_id = 0;

    (*list)->order = order;

    (*list)->head = NULL;
    (*list)->tail = NULL;

    (*list)->v_compare = compare_f;
    (*list)->v_copy = copy_f;
    (*list)->v_display = display_f;
    (*list)->v_free = free_f;

    return DS_OK;
}

/// \brief Frees from memory a SortedList_s and all its elements.
///
/// This function frees from memory all the list's elements using its default
/// free function and then frees the list's structure. The variable is then set
/// to \c NULL.
///
/// \param[in,out] list The SortedList_s to be freed from memory.
///
/// \return DS_ERR_INCOMPLETE_TYPE if a default free function is not set.
/// \return DS_ERR_NULL_POINTER if the list references to \c NULL.
/// \return DS_OK if all operations are successful.
Status sli_free(SortedList *list)
{
    if (*list == NULL)
        return DS_ERR_NULL_POINTER;

    if ((*list)->v_free == NULL)
        return DS_ERR_INCOMPLETE_TYPE;

    SortedListNode prev = (*list)->head;

    Status st;

    while ((*list)->head != NULL)
    {
        (*list)->head = (*list)->head->next;

        st = sli_free_node(&prev, (*list)->v_free);

        if (st != DS_OK)
            return st;

        prev = (*list)->head;
    }

    free((*list));

    (*list) = NULL;

    return DS_OK;
}

/// \brief Frees from memory a SortedList_s.
///
/// This function frees from memory all the list's nodes without freeing its
/// data and then frees the list's structure. The variable is then set to
/// \c NULL.
///
/// \param[in,out] list SortedList_s to be freed from memory.
///
/// \return DS_ERR_NULL_POINTER if the list references to \c NULL.
/// \return DS_OK if all operations are successful.
Status sli_free_shallow(SortedList *list)
{
    if (*list == NULL)
        return DS_ERR_NULL_POINTER;

    SortedListNode prev = (*list)->head;

    Status st;

    while ((*list)->head != NULL)
    {
        (*list)->head = (*list)->head->next;

        st = sli_free_node_shallow(&prev);

        if (st != DS_OK)
            return st;

        prev = (*list)->head;
    }

    free((*list));

    (*list) = NULL;

    return DS_OK;
}

/// \brief Erases a SortedList_s.
///
/// This function is equivalent to freeing a list and the creating it again.
/// This will reset the list to its initial state with no elements, but will
/// keep all of its default functions and the order of elements.
///
/// \param[in,out] list SortedList_s to be erased.
///
/// \return DS_ERR_ALLOC if list allocation failed.
/// \return DS_ERR_INCOMPLETE_TYPE if a default free function is not set.
/// \return DS_ERR_NULL_POINTER if the list references to \c NULL.
/// \return DS_OK if all operations are successful.
Status sli_erase(SortedList *list)
{
    if (*list == NULL)
        return DS_ERR_NULL_POINTER;

    SortedList new_list;

    Status st = sli_create(&new_list, (*list)->order, (*list)->v_compare,
            (*list)->v_copy, (*list)->v_display, (*list)->v_free);

    if (st !=  DS_OK)
        return st;

    st = sli_free(list);

    // Probably didn't set the free function...
    if (st !=  DS_OK)
    {
        free(new_list);

        return st;
    }

    *list = new_list;

    return DS_OK;
}

/// \brief Sets the default compare function.
///
/// Use this function to set a default compare function. It can only be done
/// when the list is empty, otherwise you would have elements sorted with a
/// different logic. This function needs to comply with the sli_compare_f
/// specifications.
///
/// \param[in] list SortedList_s to set the default compare function.
/// \param[in] function An sli_compare_f kind of function.
///
/// \return DS_ERR_INVALID_OPERATION if the list is not empty.
/// \return DS_ERR_NULL_POINTER if the list references to \c NULL.
/// \return DS_OK if all operations are successful.
Status sli_set_v_compare(SortedList list, sli_compare_f function)
{
    if (list == NULL)
        return DS_ERR_NULL_POINTER;

    // Can only set a new compare function if the list is empty, otherwise you
    // would be adding new elements in the list with a different logic than the
    // elements already in the list.
    if (!sli_empty(list))
        return DS_ERR_INVALID_OPERATION;

    list->v_compare = function;

    return DS_OK;
}

/// \brief Sets the default copy function.
///
/// Use this function to set a default compare function. It needs to comply
/// with the sli_copy_f specifications.
///
/// \param[in] list SortedList_s to set the default copy function.
/// \param[in] function An sli_copy_f kind of function.
///
/// \return DS_ERR_NULL_POINTER if the list references to \c NULL.
/// \return DS_OK if all operations are successful.
Status sli_set_v_copy(SortedList list, sli_copy_f function)
{
    if (list == NULL)
        return DS_ERR_NULL_POINTER;

    list->v_copy = function;

    return DS_OK;
}

/// \brief Sets the default display function.
///
/// Use this function to set a default display function. It needs to comply
/// with the sli_display_f specifications. Useful for debugging.
///
/// \param[in] list SortedList_s to set the default display function.
/// \param[in] function An sli_display_f kind of function.
///
/// \return DS_ERR_NULL_POINTER if the list references to \c NULL.
/// \return DS_OK if all operations are successful.
Status sli_set_v_display(SortedList list, sli_display_f function)
{
    if (list == NULL)
        return DS_ERR_NULL_POINTER;

    list->v_display = function;

    return DS_OK;
}

/// \brief Sets the default free function.
///
/// Use this function to set a default free function. It needs to comply
/// with the sli_free_f specifications.
///
/// \param[in] list SortedList_s to set the default free function.
/// \param[in] function An sli_free_f kind of function.
///
/// \return DS_ERR_NULL_POINTER if the list references to \c NULL.
/// \return DS_OK if all operations are successful.
Status sli_set_v_free(SortedList list, sli_free_f function)
{
    if (list == NULL)
        return DS_ERR_NULL_POINTER;

    list->v_free = function;

    return DS_OK;
}

/// \brief Sets a limit to the specified SortedList_s's length.
///
/// Limit's the SortedList_s's length. You can only set a limit greater or
/// equal to the list's current length and greater than 0. To remove this
/// limitation simply set the limit to 0 or less.
///
/// \param[in] list SortedList_s reference.
/// \param[in] limit Maximum list length.
///
/// \return DS_ERR_INVALID_OPERATION if the limitation is less than the list's
/// current length.
/// \return DS_ERR_NULL_POINTER if the list references to \c NULL.
/// \return DS_OK if all operations are successful.
Status sli_set_limit(SortedList list, integer_t limit)
{
    if (list == NULL)
        return DS_ERR_NULL_POINTER;

    // The new limit can't be lower than the list's current length.
    if (list->length > limit && limit > 0)
        return DS_ERR_INVALID_OPERATION;

    list->limit = limit;

    return DS_OK;
}

/// \brief Sets the sorting order of elements of the specified SortedList_s.
///
/// Sets the sorting order of elements to either \c ASCENDING or \c DESCENDING.
/// You can only set it when the list is empty.
///
/// \param[in] list SortedList_s reference.
/// \param[in] order Sorting order of elements.
///
/// \return DS_ERR_INVALID_OPERATION if the list is not empty.
/// \return DS_ERR_NULL_POINTER if the list references to \c NULL.
/// \return DS_OK if all operations are successful.
Status sli_set_order(SortedList list, SortOrder order)
{
    if (list == NULL)
        return DS_ERR_NULL_POINTER;

    if (!sli_empty(list))
        return DS_ERR_INVALID_OPERATION;

    list->order = order;

    return DS_OK;
}

/// \brief Returns the SortedList_s's length.
///
/// Returns the list's current length or -1 if the list references to \c NULL.
///
/// \param[in] list SortedList_s reference.
///
/// \return -1 if the list references to \c NULL.
/// \return The list's length.
integer_t sli_length(SortedList list)
{
    if (list == NULL)
        return -1;

    return list->length;
}

/// \brief Returns the SortedList_s's limit.
///
/// Returns the list limit or -1 if the list references to \c NULL.
///
/// \param[in] list SortedList_s reference.
///
/// \return -1 if the list references to \c NULL.
/// \return The list's limit.
integer_t sli_limit(SortedList list)
{
    if (list == NULL)
        return -1;

    return list->limit;
}

/// \brief Returns the SortedList_s's sorting order.
///
/// Return the list's sorting order, either ASCENDING, DESCENDING or 0 if the
/// list references to \c NULL.
///
/// \param[in] list SortedList_s reference.
///
/// \return 0 if the list references to \c NULL.
/// \return The list order.
SortOrder sli_order(SortedList list)
{
    if (list == NULL)
        return 0;

    return list->order;
}

/// \brief Returns a copy of an element at a given position.
///
/// This function is zero-based and returns a copy of the element located at
/// the specified index.
///
/// \param[in] list SortedList_s reference.
/// \param[out] result Resulting copy of the element.
/// \param[in] index Element position.
///
/// \return DS_ERR_INCOMPLETE_TYPE if a default copy function is not set.
/// \return DS_ERR_INVALID_OPERATION if the list is empty.
/// \return DS_ERR_NEGATIVE_VALUE if index parameter is negative.
/// \return DS_ERR_NULL_POINTER if the list references to \c NULL.
/// \return DS_ERR_OUT_OF_RANGE if index parameter is greater than or equal
/// to the list's length.
/// \return DS_OK if all operations are successful.
Status sli_get(SortedList list, void **result, integer_t index)
{
    *result = NULL;

    if (list == NULL)
        return DS_ERR_NULL_POINTER;

    if (sli_empty(list))
        return DS_ERR_INVALID_OPERATION;

    if (index < 0)
        return DS_ERR_NEGATIVE_VALUE;

    if (index >= list->length)
        return DS_ERR_OUT_OF_RANGE;

    if (list->v_copy == NULL)
        return DS_ERR_INCOMPLETE_TYPE;

    SortedListNode node;

    Status st = sli_get_node_at(list, &node, index);

    if (st != DS_OK)
        return st;

    *result = list->v_copy(node->data);

    return DS_OK;
}

/// \brief Inserts an element to the specified SortedList_s.
///
/// Inserts an element according to the sort order specified by the list. This
/// function can take up to O(n) to add an element in its correct position.
///
/// \param[in] list SortedList_s reference where the element is to be inserted.
/// \param[in] element Element to be inserted in the list.
///
/// \return DS_ERR_ALLOC if node allocation failed.
/// \return DS_ERR_FULL if \c limit is set (different than 0) and the list
/// length reached the specified limit.
/// \return DS_ERR_INCOMPLETE_TYPE if a default compare function is not set.
/// \return DS_ERR_NULL_POINTER if the list references to \c NULL.
/// \return DS_OK if all operations are successful.
Status sli_insert(SortedList list, void *element)
{
    if (list == NULL)
        return DS_ERR_NULL_POINTER;

    if (list->v_compare == NULL)
        return DS_ERR_INCOMPLETE_TYPE;

    if (sli_full(list))
        return DS_ERR_FULL;

    SortedListNode node;

    Status st = sli_make_node(&node, element);

    if (st != DS_OK)
        return st;

    // First node.
    if (sli_empty(list))
    {
        list->head = node;
        list->tail = node;
    }
    // Insert node in its position.
    else
    {
        SortedListNode scan = list->head;
        SortedListNode before = NULL;

        if (list->order == ASCENDING)
        {
            // Insert 'head'. Change list->head.
            if (list->v_compare(node->data, list->head->data) <= 0)
            {
                // The new element will be the new smallest element.
                node->next = list->head;

                list->head->prev = node;

                list->head = node;
            }
            else
            {
                while (scan != NULL &&
                       list->v_compare(node->data, scan->data) > 0)
                {
                    before = scan;

                    scan = scan->next;
                }

                // Insert 'tail'. Change list->tail.
                if (scan == NULL)
                {
                    // The new element will be the new biggest element.
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
        // Defaults to DESCENDING.
        else
        {
            // Insert 'head'. Change list->head.
            if (list->v_compare(node->data, list->head->data) >= 0)
            {
                // The new element will be the new biggest element.
                node->next = list->head;

                list->head->prev = node;

                list->head = node;
            }
            else
            {
                while (scan != NULL &&
                       list->v_compare(node->data, scan->data) < 0)
                {
                    before = scan;

                    scan = scan->next;
                }

                // Insert 'tail'. Change list->tail.
                if (scan == NULL)
                {
                    // The new element will be the new smallest element.
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

    list->version_id++;

    return DS_OK;
}

/// \brief Inserts an array of elements to the specified SortedList_s.
///
/// Inserts an array of void pointers into the list, with a size of \c count.
///
/// \param[in] list SortedList_s reference where all elements are to be
/// inserted.
/// \param[in] elements Elements to be inserted in the list.
/// \param[in] count Amount of elements to be inserted.
///
/// \return DS_ERR_NEGATIVE_VALUE if count parameter is negative.
/// \return DS_ERR_NULL_POINTER if the list references to \c NULL.
/// \return DS_OK if all operations are successful.
Status sli_insert_all(SortedList list, void **elements, integer_t count)
{
    if (list == NULL)
        return DS_ERR_NULL_POINTER;

    if (count < 0)
        return DS_ERR_NEGATIVE_VALUE;

    Status st;

    for (integer_t i = 0; i < count; i++)
    {
        st = sli_insert(list, elements[i]);

        if (st != DS_OK)
            return st;
    }

    return DS_OK;
}

/// \brief Removes an element at a specified position from a SortedList_s.
///
/// Removes an element at the specified position. The position is 0 based so
/// the first element is at the position 0.
///
/// \param[in] list SortedList_s reference where an element is to be removed.
/// \param[out] result Resulting element removed from the list.
/// \param[in] position Element's position.
///
/// \return DS_ERR_INVALID_OPERATION if list is empty.
/// \return DS_ERR_ITER if during iteration the scanner references to \c NULL.
/// \return DS_ERR_NEGATIVE_VALUE if position parameter is negative.
/// \return DS_ERR_NULL_POINTER if the list references to \c NULL.
/// \return DS_ERR_OUT_OF_RANGE if position parameter is greater than or equal
/// to the list's length.
/// \return DS_OK if all operations are successful.
Status sli_remove(SortedList list, void **result, integer_t position)
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

    free(node);

    list->length--;

    list->version_id++;

    if (sli_empty(list))
    {
        list->head = NULL;
        list->tail = NULL;
    }

    return DS_OK;
}

/// \brief Removes the highest value from the specified SortedList_s.
///
/// Removes the highest value from the list. Depending on the sort order of
/// elements this function is equivalent to removing an element from the head
/// of the list or from tail.
///
/// \param[in] list SortedList_s reference where an element is to be removed.
/// \param[out] result Resulting element removed from the list.
///
/// \return DS_ERR_INVALID_OPERATION if list is empty.
/// \return DS_ERR_NULL_POINTER if the list references to \c NULL.
/// \return DS_OK if all operations are successful.
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

        if (list->tail != NULL)
            list->tail->next = NULL;

        free(node);
    }
    // Remove from head.
    else
    {
        node = list->head;

        *result = node->data;

        list->head = list->head->next;

        if (list->head != NULL)
            list->head->prev = NULL;

        free(node);
    }

    list->length--;

    if (sli_empty(list))
    {
        list->head = NULL;
        list->tail = NULL;
    }

    list->version_id++;

    return DS_OK;
}

/// \brief Removes the smallest value from the specified SortedList_s.
///
/// Removes the smallest value from the list. Depending on the sort order of
/// elements this function is equivalent to removing an element from the head
/// of the list or from tail.
///
/// \param[in] list SortedList_s reference where an element is to be removed.
/// \param[out] result Resulting element removed from the list.
///
/// \return DS_ERR_INVALID_OPERATION if list is empty.
/// \return DS_ERR_NULL_POINTER if the list references to \c NULL.
/// \return DS_OK if all operations are successful.
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

        if (list->head != NULL)
            list->head->prev = NULL;

        free(node);
    }
    // Remove from tail.
    else
    {
        node = list->tail;

        *result = node->data;

        list->tail = list->tail->prev;

        if (list->tail != NULL)
            list->tail->next = NULL;

        free(node);
    }

    list->length--;

    if (sli_empty(list))
    {
        list->head = NULL;
        list->tail = NULL;
    }

    list->version_id++;

    return DS_OK;
}

/// \brief Checks if the specified SortedList_s is full.
///
/// Returns true if the list is full or false otherwise. The list can only be
/// full if its limit is set to a value higher than 0 and respecting all rules
/// from sli_set_limit().
///
/// \warning This function does not checks for \c NULL references and is prone
/// to provoke run-time errors if not used correctly.
///
/// \param[in] list SortedList_s reference.
///
/// \return True if the list is full.
/// \return False if the list is not full.
bool sli_full(SortedList list)
{
    return list->limit > 0 && list->length >= list->limit;
}

/// \brief Checks if specified SortedList_s list is empty.
///
/// Returns true if the list is empty or false otherwise. The list is empty
/// when its length is 0. If every function works properly there is no need to
/// check if the head or tail pointers are \c NULL or not. The length variable
/// already tracks it.
///
/// \warning This function does not checks for \c NULL references and is prone
/// to provoke run-time errors if not used correctly.
///
/// \param[in] list SortedList_s reference.
///
/// \return True if the list is empty.
/// \return False if the list is not empty.
bool sli_empty(SortedList list)
{
    return list->length == 0;
}

/// \brief Returns the highest element from the specified SortedList_s.
///
/// Returns a reference to the highest element in the list or NULL if the list
/// is empty. Use this as a read-only. If you make any changes to this element
/// the internal structure of the list will change and may cause undefined
/// behaviour.
///
/// \param[in] list SortedList_s reference.
///
/// \return NULL if the list references to \c NULL or if the list is empty.
/// \return The highest element in the list.
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

/// \brief Returns the smallest element from the specified SortedList_s.
///
/// Returns a reference to the smallest element in the list or NULL if the list
/// is empty. Use this as a read-only. If you make any changes to this element
/// the internal structure of the list will change and may cause undefined
/// behaviour.
///
/// \param[in] list SortedList_s reference.
///
/// \return NULL if the list references to \c NULL or if the list is empty.
/// \return The highest element in the list.
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

/// \brief Returns the index of the first element that matches a key.
///
/// Returns the index of the first element that matches a given key or -1 if it
/// could not be found.
///
/// \param[in] list SortedList_s reference.
/// \param[in] key Key to be searched in the list.
///
/// \return -3 if the list references to \c NULL.
/// \return -2 if there is no default compare function.
/// \return -1 if the element was not found.
/// \return The index of the matched element.
integer_t sli_index_first(SortedList list, void *key)
{
    if (list == NULL)
        return -3;

    if (list->v_compare == NULL)
        return -2;

    SortedListNode scan = list->head;

    integer_t index = 0;

    while (scan != NULL)
    {
        if (list->v_compare(scan->data, key) == 0)
            return index;

        index++;

        scan = scan->next;
    }

    return -1;
}

/// \brief Returns the index of the last element that matches a key.
///
/// Returns the index of the first element that matches a given key or -1 if it
/// could not be found.
///
/// \param[in] list SortedList_s reference.
/// \param[in] key Key to be searched in the list.
///
/// \return -3 if the list references to \c NULL.
/// \return -2 if there is no default compare function.
/// \return -1 if the element was not found.
/// \return The index of the matched element.
integer_t sli_index_last(SortedList list, void *key)
{
    if (list == NULL)
        return -3;

    if (list->v_compare == NULL)
        return -2;

    SortedListNode scan = list->tail;

    integer_t index = 0;

    while (scan != NULL)
    {
        if (list->v_compare(scan->data, key) == 0)
            return list->length - 1 - index;

        index++;

        scan = scan->prev;
    }

    return -1;
}

/// \brief Checks if a given element is present in the specified SortedList_s.
///
/// Returns true if the element is present in the list, otherwise false.
///
/// \warning This function does not checks for \c NULL references for either
/// the list parameter or if the default compare function is set.
///
/// \param[in] list SortedList_s reference.
/// \param[in] key Key to be matched.
///
/// \return True if the element is present in the list.
/// \return False if the element is not present in the list.
bool sli_contains(SortedList list, void *key)
{
    SortedListNode scan = list->head;

    while (scan != NULL)
    {
        if (list->v_compare(scan->data, key) == 0)
            return true;

        scan = scan->next;
    }

    return false;
}

/// \brief Reverses a SortedList_s.
///
/// Reverses the chain of elements from the list and changes the sort order of
/// elements. This function only changes the \c next and \c prev pointers from
/// each element and the \c head and \c tail pointers of the list struct.
///
/// \param[in] list SortedList_s reference to be reversed.
///
/// \return DS_ERR_NULL_POINTER if node references to \c NULL.
/// \return DS_OK if all operations are successful.
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

    list->version_id++;

    return DS_OK;
}

/// \brief Makes a copy of the specified SortedList_s.
///
/// Makes an exact copy of a list.
///
/// \param[in] list SortedList_s to be copied.
/// \param[out] result Resulting copy.
///
/// \return DS_ERR_INCOMPLETE_TYPE if either a default copy or a default free
/// functions are not set.
/// \return DS_ERR_NULL_POINTER if list references to \c NULL.
/// \return DS_OK if all operations are successful.
Status sli_copy(SortedList list, SortedList *result)
{
    *result = NULL;

    if (list == NULL)
        return DS_ERR_NULL_POINTER;

    if (list->v_copy == NULL || list->v_free == NULL)
        return DS_ERR_INCOMPLETE_TYPE;

    Status st = sli_create(result, list->order, list->v_compare, list->v_copy,
            list->v_display, list->v_free);

    if (st != DS_OK)
        return st;

    (*result)->limit = list->limit;

    SortedListNode scan = list->head;

    void *elem;

    while (scan != NULL)
    {
        elem = list->v_copy(scan->data);

        st = sli_insert_tail(*result, elem);

        if (st != DS_OK)
        {
            list->v_free(elem);

            return st;
        }

        scan = scan->next;
    }

    return DS_OK;
}

/// \brief Copies the elements of the list to a C array.
///
/// Makes a copy of every element into an array respecting the order of the
/// elements in the list. The array needs to be an array of void pointers and
/// uninitialized. If any error is returned, the default values for \c result
/// and \c length are \c NULL and -1 respectively.
///
/// \param[in] list SortedList_s reference.
/// \param[out] result Resulting array.
/// \param[out] length Resulting array's length.
///
/// \return DS_ERR_INCOMPLETE_TYPE if a default copy function is not set.
/// \return DS_ERR_NULL_POINTER if list references to \c NULL.
/// \return DS_OK if all operations are successful.
Status sli_to_array(SortedList list, void ***result, integer_t *length)
{
    // If anything goes wrong...
    *result = NULL;
    *length = -1;

    if (list == NULL)
        return DS_ERR_NULL_POINTER;

    if (sli_empty(list))
        return DS_ERR_INVALID_OPERATION;

    if (list->v_copy == NULL)
        return DS_ERR_INCOMPLETE_TYPE;

    *length = list->length;

    *result = malloc(sizeof(void*) * (*length));

    if (!(*result))
        return DS_ERR_ALLOC;

    SortedListNode scan = list->head;

    for (integer_t i = 0; i < *length; i++)
    {
        (*result)[i] = list->v_copy(scan->data);

        scan = scan->next;
    }

    return DS_OK;
}

/// \brief Merge two SortedList_s.
///
/// Removes all elements from list2 and inserts them into list1.
///
/// \param[in] list1 SortedList_s where elements are added to.
/// \param[in] list2 SortedList_s where elements are removed from.
///
/// \return DS_ERR_NULL_POINTER if either list1 or list2 references are
/// \c NULL.
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

/// \brief Unlinks elements from the specified SortedList_s.
///
/// Unlinks all elements starting from \c position all the way to the end of
/// the list. The \c result list must not have been initialized.
///
/// \param[in] list SortedList_s reference.
/// \param[out] result Resulting sublist.
/// \param[in] position Position to unlink the elements from the list.
///
/// \return DS_ERR_INVALID_OPERATION if the list is empty.
/// \return DS_ERR_NEGATIVE_VALUE if position parameter is negative.
/// \return DS_ERR_NULL_POINTER if the list references to \c NULL.
/// \return DS_ERR_OUT_OF_RANGE if position parameter is greater than or equal
/// to the list's length.
/// \return DS_OK if all operations are successful.
Status sli_unlink(SortedList list, SortedList *result, integer_t position)
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

    Status st = sli_create(result, list->order, list->v_compare, list->v_copy,
            list->v_display, list->v_free);

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

    list->version_id++;

    return DS_OK;
}

/// \brief Extracts a sublist from the specified SortedList_s.
///
/// Extracts a sublist from the specified SortedList_s. The sublist is stored
/// in the \c result SortedList_s.
///
/// \param[in] list SortedList_s where the sublist is to be removed from.
/// \param[out] result An uninitialized SortedList_s to receive the resulting
/// sublist.
/// \param[in] start Start of the sublist.
/// \param[in] end End of the sublist.
///
/// \return DS_ERR_INVALID_ARGUMENT if the start parameter is greater than the
/// end parameter.
/// \return DS_ERR_INVALID_OPERATION if the list is empty.
/// \return DS_ERR_NEGATIVE_VALUE if either start or end parameters are
/// negative.
/// \return DS_ERR_NULL_POINTER if the list references to \c NULL.
/// \return DS_ERR_OUT_OF_RANGE if the end parameter is greater than or equal
/// to the list's length.
/// \return DS_OK if all operations are successful.
Status sli_sublist(SortedList list, SortedList *result, integer_t start,
        integer_t end)
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

    Status st = sli_create(result, list->order, list->v_compare, list->v_copy,
            list->v_display, list->v_free);

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

    list->version_id++;

    return DS_OK;
}

/// \brief Displays a SortedList_s in the console.
///
/// Displays a SortedList_s in the console with its elements separated by
/// arrows indicating a doubly-linked list.
///
/// \param[in] list The SortedList_s to be displayed in the console.
///
/// \return DS_ERR_INCOMPLETE_TYPE if a default display function is not set.
/// \return DS_ERR_NULL_POINTER if the list references to \c NULL.
/// \return DS_OK if all operations were successful.
Status sli_display(SortedList list)
{
    if (list == NULL)
        return DS_ERR_NULL_POINTER;

    if (list->v_display == NULL)
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
        list->v_display(scan->data);

        printf(" <-> ");

        scan = scan->next;
    }

    printf(" NULL\n");

    return DS_OK;
}

/// \brief Displays a SortedList_s in the console.
///
/// Displays a SortedList_s in the console like an array with its elements
/// separated by commas, delimited with brackets.
///
/// \param[in] list The SortedList_s to be displayed in the console.
///
/// \return DS_ERR_INCOMPLETE_TYPE if a default display function is not set.
/// \return DS_ERR_NULL_POINTER if the list references to \c NULL.
/// \return DS_OK if all operations were successful.
Status sli_display_array(SortedList list)
{
    if (list == NULL)
        return DS_ERR_NULL_POINTER;

    if (list->v_display == NULL)
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
        list->v_display(scan->data);

        printf(", ");

        scan = scan->next;
    }

    list->v_display(scan->data);

    printf(" ]\n");

    return DS_OK;
}

/// \brief Displays a SortedList_s in the console.
///
/// Displays a SortedList_s in the console with its elements separated by
/// spaces.
///
/// \param[in] list The SortedList_s to be displayed in the console.
///
/// \return DS_ERR_INCOMPLETE_TYPE if a default display function is not set.
/// \return DS_ERR_NULL_POINTER if the list references to \c NULL.
/// \return DS_OK if all operations were successful.
Status sli_display_raw(SortedList list)
{
    if (list == NULL)
        return DS_ERR_NULL_POINTER;

    if (list->v_display == NULL)
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
        list->v_display(scan->data);

        printf(" ");

        scan = scan->next;
    }

    list->v_display(scan->data);

    printf("\n");

    return DS_OK;
}

///////////////////////////////////////////////////// NOT EXPOSED FUNCTIONS ///

/// \brief Builds a new SortedListNode_s.
///
/// Implementation detail. Function responsible for allocating a new
/// SortedListNode_s.
///
/// \param[in,out] node SortedListNode_s to be allocated.
/// \param[in] data Node's data member.
///
/// \return DS_ERR_ALLOC if node allocation failed.
/// \return DS_OK if all operations are successful.
static Status sli_make_node(SortedListNode *node, void *data)
{
    *node = malloc(sizeof(SortedListNode_t));

    if (!(*node))
        return DS_ERR_ALLOC;

    (*node)->data = data;

    (*node)->next = NULL;
    (*node)->prev = NULL;

    return DS_OK;
}

/// \brief Frees a SortedListNode_s and its data.
///
/// Implementation detail. Frees a SortedListNode_s and its data using the
/// list's default free function.
///
/// \param[in,out] node SortedListNode_s to be freed from memory.
/// \param[in] free_f List's default free function.
///
/// \return DS_ERR_NULL_POINTER if node references to \c NULL.
/// \return DS_OK if all operations are successful.
static Status sli_free_node(SortedListNode *node, sli_free_f free_f)
{
    if (*node == NULL)
        return DS_ERR_NULL_POINTER;

    free_f((*node)->data);

    free(*node);

    *node = NULL;

    return DS_OK;
}

/// \brief Frees a SortedListNode_s.
///
/// Implementation detail. Frees a SortedListNode_s and leaves its data
/// untouched.
///
/// \param[in,out] node SortedListNode_s to be freed from memory.
///
/// \return DS_ERR_NULL_POINTER if node references to \c NULL.
/// \return DS_OK if all operations are successful.
static Status sli_free_node_shallow(SortedListNode *node)
{
    if (*node == NULL)
        return DS_ERR_NULL_POINTER;

    free(*node);

    *node = NULL;

    return DS_OK;
}

/// \brief Gets a node from a specific position.
///
/// Implementation detail. Searches for a node in O(n / 2), the search starts
/// at the tail pointer if position is greater than half the list's length,
/// otherwise it starts at the head pointer.
///
/// \param[in] list SortedList_s to search for the node.
/// \param[out] result Resulting node.
/// \param[in] position Node's position.
///
/// \return DS_ERR_INVALID_OPERATION if list is empty.
/// \return DS_ERR_ITER if during iteration the scanner references to \c NULL.
/// \return DS_ERR_NEGATIVE_VALUE if position parameter is negative.
/// \return DS_ERR_NULL_POINTER if the list references to \c NULL.
/// \return DS_ERR_OUT_OF_RANGE if position parameter is greater than or equal
/// to the list's length.
/// \return DS_OK if all operations are successful.
static Status sli_get_node_at(SortedList list, SortedListNode *result,
        integer_t position)
{
    // This function effectively searches for a given node. If the position is
    // greater than the list length the search will begin at the end of the list,
    // reducing the amount of iterations needed. This effectively reduces searches
    // to O(n / 2) iterations.
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

        for (integer_t i = 0; i < position; i++)
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

        for (integer_t i = list->length - 1; i > position; i--)
        {
            // Bad iteration :(
            if ((*result) == NULL)
                return DS_ERR_ITER;

            (*result) = (*result)->prev;
        }
    }

    return DS_OK;
}

/// \brief Inserts an element at the tail of the list.
///
/// Implementation detail. Used to copy a list.
///
/// \param[in] list SortedList_s to insert the element.
/// \param[in] element Element to be inserted at the tail of the list.
///
/// \return DS_ERR_FULL if the list has reached its limited length.
/// \return DS_ERR_NULL_POINTER if node references to \c NULL.
/// \return DS_OK if all operations are successful.
static Status sli_insert_tail(SortedList list, void *element)
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

/// \brief An iterator for a SortedList_s
///
/// This iterator provides many useful functions to iterate along a SortedList.
/// It only provides functions that won't change the sorted property so adding
/// elements with the iterator is forbidden.
///
/// To initialize an iterator use sli_iter_init(). To free it from memory use
/// sli_iter_free(). Note that this will not free its target SortedList_s.
///
/// If any modifications are done to the structure of the iterator's target,
/// it will no longer function because the iterator keeps a copy of the
/// version_id of its target when initialized. You can use sli_iter_retarget()
/// to target the iterator to another SortedList_s or the same one if it was
/// modified.
///
/// You can use sli_iter_next() and sli_iter_prev() to iterate through the
/// list. You can also use sli_iter_to_head() to move the cursor to the head of
/// the list or sli_iter_to_tail() to move the cursor to the tail of the list.
///
/// You can also remove elements during iteration. This will update both IDs
/// from the iterator's and the target's. This is done so that if you have two
/// iterators targeting the same structure, the first modification caused by
/// one of them will disable the other. This is to prevent any undefined
/// behaviour.
///
/// \b Functions \b List
/// - sli_iter_init()
/// - sli_iter_free()
/// - sli_iter_next()
/// - sli_iter_prev()
/// - sli_iter_to_head()
/// - sli_iter_to_tail()
/// - sli_iter_has_next()
/// - sli_iter_has_prev()
/// - sli_iter_get()
/// - sli_iter_remove_next()
/// - sli_iter_remove_curr()
/// - sli_iter_remove_prev()
/// - sli_iter_peek_next()
/// - sli_iter_peek()
/// - sli_iter_peek_prev()
struct SortedListIterator_s
{
    /// \brief Target SortedList_s.
    ///
    /// Target SortedList_s. The iterator might need to use some information
    /// provided by the list or change some of its data members.
    struct SortedList_s *target;

    /// \brief Current element.
    ///
    /// Points to the current node. The iterator is always initialized with the
    /// cursor pointing to the start (head) of the list.
    struct SortedListNode_s *cursor;

    /// \brief Target version ID.
    ///
    /// When the iterator is initialized it stores the version_id of the target
    /// structure. This is kept to prevent iteration on the target structure
    /// that may have been modified and thus causing undefined behaviours or
    /// run-time crashes.
    integer_t target_id;
};

///////////////////////////////////////////////////// NOT EXPOSED FUNCTIONS ///

static bool sli_iter_target_modified(SortedListIterator iter);

static bool sli_iter_invalid_state(SortedListIterator iter);

////////////////////////////////////////////// END OF NOT EXPOSED FUNCTIONS ///

/// \brief Initializes a SortedListIterator_s.
///
/// Initializes a SortedListIterator_s with a target SortedList_s. The iterator
/// requires no default functions.
///
/// \param[in,out] iter Iterator to be initialized.
/// \param[in] target Target SortedList_s.
///
/// \return DS_ERR_ALLOC if iterator allocation failed.
/// \return DS_ERR_NULL_POINTER if the target parameter references to \c NULL.
/// \return DS_OK if all operations are successful.
Status sli_iter_init(SortedListIterator *iter, SortedList target)
{
    if (target == NULL)
        return DS_ERR_NULL_POINTER;

    *iter = malloc(sizeof(SortedListIterator_t));

    if (!(*iter))
        return DS_ERR_ALLOC;

    (*iter)->target = target;
    (*iter)->target_id = target->version_id;
    (*iter)->cursor = target->head;

    return DS_OK;
}

/// \brief Re-targets the iterator.
///
/// This function is equivalent to calling sli_iter_free() and then
/// sli_iter_init().
///
/// \param[in,out] iter Iterator to be re-targeted.
/// \param[in] target Target SortedList_s.
///
/// \return DS_ERR_ALLOC if iterator reallocation failed.
/// \return DS_ERR_NULL_POINTER if the list references to \c NULL.
/// \return DS_OK if all operations are successful.
Status sli_iter_retarget(SortedListIterator *iter, SortedList target)
{
    Status st = sli_iter_free(iter);

    if (st != DS_OK)
        return st;

    st = sli_iter_init(iter, target);

    if (st != DS_OK)
        return st;

    return DS_OK;
}

/// \brief Frees from memory a SortedListIterator_s.
///
/// Frees from memory a SortedListIterator_s. Note that this does not
/// deallocates the target SortedList_s.
///
/// \param[in,out] iter SortedListIterator_s to be freed from memory.
///
/// \return DS_ERR_NULL_POINTER if the list references to \c NULL.
/// \return DS_OK if all operations are successful.
Status sli_iter_free(SortedListIterator *iter)
{
    if (*iter == NULL)
        return DS_ERR_NULL_POINTER;

    free(*iter);

    *iter = NULL;

    return DS_OK;
}

/// \brief Iterates to the next element.
///
/// Moves the cursor to the next element if possible.
///
/// \param[in] iter Iterator reference.
///
/// \return DS_ERR_NULL_POINTER if the iterator references to \c NULL.
/// \return DS_ERR_ITER_STATE if either the iterator's target or the cursor are
/// referencing to \c NULL.
/// \return DS_ERR_ITER_MODIFICATION if the target list has a different
/// version_id than the iterator's.
/// \return DS_ERR_ITER if there is no next element.
/// \return DS_OK if all operations are successful.
Status sli_iter_next(SortedListIterator iter)
{
    if (iter == NULL)
        return DS_ERR_NULL_POINTER;

    if (sli_iter_invalid_state(iter))
        return DS_ERR_ITER_STATE;

    if (sli_iter_target_modified(iter))
        return DS_ERR_ITER_MODIFICATION;

    if (!sli_iter_has_next(iter))
        return DS_ERR_ITER;

    iter->cursor = iter->cursor->next;

    return DS_OK;
}

/// \brief Iterates to the previous element.
///
/// Moves the cursor to the previous element if possible.
///
/// \param[in] iter Iterator reference.
///
/// \return DS_ERR_NULL_POINTER if the iterator references to \c NULL.
/// \return DS_ERR_ITER_STATE if either the iterator's target or the cursor are
/// referencing to \c NULL.
/// \return DS_ERR_ITER_MODIFICATION if the target list has a different
/// version_id than the iterator's.
/// \return DS_ERR_ITER if there is no previous element.
/// \return DS_OK if all operations are successful.
Status sli_iter_prev(SortedListIterator iter)
{
    if (iter == NULL)
        return DS_ERR_NULL_POINTER;

    if (sli_iter_invalid_state(iter))
        return DS_ERR_ITER_STATE;

    if (sli_iter_target_modified(iter))
        return DS_ERR_ITER_MODIFICATION;

    if (!sli_iter_has_prev(iter))
        return DS_ERR_ITER;

    iter->cursor = iter->cursor->prev;

    return DS_OK;
}

/// \brief Sends the cursor to the head of the list.
///
/// Sends the cursor to the head of the list.
///
/// \param[in] iter Iterator reference.
///
/// \return DS_ERR_NULL_POINTER if the iterator references to \c NULL.
/// \return DS_ERR_ITER_STATE if either the iterator's target or the cursor are
/// referencing to \c NULL.
/// \return DS_ERR_ITER_MODIFICATION if the target list has a different
/// version_id than the iterator's.
/// \return DS_OK if all operations are successful.
Status sli_iter_to_head(SortedListIterator iter)
{
    if (iter == NULL)
        return DS_ERR_NULL_POINTER;

    if (sli_iter_invalid_state(iter))
        return DS_ERR_ITER_STATE;

    if (sli_iter_target_modified(iter))
        return DS_ERR_ITER_MODIFICATION;

    iter->cursor = iter->target->head;

    return DS_OK;
}

/// \brief Sends the cursor to the tail of the list.
///
/// Sends the cursor to the tail of the list.
///
/// \param[in] iter Iterator reference.
///
/// \return DS_ERR_NULL_POINTER if the iterator references to \c NULL.
/// \return DS_ERR_ITER_STATE if either the iterator's target or the cursor are
/// referencing to \c NULL.
/// \return DS_ERR_ITER_MODIFICATION if the target list has a different
/// version_id than the iterator's.
/// \return DS_OK if all operations are successful.
Status sli_iter_to_tail(SortedListIterator iter)
{
    if (iter == NULL)
        return DS_ERR_NULL_POINTER;

    if (sli_iter_invalid_state(iter))
        return DS_ERR_ITER_STATE;

    if (sli_iter_target_modified(iter))
        return DS_ERR_ITER_MODIFICATION;

    iter->cursor = iter->target->tail;

    return DS_OK;
}

/// \brief Checks if the iterator has a next element.
///
/// Returns true if the current iterator's cursor has a next element. Returns
/// false if it is the tail node.
///
/// \warning This function does not checks for \c NULL references and is prone
/// to provoke run-time errors if not used correctly.
///
/// \param[in] iter Iterator reference.
///
/// \return True if there is a next element.
/// \return False if there is no next element.
bool sli_iter_has_next(SortedListIterator iter)
{
    return iter->cursor->next != NULL;
}

/// \brief Checks if the iterator has a previous element.
///
/// Returns true if the current iterator's cursor has a previous element.
/// Returns false if it is the head node.
///
/// \warning This function does not checks for \c NULL references and is prone
/// to provoke run-time errors if not used correctly.
///
/// \param[in] iter Iterator reference.
///
/// \return True if there is a previous element.
/// \return False if there is no previous element.
bool sli_iter_has_prev(SortedListIterator iter)
{
    return iter->cursor->prev != NULL;
}

/// \brief Gets a copy of the cursors's data.
///
/// Gets a copy of the cursor's data. The result defaults to \c NULL if any
/// errors are returned.
///
/// \param[in] iter Iterator reference.
/// \param[out] result Resulting element.
///
/// \return DS_ERR_NULL_POINTER if the iterator references to \c NULL.
/// \return DS_ERR_ITER_STATE if either the iterator's target or the cursor are
/// referencing to \c NULL.
/// \return DS_ERR_ITER_MODIFICATION if the target list has a different
/// version_id than the iterator's.
/// \return DS_ERR_INCOMPLETE_TYPE if a default copy function is not set.
/// \return DS_OK if all operations are successful.
Status sli_iter_get(SortedListIterator iter, void **result)
{
    *result = NULL;

    if (iter == NULL)
        return DS_ERR_NULL_POINTER;

    if (sli_iter_invalid_state(iter))
        return DS_ERR_ITER_STATE;

    if (sli_iter_target_modified(iter))
        return DS_ERR_ITER_MODIFICATION;

    if (iter->target->v_copy == NULL)
        return DS_ERR_INCOMPLETE_TYPE;

    // Makes a copy of the element to preserve the list integrity.
    *result = iter->target->v_copy(iter->cursor->data);

    return DS_OK;
}

/// \brief Removes the next element relative to the iterator's cursor.
///
/// Removes the next element relative to the iterator's cursor. This function
/// will change both version IDs, from the list and the iterator's.
///
/// \param[in] iter Iterator reference.
/// \param[out] result Resulting element.
///
/// \return DS_ERR_NULL_POINTER if the iterator references to \c NULL.
/// \return DS_ERR_ITER_STATE if either the iterator's target or the cursor are
/// referencing to \c NULL.
/// \return DS_ERR_ITER_MODIFICATION if the target list has a different
/// version_id than the iterator's.
/// \return DS_ERR_ITER if there is no next element.
/// \return DS_OK if all operations are successful.
Status sli_iter_remove_next(SortedListIterator iter, void **result)
{
    if (iter == NULL)
        return DS_ERR_NULL_POINTER;

    if (sli_iter_invalid_state(iter))
        return DS_ERR_ITER_STATE;

    if (sli_iter_target_modified(iter))
        return DS_ERR_ITER_MODIFICATION;

    if (!sli_iter_has_next(iter))
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

    iter->target_id++;
    iter->target->version_id++;

    return DS_OK;
}

/// \brief Removes the current element pointed by the iterator's cursor.
///
/// Removes the current element pointed by the iterator's cursor. This function
/// will change both version IDs, from the list and the iterator's.
///
/// \param[in] iter Iterator reference.
/// \param[out] result Resulting element.
///
/// \return DS_ERR_NULL_POINTER if the iterator references to \c NULL.
/// \return DS_ERR_ITER_STATE if either the iterator's target or the cursor are
/// referencing to \c NULL.
/// \return DS_ERR_ITER_MODIFICATION if the target list has a different
/// version_id than the iterator's.
/// \return DS_OK if all operations are successful.
Status sli_iter_remove_curr(SortedListIterator iter, void **result)
{
    if (iter == NULL)
        return DS_ERR_NULL_POINTER;

    if (sli_iter_invalid_state(iter))
        return DS_ERR_ITER_STATE;

    if (sli_iter_target_modified(iter))
        return DS_ERR_ITER_MODIFICATION;

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

    iter->target_id++;
    iter->target->version_id++;

    return DS_OK;
}

/// \brief Removes the previous element relative to the iterator's cursor.
///
/// Removes the previous element relative to the iterator's cursor. This
/// function will change both version IDs, from the list and the iterator's.
///
/// \param[in] iter Iterator reference.
/// \param[out] result Resulting element.
///
/// \return DS_ERR_NULL_POINTER if the iterator references to \c NULL.
/// \return DS_ERR_ITER_STATE if either the iterator's target or the cursor are
/// referencing to \c NULL.
/// \return DS_ERR_ITER_MODIFICATION if the target list has a different
/// version_id than the iterator's.
/// \return DS_ERR_ITER if there is no previous element.
/// \return DS_OK if all operations are successful.
Status sli_iter_remove_prev(SortedListIterator iter, void **result)
{
    if (iter == NULL)
        return DS_ERR_NULL_POINTER;

    if (sli_iter_invalid_state(iter))
        return DS_ERR_ITER_STATE;

    if (sli_iter_target_modified(iter))
        return DS_ERR_ITER_MODIFICATION;

    if (!sli_iter_has_prev(iter))
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

    iter->target_id++;
    iter->target->version_id++;

    return DS_OK;
}

/// \brief Returns the next element if available.
///
/// Returns the pointer to the next element in the list. Use it as a read-only.
/// If you make any changes to this element the internal structure of the list
/// will change and may cause undefined behaviour.
///
/// \param[in] iter Iterator reference.
///
/// \return NULL if the iterator references to \c NULL, if either the cursor or
/// the target pointers are referencing to \c NULL, if the list's version_id is
/// different from the iterator's or if there is no next element.
/// \return The next element.
void *sli_iter_peek_next(SortedListIterator iter)
{
    if (iter == NULL)
        return NULL;

    if (sli_iter_invalid_state(iter))
        return NULL;

    if (sli_iter_target_modified(iter))
        return NULL;

    if (!sli_iter_has_next(iter))
        return NULL;

    return iter->cursor->next->data;
}

/// \brief Returns the element pointed by the cursor.
///
/// Returns the element pointed by the iterator cursor. Use it as a read-only
/// If you make any changes to this element the internal structure of the list
/// list will change and may cause undefined behaviour.
///
/// \param[in] iter Iterator reference.
///
/// \return NULL if the iterator references to \c NULL, if either the cursor or
/// the target pointers are referencing to \c NULL, if the list's version_id is
/// different from the iterator's or if there is no next element.
/// \return The current element.
void *sli_iter_peek(SortedListIterator iter)
{
    if (iter == NULL)
        return NULL;

    if (sli_iter_invalid_state(iter))
        return NULL;

    if (sli_iter_target_modified(iter))
        return NULL;

    return iter->cursor->data;
}

/// \brief Returns the previous element if available.
///
/// Returns the pointer to the previous element in the list. Use it as a
/// read-only. If you make any changes to this element the internal structure
/// of the lists will change and may cause undefined behaviour.
///
/// \param[in] iter Iterator reference.
///
/// \return NULL if the iterator references to \c NULL, if either the cursor or
/// the target pointers are referencing to \c NULL, if the list's version_id is
/// different from the iterator's or if there is no next element.
/// \return The previous element.
void *sli_iter_peek_prev(SortedListIterator iter)
{
    if (iter == NULL)
        return NULL;

    if (sli_iter_invalid_state(iter))
        return NULL;

    if (sli_iter_target_modified(iter))
        return NULL;

    if (!sli_iter_has_prev(iter))
        return NULL;

    return iter->cursor->prev->data;
}

///////////////////////////////////////////////////// NOT EXPOSED FUNCTIONS ///

/// \brief Checks if the target list was modified.
///
/// Implementation detail. Checks both IDs. If they don't match, then the list
/// was change since after the creation of the iterator.
///
/// \param[in] iter Iterator reference.
///
/// \return True if the target was modified.
/// \return False if the target was not modified.
static bool sli_iter_target_modified(SortedListIterator iter)
{
    return iter->target_id != iter->target->version_id;
}

/// \brief Checks if the iterator is in an invalid state.
///
/// Implementation detail. Returns true if either the cursor or the target
/// pointers are referencing to \c NULL.
///
/// \param[in] iter Iterator reference.
///
/// \return True if either the cursor or the target are referencing to \c NULL.
/// \return False if neither the cursor or the target are referencing to
/// \c NULL.
static bool sli_iter_invalid_state(SortedListIterator iter)
{
    return iter->cursor == NULL || iter->target == NULL;
}

////////////////////////////////////////////// END OF NOT EXPOSED FUNCTIONS ///

///////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////// Wrapper ///
///////////////////////////////////////////////////////////////////////////////

// Global sorted list used by the Wrapper.
static SortedList GlobalSortedList = NULL;

///////////////////////////////////////////////////// NOT EXPOSED FUNCTIONS ///

// All these functions are part of the SortedListWrapper.

static Status sli_wrap_set_compare(sli_compare_f function);

static Status sli_wrap_set_copy(sli_copy_f function);

static Status sli_wrap_set_display(sli_display_f function);

static Status sli_wrap_set_free(sli_free_f function);

static Status sli_wrap_set_limit(integer_t limit);

static Status sli_wrap_set_order(SortOrder order);

static integer_t sli_wrap_length(void);

static integer_t sli_wrap_limit(void);

static SortOrder sli_wrap_order(void);

static Status sli_wrap_get(void **result, integer_t index);

static Status sli_wrap_insert(void *element);

static Status sli_wrap_insert_all(void **elements, integer_t count);

static Status sli_wrap_remove(void **result, integer_t position);

static Status sli_wrap_remove_max(void **result);

static Status sli_wrap_remove_min(void **result);

static bool sli_wrap_full(void);

static bool sli_wrap_empty(void);

static void *sli_wrap_max(void);

static void *sli_wrap_min(void);

static integer_t sli_wrap_index_first(void *key);

static integer_t sli_wrap_index_last(void *key);

static bool sli_wrap_contains(void *key);

static Status sli_wrap_reverse(void);

static Status sli_wrap_copy(SortedList *result);

static Status sli_wrap_to_array(void ***result, integer_t *length);

static Status sli_wrap_merge(SortedList list);

static Status sli_wrap_unlink(SortedList *result, integer_t position);

static Status sli_wrap_sublist(SortedList *result, integer_t start, integer_t end);

static Status sli_wrap_display(void);

static Status sli_wrap_display_array(void);

static Status sli_wrap_display_raw(void);

////////////////////////////////////////////// END OF NOT EXPOSED FUNCTIONS ///

/// \brief Initializes a SortedListWrapper_s.
///
/// Initializes a SortedListWrapper_s with a given target.
///
/// \param wrapper Wrapper to be initialized.
/// \param target Target SortedList_s.
///
/// \return DS_ERR_ALLOC if wrapper allocation failed.
/// \return DS_ERR_NULL_POINTER if the target parameter references to \c NULL.
/// \return DS_OK if all operations are successful.
Status sli_wrap_init(SortedListWrapper *wrapper, SortedList target)
{
    if (target == NULL)
        return DS_ERR_NULL_POINTER;

    *wrapper = malloc(sizeof(SortedListWrapper_t));

    if (!(*wrapper))
        return DS_ERR_ALLOC;

    GlobalSortedList = target;

    // Setting all wrapper functions
    (*wrapper)->set_compare = sli_wrap_set_compare;
    (*wrapper)->set_copy = sli_wrap_set_copy;
    (*wrapper)->set_display = sli_wrap_set_display;
    (*wrapper)->set_free = sli_wrap_set_free;
    (*wrapper)->set_limit = sli_wrap_set_limit;
    (*wrapper)->set_order = sli_wrap_set_order;

    (*wrapper)->length = sli_wrap_length;
    (*wrapper)->limit = sli_wrap_limit;
    (*wrapper)->order = sli_wrap_order;
    (*wrapper)->get_copy = sli_wrap_get;

    (*wrapper)->insert = sli_wrap_insert;
    (*wrapper)->insert_all = sli_wrap_insert_all;
    (*wrapper)->remove = sli_wrap_remove;
    (*wrapper)->remove_max = sli_wrap_remove_max;
    (*wrapper)->remove_min = sli_wrap_remove_min;

    (*wrapper)->full = sli_wrap_full;
    (*wrapper)->empty = sli_wrap_empty;

    (*wrapper)->max = sli_wrap_max;
    (*wrapper)->min = sli_wrap_min;
    (*wrapper)->index_first = sli_wrap_index_first;
    (*wrapper)->index_last = sli_wrap_index_last;
    (*wrapper)->contains = sli_wrap_contains;
    (*wrapper)->reverse = sli_wrap_reverse;
    (*wrapper)->copy = sli_wrap_copy;
    (*wrapper)->to_array = sli_wrap_to_array;

    (*wrapper)->merge = sli_wrap_merge;
    (*wrapper)->unlink = sli_wrap_unlink;
    (*wrapper)->sublist = sli_wrap_sublist;

    (*wrapper)->display = sli_wrap_display;
    (*wrapper)->display_array = sli_wrap_display_array;
    (*wrapper)->display_raw = sli_wrap_display_raw;

    return DS_OK;
}

/// \brief Re-targets the wrapper.
///
/// This function simply changes the value of the GlobalSortedList to a new
/// target.
///
/// \param wrapper Wrapper to be re-targeted.
/// \param target Target SortedList_s.
///
/// \return DS_ERR_NULL_POINTER if either the wrapper or the target are
/// referencing to \c NULL.
/// \return DS_OK if all operations are successful.
Status sli_wrap_retarget(SortedListWrapper wrapper, SortedList target)
{
    if (wrapper == NULL || target == NULL)
        return DS_ERR_NULL_POINTER;

    GlobalSortedList = target;

    return DS_OK;
}

/// \brief Frees from memory a SortedListWrapper_s.
///
/// Frees from memory a SortedListIterator_s. Note that this does not
/// deallocates the target SortedList_s.
///
/// \param[in,out] wrapper SortedListWrapper_s to be freed from memory.
///
/// \return DS_ERR_NULL_POINTER if the wrapper references to \c NULL.
/// \return DS_OK if all operations are successful.
Status sli_wrap_free(SortedListWrapper *wrapper)
{
    if (*wrapper == NULL)
        return DS_ERR_NULL_POINTER;

    free(*wrapper);

    *wrapper = NULL;

    GlobalSortedList = NULL;

    return DS_OK;
}

///////////////////////////////////////////////////// NOT EXPOSED FUNCTIONS ///

static Status sli_wrap_set_compare(sli_compare_f function)
{
    if (GlobalSortedList == NULL)
        return DS_ERR_WRAPPER;

    return sli_set_v_compare(GlobalSortedList, function);
}

static Status sli_wrap_set_copy(sli_copy_f function)
{
    if (GlobalSortedList == NULL)
        return DS_ERR_WRAPPER;

    return sli_set_v_copy(GlobalSortedList, function);
}

static Status sli_wrap_set_display(sli_display_f function)
{
    if (GlobalSortedList == NULL)
        return DS_ERR_WRAPPER;

    return sli_set_v_display(GlobalSortedList, function);
}

static Status sli_wrap_set_free(sli_free_f function)
{
    if (GlobalSortedList == NULL)
        return DS_ERR_WRAPPER;

    return sli_set_v_free(GlobalSortedList, function);
}

static Status sli_wrap_set_limit(integer_t limit)
{
    if (GlobalSortedList == NULL)
        return DS_ERR_WRAPPER;

    return sli_set_limit(GlobalSortedList, limit);
}

static Status sli_wrap_set_order(SortOrder order)
{
    if (GlobalSortedList == NULL)
        return DS_ERR_WRAPPER;

    return sli_set_order(GlobalSortedList, order);
}

static integer_t sli_wrap_length(void)
{
    return sli_length(GlobalSortedList);
}

static integer_t sli_wrap_limit(void)
{
    return sli_limit(GlobalSortedList);
}

static SortOrder sli_wrap_order(void)
{
    return sli_order(GlobalSortedList);
}

static Status sli_wrap_get(void **result, integer_t index)
{
    if (GlobalSortedList == NULL)
        return DS_ERR_WRAPPER;

    return sli_get(GlobalSortedList, result, index);
}

static Status sli_wrap_insert(void *element)
{
    if (GlobalSortedList == NULL)
        return DS_ERR_WRAPPER;

    return sli_insert(GlobalSortedList, element);
}

static Status sli_wrap_insert_all(void **elements, integer_t count)
{
    if (GlobalSortedList == NULL)
        return DS_ERR_WRAPPER;

    return sli_insert_all(GlobalSortedList, elements, count);
}

static Status sli_wrap_remove(void **result, integer_t position)
{
    if (GlobalSortedList == NULL)
        return DS_ERR_WRAPPER;

    return sli_remove(GlobalSortedList, result, position);
}

static Status sli_wrap_remove_max(void **result)
{
    if (GlobalSortedList == NULL)
        return DS_ERR_WRAPPER;

    return sli_remove_max(GlobalSortedList, result);
}

static Status sli_wrap_remove_min(void **result)
{
    if (GlobalSortedList == NULL)
        return DS_ERR_WRAPPER;

    return sli_remove_min(GlobalSortedList, result);
}

static bool sli_wrap_full(void)
{
    return sli_full(GlobalSortedList);
}

static bool sli_wrap_empty(void)
{
    return sli_empty(GlobalSortedList);
}

static void *sli_wrap_max(void)
{
    return sli_max(GlobalSortedList);
}

static void *sli_wrap_min(void)
{
    return sli_min(GlobalSortedList);
}

static integer_t sli_wrap_index_first(void *key)
{
    return sli_index_first(GlobalSortedList, key);
}

static integer_t sli_wrap_index_last(void *key)
{
    return sli_index_last(GlobalSortedList, key);
}

static bool sli_wrap_contains(void *key)
{
    return sli_contains(GlobalSortedList, key);
}

static Status sli_wrap_reverse(void)
{
    if (GlobalSortedList == NULL)
        return DS_ERR_WRAPPER;

    return sli_reverse(GlobalSortedList);
}

static Status sli_wrap_copy(SortedList *result)
{
    if (GlobalSortedList == NULL)
        return DS_ERR_WRAPPER;

    return sli_copy(GlobalSortedList, result);
}

static Status sli_wrap_to_array(void ***result, integer_t *length)
{
    if (GlobalSortedList == NULL)
        return DS_ERR_WRAPPER;

    return sli_to_array(GlobalSortedList, result, length);
}

static Status sli_wrap_merge(SortedList list)
{
    if (GlobalSortedList == NULL)
        return DS_ERR_WRAPPER;

    return sli_merge(GlobalSortedList, list);
}

static Status sli_wrap_unlink(SortedList *result, integer_t position)
{
    if (GlobalSortedList == NULL)
        return DS_ERR_WRAPPER;

    return sli_unlink(GlobalSortedList, result, position);
}

static Status sli_wrap_sublist(SortedList *result, integer_t start, integer_t end)
{
    if (GlobalSortedList == NULL)
        return DS_ERR_WRAPPER;

    return sli_sublist(GlobalSortedList, result, start, end);
}

static Status sli_wrap_display(void)
{
    if (GlobalSortedList == NULL)
        return DS_ERR_WRAPPER;

    return sli_display(GlobalSortedList);
}

static Status sli_wrap_display_array(void)
{
    if (GlobalSortedList == NULL)
        return DS_ERR_WRAPPER;

    return sli_display_array(GlobalSortedList);
}

static Status sli_wrap_display_raw(void)
{
    if (GlobalSortedList == NULL)
        return DS_ERR_WRAPPER;

    return sli_display_raw(GlobalSortedList);
}

////////////////////////////////////////////// END OF NOT EXPOSED FUNCTIONS ///