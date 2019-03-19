/**
 * @file DequeList.c
 * 
 * @author Leonardo Vencovsky (https://github.com/LeoVen)
 *
 * @date 27/09/2018
 */

#include "DequeList.h"

/// This is a linked list implementation of a Deque. A Deque is a double-ended
/// queue, so you can insert and remove elements from both ends of the queue.
/// It is implemented as a \c DoublyLinkedList but with restricted operations
/// which can only insert or remove elements at the edges of the linked list.
/// With this implementation every insertion and removal operations take
/// constant time O(1). This simplifies things a lot but every node needs to
/// maintain two pointers, one to its left and another to its right.
///
/// \par Advantages over DequeArray_s
/// - Indefinitely grows
/// - No need to reallocate buffers or shift elements
///
/// \par Drawbacks
/// - No random access
/// - More memory usage as in every node there are two pointers to the
/// neighbouring nodes
///
/// \par Functions
/// Located in the file DequeList.c
struct DequeList_s
{
    /// \brief Current amount of elements in the DequeList_s.
    ///
    /// Current amount of elements in the DequeList_s.
    integer_t count;

    /// \brief DequeList length limit.
    ///
    /// If it is set to 0 or a negative value then the deque has no limit to
    /// its length. Otherwise it won't be able to have more elements than the
    /// specified value. The deque is always initialized with no restrictions
    /// to its length, that is, \c limit equals 0. The user won't be able to
    /// limit the deque length if it already has more elements than the
    /// specified limit.
    integer_t limit;

    /// \brief Points to the first Node on the deque.
    ///
    /// Points to the first Node on the deque or \c NULL if the deque is empty.
    struct DequeListNode_s *front;

    /// \brief Points to the last Node on the deque.
    ///
    /// Points to the first Node on the deque or \c NULL if the deque is empty.
    struct DequeListNode_s *rear;

    /// \brief DequeList_s interface.
    ///
    /// An interface is like a table that has function pointers for functions
    /// that will manipulate a desired data type.
    struct Interface_s *interface;

    /// \brief A version id to keep track of modifications.
    ///
    /// This version id is used by the iterator to check if the structure was
    /// modified. The iterator can only function if its version_id is the same
    /// as the structure's version id, that is, there have been no structural
    /// modifications (except for those done by the iterator itself).
    integer_t version_id;
};

/// This can be used together with VLAs to allocate the structure on the stack
/// instead of a heap allocation.
const unsigned_t dql_size = sizeof(DequeList_t);

/// \brief A DequeList_s node.
///
/// Implementation detail. This is a doubly-linked node with a pointer to the
/// previous node (or \c NULL if it is the front node) and another pointer to
/// the next node (or \c NULL if it is the rear node).
struct DequeListNode_s
{
    /// \brief Data pointer.
    ///
    /// Points to node's data. The data needs to be dynamically allocated.
    void *data;

    /// \brief Next node on the deque.
    ///
    /// Next node on the deque or \c NULL if this is the rear node.
    struct DequeListNode_s *next;

    /// \brief Previous node on the deque.
    ///
    /// Previous node on the deque or \c NULL if this is the front node.
    struct DequeListNode_s *prev;
};

/// \brief A type for a deque node.
///
/// Defines a type to a <code> struct DequeListNode_s </code>.
typedef struct DequeListNode_s DequeListNode_t;

/// \brief A pointer type for a deque node.
///
/// Defines a pointer type to a <code> struct DequeListNode_s </code>.
typedef struct DequeListNode_s *DequeListNode;

///////////////////////////////////////////////////// NOT EXPOSED FUNCTIONS ///

static DequeListNode_t *
dql_new_node(void *element);

static void
dql_free_node(DequeListNode_t *node, free_f function);

static void
dql_free_node_shallow(DequeListNode_t *node);

////////////////////////////////////////////// END OF NOT EXPOSED FUNCTIONS ///

/// Initializes a new DequeList_s with a given interface that contains
/// functions to handle any user-defined type.
/// \par Interface Requirements
/// - None
///
/// \param[in] interface An interface defining all necessary functions for the
/// deque to operate.
///
/// \return A new DequeList_s or NULL if allocation failed.
DequeList_t *
dql_new(Interface_t *interface)
{
    DequeList_t *deque = malloc(sizeof(DequeList_t));

    if (!deque)
        return NULL;

    deque->count = 0;
    deque->limit = 0;
    deque->version_id = 0;
    deque->front = NULL;
    deque->rear = NULL;

    deque->interface = interface;

    return deque;
}

