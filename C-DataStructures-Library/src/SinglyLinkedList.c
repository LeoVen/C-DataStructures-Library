/**
 * @file SinglyLinkedList.c
 *
 * @author Leonardo Vencovsky (https://github.com/LeoVen)
 *
 * @date 25/09/2018
 */

#include "SinglyLinkedList.h"

/// \brief A generic singly-linked list.
///
/// A SinglyLinkedList_s is a linear structure where its elements are not
/// stored in contiguous memory allowing constant insertion and removal at both
/// ends of the list. Insertion and removal at the middle of the list are at
/// most O(n - 1). There are no buffer reallocation nor shifting of elements
/// since the data is 'linked' by nodes and at any time these links can be
/// removed; this makes it trivial to remove elements at the middle without
/// having to shift elements.
///
/// \todo Add more description on how the SinglyLinkedList_s works.
///
/// \b Advantages over \c DynamicArray_s
/// - Dynamic size
/// - Easy insertion/removal
///
/// \b Drawbacks
/// - No random access
/// - Extra memory for a pointer on each element
///
/// \b Functions
///
/// Located in the file SinglyLinkedList.c
struct SinglyLinkedList_s
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
    struct SinglyLinkedNode_s *head;

    /// \brief Points to the last Node on the list.
    ///
    /// Points to the first Node on the list or \c NULL if the list is empty.
    struct SinglyLinkedNode_s *tail;

    /// \brief Comparator function.
    ///
    /// A function that compares one element with another that returns an int
    /// with the following rules:
    ///
    /// - <code>[ > 0 ]</code> if first element is greater than the second;
    /// - <code>[ < 0 ]</code> if second element is greater than the first;
    /// - <code>[ 0 ]</code> if elements are equal.
    sll_compare_f v_compare;

    /// \brief Copy function.
    ///
    /// A function that returns an exact copy of an element.
    sll_copy_f v_copy;

    /// \brief Display function.
    ///
    /// A function that displays an element in the console. Useful for
    /// debugging.
    sll_display_f v_display;

    /// \brief Deallocator function.
    ///
    /// A function that completely frees an element from memory.
    sll_free_f v_free;

    /// \brief A version id to keep track of modifications.
    ///
    /// This version id is used by the iterator to check if the structure was
    /// modified. The iterator can only function if its version_id is the same
    /// as the structure's version id, that is, there have been no structural
    /// modifications (except for those done by the iterator itself).
    integer_t version_id;
};

/// \brief A SinglyLinkedList_s node.
///
/// Implementation detail. This is a singly-linked node. It has one data member
/// and one pointer to the next node or \c NULL if it is the tail node.
struct SinglyLinkedNode_s
{
    /// \brief Data pointer.
    ///
    /// Points to node's data. The data needs to be dynamically allocated.
    void *data;

    /// \brief Next node on the list.
    ///
    /// Next node on the list or \c NULL if this is the tail node.
    struct SinglyLinkedNode_s *next;
};

/// \brief A type for a singly-linked list node.
///
/// Defines a type to a <code> struct SinglyLinkedNode_s </code>.
typedef struct SinglyLinkedNode_s SinglyLinkedNode_t;

/// \brief A pointer type for a singly-linked list node.
///
/// Defines a pointer type to a <code> struct SinglyLinkedNode_s </code>.
typedef struct SinglyLinkedNode_s *SinglyLinkedNode;

///////////////////////////////////////////////////// NOT EXPOSED FUNCTIONS ///

static Status sll_make_node(SinglyLinkedNode *node, void *element);

static Status sll_free_node(SinglyLinkedNode *node, sll_free_f free_f);

static Status sll_free_node_shallow(SinglyLinkedNode *node);

static Status sll_get_node_at(SinglyLinkedList list, SinglyLinkedNode *result,
        integer_t position);

////////////////////////////////////////////// END OF NOT EXPOSED FUNCTIONS ///

/// \brief Initializes a SinglyLinkedList_s structure.
///
/// Initializes a new SinglyLinkedList_s structure with initial length and
/// limit as 0. Note that it does not sets any default functions. If you don't
/// set them later you won't be able to do certain operations that depend on a
/// user-defined function.
///
/// \param[in,out] list The list to be initialized.
///
/// \return DS_ERR_ALLOC if list allocation failed.
/// \return DS_OK if all operations were successful.
Status sll_init(SinglyLinkedList *list)
{
    (*list) = malloc(sizeof(SinglyLinkedList_t));

    if (!(*list))
        return DS_ERR_ALLOC;

    (*list)->length = 0;
    (*list)->limit = 0;
    (*list)->version_id = 0;

    (*list)->head = NULL;
    (*list)->tail = NULL;

    (*list)->v_compare = NULL;
    (*list)->v_copy = NULL;
    (*list)->v_display = NULL;
    (*list)->v_free = NULL;

    return DS_OK;
}

