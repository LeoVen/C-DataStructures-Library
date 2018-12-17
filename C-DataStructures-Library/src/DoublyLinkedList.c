/**
 * @file DoublyLinkedList.c
 * 
 * @author Leonardo Vencovsky (https://github.com/LeoVen)
 *
 * @date 25/09/2018
 */

#include "DoublyLinkedList.h"

/// \brief A generic doubly-linked list.
///
/// A DoublyLinkedList is a linear structure like a SinglyLinkedList_s where
/// each node have two pointers. One to the previous node and another to the
/// next node. This allows a quicker access to elements; insertion and removal
/// are also improved being at most O(n / 2) since we can be looking for a node
/// either at the start or the end of the list.
///
/// \todo Add more description on how the DoublyLinkedList_s works.
///
/// \b Advantages over \c SinglyLinkedList_s
/// - Inserting, removing and accessing nodes takes half the time on average
///
/// \b Drawbacks
/// - Extra memory for yet another pointer on each element
///
/// \b Functions
///
/// Located in the file DoublyLinkedList.c
///
/// \todo Add Link and Unlink functions
/// \todo Add support to negative index searches
struct DoublyLinkedList_s
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
    struct DoublyLinkedNode_s *head;

    /// \brief Points to the last Node on the list.
    ///
    /// Points to the first Node on the list or \c NULL if the list is empty.
    struct DoublyLinkedNode_s *tail;

    /// \brief Comparator function.
    ///
    /// A function that compares one element with another that returns an int
    /// with the following rules:
    ///
    /// - <code>[ > 0 ]</code> if first element is greater than the second;
    /// - <code>[ < 0 ]</code> if second element is greater than the first;
    /// - <code>[ 0 ]</code> if elements are equal.
    dll_compare_f v_compare;

    /// \brief Copy function.
    ///
    /// A function that returns an exact copy of an element.
    dll_copy_f v_copy;

    /// \brief Display function.
    ///
    /// A function that displays an element in the console. Useful for
    /// debugging.
    dll_display_f v_display;

    /// \brief Deallocator function.
    ///
    /// A function that completely frees an element from memory.
    dll_free_f v_free;

    /// \brief A version id to keep track of modifications.
    ///
    /// This version id is used by the iterator to check if the structure was
    /// modified. The iterator can only function if its version_id is the same
    /// as the structure's version id, that is, there have been no structural
    /// modifications (except for those done by the iterator itself).
    integer_t version_id;
};

/// \brief A DoublyLinkedList_s node.
///
/// Implementation detail. This is a doubly-linked node with a pointer to the
/// previous node (or \c NULL if it is the head node) and another pointer to
/// the next node (or \c NULL if it is the tail node).
struct DoublyLinkedNode_s
{
    /// \brief Data pointer.
    ///
    /// Points to node's data. The data needs to be dynamically allocated.
    void *data;

    /// \brief Next node on the list.
    ///
    /// Next node on the list or \c NULL if this is the tail node.
    struct DoublyLinkedNode_s *next;

    /// \brief Previous node on the list.
    ///
    /// Previous node on the list or \c NULL if this is the head node.
    struct DoublyLinkedNode_s *prev;
};

/// \brief A type for a doubly-linked list node.
///
/// Defines a type to a <code> struct DoublyLinkedNode_s </code>.
typedef struct DoublyLinkedNode_s DoublyLinkedNode_t;

/// \brief A pointer type for a doubly-linked list node.
///
/// Defines a pointer type to a <code> struct DoublyLinkedNode_s </code>.
typedef struct DoublyLinkedNode_s *DoublyLinkedNode;

///////////////////////////////////////////////////// NOT EXPOSED FUNCTIONS ///

static Status dll_make_node(DoublyLinkedNode *node, void *element);

static Status dll_free_node(DoublyLinkedNode *node, dll_free_f free_f);

static Status dll_free_node_shallow(DoublyLinkedNode *node);

static Status dll_get_node_at(DoublyLinkedList list, DoublyLinkedNode *result, integer_t position);

////////////////////////////////////////////// END OF NOT EXPOSED FUNCTIONS ///