/// Initializes a deque allocated on the stack with a given interface.
///
/// \param[in] deque The deque to be initialized.
/// \param[in] interface An interface defining all necessary functions for the
/// deque to operate.
///
/// \return True if all operations were successful.
bool
dql_init(DequeList_t *deque, Interface_t *interface)
{
    deque->count = 0;
    deque->limit = 0;
    deque->version_id = 0;
    deque->front = NULL;
    deque->rear = NULL;
    deque->interface = interface;

    return true;
}

/// Frees each element at the deque using its interface's \c free and then
/// frees the deque struct.
/// \par Interface Requirements
/// - free
///
/// \param[in] deque The deque to be freed from memory.
void
dql_free(DequeList_t *deque)
{
    DequeListNode_t *prev = deque->front;

    while (deque->front != NULL)
    {
        deque->front = deque->front->prev;

        dql_free_node(prev, deque->interface->free);

        prev = deque->front;
    }

    free(deque);
}

/// Frees the DequeList_s structure and its nodes, leaves all the elements
/// intact. Be careful as this might cause severe memory leaks. Only use this
/// if your deque elements are also handled by another structure or algorithm.
/// \par Interface Requirements
/// - None
///
/// \param[in] deque The deque to be freed from memory.
void
dql_free_shallow(DequeList_t *deque)
{
    DequeListNode_t *prev = deque->front;

    while (deque->front != NULL)
    {
        deque->front = deque->front->prev;

        dql_free_node_shallow(prev);

        prev = deque->front;
    }

    free(deque);
}

/// This function will reset the DequeList_s, freeing all of its nodes along
/// with its elements, keeping the structure intact including its original
/// interface.
/// \par Interface Requirements
/// - free
///
/// \param[in] deque The deque to have its elements erased.
void
dql_erase(DequeList_t *deque)
{
    DequeListNode_t *prev = deque->front;

    while (deque->front != NULL)
    {
        deque->front = deque->front->prev;

        dql_free_node(prev, deque->interface->free);

        prev = deque->front;
    }

    deque->count = 0;
    deque->version_id++;
    deque->front = NULL;
    deque->rear = NULL;
}

/// This function will reset the DequeList_s, freeing all of its nodes but not
/// its elements, keeping the structure intact including its original
/// interface.
/// \par Interface Requirements
/// - None
///
/// \param[in] deque The deque to have its nodes erased.
void
dql_erase_shallow(DequeList_t *deque)
{
    DequeListNode_t *prev = deque->front;

    while (deque->front != NULL)
    {
        deque->front = deque->front->prev;

        dql_free_node_shallow(prev);

        prev = deque->front;
    }

    deque->count = 0;
    deque->version_id++;
    deque->front = NULL;
    deque->rear = NULL;
}

/// Sets a new interface for the specified DequeList_s.
/// \par Interface Requirements
/// - None
///
/// \param[in] deque DequeList_s to change the interface.
/// \param[in] new_interface New interface for the specified structure.
void
dql_config(DequeList_t *deque, Interface_t *new_interface)
{
    deque->interface = new_interface;
}

/// Returns the current amount of elements in the specified deque.
/// \par Interface Requirements
/// - None
///
/// \param[in] deque DequeList_s reference.
///
/// \return The deque total amount of elements.
integer_t
dql_count(DequeList_t *deque)
{
    return deque->count;
}

/// Returns the current deque limit.
/// \par Interface Requirements
/// - None
///
/// \param[in] deque DequeList_s reference.
///
/// \return The current deque limit.
integer_t
dql_limit(DequeList_t *deque)
{
    return deque->limit;
}

/// Limit's the DequeList_s's length. You can only set a limit greater or equal to
/// the deque's current length and greater than 0. To remove this limitation
/// simply set the limit to 0 or less.
///
/// \param[in] deque DequeList_s reference.
/// \param[in] limit New deque limit.
///
/// \return True if a new limit was set. False if the new limit is lower than
/// the current amount of elements in the deque.
bool
dql_set_limit(DequeList_t *deque, integer_t limit)
{
    // The new limit can't be lower than the deque's current length.
    if (deque->count > limit && limit > 0)
        return false;

    deque->limit = limit;

    return true;
}