/// \brief Creates a SinglyLinkedList_s.
///
/// This function completely creates a SinglyLinkedList_s, setting all of its
/// default functions.
///
/// \param[in,out] list SinglyLinkedList_s to be allocated.
/// \param[in] compare_f A function that compares two elements.
/// \param[in] copy_f A function that makes an exact copy of an element.
/// \param[in] display_f A function that displays in the console an element.
/// \param[in] free_f A function that completely frees from memory an element.
///
/// \return DS_ERR_ALLOC if list allocation failed.
/// \return DS_OK if all operations are successful.
Status sll_create(SinglyLinkedList *list, sll_compare_f compare_f,
        sll_copy_f copy_f, sll_display_f display_f, sll_free_f free_f)
{
    *list = malloc(sizeof(SinglyLinkedList_t));

    if (!(*list))
        return DS_ERR_ALLOC;

    (*list)->length = 0;
    (*list)->limit = 0;
    (*list)->version_id = 0;

    (*list)->head = NULL;
    (*list)->tail = NULL;

    (*list)->v_compare = compare_f;
    (*list)->v_copy = copy_f;
    (*list)->v_display = display_f;
    (*list)->v_free = free_f;

    return DS_OK;
}

/// \brief Frees from memory a SinglyLinkedList_s and all its elements.
///
/// This function frees from memory all the list's elements using its default
/// free function and then frees the list's structure. The variable is then set
/// to \c NULL.
///
/// \param[in,out] list The SinglyLinkedList_s to be freed from memory.
///
/// \return DS_ERR_INCOMPLETE_TYPE if a default free function is not set.
/// \return DS_ERR_NULL_POINTER if the list references to \c NULL.
/// \return DS_OK if all operations were successful.
Status sll_free(SinglyLinkedList *list)
{
    if (*list == NULL)
        return DS_ERR_NULL_POINTER;

    if ((*list)->v_free == NULL)
        return DS_ERR_INCOMPLETE_TYPE;

    SinglyLinkedNode prev = (*list)->head;

    Status st;

    while ((*list)->head != NULL)
    {
        (*list)->head = (*list)->head->next;

        st = sll_free_node(&prev, (*list)->v_free);

        if (st != DS_OK)
            return st;

        prev = (*list)->head;
    }

    free(*list);

    *list = NULL;

    return DS_OK;
}

/// \brief Frees from memory a SinglyLinkedList_s.
///
/// This function frees from memory all the list's nodes without freeing its
/// data and then frees the list's structure. The variable is then set to
/// \c NULL.
///
/// \param[in,out] list SinglyLinkedList_s to be freed from memory.
///
/// \return DS_ERR_NULL_POINTER if the list references to \c NULL.
/// \return DS_OK if all operations are successful.
Status sll_free_shallow(SinglyLinkedList *list)
{
    if (*list == NULL)
        return DS_ERR_NULL_POINTER;

    SinglyLinkedNode prev = (*list)->head;

    Status st;

    while ((*list)->head != NULL)
    {
        (*list)->head = (*list)->head->next;

        st = sll_free_node_shallow(&prev);

        if (st != DS_OK)
            return st;

        prev = (*list)->head;
    }

    free(*list);

    *list = NULL;

    return DS_OK;
}