/// \brief Initializes a DoublyLinkedList_s structure.
///
/// Initializes a new DoublyLinkedList_s structure with initial length and
/// limit as 0. Note that it does not sets any default functions. If you don't
/// set them later you won't be able to do certain operations that depend on a
/// user-defined function.
///
/// \param[in,out] list The list to be initialized.
///
/// \return DS_ERR_ALLOC if list allocation failed.
/// \return DS_OK if all operations were successful.
Status dll_init(DoublyLinkedList *list)
{
    (*list) = malloc(sizeof(DoublyLinkedList_t));

    if (!(*list))
        return DS_ERR_ALLOC;

    (*list)->length = 0;
    (*list)->limit = 0;

    (*list)->head = NULL;
    (*list)->tail = NULL;

    (*list)->v_compare = NULL;
    (*list)->v_copy = NULL;
    (*list)->v_display = NULL;
    (*list)->v_free = NULL;

    return DS_OK;
}

/// \brief Creates a DoublyLinkedList_s.
///
/// This function completely creates a DoublyLinkedList_s, setting all of its
/// default functions.
///
/// \param[in,out] list DoublyLinkedList_s to be allocated.
/// \param[in] compare_f A function that compares two elements.
/// \param[in] copy_f A function that makes an exact copy of an element.
/// \param[in] display_f A function that displays in the console an element.
/// \param[in] free_f A function that completely frees from memory an element.
///
/// \return DS_ERR_ALLOC if list allocation failed.
/// \return DS_OK if all operations are successful.
Status dll_create(DoublyLinkedList *list, dll_compare_f compare_f,
        dll_copy_f copy_f, dll_display_f display_f, dll_free_f free_f)
{
    *list = malloc(sizeof(DoublyLinkedList_t));

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

/// \brief Frees from memory a DoublyLinkedList_s and all its elements.
///
/// This function frees from memory all the list's elements using its default
/// free function and then frees the list's structure. The variable is then set
/// to \c NULL.
///
/// \param[in,out] list The DoublyLinkedList_s to be freed from memory.
///
/// \return DS_ERR_INCOMPLETE_TYPE if a default free function is not set.
/// \return DS_ERR_NULL_POINTER if the list references to \c NULL.
/// \return DS_OK if all operations were successful.
Status dll_free(DoublyLinkedList *list)
{
    if (*list == NULL)
        return DS_ERR_NULL_POINTER;

    if ((*list)->v_free == NULL)
        return DS_ERR_INCOMPLETE_TYPE;

    DoublyLinkedNode prev = (*list)->head;

    Status st;

    while ((*list)->head != NULL)
    {
        (*list)->head = (*list)->head->next;

        st = dll_free_node(&prev, (*list)->v_free);

        if (st != DS_OK)
            return st;

        prev = (*list)->head;
    }

    free(*list);

    (*list) = NULL;

    return DS_OK;
}

/// \brief Frees from memory a DoublyLinkedList_s.
///
/// This function frees from memory all the list's nodes without freeing its
/// data and then frees the list's structure. The variable is then set to
/// \c NULL.
///
/// \param[in,out] list DoublyLinkedList_s to be freed from memory.
///
/// \return DS_ERR_NULL_POINTER if the list references to \c NULL.
/// \return DS_OK if all operations are successful.
Status dll_free_shallow(DoublyLinkedList *list)
{
    if ((*list) == NULL)
        return DS_ERR_NULL_POINTER;

    DoublyLinkedNode prev = (*list)->head;

    Status st;

    while ((*list)->head != NULL)
    {
        (*list)->head = (*list)->head->next;

        st = dll_free_node_shallow(&prev);

        if (st != DS_OK)
            return st;

        prev = (*list)->head;
    }

    free(*list);

    (*list) = NULL;

    return DS_OK;
}

/// \brief Erases a DoublyLinkedList_s.
///
/// This function is equivalent to freeing a list and then creating it again.
/// This will reset the list to its initial state with no elements, but will
/// keep all of its default functions.
///
/// \param[in,out] list DoublyLinkedList_s to be erased.
///
/// \return DS_ERR_ALLOC if list allocation failed.
/// \return DS_ERR_INCOMPLETE_TYPE if a default free function is not set.
/// \return DS_ERR_NULL_POINTER if the list references to \c NULL.
/// \return DS_OK if all operations are successful.
Status dll_erase(DoublyLinkedList *list)
{
    if (*list == NULL)
        return DS_ERR_NULL_POINTER;

    DoublyLinkedList new_list;

    Status st = dll_create(&new_list, (*list)->v_compare, (*list)->v_copy,
                           (*list)->v_display, (*list)->v_free);

    if (st !=  DS_OK)
        return st;

    st = dll_free(list);

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
/// with the dll_compare_f specifications.
///
/// \param[in] list DoublyLinkedList_s to set the default compare function.
/// \param[in] function An dll_compare_f kind of function.
///
/// \return DS_ERR_NULL_POINTER if the list references to \c NULL.
/// \return DS_OK if all operations are successful.
Status dll_set_v_compare(DoublyLinkedList list, dll_compare_f function)
{
    if (list == NULL)
        return DS_ERR_NULL_POINTER;

    list->v_compare = function;

    return DS_OK;
}

/// \brief Sets the default copy function.
///
/// Use this function to set a default compare function. It needs to comply
/// with the dll_copy_f specifications.
///
/// \param[in] list DoublyLinkedList_s to set the default copy function.
/// \param[in] function An dll_copy_f kind of function.
///
/// \return DS_ERR_NULL_POINTER if the list references to \c NULL.
/// \return DS_OK if all operations are successful.
Status dll_set_v_copy(DoublyLinkedList list, dll_copy_f function)
{
    if (list == NULL)
        return DS_ERR_NULL_POINTER;

    list->v_copy = function;

    return DS_OK;
}

/// \brief Sets the default display function.
///
/// Use this function to set a default display function. It needs to comply
/// with the dll_display_f specifications. Useful for debugging.
///
/// \param[in] list DoublyLinkedList_s to set the default display function.
/// \param[in] function An dll_display_f kind of function.
///
/// \return DS_ERR_NULL_POINTER if the list references to \c NULL.
/// \return DS_OK if all operations are successful.
Status dll_set_v_display(DoublyLinkedList list, dll_display_f function)
{
    if (list == NULL)
        return DS_ERR_NULL_POINTER;

    list->v_display = function;

    return DS_OK;
}

/// \brief Sets the default free function.
///
/// Use this function to set a default free function. It needs to comply
/// with the dll_free_f specifications.
///
/// \param[in] list DoublyLinkedList_s to set the default free function.
/// \param[in] function An dll_free_f kind of function.
///
/// \return DS_ERR_NULL_POINTER if the list references to \c NULL.
/// \return DS_OK if all operations are successful.
Status dll_set_v_free(DoublyLinkedList list, dll_free_f function)
{
    if (list == NULL)
        return DS_ERR_NULL_POINTER;

    list->v_free = function;

    return DS_OK;
}

/// \brief Sets a limit to the specified DoublyLinkedList_s's length.
///
/// Limit's the DoublyLinkedList_s's length. You can only set a limit greater
/// or equal to the list's current length and greater than 0. To remove this
/// limitation simply set the limit to 0 or less.
///
/// \param[in] list DoublyLinkedList_s reference.
/// \param[in] limit Maximum list length.
///
/// \return DS_ERR_INVALID_OPERATION if the limitation is less than the list's
/// current length.
/// \return DS_ERR_NULL_POINTER if the list references to \c NULL.
/// \return DS_OK if all operations are successful.
Status dll_set_limit(DoublyLinkedList list, integer_t limit)
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
/// must have a set dll_free_f in order to erase the old element and set the
/// new one.
///
/// \param[in] list The DoublyLinkedList_s where the element is to be updated.
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
Status dll_set(DoublyLinkedList list, void *element, integer_t position)
{
    if (list == NULL)
        return DS_ERR_NULL_POINTER;

    if (dll_empty(list))
        return DS_ERR_INVALID_OPERATION;

    if (position >= list->length)
        return DS_ERR_OUT_OF_RANGE;

    if (position < 0)
        return DS_ERR_NEGATIVE_VALUE;

    if (list->v_free == NULL)
        return DS_ERR_INCOMPLETE_TYPE;

    DoublyLinkedNode node = NULL;

    Status st = dll_get_node_at(list, &node, position);

    if (st != DS_OK)
        return st;

    list->v_free(node->data);

    node->data = element;

    return DS_OK;
}

/// \brief Returns the DoublyLinkedList_s's length.
///
/// Returns the list's current length or -1 if the list references to \c NULL.
///
/// \param[in] list DoublyLinkedList_s reference.
///
/// \return -1 if the list references to \c NULL.
/// \return The list's length.
integer_t dll_length(DoublyLinkedList list)
{
    if (list == NULL)
        return -1;

    return list->length;
}

/// \brief Returns the DoublyLinkedList_s's limit.
///
/// Returns the list limit or -1 if the list references to \c NULL.
///
/// \param[in] list DoublyLinkedList_s reference.
///
/// \return -1 if the list references to \c NULL.
/// \return The list's limit.
integer_t dll_limit(DoublyLinkedList list)
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
/// \param[in] list The DoublyLinkedList_s to retrieve the element from.
/// \param[out] result The resulting element.
/// \param[in] position The position of the element to be retrieved.
///
/// \return DS_ERR_INVALID_OPERATION if the list is empty.
/// \return DS_ERR_NEGATIVE_VALUE if \c position parameter is a negative value.
/// \return DS_ERR_OUT_OF_RANGE if \c position parameter is greater than or
/// equal to the list \c length.
/// \return DS_ERR_NULL_POINTER if the list references to \c NULL.
/// \return DS_OK if all operations were successful.
Status dll_get(DoublyLinkedList list, void **result, integer_t position)
{
    *result = NULL;

    if (list == NULL)
        return DS_ERR_NULL_POINTER;

    if (dll_empty(list))
        return DS_ERR_INVALID_OPERATION;

    if (position >= list->length)
        return DS_ERR_OUT_OF_RANGE;

    if (position < 0)
        return DS_ERR_NEGATIVE_VALUE;

    DoublyLinkedNode node = NULL;

    Status st = dll_get_node_at(list, &node, position);

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
/// \return DS_ERR_FULL if \c limit is set (greater than 0) and the list
/// length reached the specified limit.
/// \return DS_ERR_NULL_POINTER if the list reference is \c NULL.
/// \return DS_OK if all operations were successful.
Status dll_insert_head(DoublyLinkedList list, void *element)
{
    if (list == NULL)
        return DS_ERR_NULL_POINTER;

    if (dll_full(list))
        return DS_ERR_FULL;

    DoublyLinkedNode node;

    Status st = dll_make_node(&node, element);

    if (st != DS_OK)
        return st;

    if (dll_empty(list))
    {
        list->head = node;
        list->tail = node;
    }
    else
    {
        node->next = list->head;

        list->head->prev = node;

        list->head = node;
    }

    list->length++;
    list->version_id++;

    return DS_OK;
}

/// \brief Inserts an element at a given position.
///
/// Inserts a new element at the middle of the list. If the chosen position
/// equals 0 \c dll_insert_head() is called; if the position equals the list
/// length \c dll_insert_tail() is called.
///
/// \param[in] list The list where the element is to be inserted.
/// \param[in] element The element to be inserted in the list.
/// \param[in] position Where the new element is to be inserted.
///
/// \return DS_ERR_ALLOC if node allocation failed.
/// \return DS_ERR_FULL if \c limit is set (greater than 0) and the list
/// length reached the specified limit.
/// \return DS_ERR_NEGATIVE_VALUE if \c position parameter is a negative value.
/// \return DS_ERR_OUT_OF_RANGE if \c position parameter is greater than the
/// list \c length.
/// \return DS_ERR_NULL_POINTER if the list reference is \c NULL.
/// \return DS_OK if all operations were successful.
Status dll_insert_at(DoublyLinkedList list, void *element, integer_t position)
{
    if (list == NULL)
        return DS_ERR_NULL_POINTER;

    if (dll_full(list))
        return DS_ERR_FULL;

    if (position > list->length)
        return DS_ERR_OUT_OF_RANGE;

    if (position < 0)
        return DS_ERR_NEGATIVE_VALUE;

    Status st;

    if (position == 0)
    {
        st = dll_insert_head(list, element);

        if (st != DS_OK)
            return st;

        return DS_OK;
    }
    else if (position == list->length)
    {
        st = dll_insert_tail(list, element);

        if (st != DS_OK)
            return st;

        return DS_OK;
    }
    else
    {
        DoublyLinkedNode curr = NULL;

        st = dll_get_node_at(list, &curr, position);

        if (st != DS_OK)
            return st;

        DoublyLinkedNode node = NULL;

        st = dll_make_node(&node, element);

        if (st != DS_OK)
            return st;

        node->prev = curr->prev;
        node->next = curr;

        curr->prev->next = node;
        curr->prev = node;

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
/// \return DS_ERR_FULL if \c limit is set (greater than 0) and the list
/// length reached the specified limit.
/// \return DS_ERR_NULL_POINTER if the list reference is \c NULL.
/// \return DS_OK if all operations were successful.
Status dll_insert_tail(DoublyLinkedList list, void *element)
{
    if (list == NULL)
        return DS_ERR_NULL_POINTER;

    if (dll_full(list))
        return DS_ERR_FULL;

    DoublyLinkedNode node;

    Status st = dll_make_node(&node, element);

    if (st != DS_OK)
        return st;

    if (dll_empty(list))
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
/// \return DS_ERR_NULL_POINTER if the list reference is \c NULL.
/// \return DS_OK if all operations were successful.
Status dll_remove_head(DoublyLinkedList list, void **result)
{
    if (list == NULL)
        return DS_ERR_NULL_POINTER;

    if (dll_empty(list))
        return DS_ERR_INVALID_OPERATION;

    DoublyLinkedNode node = list->head;

    *result = node->data;

    list->head = list->head->next;

    if (list->head == NULL)
        list->tail = NULL;
    else
        list->head->prev = NULL;

    dll_free_node_shallow(&node);

    list->length--;
    list->version_id++;

    if (dll_empty(list))
    {
        list->head = NULL;
        list->tail = NULL;
    }

    return DS_OK;
}

/// \brief Removes an element at a given position.
///
/// Removes an element at the middle of the list. If the chosen position equals
/// 0 \c dll_remove_head() is called; if the position equals the list length -
/// 1 \c dll_remove_tail() is called.
///
/// \param[in] list The list where the element is to be removed from.
/// \param[out] result The resulting element.
/// \param[in] position Where the element is to be removed from.
///
/// \return DS_ERR_INVALID_OPERATION if the list is empty.
/// \return DS_ERR_NEGATIVE_VALUE if \c position parameter is a negative value.
/// \return DS_ERR_OUT_OF_RANGE if \c position parameter is greater than or
/// equal to the list \c length.
/// \return DS_ERR_NULL_POINTER if the list reference is \c NULL.
/// \return DS_OK if all operations were successful.
Status dll_remove_at(DoublyLinkedList list, void **result, integer_t position)
{
    if (list == NULL)
        return DS_ERR_NULL_POINTER;

    if (dll_empty(list))
        return DS_ERR_INVALID_OPERATION;

    if (position >= list->length)
        return DS_ERR_OUT_OF_RANGE;

    if (position < 0)
        return DS_ERR_NEGATIVE_VALUE;

    Status st;

    if (position == 0)
    {
        st = dll_remove_head(list, result);

        if (st != DS_OK)
            return st;

        return DS_OK;
    }
    else if (position == list->length - 1)
    {
        st = dll_remove_tail(list, result);

        if (st != DS_OK)
            return st;

        return DS_OK;
    }
    else
    {
        DoublyLinkedNode node = NULL;

        st = dll_get_node_at(list, &node, position);

        if (st != DS_OK)
            return st;

        node->prev->next = node->next;
        node->next->prev = node->prev;

        *result = node->data;

        dll_free_node_shallow(&node);

        list->length--;
        list->version_id++;

        if (dll_empty(list))
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
/// \return DS_ERR_NULL_POINTER if the list reference is \c NULL.
/// \return DS_OK if all operations were successful.
Status dll_remove_tail(DoublyLinkedList list, void **result)
{
    if (list == NULL)
        return DS_ERR_NULL_POINTER;

    if (dll_empty(list))
        return DS_ERR_INVALID_OPERATION;

    DoublyLinkedNode node = list->tail;

    *result = node->data;

    list->tail = list->tail->prev;

    if (list->tail == NULL)
        list->head = NULL;
    else
        list->tail->next = NULL;

    dll_free_node_shallow(&node);

    list->length--;
    list->version_id++;

    if (dll_empty(list))
    {
        list->head = NULL;
        list->tail = NULL;
    }

    return DS_OK;
}

/// \brief Checks if the specified DoublyLinkedList_s is full.
///
/// Returns true if the list is full or false otherwise. The list can only be
/// full if its limit is set to a value higher than 0 and respecting all rules
/// from dll_set_limit().
///
/// \warning This function does not checks for \c NULL references and is prone
/// to provoke run-time errors if not used correctly.
///
/// \param[in] list DoublyLinkedList_s reference.
///
/// \return True if the list is full.
/// \return False if the list is not full.
bool dll_full(DoublyLinkedList list)
{
    return list->limit > 0 && list->length >= list->limit;
}

/// \brief Checks if specified DoublyLinkedList_s list is empty.
///
/// Returns true if the list is empty or false otherwise. The list is empty
/// when its length is 0. If every function works properly there is no need to
/// check if the head or tail pointers are \c NULL or not. The length variable
/// already tracks it.
///
/// \warning This function does not checks for \c NULL references and is prone
/// to provoke run-time errors if not used correctly.
///
/// \param[in] list DoublyLinkedList_s reference.
///
/// \return True if the list is empty.
/// \return False if the list is not empty.
bool dll_empty(DoublyLinkedList list)
{
    return list->length == 0;
}

/// \brief Returns the highest element from the specified DoublyLinkedList_s.
///
/// Returns a reference to the highest element in the list or \c NULL if the
/// list is empty.
///
/// \param[in] list DoublyLinkedList_s reference.
///
/// \return NULL if the list references to \c NULL, if the list is empty or if
/// the compare function is not set.
/// \return The highest element in the list.
void *dll_max(DoublyLinkedList list)
{
    if (list == NULL)
        return NULL;

    if (dll_empty(list))
        return NULL;

    if (list->v_compare == NULL)
        return NULL;

    DoublyLinkedNode scan = list->head;

    void *result = scan->data;

    while (scan != NULL)
    {
        if (list->v_compare(scan->data, result) > 0)
            result = scan->data;

        scan = scan->next;
    }

    return result;
}

/// \brief Returns the smallest element from the specified DoublyLinkedList_s.
///
/// Returns a reference to the smallest element in the list or \c NULL if the
/// list is empty.
///
/// \param[in] list DoublyLinkedList_s reference.
///
/// \return NULL if the list references to \c NULL, if the list is empty or if
/// the compare function is not set.
/// \return The smallest element in the list.
void *dll_min(DoublyLinkedList list)
{
    if (list == NULL)
        return NULL;

    if (dll_empty(list))
        return NULL;

    if (list->v_compare == NULL)
        return NULL;

    DoublyLinkedNode scan = list->head;

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
/// \param[in] list DoublyLinkedList_s reference.
/// \param[in] key Key to be searched in the list.
///
/// \return -3 if there is no default compare function.
/// \return -2 if the list references to \c NULL.
/// \return -1 if the element was not found.
/// \return The index of the matched element.
integer_t dll_index_first(DoublyLinkedList list, void *key)
{
    if (list == NULL)
        return -2;

    if (list->v_compare == NULL)
        return 3;

    DoublyLinkedNode scan = list->head;

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
/// \param[in] list DoublyLinkedList_s reference.
/// \param[in] key Key to be searched in the list.
///
/// \return -3 if there is no default compare function.
/// \return -2 if the list references to \c NULL.
/// \return -1 if the element was not found.
/// \return The index of the matched element.
integer_t dll_index_last(DoublyLinkedList list, void *key)
{
    if (list == NULL)
        return -2;

    if (list->v_compare == NULL)
        return -3;

    DoublyLinkedNode scan = list->tail;

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

/// \brief Checks if a given element is present in the specified list.
///
/// Returns true if the element is present in the list, otherwise false.
///
/// \warning This function does not checks for \c NULL references for either
/// the list parameter or if the default compare function is set.
///
/// \param[in] list DoublyLinkedList_s reference.
/// \param[in] key Key to be matched.
///
/// \return True if the element is present in the list.
/// \return False if the element is not present in the list.
bool dll_contains(DoublyLinkedList list, void *key)
{
    if (list == NULL)
        return false;

    if (dll_empty(list))
        return false;

    DoublyLinkedNode scan = list->head;

    while (scan != NULL)
    {
        if (list->v_compare(scan->data, key) == 0)
            return true;

        scan = scan->next;
    }

    return false;
}

/// \brief Reverses a DoublyLinkedList_s.
///
/// Reverses the chain of elements from the list. This function only changes
/// the \c next and \c prev pointers from each element and the \c head and
/// \c tail pointers of the list struct.
///
/// \param[in] list DoublyLinkedList_s reference to be reversed.
///
/// \return DS_ERR_NULL_POINTER if list references to \c NULL.
/// \return DS_OK if all operations are successful.
Status dll_reverse(DoublyLinkedList list)
{
    if (list == NULL)
        return DS_ERR_NULL_POINTER;

    // If there is only one node
    if (list->length < 2)
        return DS_OK;

    DoublyLinkedNode prev = NULL;
    DoublyLinkedNode curr = list->head;
    DoublyLinkedNode next = NULL;

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

    list->version_id++;

    return DS_OK;
}

/// \brief Makes a copy of the specified DoublyLinkedList_s.
///
/// Makes an exact copy of a list, copying each element using the default copy
/// function.
///
/// \param[in] list DoublyLinkedList_s to be copied.
/// \param[out] result Resulting copy.
///
/// \return DS_ERR_INCOMPLETE_TYPE if either a default copy or a default free
/// functions are not set.
/// \return DS_ERR_NULL_POINTER if list references to \c NULL.
/// \return DS_OK if all operations are successful.
Status dll_copy(DoublyLinkedList list, DoublyLinkedList *result)
{
    *result = NULL;

    if (list == NULL)
        return DS_ERR_NULL_POINTER;

    if (list->v_copy == NULL || list->v_free == NULL)
        return DS_ERR_INCOMPLETE_TYPE;

    Status st = dll_create(result, list->v_compare, list->v_copy,
                           list->v_display, list->v_free);

    if (st != DS_OK)
        return st;

    (*result)->limit = list->limit;

    DoublyLinkedNode scan = list->head;

    void *elem;

    while (scan != NULL)
    {
        elem = list->v_copy(scan->data);

        st = dll_insert_tail(*result, elem);

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
/// \param[in] list DoublyLinkedList_s reference.
/// \param[out] result Resulting array.
/// \param[out] length Resulting array's length.
///
/// \return DS_ERR_INCOMPLETE_TYPE if a default copy function is not set.
/// \return DS_ERR_NULL_POINTER if list references to \c NULL.
/// \return DS_OK if all operations are successful.
Status dll_to_array(DoublyLinkedList list, void ***result, integer_t *length)
{
// If anything goes wrong...
    *result = NULL;
    *length = -1;

    if (list == NULL)
        return DS_ERR_NULL_POINTER;

    if (dll_empty(list))
        return DS_ERR_INVALID_OPERATION;

    if (list->v_copy == NULL)
        return DS_ERR_INCOMPLETE_TYPE;

    *length = list->length;

    *result = malloc(sizeof(void*) * (*length));

    if (!(*result))
        return DS_ERR_ALLOC;

    DoublyLinkedNode scan = list->head;

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
Status dll_link(DoublyLinkedList list1, DoublyLinkedList list2)
{
    /// \todo dll_link
    return DS_OK;
}

/// \brief Links list2 at the specified position of list1.
///
/// \param list1
/// \param list2
/// \param position
///
/// \return
Status dll_link_at(DoublyLinkedList list1, DoublyLinkedList list2,
        integer_t position)
{
    /// \todo dll_link_at
    return DS_OK;
}

/// \brief Unlinks elements from the specified position to the end.
///
/// \param list
/// \param result
/// \param position
///
/// \return
Status dll_unlink(DoublyLinkedList list, DoublyLinkedList result,
        integer_t position)
{
    /// \todo dll_unlink
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
Status dll_unlink_at(DoublyLinkedList list, DoublyLinkedList result,
        integer_t position1, integer_t position2)
{
    /// \todo dll_unlink_at
    return DS_OK;
}

/// \brief Displays a DoublyLinkedList_s in the console.
///
/// Displays a DoublyLinkedList_s in the console with its elements separated by
/// arrows indicating a doubly-linked list.
///
/// \param[in] list The DoublyLinkedList_s to be displayed in the console.
///
/// \return DS_ERR_INCOMPLETE_TYPE if a default display function is not set.
/// \return DS_ERR_NULL_POINTER if the list references to \c NULL.
/// \return DS_OK if all operations were successful.
Status dll_display(DoublyLinkedList list)
{
    if (list == NULL)
        return DS_ERR_NULL_POINTER;

    if (list->v_display == NULL)
        return DS_ERR_INCOMPLETE_TYPE;

    if (dll_empty(list))
    {
        printf("\nDoubly Linked List\n[ empty ]\n");

        return DS_OK;
    }

    DoublyLinkedNode scan = list->head;

    printf("\nDoubly Linked List\nNULL <->");

    while (scan != NULL)
    {
        list->v_display(scan->data);

        printf(" <-> ");

        scan = scan->next;
    }

    printf("NULL\n");

    return DS_OK;
}

/// \brief Displays a DoublyLinkedList_s in the console.
///
/// Displays a DoublyLinkedList_s in the console like an array with its
/// elements separated by commas, delimited with brackets.
///
/// \param[in] list The DoublyLinkedList_s to be displayed in the console.
///
/// \return DS_ERR_INCOMPLETE_TYPE if a default display function is not set.
/// \return DS_ERR_NULL_POINTER if the list references to \c NULL.
/// \return DS_OK if all operations were successful.
Status dll_display_array(DoublyLinkedList list)
{
    if (list == NULL)
        return DS_ERR_NULL_POINTER;

    if (list->v_display == NULL)
        return DS_ERR_INCOMPLETE_TYPE;

    if (dll_empty(list))
    {
        printf("\n[ empty ]\n");

        return DS_OK;
    }

    DoublyLinkedNode scan = list->head;

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

/// \brief Displays a DoublyLinkedList_s in the console.
///
/// Displays a DoublyLinkedList_s in the console with its elements separated by
/// spaces.
///
/// \param[in] list The DoublyLinkedList_s to be displayed in the console.
///
/// \return DS_ERR_INCOMPLETE_TYPE if a default display function is not set.
/// \return DS_ERR_NULL_POINTER if the list references to \c NULL.
/// \return DS_OK if all operations were successful.
Status dll_display_raw(DoublyLinkedList list)
{
    if (list == NULL)
        return DS_ERR_NULL_POINTER;

    if (list->v_display == NULL)
        return DS_ERR_INCOMPLETE_TYPE;

    printf("\n");

    if (dll_empty(list))
        return DS_OK;

    DoublyLinkedNode scan = list->head;

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

/// \brief Builds a new DoublyLinkedNode_s.
///
/// Implementation detail. Function responsible for allocating a new
/// DoublyLinkedNode_s.
///
/// \param[in,out] node DoublyLinkedNode_s to be allocated.
/// \param[in] element Node's data member.
///
/// \return DS_ERR_ALLOC if node allocation failed.
/// \return DS_OK if all operations are successful.
static Status dll_make_node(DoublyLinkedNode *node, void *element)
{
    (*node) = malloc(sizeof(DoublyLinkedNode_t));

    if (!(*node))
        return DS_ERR_ALLOC;

    (*node)->next = NULL;
    (*node)->prev = NULL;
    (*node)->data = element;

    return DS_OK;
}

/// \brief Frees a DoublyLinkedNode_s and its data.
///
/// Implementation detail. Frees a DoublyLinkedNode_s and its data using the
/// list's default free function.
///
/// \param[in,out] node DoublyLinkedNode_s to be freed from memory.
/// \param[in] free_f List's default free function.
///
/// \return DS_ERR_NULL_POINTER if node references to \c NULL.
/// \return DS_OK if all operations are successful.
static Status dll_free_node(DoublyLinkedNode *node, dll_free_f free_f)
{
    if ((*node) == NULL)
        return DS_ERR_NULL_POINTER;

    free_f((*node)->data);

    free(*node);

    (*node) = NULL;

    return DS_OK;
}

/// \brief Frees a DoublyLinkedNode_s.
///
/// Implementation detail. Frees a DoublyLinkedNode_s and leaves its data
/// untouched.
///
/// \param[in,out] node DoublyLinkedNode_s to be freed from memory.
///
/// \return DS_ERR_NULL_POINTER if node references to \c NULL.
/// \return DS_OK if all operations are successful.
static Status dll_free_node_shallow(DoublyLinkedNode *node)
{
    if ((*node) == NULL)
        return DS_ERR_NULL_POINTER;

    free(*node);

    (*node) = NULL;

    return DS_OK;
}

/// \brief Gets a node from a specific position.
///
/// Implementation detail. Searches for a node in O(n / 2) where the search starts
/// at the head or tail of the list.
///
/// \param[in] list DoublyLinkedList_s to search for the node.
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
static Status dll_get_node_at(DoublyLinkedList list, DoublyLinkedNode *result, integer_t position)
{
    // This function effectively searches for a given node. If the position is
    // greater than the list length the search will begin at the end of the list,
    // reducing the amount of iterations needed. This effectively reduces searches
    // to O(n / 2) iterations.
    *result = NULL;

    if (list == NULL)
        return DS_ERR_NULL_POINTER;

    if (dll_empty(list))
        return DS_ERR_INVALID_OPERATION;

    if (position < 0)
        return DS_ERR_NEGATIVE_VALUE;

    if (position >= list->length)
        return DS_ERR_OUT_OF_RANGE;

    if (position <= list->length / 2)
    {
        (*result) = list->head;

        for (integer_t i = 0; i < position; i++)
        {
            if ((*result) == NULL)
                return DS_ERR_ITER;

            (*result) = (*result)->next;
        }
    }
    else
    {
        (*result) = list->tail;

        for (integer_t i = list->length - 1; i > position; i--)
        {
            if ((*result) == NULL)
                return DS_ERR_ITER;

            (*result) = (*result)->prev;
        }
    }

    return DS_OK;
}

////////////////////////////////////////////// END OF NOT EXPOSED FUNCTIONS ///

///////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////// Iterator ///
///////////////////////////////////////////////////////////////////////////////

/// \todo DoublyLinkedListIterator

///////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////// Wrapper ///
///////////////////////////////////////////////////////////////////////////////

/// \todo DoublyLinkedListWrapper