/// Inserts an element at the front of the specified deque.
///
/// \param[in] deque The deque where the element is to be inserted.
/// \param[in] element The element to be inserted in the deque.
///
/// \return True if the element was successfully added to the deque or false if
/// the deque reached its limit size or node allocation failed.
bool
dql_enqueue_front(DequeList_t *deque, void *element)
{
    if (dql_full(deque))
        return false;

    DequeListNode_t *node = dql_new_node(element);

    if (!node)
        return false;

    if (dql_empty(deque))
    {
        deque->front = node;
        deque->rear = node;
    }
    else
    {
        node->prev = deque->front;

        deque->front->next = node;
        deque->front = node;
    }

    deque->count++;
    deque->version_id++;

    return true;
}

/// Inserts an element at the rear of the specified deque.
///
/// \param[in] deque The deque where the element is to be inserted.
/// \param[in] element The element to be inserted in the deque.
///
/// \return True if the element was successfully added to the deque or false if
/// the deque reached its limit size or node allocation failed.
bool
dql_enqueue_rear(DequeList_t *deque, void *element)
{
    if (dql_full(deque))
        return false;

    DequeListNode_t *node = dql_new_node(element);

    if (!node)
        return false;

    if (dql_empty(deque))
    {
        deque->front = node;
        deque->rear = node;
    }
    else
    {
        node->next = deque->rear;

        deque->rear->prev = node;
        deque->rear = node;
    }

    deque->count++;
    deque->version_id++;

    return true;
}

/// Removes an element from the front of the specified deque.
///
/// \param[in] deque The deque where the element is to be removed from.
/// \param[out] result The resulting element removed from the deque.
///
/// \return True if the element was successfully removed from the deque or
/// false if the deque is empty.
bool
dql_dequeue_front(DequeList_t *deque, void **result)
{
    *result = NULL;

    if (dql_empty(deque))
        return false;

    DequeListNode_t *node = deque->front;

    *result = node->data;

    deque->front = deque->front->prev;

    if (deque->front == NULL)
        deque->rear = NULL;
    else
        deque->front->next = NULL;

    dql_free_node_shallow(node);

    deque->count--;
    deque->version_id++;

    return true;
}

/// Removes an element from the rear of the specified deque.
///
/// \param[in] deque The deque where the element is to be removed from.
/// \param[out] result The resulting element removed from the deque.
///
/// \return True if the element was successfully removed from the deque or
/// false if the deque is empty.
bool
dql_dequeue_rear(DequeList_t *deque, void **result)
{
    *result = NULL;

    if (dql_empty(deque))
        return false;

    DequeListNode_t *node = deque->rear;

    *result = node->data;

    deque->rear = deque->rear->next;

    if (deque->rear == NULL)
        deque->front = NULL;
    else
        deque->rear->prev = NULL;

    dql_free_node_shallow(node);

    deque->count--;
    deque->version_id++;

    return true;
}

/// Returns the element located at the front of the deque or NULL if the deque
/// is empty.
/// \par Interface Requirements
/// - None
///
/// \param[in] deque The target deque.
///
/// \return NULL if the deque is empty or the element at the front of the
/// deque.
void *
dql_peek_front(DequeList_t *deque)
{
    if (dql_empty(deque))
        return NULL;

    return deque->front->data;
}

/// Returns the element located at the rear of the deque or NULL if the deque
/// is empty.
/// \par Interface Requirements
/// - None
///
/// \param[in] deque The target deque.
///
/// \return NULL if the deque is empty or the element at the rear of the deque.
void *
dql_peek_rear(DequeList_t *deque)
{
    if (dql_empty(deque))
        return NULL;

    return deque->rear->data;
}

/// Returns true if the deque is empty, or false if there are elements in the
/// deque.
/// \par Interface Requirements
/// - None
///
/// \param[in] deque The target deque.
///
/// \return True if the deque is empty, otherwise false.
bool
dql_empty(DequeList_t *deque)
{
    return deque->count == 0;
}

/// Returns true if the deque is full or false otherwise. The deque can only be
/// full if its limit is set to a value higher than 0 and respecting all the
/// rules from dql_set_limit().
/// \par Interface Requirements
/// - None
///
/// \param[in] deque The target deque.
///
/// \return True if the amount of elements in the deque have reached a limit.
bool
dql_full(DequeList_t *deque)
{
    return deque->limit > 0 && deque->count >= deque->limit;
}