/// \brief Erases a SinglyLinkedList_s.
///
/// This function is equivalent to freeing a list and then creating it again.
/// This will reset the list to its initial state with no elements, but will
/// keep all of its default functions.
///
/// \param[in,out] list SinglyLinkedList_s to be erased.
///
/// \return DS_ERR_ALLOC if list allocation failed.
/// \return DS_ERR_INCOMPLETE_TYPE if a default free function is not set.
/// \return DS_ERR_NULL_POINTER if the list references to \c NULL.
/// \return DS_OK if all operations are successful.
Status sll_erase(SinglyLinkedList *list)
{
    if (*list == NULL)
        return DS_ERR_NULL_POINTER;

    SinglyLinkedList new_list;

    Status st = sll_create(&new_list, (*list)->v_compare, (*list)->v_copy,
            (*list)->v_display, (*list)->v_free);

    if (st !=  DS_OK)
        return st;

    st = sll_free(list);

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
/// Use this function to set a default compare function. It needs to comply
/// with the sll_compare_f specifications.
///
/// \param[in] list SinglyLinkedList_s to set the default compare function.
/// \param[in] function An sll_compare_f kind of function.
///
/// \return DS_ERR_NULL_POINTER if the list references to \c NULL.
/// \return DS_OK if all operations are successful.
Status sll_set_v_compare(SinglyLinkedList list, sll_compare_f function)
{
    if (list == NULL)
        return DS_ERR_NULL_POINTER;

    list->v_compare = function;

    return DS_OK;
}

/// \brief Sets the default copy function.
///
/// Use this function to set a default compare function. It needs to comply
/// with the sll_copy_f specifications.
///
/// \param[in] list SinglyLinkedList_s to set the default copy function.
/// \param[in] function An sll_copy_f kind of function.
///
/// \return DS_ERR_NULL_POINTER if the list references to \c NULL.
/// \return DS_OK if all operations are successful.
Status sll_set_v_copy(SinglyLinkedList list, sll_copy_f function)
{
    if (list == NULL)
        return DS_ERR_NULL_POINTER;

    list->v_copy = function;

    return DS_OK;
}

/// \brief Sets the default display function.
///
/// Use this function to set a default display function. It needs to comply
/// with the sll_display_f specifications. Useful for debugging.
///
/// \param[in] list SinglyLinkedList_s to set the default display function.
/// \param[in] function An sll_display_f kind of function.
///
/// \return DS_ERR_NULL_POINTER if the list references to \c NULL.
/// \return DS_OK if all operations are successful.
Status sll_set_v_display(SinglyLinkedList list, sll_display_f function)
{
    if (list == NULL)
        return DS_ERR_NULL_POINTER;

    list->v_display = function;

    return DS_OK;
}

/// \brief Sets the default free function.
///
/// Use this function to set a default free function. It needs to comply
/// with the sll_free_f specifications.
///
/// \param[in] list SinglyLinkedList_s to set the default free function.
/// \param[in] function An sll_free_f kind of function.
///
/// \return DS_ERR_NULL_POINTER if the list references to \c NULL.
/// \return DS_OK if all operations are successful.
Status sll_set_v_free(SinglyLinkedList list, sll_free_f function)
{
    if (list == NULL)
        return DS_ERR_NULL_POINTER;

    list->v_free = function;

    return DS_OK;
}

/// \brief Sets a limit to the specified SinglyLinkedList_s's length.
///
/// Limit's the SinglyLinkedList_s's length. You can only set a limit greater
/// or equal to the list's current length and greater than 0. To remove this
/// limitation simply set the limit to 0 or less.
///
/// \param[in] list SinglyLinkedList_s reference.
/// \param[in] limit Maximum list length.
///
/// \return DS_ERR_INVALID_OPERATION if the limitation is less than the list's
/// current length.
/// \return DS_ERR_NULL_POINTER if the list references to \c NULL.
/// \return DS_OK if all operations are successful.
Status sll_set_limit(SinglyLinkedList list, integer_t limit)
{
    if (list == NULL)
        return DS_ERR_NULL_POINTER;

    // The new limit can't be lower than the list's current length.
    if (list->length > limit && limit > 0)
        return DS_ERR_INVALID_OPERATION;

    list->limit = limit;

    return DS_OK;
}

/// \brief Sets the given position to a given element erasing the old one.
///
/// Sets an element at a given position. This function is 0 based, that is, the
/// element 0 is the first element. For this operation to be successful you
/// must have a set sll_free_f in order to erase the old element and set the
/// new one.
///
/// \param[in] list The SinglyLinkedList_s where the element is to be updated.
/// \param[in] element The new element value.
/// \param[in] position The position of the element to be updated.
///
/// \return DS_ERR_INCOMPLETE_TYPE if a default free function is not set.
/// \return DS_ERR_INVALID_OPERATION if the list is empty.
/// \return DS_ERR_NEGATIVE_VALUE if \c position parameter is a negative value.
/// \return DS_ERR_OUT_OF_RANGE if \c position parameter is greater than or
/// equal to the list \c length.
/// \return DS_ERR_NULL_POINTER if the list references to \c NULL.
/// \return DS_OK if all operations were successful.
Status sll_set(SinglyLinkedList list, void *element, integer_t position)
{
    if (list == NULL)
        return DS_ERR_NULL_POINTER;

    if (sll_empty(list))
        return DS_ERR_INVALID_OPERATION;

    if (position >= list->length)
        return DS_ERR_OUT_OF_RANGE;

    if (position < 0)
        return DS_ERR_NEGATIVE_VALUE;

    if (list->v_free == NULL)
        return DS_ERR_INCOMPLETE_TYPE;

    SinglyLinkedNode node = NULL;

    Status st = sll_get_node_at(list, &node, position);

    if (st != DS_OK)
        return st;

    list->v_free(node->data);

    node->data = element;

    return DS_OK;
}

/// \brief Returns the SinglyLinkedList_s's length.
///
/// Returns the list's current length or -1 if the list references to \c NULL.
///
/// \param[in] list SinglyLinkedList_s reference.
///
/// \return -1 if the list references to \c NULL.
/// \return The list's length.
integer_t sll_length(SinglyLinkedList list)
{
    if (list == NULL)
        return -1;

    return list->length;
}

/// \brief Returns the SinglyLinkedList_s's limit.
///
/// Returns the list limit or -1 if the list references to \c NULL.
///
/// \param[in] list SinglyLinkedList_s reference.
///
/// \return -1 if the list references to \c NULL.
/// \return The list's limit.
integer_t sll_limit(SinglyLinkedList list)
{
    if (list == NULL)
        return -1;

    return list->limit;
}

/// \brief Retrieves an element at a given position.
///
/// Retrieves an element at a given position without removing it. This function
/// simulates an index access like in arrays.
///
/// \param[in] list The SinglyLinkedList_s to retrieve the element from.
/// \param[out] result The resulting element.
/// \param[in] position The position of the element to be retrieved.
///
/// \return DS_ERR_INVALID_OPERATION if the list is empty.
/// \return DS_ERR_NEGATIVE_VALUE if \c position parameter is a negative value.
/// \return DS_ERR_OUT_OF_RANGE if \c position parameter is greater than or
/// equal to the list \c length.
/// \return DS_ERR_NULL_POINTER if the list references to \c NULL.
/// \return DS_OK if all operations were successful.
Status sll_get(SinglyLinkedList list, void **result, integer_t position)
{
    *result = NULL;

    if (list == NULL)
        return DS_ERR_NULL_POINTER;

    if (sll_empty(list))
        return DS_ERR_INVALID_OPERATION;

    if (position >= list->length)
        return DS_ERR_OUT_OF_RANGE;

    if (position < 0)
        return DS_ERR_NEGATIVE_VALUE;

    SinglyLinkedNode node = NULL;

    Status st = sll_get_node_at(list, &node, position);

    if (st != DS_OK)
        return st;

    *result = node->data;

    return DS_OK;
}

/// \brief Inserts an element at the head of the list.
///
/// Inserts a new element at the beginning of the list. If the list is empty
/// and this is the first element being added, c\ tail will also be pointing
/// to it; in any case the \c head pointer will be pointing to this newly
/// inserted element.
///
/// \param[in] list The list where the element is to be inserted.
/// \param[in] element The element to be inserted in the list.
///
/// \return DS_ERR_ALLOC if node allocation failed.
/// \return DS_ERR_FULL if \c limit is set (different than 0) and the list
/// length reached the specified limit.
/// \return DS_ERR_NULL_POINTER if the list references to \c NULL.
/// \return DS_OK if all operations were successful.
Status sll_insert_head(SinglyLinkedList list, void *element)
{
    if (list == NULL)
        return DS_ERR_NULL_POINTER;

    if (sll_full(list))
        return DS_ERR_FULL;

    SinglyLinkedNode node;

    Status st = sll_make_node(&node, element);

    if (st != DS_OK)
        return st;

    if (!node)
        return DS_ERR_ALLOC;

    if (sll_empty(list))
    {
        list->head = node;
        list->tail = node;
    }
    else
    {
        node->next = list->head;
        list->head = node;
    }

    list->length++;
    list->version_id++;

    return DS_OK;
}

/// \brief Inserts an element at a given position.
///
/// Inserts a new element at the middle of the list. If the chosen position
/// equals 0 \c sll_insert_head() is called; if the position equals the list
/// length \c sll_insert_tail() is called.
///
/// \param[in] list The list where the element is to be inserted.
/// \param[in] element The element to be inserted in the list.
/// \param[in] position Where the new element is to be inserted.
///
/// \return DS_ERR_ALLOC if node allocation failed.
/// \return DS_ERR_NEGATIVE_VALUE if \c position parameter is a negative value.
/// \return DS_ERR_FULL if \c limit is set (different than 0) and the list
/// length reached the specified limit.
/// \return DS_ERR_OUT_OF_RANGE if \c position parameter is greater than the
/// list \c length.
/// \return DS_ERR_NULL_POINTER if the list references to \c NULL.
/// \return DS_OK if all operations were successful.
Status sll_insert_at(SinglyLinkedList list, void *element, integer_t position)
{
    if (list == NULL)
        return DS_ERR_NULL_POINTER;

    if (sll_full(list))
        return DS_ERR_FULL;

    if (position > list->length)
        return DS_ERR_OUT_OF_RANGE;

    if (position < 0)
        return DS_ERR_NEGATIVE_VALUE;

    Status st;

    if (position == 0)
    {
        st = sll_insert_head(list, element);

        if (st != DS_OK)
            return st;

        return DS_OK;
    }
    else if (position == list->length)
    {
        st = sll_insert_tail(list, element);

        if (st != DS_OK)
            return st;

        return DS_OK;
    }
    else
    {
        SinglyLinkedNode prev = NULL;

        st = sll_get_node_at(list, &prev, position - 1);

        if (st != DS_OK)
            return st;

        SinglyLinkedNode node = NULL;

        st = sll_make_node(&node, element);

        if (st != DS_OK)
            return st;

        if (!node)
            return DS_ERR_ALLOC;

        node->next = prev->next;
        prev->next = node;

        list->length++;
        list->version_id++;

        return DS_OK;
    }
}

/// \brief Inserts an element at the tail of the list.
///
/// Inserts a new element at the end of the list. If the list is empty and this
/// is the first element being added, c\ head will also be pointing to it; in
/// any case the \c tail pointer will be pointing to this newly inserted
/// element.
///
/// \param[in] list The list where the element is to be inserted.
/// \param[in] element The element to be inserted in the list.
///
/// \return DS_ERR_ALLOC if node allocation failed.
/// \return DS_ERR_FULL if \c limit is set (different than 0) and the list
/// length reached the specified limit.
/// \return DS_ERR_NULL_POINTER if the list references to \c NULL.
/// \return DS_OK if all operations were successful.
Status sll_insert_tail(SinglyLinkedList list, void *element)
{
    if (list == NULL)
        return DS_ERR_NULL_POINTER;

    if (sll_full(list))
        return DS_ERR_FULL;

    SinglyLinkedNode node;

    Status st = sll_make_node(&node, element);

    if (st != DS_OK)
        return st;

    if (!node)
        return DS_ERR_ALLOC;

    if (sll_empty(list))
    {
        list->head = node;
        list->tail = node;
    }
    else
    {
        list->tail->next = node;
        list->tail = node;
    }

    list->length++;
    list->version_id++;

    return DS_OK;
}

/// \brief Removes an element at the head of the list.
///
/// Removes and retrieves the first element in the list located at the \c head
/// pointer.
///
/// \param[in] list The list where the element is to be removed from.
/// \param[out] result The resulting element.
///
/// \return DS_ERR_INVALID_OPERATION if the list is empty.
/// \return DS_ERR_NULL_POINTER if the list references to \c NULL.
/// \return DS_OK if all operations were successful.
Status sll_remove_head(SinglyLinkedList list, void **result)
{
    if (list == NULL)
        return DS_ERR_NULL_POINTER;

    if (sll_empty(list))
        return DS_ERR_INVALID_OPERATION;

    SinglyLinkedNode node = list->head;

    *result = node->data;

    list->head = list->head->next;

    sll_free_node_shallow(&node);

    list->length--;
    list->version_id++;

    if (sll_empty(list))
    {
        list->head = NULL;
        list->tail = NULL;
    }

    return DS_OK;
}

/// \brief Removes an element at a given position.
///
/// Removes an element at the middle of the list. If the chosen position equals
/// 0 \c sll_remove_head() is called; if the position equals the list length
/// minus 1 \c sll_remove_tail() is called.
///
/// \param[in] list The list where the element is to be removed from.
/// \param[out] result The resulting element.
/// \param[in] position Where the element is to be removed from.
///
/// \return DS_ERR_INVALID_OPERATION if the list is empty.
/// \return DS_ERR_NEGATIVE_VALUE if \c position parameter is a negative value.
/// \return DS_ERR_OUT_OF_RANGE if \c position parameter is greater than or
/// equal to the list \c length.
/// \return DS_ERR_NULL_POINTER if the list references to \c NULL.
/// \return DS_OK if all operations were successful.
Status sll_remove_at(SinglyLinkedList list, void **result, integer_t position)
{
    if (list == NULL)
        return DS_ERR_NULL_POINTER;

    if (sll_empty(list))
        return DS_ERR_INVALID_OPERATION;

    if (position >= list->length)
        return DS_ERR_OUT_OF_RANGE;

    if (position < 0)
        return DS_ERR_NEGATIVE_VALUE;

    Status st;

    if (position == 0)
    {
        st = sll_remove_head(list, result);

        if (st != DS_OK)
            return st;

        return DS_OK;
    }
    else if (position == list->length - 1)
    {
        st = sll_remove_tail(list, result);

        if (st != DS_OK)
            return st;

        return DS_OK;
    }
    else
    {
        SinglyLinkedNode prev = NULL;
        SinglyLinkedNode node = NULL;

        st = sll_get_node_at(list, &prev, position - 1);

        if (st != DS_OK)
            return st;

        node = prev->next;

        prev->next = node->next;

        *result = node->data;

        sll_free_node_shallow(&node);

        list->length--;
        list->version_id++;

        if (sll_empty(list))
        {
            list->head = NULL;
            list->tail = NULL;
        }

        return DS_OK;
    }
}

/// \brief Removes an element at the tail of the list.
///
/// Removes and retrieves the last element in the list located at the \c tail
/// pointer.
///
/// \param[in] list The list where the element is to be removed from.
/// \param[out] result The resulting element.
///
/// \return DS_ERR_INVALID_OPERATION if the list is empty.
/// \return DS_ERR_NULL_POINTER if the list references to \c NULL.
/// \return DS_OK if all operations were successful.
Status sll_remove_tail(SinglyLinkedList list, void **result)
{
    if (list == NULL)
        return DS_ERR_NULL_POINTER;

    if (sll_empty(list))
        return DS_ERR_INVALID_OPERATION;

    SinglyLinkedNode prev = NULL;
    SinglyLinkedNode curr = list->head;

    while (curr->next != NULL)
    {
        prev = curr;
        curr = curr->next;
    }

    *result = curr->data;

    if (prev == NULL)
    {
        list->head = NULL;
        list->tail = NULL;
    }
    else
    {
        prev->next = NULL;
        list->tail = prev;
    }

    sll_free_node_shallow(&curr);

    list->length--;
    list->version_id++;

    if (sll_empty(list))
    {
        list->head = NULL;
        list->tail = NULL;
    }

    return DS_OK;
}

/// \brief Checks if the specified SinglyLinkedList_s is full.
///
/// Returns true if the list is full or false otherwise. The list can only be
/// full if its limit is set to a value higher than 0 and respecting all rules
/// from sll_set_limit().
///
/// \warning This function does not checks for \c NULL references and is prone
/// to provoke run-time errors if not used correctly.
///
/// \param[in] list SinglyLinkedList_s reference.
///
/// \return True if the list is full.
/// \return False if the list is not full.
bool sll_full(SinglyLinkedList list)
{
    return list->limit > 0 && list->length >= list->limit;
}

/// \brief Checks if specified SinglyLinkedList_s list is empty.
///
/// Returns true if the list is empty or false otherwise. The list is empty
/// when its length is 0. If every function works properly there is no need to
/// check if the head or tail pointers are \c NULL or not. The length variable
/// already tracks it.
///
/// \warning This function does not checks for \c NULL references and is prone
/// to provoke run-time errors if not used correctly.
///
/// \param[in] list SinglyLinkedList_s reference.
///
/// \return True if the list is empty.
/// \return False if the list is not empty.
bool sll_empty(SinglyLinkedList list)
{
    return list->length == 0;
}

/// \brief Returns the highest element from the specified SinglyLinkedList_s.
///
/// Returns a reference to the highest element in the list or \c NULL if the
/// list is empty.
///
/// \param[in] list SinglyLinkedList_s reference.
///
/// \return NULL if the list references to \c NULL, if the list is empty or if
/// the compare function is not set.
/// \return The highest element in the list.
void *sll_max(SinglyLinkedList list)
{
    if (list == NULL)
        return NULL;

    if (sll_empty(list))
        return NULL;

    if (list->v_compare == NULL)
        return NULL;

    SinglyLinkedNode scan = list->head;

    void *result = scan->data;

    while (scan != NULL)
    {
        if (list->v_compare(scan->data, result) > 0)
            result = scan->data;

        scan = scan->next;
    }

    return result;
}

/// \brief Returns the smallest element from the specified SinglyLinkedList_s.
///
/// Returns a reference to the smallest element in the list or \c NULL if the
/// list is empty.
///
/// \param[in] list SinglyLinkedList_s reference.
///
/// \return NULL if the list references to \c NULL, if the list is empty or if
/// the compare function is not set.
/// \return The smallest element in the list.
void *sll_min(SinglyLinkedList list)
{
    if (list == NULL)
        return NULL;

    if (sll_empty(list))
        return NULL;

    if (list->v_compare == NULL)
        return NULL;

    SinglyLinkedNode scan = list->head;

    void *result = scan->data;

    while (scan != NULL)
    {
        if (list->v_compare(scan->data, result) < 0)
            result = scan->data;

        scan = scan->next;
    }

    return result;
}

/// \brief Returns the index of the first element that matches a key.
///
/// Returns the index of the first element that matches a given key or -1 if it
/// could not be found. It will return -2 if the list references \c NULL and -3
/// if a default compare function is not set.
///
/// \param[in] list SinglyLinkedList_s reference.
/// \param[in] key Key to be searched in the list.
///
/// \return -3 if there is no default compare function.
/// \return -2 if the list references to \c NULL.
/// \return -1 if the element was not found.
/// \return The index of the matched element.
integer_t sll_index_first(SinglyLinkedList list, void *key)
{
    if (list == NULL)
        return -2;

    if (list->v_compare == NULL)
        return -3;

    SinglyLinkedNode scan = list->head;

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
/// Returns the index of the last element that matches a given key or -1 if it
/// could not be found. It will return -2 if the list references \c NULL and -3
/// if a default compare function is not set.
///
/// \param[in] list SinglyLinkedList_s reference.
/// \param[in] key Key to be searched in the list.
///
/// \return -3 if there is no default compare function.
/// \return -2 if the list references to \c NULL.
/// \return -1 if the element was not found.
/// \return The index of the matched element.
integer_t sll_index_last(SinglyLinkedList list, void *key)
{
    if (list == NULL)
        return -2;

    if (list->v_compare == NULL)
        return -3;

    SinglyLinkedNode scan = list->head;

    integer_t result = -1, index = 0;

    while (scan != NULL)
    {
        if (list->v_compare(scan->data, key) == 0)
            result = index;

        index++;

        scan = scan->next;
    }

    return result;
}

/// \brief Checks if a given element is present in the specified list.
///
/// Returns true if the element is present in the list, otherwise false.
///
/// \warning This function does not checks for \c NULL references for either
/// the list parameter or if the default compare function is set.
///
/// \param[in] list SinglyLinkedList_s reference.
/// \param[in] key Key to be matched.
///
/// \return True if the element is present in the list.
/// \return False if the element is not present in the list.
bool sll_contains(SinglyLinkedList list, void *key)
{
    SinglyLinkedNode scan = list->head;

    while (scan != NULL)
    {
        if (list->v_compare(scan->data, key) == 0)
            return true;

        scan = scan->next;
    }

    return false;
}

/// \brief Reverses a SinglyLinkedList_s.
///
/// Reverses the chain of elements from the list. This function only changes
/// the \c next pointers from each element and the \c head and \c tail pointers
/// of the list struct.
///
/// \param[in] list SinglyLinkedList_s reference to be reversed.
///
/// \return DS_ERR_NULL_POINTER if list references to \c NULL.
/// \return DS_OK if all operations are successful.
Status sll_reverse(SinglyLinkedList list)
{
    if (list == NULL)
        return DS_ERR_NULL_POINTER;

    // If there is only one node
    if (list->length < 2)
        return DS_OK;

    SinglyLinkedNode prev = NULL;
    SinglyLinkedNode curr = list->head;
    SinglyLinkedNode next = NULL;

    list->tail = list->head;

    while (curr != NULL)
    {
        next = curr->next;

        curr->next = prev;

        prev = curr;

        curr = next;
    }

    list->head = prev;

    list->version_id++;

    return DS_OK;
}

/// \brief Makes a copy of the specified SinglyLinkedList_s.
///
/// Makes an exact copy of a list, copying each element using the default copy
/// function.
///
/// \param[in] list SinglyLinkedList_s to be copied.
/// \param[out] result Resulting copy.
///
/// \return DS_ERR_INCOMPLETE_TYPE if either a default copy or a default free
/// functions are not set.
/// \return DS_ERR_NULL_POINTER if list references to \c NULL.
/// \return DS_OK if all operations are successful.
Status sll_copy(SinglyLinkedList list, SinglyLinkedList *result)
{
    *result = NULL;

    if (list == NULL)
        return DS_ERR_NULL_POINTER;

    if (list->v_copy == NULL || list->v_free == NULL)
        return DS_ERR_INCOMPLETE_TYPE;

    Status st = sll_create(result, list->v_compare, list->v_copy,
            list->v_display, list->v_free);

    if (st != DS_OK)
        return st;

    (*result)->limit = list->limit;

    SinglyLinkedNode scan = list->head;

    void *elem;

    while (scan != NULL)
    {
        elem = list->v_copy(scan->data);

        st = sll_insert_tail(*result, elem);

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
/// \param[in] list SinglyLinkedList_s reference.
/// \param[out] result Resulting array.
/// \param[out] length Resulting array's length.
///
/// \return DS_ERR_INCOMPLETE_TYPE if a default copy function is not set.
/// \return DS_ERR_NULL_POINTER if list references to \c NULL.
/// \return DS_OK if all operations are successful.
Status sll_to_array(SinglyLinkedList list, void ***result, integer_t *length)
{
    // If anything goes wrong...
    *result = NULL;
    *length = -1;

    if (list == NULL)
        return DS_ERR_NULL_POINTER;

    if (sll_empty(list))
        return DS_ERR_INVALID_OPERATION;

    if (list->v_copy == NULL)
        return DS_ERR_INCOMPLETE_TYPE;

    *length = list->length;

    *result = malloc(sizeof(void*) * (*length));

    if (!(*result))
        return DS_ERR_ALLOC;

    SinglyLinkedNode scan = list->head;

    for (integer_t i = 0; i < *length; i++)
    {
        (*result)[i] = list->v_copy(scan->data);

        scan = scan->next;
    }

    return DS_OK;
}

/// \brief Appends list2 at the end of list1.
///
/// \param list1
/// \param list2
///
/// \return
///
/// \todo revise sll_link
Status sll_link(SinglyLinkedList list1, SinglyLinkedList list2)
{
    if (list1 == NULL || list2 == NULL)
        return DS_ERR_NULL_POINTER;

    if (sll_empty(list2))
        return DS_ERR_INVALID_OPERATION;

    if (sll_empty(list1))
    {
        list1->head = list2->head;
        list1->tail = list2->tail;

        list1->length = list2->length;
    }
    else
    {
        list1->tail->next = list2->head;
        list1->tail = list2->tail;

        list2->length += list2->length;
    }

    list2->head = NULL;
    list2->tail = NULL;

    list2->length = 0;

    return DS_OK;
}

/// \brief Links list2 at the specified position of list1.
///
/// \param list1
/// \param list2
/// \param position
///
/// \return
///
/// \todo revise sll_link_at
Status sll_link_at(SinglyLinkedList list1, SinglyLinkedList list2,
        integer_t position)
{
    if (list1 == NULL || list2 == NULL)
        return DS_ERR_NULL_POINTER;

    if (position > list1->length)
        return DS_ERR_OUT_OF_RANGE;

    if (sll_empty(list1) || sll_empty(list2))
        return DS_ERR_INVALID_OPERATION;

    if (position == 0)
    {
        list2->tail->next = list1->head;

        list1->head = list2->head;
    }
    else if (position == list1->length)
    {
        list1->tail->next = list2->head;
        list1->tail = list2->tail;
    }
    else
    {
        SinglyLinkedNode prev;

        sll_get_node_at(list1, &prev, position - 1);

        list2->tail->next = prev->next;

        prev->next = list2->head;
    }

    list2->head = NULL;
    list2->tail = NULL;

    list1->length += list2->length;
    list2->length = 0;

    return DS_OK;
}

/// \brief Unlinks elements from the specified position to the end.
///
/// \param list
/// \param result
/// \param position
///
/// \return
///
/// \todo revise sll_unlink
Status sll_unlink(SinglyLinkedList list, SinglyLinkedList result,
        integer_t position)
{
    if (list == NULL)
        return DS_ERR_NULL_POINTER;

    if (!sll_empty(result))
        return DS_ERR_INVALID_OPERATION;

    if (position >= list->length)
        return DS_ERR_OUT_OF_RANGE;

    integer_t len = sll_length(list);

    if (position == 0)
    {
        result->head = list->head;
        result->tail = list->tail;

        result->length = list->length;

        result->tail = NULL;
        result->head = NULL;
    }
    else
    {
        SinglyLinkedNode prev;

        Status st = sll_get_node_at(list, &prev, position - 1);

        if (st != DS_OK)
            return st;

        result->head = prev->next;
        result->tail = list->tail;

        list->tail = prev;

        prev->next = NULL;
    }

    list->length = position;

    result->length = len - position;

    return DS_OK;
}

/// \brief Unlinks elements from a given position to another.
///
/// \param list
/// \param result
/// \param position1
/// \param position2
///
/// \return
Status sll_unlink_at(SinglyLinkedList list, SinglyLinkedList result,
        integer_t position1, integer_t position2)
{
    /// \todo sll_unlink_at
    return DS_OK;
}

/// \brief Displays a SinglyLinkedList_s in the console.
///
/// Displays a SinglyLinkedList_s in the console with its elements separated by
/// arrows indicating a singly-linked list.
///
/// \param[in] list The SinglyLinkedList_s to be displayed in the console.
///
/// \return DS_ERR_INCOMPLETE_TYPE if a default display function is not set.
/// \return DS_ERR_NULL_POINTER if the list references to \c NULL.
/// \return DS_OK if all operations were successful.
Status sll_display(SinglyLinkedList list)
{
    if (list == NULL)
        return DS_ERR_NULL_POINTER;

    if (list->v_display == NULL)
        return DS_ERR_INCOMPLETE_TYPE;

    if (sll_empty(list))
    {
        printf("\nSingly Linked List\n[ empty ]\n");

        return DS_OK;
    }

    SinglyLinkedNode scan = list->head;

    printf("\nSingly Linked List\n");

    while (scan != NULL)
    {
        list->v_display(scan->data);

        printf(" -> ");

        scan = scan->next;
    }

    printf("NULL\n");

    return DS_OK;
}

/// \brief Displays a SinglyLinkedList_s in the console.
///
/// Displays a SinglyLinkedList_s in the console like an array with its
/// elements separated by commas, delimited with brackets.
///
/// \param[in] list The SinglyLinkedList_s to be displayed in the console.
///
/// \return DS_ERR_INCOMPLETE_TYPE if a default display function is not set.
/// \return DS_ERR_NULL_POINTER if the list references to \c NULL.
/// \return DS_OK if all operations were successful.
Status sll_display_array(SinglyLinkedList list)
{
    if (list == NULL)
        return DS_ERR_NULL_POINTER;

    if (list->v_display == NULL)
        return DS_ERR_INCOMPLETE_TYPE;

    if (sll_empty(list))
    {
        printf("\n[ empty ]\n");

        return DS_OK;
    }

    SinglyLinkedNode scan = list->head;

    printf("\n[ ");

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

/// \brief Displays a SinglyLinkedList_s in the console.
///
/// Displays a SinglyLinkedList_s in the console with its elements separated by
/// spaces.
///
/// \param[in] list The SinglyLinkedList_s to be displayed in the console.
///
/// \return DS_ERR_INCOMPLETE_TYPE if a default display function is not set.
/// \return DS_ERR_NULL_POINTER if the list references to \c NULL.
/// \return DS_OK if all operations were successful.
Status sll_display_raw(SinglyLinkedList list)
{
    if (list == NULL)
        return DS_ERR_NULL_POINTER;

    if (list->v_display == NULL)
        return DS_ERR_INCOMPLETE_TYPE;

    printf("\n");

    if (sll_empty(list))
        return DS_OK;

    SinglyLinkedNode scan = list->head;

    while (scan != NULL)
    {
        list->v_display(scan->data);

        printf(" ");

        scan = scan->next;
    }

    printf("\n");

    return DS_OK;
}

///////////////////////////////////////////////////// NOT EXPOSED FUNCTIONS ///

/// \brief Builds a new SinglyLinkedNode_s.
///
/// Implementation detail. Function responsible for allocating a new
/// SinglyLinkedNode_s.
///
/// \param[in,out] node SinglyLinkedNode_s to be allocated.
/// \param[in] element Node's data member.
///
/// \return DS_ERR_ALLOC if node allocation failed.
/// \return DS_OK if all operations are successful.
static Status sll_make_node(SinglyLinkedNode *node, void *element)
{
    (*node) = malloc(sizeof(SinglyLinkedNode_t));

    if (!(*node))
        return DS_ERR_ALLOC;

    (*node)->next = NULL;
    (*node)->data = element;

    return DS_OK;
}

/// \brief Frees a SinglyLinkedNode_s and its data.
///
/// Implementation detail. Frees a SinglyLinkedNode_s and its data using the
/// list's default free function.
///
/// \param[in,out] node SinglyLinkedNode_s to be freed from memory.
/// \param[in] free_f List's default free function.
///
/// \return DS_ERR_NULL_POINTER if node references to \c NULL.
/// \return DS_OK if all operations are successful.
static Status sll_free_node(SinglyLinkedNode *node, sll_free_f free_f)
{
    if (*node == NULL)
        return DS_ERR_NULL_POINTER;

    free_f((*node)->data);

    free(*node);

    (*node) = NULL;

    return DS_OK;
}

/// \brief Frees a SinglyLinkedNode_s.
///
/// Implementation detail. Frees a SinglyLinkedNode_s and leaves its data
/// untouched.
///
/// \param[in,out] node SinglyLinkedNode_s to be freed from memory.
///
/// \return DS_ERR_NULL_POINTER if node references to \c NULL.
/// \return DS_OK if all operations are successful.
static Status sll_free_node_shallow(SinglyLinkedNode *node)
{
    if (*node == NULL)
        return DS_ERR_NULL_POINTER;

    free(*node);

    (*node) = NULL;

    return DS_OK;
}

/// \brief Gets a node from a specific position.
///
/// Implementation detail. Searches for a node in O(n) where the search starts
/// at the head of the list.
///
/// \param[in] list SinglyLinkedList_s to search for the node.
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
static Status sll_get_node_at(SinglyLinkedList list, SinglyLinkedNode *result, integer_t position)
{
    *result = NULL;

    if (list == NULL)
        return DS_ERR_NULL_POINTER;

    if (sll_empty(list))
        return DS_ERR_INVALID_OPERATION;

    if (position < 0)
        return DS_ERR_NEGATIVE_VALUE;

    if (position >= list->length)
        return DS_ERR_OUT_OF_RANGE;

    (*result) = list->head;

    for (integer_t i = 0; i < position; i++)
    {
        if ((*result) == NULL)
            return DS_ERR_ITER;

        (*result) = (*result)->next;
    }

    return DS_OK;
}

////////////////////////////////////////////// END OF NOT EXPOSED FUNCTIONS ///

///////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////// Iterator ///
///////////////////////////////////////////////////////////////////////////////

/// \todo SinglyLinkedListIterator

///////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////// Wrapper ///
///////////////////////////////////////////////////////////////////////////////

/// \todo SinglyLinkedListWrapper