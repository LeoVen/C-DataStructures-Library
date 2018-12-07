/**
 * @file CircularLinkedList.c
 * 
 * @author Leonardo Vencovsky (https://github.com/LeoVen)
 *
 * @date 09/10/2018
 */

#include "CircularLinkedList.h"

/// \brief A generic circular doubly-linked list.
///
/// A CircularLinkedList_s is a doubly-linked list where all nodes are
/// connected to form a circular structure. There are no \c head or \c tail
/// pointers; the only reference is the current node where all operations work
/// relatively to this pointer. This implementation allows insertions before or
/// after the current node. Elements can be removed from before or after the
/// current node or the current node itself.
///
/// This circular linked list is implemented as a DoublyLinkedList. This allows
/// much faster insertions and removals, where they all practically made in
/// O(1) time.
///
/// To initialize a CircularLinkedList_s use cll_init(). This only initializes
/// the structure. If you don't set the default functions later you won't be
/// able to do certain operations. If you want to initialize it completely, use
/// instead cll_create(). Here you must pass in default functions (compare,
/// copy, display and free) according with the specifications of each type of
/// function.
///
/// All input and output functions operate relative to the list cursor. To add
/// elements to the list use cll_insert_after() that add an element after the
/// cursor's node; or cll_insert_before() that adds an element before it. If it
/// is the first element being inserted then it will become the new reference
/// (cursor). To remove elements from the list you can use cll_remove_before()
/// or cll_remove_after(). In either option if the node is the last one in the
/// list then the cursor is removed (because it is the remaining node). Also
/// cll_remove_current() removes the current element pointed by the cursor. The
/// cursor then goes one position backwards if possible or is set to NULL if it
/// is the last element. You can change in the code whether the cursor goes
/// backwards or forwards.
///
/// To delete a list use cll_free(). This completely frees all elements and
/// sets the list pointer to \c NULL. Note that if you haven't set a default
/// free function you won't be able to delete the list or its elements. You
/// must set a free function that will be responsible for freeing from memory
/// all elements. You can also use cll_free_shallow() that will only free the
/// list structure.
///
/// \b Advantages over \c DoublyLinkedList
/// - Inserting, removing and accessing nodes takes half the time on average
///
/// \b Drawbacks
/// - Extra memory for yet another pointer on each element
///
/// \b Functions
///
/// Located in the file CircularLinkedList.c
struct CircularLinkedList_s
{
    /// \brief List length.
    ///
    /// List current amount of elements.
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

    /// \brief Pointer to current node.
    ///
    /// Pointer to current node. Functions operate relative to this node.
    struct CircularLinkedNode_s *cursor;

    /// \brief Comparator function.
    ///
    /// A function that compares one element with another that returns an int
    /// with the following rules:
    ///
    /// - <code>[ > 0 ]</code> if first element is greater than the second;
    /// - <code>[ < 0 ]</code> if second element is greater than the first;
    /// - <code>[ 0 ]</code> if elements are equal.
    cll_compare_f v_compare;

    /// \brief Copy function.
    ///
    /// A function that returns an exact copy of an element.
    cll_copy_f v_copy;

    /// \brief Display function.
    ///
    /// A function that displays an element in the console. Useful for
    /// debugging.
    cll_display_f v_display;

    /// \brief Deallocator function.
    ///
    /// A function that completely frees an element from memory.
    cll_free_f v_free;

    /// \brief A version id to keep track of modifications.
    ///
    /// This version id is used by the iterator to check if the structure was
    /// modified. The iterator can only function if its version_id is the same
    /// as the structure's version id, that is, there have been no structural
    /// modifications (except for those done by the iterator itself).
    integer_t version_id;
};

/// \brief A CircularLinkedList_s node.
///
/// Implementation detail. This is a doubly-linked node with a pointer to the
/// previous node and another pointer to the next node. If it is the last
/// element then both pointers will be pointing to the node itself.
struct CircularLinkedNode_s
{
    /// \brief Data pointer.
    ///
    /// Points to node's data. The data needs to be dynamically allocated.
    void *data;

    /// \brief Next node on the list.
    ///
    /// Pointer to the next node on the list.
    struct CircularLinkedNode_s *next;

    /// \brief Previous node on the list.
    ///
    /// Pointer to the previous node on the list.
    struct CircularLinkedNode_s *prev;
};

/// \brief A type for a circular linked list node.
///
/// Defines a type to a <code> struct CircularLinkedNode_s </code>.
typedef struct CircularLinkedNode_s CircularLinkedNode_t;