/// Returns true if the specified size will fit in the deque before it reaches
/// its limit (if the limit is set).
/// \par Interface Requirements
/// - None
///
/// \param[in] deque The target deque.
/// \param[in] size The specified size.
///
/// \return True if a given size fits inside the deque if it has a limit.
bool
dql_fits(DequeList_t *deque, unsigned_t size)
{
    if (deque->limit <= 0)
        return true;

    return (deque->count + size) <= deque->limit;
}

/// Returns true if the element is present in the deque, otherwise false.
/// \par Interface Requirements
/// - compare
///
/// \param[in] deque DequeList_s reference.
/// \param[in] key Key to be matched.
///
/// \return True if the element is present in the deque, otherwise false.
bool
dql_contains(DequeList_t *deque, void *key)
{
    DequeListNode_t *scan = deque->front;

    while (scan != NULL)
    {
        if (deque->interface->compare(scan->data, key) == 0)
            return true;

        scan = scan->next;
    }

    return false;
}

/// Returns a copy of the specified DequeList_s with the same interface. All
/// elements are copied using the deque interface's copy function.
/// \par Interface Requirements
/// - copy
/// - free
///
/// \param[in] deque The deque to be copied.
///
/// \return NULL if allocation failed or a copy of the specified deque.
DequeList_t *
dql_copy(DequeList_t *deque)
{
    DequeList_t *result = dql_new(deque->interface);

    if (!result)
        return NULL;

    result->limit = deque->limit;

    DequeListNode_t *scan = deque->front;

    while (scan != NULL)
    {
        void *element = deque->interface->copy(scan->data);
        DequeListNode_t *copy = dql_new_node(element);

        if (!copy)
        {
            deque->interface->free(element);
            return false;
        }

        if (result->front == NULL)
        {
            result->front = copy;
            result->rear = copy;
        }
        else
        {
            copy->next = result->rear;

            result->rear->prev = copy;
            result->rear = copy;
        }

        scan = scan->next;
    }

    result->count = deque->count;

    return result;
}

/// Creates a shallow copy of all elements in the deque, that is, only the
/// pointers addresses are copied to the new deque.
/// \par Interface Requirements
/// - None
///
/// \param[in] deque The deque to be copied.
///
/// \return NULL if allocation failed or a shallow copy of the specified deque.
DequeList_t *
dql_copy_shallow(DequeList_t *deque)
{
    DequeList_t *result = dql_new(deque->interface);

    if (!result)
        return NULL;

    result->limit = deque->limit;

    DequeListNode_t *scan = deque->front;

    while (scan != NULL)
    {
        DequeListNode_t *copy = dql_new_node(scan->data);

        if (!copy)
            return false;

        if (result->front == NULL)
        {
            result->front = copy;
            result->rear = copy;
        }
        else
        {
            copy->next = result->rear;

            result->rear->prev = copy;
            result->rear = copy;
        }

        scan = scan->next;
    }

    result->count = deque->count;

    return result;
}

/// Makes a comparison between two deques element by element. If one deque has
/// less elements than the other the comparison of elements will go up until
/// one deque reaches its limit. If all elements are the same until then, the
/// tie breaker goes to their element count. If it is also the same, then both
/// deques are equal.
/// \par Interface Requirements
/// - compare
///
/// \param[in] deque1 A target deque to be compared.
/// \param[in] deque2 A target deque to be compared.
///
/// \return An int according to \ref compare_f.
int
dql_compare(DequeList_t *deque1, DequeList_t *deque2)
{
    DequeListNode_t *scan1 = deque1->front, *scan2 = deque2->front;

    int comparison = 0;
    while (scan1 != NULL && scan2 != NULL)
    {
        comparison = deque1->interface->compare(scan1->data, scan2->data);
        if (comparison > 0)
            return 1;
        else if (comparison < 0)
            return -1;

        scan1 = scan1->prev;
        scan2 = scan2->prev;
    }

    // So far all elements were the same
    if (deque1->count > deque2->count)
        return 1;
    else if (deque1->count < deque2->count)
        return -1;

    return 0;
}

/// Appends \c deque2 at the rear of \c deque1, emptying deque2. Both deques
/// need to have been initialized.
/// \par Interface Requirements
/// - None
///
/// \param[in] deque1 Deque to receive elements.
/// \param[in] deque2 Deque where the elements are going to be taken from.
///
/// \return True if all operations were successful, otherwise false.
bool
dql_append(DequeList_t *deque1, DequeList_t *deque2)
{
    /// \todo dql_append
    return true;
}

/// Prepends \c deque2 at the front of \c deque1, emptying deque2. Both deques
/// need to have been initialized.
/// \par Interface Requirements
/// - None
///
/// \param[in] deque1 Deque to receive elements.
/// \param[in] deque2 Deque where the elements are going to be taken from.
///
/// \return True if all operations were successful, otherwise false.
bool
dql_prepend(DequeList_t *deque1, DequeList_t *deque2)
{
    /// \todo dql_prepend
    return true;
}

/// Makes a copy of all the elements in the deque to a C array starting from
/// the front element to the rear element.
/// \par Interface Requirements
/// - copy
///
/// \param[in] deque The deque to be copied to the array.
/// \param[out] length The resulting array's length.
///
/// \return The resulting array or NULL if the deque is empty or the array
/// allocation failed.
void **
dql_to_array(DequeList_t *deque, integer_t *length)
{
    *length = 0;

    if (dql_empty(deque))
        return NULL;

    void **array = malloc(sizeof(void*) * (size_t)deque->count);

    if (!array)
        return NULL;

    DequeListNode_t *scan = deque->front;

    integer_t i = 0;
    while (scan != NULL)
    {
        array[i++] = deque->interface->copy(scan->data);
        scan = scan->prev;
    }

    *length = deque->count;

    return array;
}

/// Displays a DequeList_s in the console starting from the front element to
/// the rear element. There are currently four modes:
/// - -1 Displays each element separated by newline;
/// -  0 Displays each element like a linked list;
/// -  1 Displays each element separated by a space;
/// - Any other number defaults to the array representation.
/// \par Interface Requirements
/// - display
///
/// \param[in] deque The deque to be displayed in the console.
/// \param[in] display_mode How the deque is to be displayed in the console.
void
dql_display(DequeList_t *deque, int display_mode)
{
    if (dql_empty(deque))
    {
        printf("\nDequeList\n[ empty ]\n");
        return;
    }

    DequeListNode_t *scan = deque->front;

    switch (display_mode)
    {
        case -1:
            printf("\nDequeList\n");
            while (scan != NULL)
            {
                deque->interface->display(scan->data);
                printf("\n");
                scan = scan->prev;
            }
            break;
        case 0:
            printf("\nDequeList\nFront -> ");
            while (scan->prev != NULL)
            {
                deque->interface->display(scan->data);
                printf(" -> ");
                scan = scan->prev;
            }
            deque->interface->display(scan->data);
            printf(" Rear\n");
            break;
        case 1:
            printf("\nDequeList\n");
            while (scan != NULL)
            {
                deque->interface->display(scan->data);
                printf(" ");
                scan = scan->prev;
            }
            printf("\n");
            break;
        default:
            printf("\nDequeList\n[ ");
            while (scan->prev != NULL)
            {
                deque->interface->display(scan->data);
                printf(", ");
                scan = scan->prev;
            }
            deque->interface->display(scan->data);
            printf(" ]\n");
            break;
    }
}

///////////////////////////////////////////////////// NOT EXPOSED FUNCTIONS ///

static DequeListNode_t *
dql_new_node(void *element)
{
    DequeListNode_t *node = malloc(sizeof(DequeListNode_t));

    if (!node)
        return NULL;

    node->data = element;
    node->prev = NULL;
    node->next = NULL;

    return node;
}

static void
dql_free_node(DequeListNode_t *node, free_f function)
{
    function(node->data);
    free(node);
}

static void
dql_free_node_shallow(DequeListNode_t *node)
{
    free(node);
}

////////////////////////////////////////////// END OF NOT EXPOSED FUNCTIONS ///

///////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////// Iterator ///
///////////////////////////////////////////////////////////////////////////////

/// This is a DequeList_s iterator and its cursor is represented by pointing to
/// the current node in the deque.
struct DequeListIterator_s
{
    /// \brief Target DequeList_s.
    ///
    /// Target DequeList_s. The iterator might need to use some information
    /// provided by the deque or change some of its data members.
    struct DequeList_s *target;

    /// \brief Current element.
    ///
    /// Points to the current node. The iterator is always initialized with the
    /// cursor pointing to the start (front) pointer.
    struct DequeListNode_s *cursor;