/// \brief A pointer type for a circular linked list node.
///
/// Defines a pointer type to a <code> struct CircularLinkedNode_s </code>.
typedef struct CircularLinkedNode_s *CircularLinkedNode;

///////////////////////////////////////////////////// NOT EXPOSED FUNCTIONS ///

static Status cll_make_node(CircularLinkedNode *node, void *value);

static Status cll_free_node(CircularLinkedNode *node, cll_free_f free_f);

static Status cll_free_node_shallow(CircularLinkedNode *node);

////////////////////////////////////////////// END OF NOT EXPOSED FUNCTIONS ///

/// \brief Initializes a CircularLinkedList_s structure.
///
/// This function initializes the CircularLinkedList_s structure but does not
/// sets any default functions. Be sure to initialize them when needed or some
/// functions might return with an error.
///
/// \param[in,out] list The CircularLinkedList_s to be allocated.
///
/// \return DS_ERR_ALLOC if list allocation failed.
/// \return DS_OK if all operations were successful.
Status cll_init(CircularLinkedList *list)
{
    (*list) = malloc(sizeof(CircularLinkedList_t));

    if (!(*list))
        return DS_ERR_ALLOC;

    (*list)->length = 0;
    (*list)->limit = 0;

    (*list)->cursor = NULL;

    (*list)->v_compare = NULL;
    (*list)->v_copy = NULL;
    (*list)->v_display = NULL;
    (*list)->v_free = NULL;

    return DS_OK;
}

Status cll_create(CircularLinkedList *list, cll_compare_f compare_f,
                cll_copy_f copy_f, cll_display_f display_f, cll_free_f free_f)
{
    *list = malloc(sizeof(CircularLinkedList_t));

    if (!(*list))
        return DS_ERR_ALLOC;

    (*list)->length = 0;
    (*list)->limit = 0;

    (*list)->cursor = NULL;

    (*list)->v_compare = compare_f;
    (*list)->v_copy = copy_f;
    (*list)->v_display = display_f;
    (*list)->v_free = free_f;

    return DS_OK;
}

/// \brief Frees from memory a CircularLinkedList_s and all its elements.
///
/// Iterates through every node of the list and frees them from memory along
/// with its data. Then the CircularLinkedList_s structure is deallocated and
/// set to \c NULL.
///
/// \param[in,out] list The list to be freed from memory.
///
/// \return DS_ERR_INCOMPLETE_TYPE if a default free function is not set.
/// \return DS_ERR_NULL_POINTER if the list reference is \c NULL.
/// \return DS_OK if all operations were successful.
Status cll_free(CircularLinkedList *list)
{
    if ((*list) == NULL)
        return DS_ERR_NULL_POINTER;

    if ((*list)->v_free == NULL)
        return DS_ERR_INCOMPLETE_TYPE;

    Status st;

    CircularLinkedNode prev = (*list)->cursor;

    for (integer_t i = 0; i < (*list)->length; i++)
    {
        (*list)->cursor = (*list)->cursor->next;

        st = cll_free_node(&prev, (*list)->v_free);

        if (st != DS_OK)
            return st;

        prev = (*list)->cursor;
    }

    free(*list);

    *list = NULL;

    return DS_OK;
}

/// \brief Frees from memory a CircularLinkedList_s.
///
/// This function frees from memory all the list's nodes without freeing its
/// data and then frees the list's structure. The variable is then set to
/// \c NULL.
///
/// \param[in,out] list CircularLinkedList_s to be freed from memory.
///
/// \return DS_ERR_NULL_POINTER if the list references to \c NULL.
/// \return DS_OK if all operations are successful.
Status cll_free_shallow(CircularLinkedList *list)
{
    if ((*list) == NULL)
        return DS_ERR_NULL_POINTER;

    if ((*list)->v_free == NULL)
        return DS_ERR_INCOMPLETE_TYPE;

    Status st;

    CircularLinkedNode prev = (*list)->cursor;

    for (integer_t i = 0; i < (*list)->length; i++)
    {
        (*list)->cursor = (*list)->cursor->next;

        st = cll_free_node_shallow(&prev);

        if (st != DS_OK)
            return st;

        prev = (*list)->cursor;
    }

    free(*list);

    *list = NULL;

    return DS_OK;
}

/// This function sets the list to its initial state, erasing all of its data
/// and re-initializing the structure. It is equivalent to calling cll_free()
/// and then cll_init().
///
/// \param cll The list to be erased.
///
/// \return DS_ERR_ALLOC if list allocation failed.
/// \return DS_ERR_NULL_POINTER if the list reference is \c NULL.
/// \return DS_OK if all operations were successful.
Status cll_erase(CircularLinkedList *cll)
{
    if ((*cll) == NULL)
        return DS_ERR_NULL_POINTER;

    Status st = cll_free(cll);

    if (st != DS_OK)
        return st;

    st = cll_init(cll);

    if (st != DS_OK)
        return st;

    return DS_OK;
}

/// \brief Sets the default compare function.
///
/// Use this function to set a default compare function. It needs to comply
/// with the cll_free_f specifications.
///
/// \param[in] list CircularLinkedList_s to set the default compare function.
/// \param[in] function A cll_compare_f kind of function.
///
/// \return DS_ERR_NULL_POINTER if the list references to \c NULL.
/// \return DS_OK if all operations are successful.
Status cll_set_v_compare(CircularLinkedList list, cll_compare_f function)
{
    if (list == NULL)
        return DS_ERR_NULL_POINTER;

    list->v_compare = function;

    return DS_OK;
}

/// \brief Sets the default copy function.
///
/// Use this function to set a default compare function. It needs to comply
/// with the cll_copy_f specifications.
///
/// \param[in] list CircularLinkedList_s to set the default copy function.
/// \param[in] function A cll_copy_f kind of function.
///
/// \return DS_ERR_NULL_POINTER if the list references to \c NULL.
/// \return DS_OK if all operations are successful.
Status cll_set_v_copy(CircularLinkedList list, cll_copy_f function)
{
    if (list == NULL)
        return DS_ERR_NULL_POINTER;

    list->v_copy = function;

    return DS_OK;
}

/// \brief Sets the default display function.
///
/// Use this function to set a default display function. It needs to comply
/// with the cll_display_f specifications. Useful for debugging.
///
/// \param[in] list CircularLinkedList_s to set the default display function.
/// \param[in] function A cll_display_f kind of function.
///
/// \return DS_ERR_NULL_POINTER if the list references to \c NULL.
/// \return DS_OK if all operations are successful.
Status cll_set_v_display(CircularLinkedList list, cll_display_f function)
{
    if (list == NULL)
        return DS_ERR_NULL_POINTER;

    list->v_display = function;

    return DS_OK;
}

/// \brief Sets the default free function.
///
/// Use this function to set a default free function. It needs to comply
/// with the cll_free_f specifications.
///
/// \param[in] list CircularLinkedList_s to set the default free function.
/// \param[in] function A cll_free_f kind of function.
///
/// \return DS_ERR_NULL_POINTER if the list references to \c NULL.
/// \return DS_OK if all operations are successful.
Status cll_set_v_free(CircularLinkedList list, cll_free_f function)
{
    if (list == NULL)
        return DS_ERR_NULL_POINTER;

    list->v_free = function;

    return DS_OK;
}

Status cll_set_limit(CircularLinkedList list, integer_t limit)
{
    if (list == NULL)
        return DS_ERR_NULL_POINTER;

    if (list->length > limit && limit > 0)
        return DS_ERR_INVALID_OPERATION;

    list->limit = limit;

    return DS_OK;
}

integer_t cll_length(CircularLinkedList list)
{
    if (list == NULL)
        return -1;

    return list->length;
}

integer_t cll_limit(CircularLinkedList list)
{
    if (list == NULL)
        return -1;

    return list->limit;
}