    /// \brief Target version ID.
    ///
    /// When the iterator is initialized it stores the version_id of the target
    /// structure. This is kept to prevent iteration on the target structure
    /// that may have been modified and thus causing undefined behaviours or
    /// run-time crashes.
    integer_t target_id;
};

///////////////////////////////////////////////////// NOT EXPOSED FUNCTIONS ///

static bool
dql_iter_target_modified(DequeListIterator_t *iter);

////////////////////////////////////////////// END OF NOT EXPOSED FUNCTIONS ///

///
/// \param[in] target
///
/// \return
DequeListIterator_t *
dql_iter_new(DequeList_t *target)
{
    if (dql_empty(target))
        return NULL;

    DequeListIterator_t *iter = malloc(sizeof(DequeListIterator_t));

    if (!iter)
        return NULL;

    iter->target = target;
    iter->target_id = target->version_id;
    iter->cursor = target->front;

    return iter;
}

///
/// \param[in] iter
/// \param[in] target
void
dql_iter_retarget(DequeListIterator_t *iter, DequeList_t *target)
{
    iter->target = target;
    iter->target_id = target->version_id;
}

///
/// \param[in] iter
void
dql_iter_free(DequeListIterator_t *iter)
{
    free(iter);
}

///
/// \param[in] iter
///
/// \return
bool
dql_iter_next(DequeListIterator_t *iter)
{
    if (dql_iter_target_modified(iter))
        return false;

    if (!dql_iter_has_next(iter))
        return false;

    iter->cursor = iter->cursor->next;

    return true;
}

///
/// \param[in] iter
///
/// \return
bool
dql_iter_prev(DequeListIterator_t *iter)
{
    if (dql_iter_target_modified(iter))
        return false;

    if (!dql_iter_has_prev(iter))
        return false;

    iter->cursor = iter->cursor->prev;

    return true;
}

///
/// \param[in] iter
///
/// \return
bool
dql_iter_to_front(DequeListIterator_t *iter)
{
    if (dql_iter_target_modified(iter))
        return false;

    iter->cursor = iter->target->front;

    return true;
}

///
/// \param[in] iter
///
/// \return
bool
dql_iter_to_rear(DequeListIterator_t *iter)
{
    if (dql_iter_target_modified(iter))
        return false;

    iter->cursor = iter->target->rear;

    return true;
}

///
/// \param[in] iter
///
/// \return
bool
dql_iter_has_next(DequeListIterator_t *iter)
{
    return iter->cursor->next != NULL;
}

///
/// \param[in] iter
///
/// \return
bool
dql_iter_has_prev(DequeListIterator_t *iter)
{
    return iter->cursor->prev != NULL;
}

///
/// \param[in] iter
/// \param[in] result
///
/// \return
bool
dql_iter_get(DequeListIterator_t *iter, void **result)
{
    if (dql_iter_target_modified(iter))
        return false;

    *result = iter->cursor->data;

    return true;
}

///
/// \param[in] iter
/// \param[in] element
///
/// \return
bool
dql_iter_set(DequeListIterator_t *iter, void *element)
{
    if (dql_iter_target_modified(iter))
        return false;

    iter->target->interface->free(iter->cursor->data);

    iter->cursor->data = element;

    return true;
}

///
/// \param[in] iter
///
/// \return
void *
dql_iter_peek_next(DequeListIterator_t *iter)
{
    if (dql_iter_target_modified(iter))
        return NULL;

    if (!dql_iter_has_next(iter))
        return NULL;

    return iter->cursor->next->data;
}

///
/// \param[in] iter
///
/// \return
void *
dql_iter_peek(DequeListIterator_t *iter)
{
    if (dql_iter_target_modified(iter))
        return NULL;

    return iter->cursor->data;
}

///
/// \param[in] iter
///
/// \return
void *
dql_iter_peek_prev(DequeListIterator_t *iter)
{
    if (dql_iter_target_modified(iter))
        return NULL;

    if (!dql_iter_has_prev(iter))
        return NULL;

    return iter->cursor->prev->data;
}

///////////////////////////////////////////////////// NOT EXPOSED FUNCTIONS ///

static bool
dql_iter_target_modified(DequeListIterator_t *iter)
{
    return iter->target_id != iter->target->version_id;
}

////////////////////////////////////////////// END OF NOT EXPOSED FUNCTIONS ///

///////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////// Wrapper ///
///////////////////////////////////////////////////////////////////////////////

/// \todo DequeListWrapper