/// Inserts a new element after the current node pointed by the list. If it is
/// the first element to be inserted, then it becomes the current node pointed
/// by \c curr.
///
/// \param cll The list where the element is to be inserted.
/// \param element The element to be inserted in the list.
///
/// \return DS_ERR_ALLOC if node allocation failed.
/// \return DS_ERR_FULL if \c limit is set (greater than 0) and the list
/// length reached the specified limit.
/// \return DS_ERR_NULL_POINTER if the list reference is \c NULL.
/// \return DS_OK if all operations were successful.
Status cll_insert_after(CircularLinkedList cll, void *element)
{
    if (cll == NULL)
        return DS_ERR_NULL_POINTER;

    CircularLinkedNode node;

    if (cll_full(cll))
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

/// Inserts a new element before the current node pointed by the list cursor.
/// If it is the first element to be inserted, then it becomes the current node
/// pointed by \c curr.
///
/// \param cll The list where the element is to be inserted.
/// \param element The element to be inserted in the list.
///
/// \return DS_ERR_ALLOC if node allocation failed.
/// \return DS_ERR_FULL if \c limit is set (greater than 0) and the list
/// length reached the specified limit.
/// \return DS_ERR_NULL_POINTER if the list reference is \c NULL.
/// \return DS_OK if all operations were successful.
Status cll_insert_before(CircularLinkedList cll, void *element)
{
    if (cll == NULL)
        return DS_ERR_NULL_POINTER;

    if (cll_full(cll))
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

/// Removes and retrieves the element located after the current node pointed
/// by the list cursor. If it is the last element, then the cursor's node is
/// retrieved.
///
/// \param[in] cll The list where the element is to be removed from.
/// \param[out] result The resulting element.
///
/// \return DS_ERR_INVALID_OPERATION if the list is empty.
/// \return DS_ERR_NULL_POINTER if the list reference is \c NULL.
/// \return DS_OK if all operations were successful.
Status cll_remove_after(CircularLinkedList cll, void **result)
{
    *result = NULL;

    if (cll == NULL)
        return DS_ERR_NULL_POINTER;

    if (cll_empty(cll))
        return DS_ERR_INVALID_OPERATION;

    Status st;

    if (cll->length == 1)
    {
        *result = cll->cursor->data;

        st = cll_free_node_shallow(&(cll->cursor));

        if (st != DS_OK)
            return st;
    }
    else
    {
        CircularLinkedNode node = cll->cursor->next;

        *result = node->data;

        node->prev->next = node->next;
        node->next->prev = node->prev;

        st = cll_free_node_shallow(&node);

        if (st != DS_OK)
            return st;
    }

    (cll->length)--;

    return DS_OK;
}

/// Removes and retrieves the current element pointed by the \c cursor pointer.
/// At this point the \c cursor pointer can either move to the next element or
/// the previous one. Both ways are written but one must remain commented.
///
/// \param[in] cll The list where the element is to be removed from.
/// \param[out] result The resulting element.
///
/// \return DS_ERR_INVALID_OPERATION if the list is empty.
/// \return DS_ERR_NULL_POINTER if the list reference is \c NULL.
/// \return DS_OK if all operations were successful.
Status cll_remove_current(CircularLinkedList cll, void **result)
{
    *result = NULL;

    if (cll == NULL)
        return DS_ERR_NULL_POINTER;

    if (cll_empty(cll))
        return DS_ERR_INVALID_OPERATION;

    Status st;

    if (cll->length == 1)
    {
        *result = cll->cursor->data;

        st = cll_free_node_shallow(&(cll->cursor));

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

        st = cll_free_node_shallow(&node);

        if (st != DS_OK)
            return st;
    }

    (cll->length)--;

    return DS_OK;
}

/// Removes and retrieves the element located before the current node pointed
/// by the list cursor. If it is the last element, then the cursor's node is
/// retrieved.
///
/// \param[in] cll The list where the element is to be removed from.
/// \param[out] result The resulting element.
///
/// \return DS_ERR_INVALID_OPERATION if the list is empty.
/// \return DS_ERR_NULL_POINTER if the list reference is \c NULL.
/// \return DS_OK if all operations were successful.
Status cll_remove_before(CircularLinkedList cll, void **result)
{
    *result = NULL;

    if (cll == NULL)
        return DS_ERR_NULL_POINTER;

    if (cll_empty(cll))
        return DS_ERR_INVALID_OPERATION;

    Status st;

    if (cll->length == 1)
    {
        *result = cll->cursor->data;

        st = cll_free_node_shallow(&(cll->cursor));

        if (st != DS_OK)
            return st;
    }
    else
    {
        CircularLinkedNode node = cll->cursor->prev;

        *result = node->data;

        node->prev->next = node->next;
        node->next->prev = node->prev;

        st = cll_free_node_shallow(&node);

        if (st != DS_OK)
            return st;
    }

    (cll->length)--;

    return DS_OK;
}

bool cll_full(CircularLinkedList cll)
{
    return cll->limit > 0 && cll->length >= cll->limit;
}

bool cll_empty(CircularLinkedList cll)
{
    return cll->length == 0;
}

void *cll_max(CircularLinkedList cll)
{
    void *result;

    if (cll == NULL)
        return NULL;

    if (cll_empty(cll))
        return NULL;

    if (cll->v_compare == NULL)
        return NULL;

    CircularLinkedNode scan = cll->cursor;

    result = scan->data;

    for (integer_t i = 0; i < cll->length; i++)
    {
        if (cll->v_compare(scan->data, result) > 0)
            result = scan->data;

        scan = scan->next;
    }

    return result;
}

void *cll_min(CircularLinkedList cll)
{
    void *result;

    if (cll == NULL)
        return NULL;

    if (cll_empty(cll))
        return NULL;

    if (cll->v_compare == NULL)
        return NULL;

    CircularLinkedNode scan = cll->cursor;

    result = scan->data;

    for (integer_t i = 0; i < cll->length; i++)
    {
        if (cll->v_compare(scan->data, result) < 0)
            result = scan->data;

        scan = scan->next;
    }

    return result;
}

bool cll_contains(CircularLinkedList cll, void *key)
{
    CircularLinkedNode scan = cll->cursor;

    for (integer_t i = 0; i < cll->length; i++)
    {
        if (cll->v_compare(scan->data, key) == 0)
            return true;

        scan = scan->next;
    }

    return false;
}

void *cll_peek_next(CircularLinkedList cll)
{
    if (cll == NULL)
        return NULL;

    if (cll_empty(cll))
        return NULL;

    return cll->cursor->next->data;
}

void *cll_peek(CircularLinkedList cll)
{
    if (cll == NULL)
        return NULL;

    if (cll_empty(cll))
        return NULL;

    return cll->cursor->data;
}

void *cll_peek_prev(CircularLinkedList cll)
{
    if (cll == NULL)
        return NULL;

    if (cll_empty(cll))
        return NULL;

    return cll->cursor->prev->data;
}

Status cll_copy(CircularLinkedList list, CircularLinkedList *result)
{
    *result = NULL;

    if (list == NULL)
        return DS_ERR_NULL_POINTER;

    if (list->v_copy == NULL || list->v_free == NULL)
        return DS_ERR_INCOMPLETE_TYPE;

    Status st = cll_create(result, list->v_compare, list->v_copy,
            list->v_display, list->v_free);

    if (st != DS_OK)
        return st;

    if (cll_empty(list))
        return DS_OK;

    CircularLinkedNode scan = list->cursor;

    void *elem;

    for (integer_t i = 0; i < list->length; i++)
    {
        elem = list->v_copy(scan->data);

        st = cll_insert_before(*result, elem);

        if (st != DS_OK)
        {
            list->v_free(elem);

            return st;
        }

        scan = scan->next;
    }

    return DS_OK;
}

/// Iterates the list cursor forward the specified number of positions.
///
/// \param cll The list to iterate the cursor.
/// \param positions The number of nodes to iterate.
///
/// \return DS_ERR_INVALID_OPERATION if the list is empty.
/// \return DS_ERR_NEGATIVE_VALUE if \c positions is a negative value.
/// \return DS_ERR_NULL_POINTER if the list reference is \c NULL.
/// \return DS_OK if all operations were successful.
Status cll_iter_next(CircularLinkedList cll, integer_t positions)
{
    if (cll == NULL)
        return DS_ERR_NULL_POINTER;

    if (cll_empty(cll))
        return DS_ERR_INVALID_OPERATION;

    if (positions < 0)
        return DS_ERR_NEGATIVE_VALUE;

    for (integer_t i = 0; i < positions; i++)
        cll->cursor = cll->cursor->next;

    return DS_OK;
}

/// This function encapsulates cll_iter_next() and cll_iter_prev(). If a
/// negative value for \c positions is given then it will iterate backwards
/// with cll_iter_prev(), otherwise it will iterate with cll_iter_next().
///
/// \param cll The list to iterate the cursor.
/// \param positions The number of nodes to iterate.
///
/// \return DS_ERR_INVALID_OPERATION if the list is empty.
/// \return DS_ERR_NULL_POINTER if the list reference is \c NULL.
/// \return DS_OK if all operations were successful.
Status cll_iter(CircularLinkedList cll, integer_t positions)
{
    if (cll == NULL)
        return DS_ERR_NULL_POINTER;

    if (cll_empty(cll))
        return DS_ERR_INVALID_OPERATION;

    if (positions < 0)
    {
        return cll_iter_prev(cll, positions * -1);
    }
    else
    {
        return cll_iter_next(cll, positions);
    }
}

/// Iterates the list cursor backwards the specified number of positions.
///
/// \param cll The list to iterate the cursor.
/// \param positions The number of nodes to iterate.
///
/// \return DS_ERR_INVALID_OPERATION if the list is empty.
/// \return DS_ERR_NEGATIVE_VALUE if \c positions is a negative value.
/// \return DS_ERR_NULL_POINTER if the list reference is \c NULL.
/// \return DS_OK if all operations were successful.
Status cll_iter_prev(CircularLinkedList cll, integer_t positions)
{
    if (cll == NULL)
        return DS_ERR_NULL_POINTER;

    if (cll_empty(cll))
        return DS_ERR_INVALID_OPERATION;

    if (positions < 0)
        return DS_ERR_NEGATIVE_VALUE;

    for (integer_t i = 0; i < positions; i++)
        cll->cursor = cll->cursor->prev;

    return DS_OK;
}

/// Displays a \c CircularLinkedList in the console.
///
/// \param[in] list The list to be displayed in the console.
///
/// \return DS_ERR_INCOMPLETE_TYPE if a default display function is not set.
/// \return DS_ERR_NULL_POINTER if the list reference is \c NULL.
/// \return DS_OK if all operations were successful.
Status cll_display(CircularLinkedList list)
{
    if (list == NULL)
        return DS_ERR_NULL_POINTER;

    if (list->v_display == NULL)
        return DS_ERR_INCOMPLETE_TYPE;

    if (cll_empty(list))
    {
        printf("\nCircular Linked List\n[ empty ]\n");

        return DS_OK;
    }

    CircularLinkedNode scan = list->cursor;

    printf("\nCircular Linked List\n <-> ");

    for (integer_t i = 0; i < list->length; i++)
    {
        list->v_display(scan->data);

        printf(" <-> ");

        scan = scan->next;
    }

    printf("\n");

    return DS_OK;
}

/// Displays a \c CircularLinkedList in the console like an array with its
/// elements separated by commas, delimited with brackets.
///
/// \param[in] list The list to be displayed in the console.
///
/// \return DS_ERR_INCOMPLETE_TYPE if a default display function is not set.
/// \return DS_ERR_NULL_POINTER if the list reference is \c NULL.
/// \return DS_OK if all operations were successful.
Status cll_display_array(CircularLinkedList list)
{
    if (list == NULL)
        return DS_ERR_NULL_POINTER;

    if (list->v_display == NULL)
        return DS_ERR_INCOMPLETE_TYPE;

    if (cll_empty(list))
    {
        printf("\n[ empty ]\n");

        return DS_OK;
    }

    CircularLinkedNode scan = list->cursor;

    printf("\n[ ");

    for (integer_t i = 0; i < list->length - 1; i++)
    {
        list->v_display(scan->data);

        printf(", ");

        scan = scan->next;
    }

    list->v_display(scan->data);

    printf(" ]\n");

    return DS_OK;
}

/// Displays a \c CircularLinkedList in the console with its values separated
/// by spaces.
///
/// \param list The list to be displayed in the console.
///
/// \return DS_ERR_INCOMPLETE_TYPE if a default display function is not set.
/// \return DS_ERR_NULL_POINTER if the list reference is \c NULL.
/// \return DS_OK if all operations were successful.
Status cll_display_raw(CircularLinkedList list)
{
    if (list == NULL)
        return DS_ERR_NULL_POINTER;

    if (list->v_display == NULL)
        return DS_ERR_INCOMPLETE_TYPE;

    CircularLinkedNode scan = list->cursor;

    printf("\n");

    for (integer_t i = 0; i < list->length; i++)
    {
        list->v_display(scan->data);

        printf(" ");

        scan = scan->next;
    }

    printf("\n");

    return DS_OK;
}

///////////////////////////////////////////////////// NOT EXPOSED FUNCTIONS ///

static Status cll_make_node(CircularLinkedNode *node, void *value)
{
    *node = malloc(sizeof(CircularLinkedNode_t));

    if (!(*node))
        return DS_ERR_ALLOC;

    (*node)->next = NULL;
    (*node)->prev = NULL;

    (*node)->data = value;

    return DS_OK;
}

static Status cll_free_node(CircularLinkedNode *node, cll_free_f free_f)
{
    if ((*node) == NULL)
        return DS_ERR_NULL_POINTER;

    free_f((*node)->data);

    free(*node);

    *node = NULL;

    return DS_OK;
}

static Status cll_free_node_shallow(CircularLinkedNode *node)
{
    if ((*node) == NULL)
        return DS_ERR_NULL_POINTER;

    free(*node);

    *node = NULL;

    return DS_OK;
}

////////////////////////////////////////////// END OF NOT EXPOSED FUNCTIONS ///

///////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////// Iterator ///
///////////////////////////////////////////////////////////////////////////////

// TODO