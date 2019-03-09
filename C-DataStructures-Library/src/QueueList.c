/**
 * @file QueueList.c
 * 
 * @author Leonardo Vencovsky (https://github.com/LeoVen)
 *
 * @date 26/09/2018
 */

#include "QueueList.h"

/// This is a linked list implementation of a Queue with FIFO (First-in
/// First-out) or LILO (Last-in Last-out) operations, so the first item added
/// is the first one to be removed. It is implemented as a SinglyLinkedList_s
/// but with restricted operations to preserve the FIFO (LILO) order of
/// elements. The function \c qli_enqueue() is equivalent to \c
/// sll_insert_tail() and the function \c qli_dequeue() is equivalent to
/// \c sll_remove_head(). This is done in such a way that removal and
/// insertions are O(1) without the need of a second pointer to the previous
///  element (like a Deque implemented with a DoublyLinkedList).
///
/// To initialize the queue use qli_init(). This only initializes the structure.
/// If you don't set the default functions later you won't be able to do
/// certain operations. If you want to initialize it completely, use instead
/// qli_create(). Here you must pass in default functions (compare, copy,
/// display and free) according with the specifications of each type of
/// function. You can also use qli_free_shallow() that will only free the queue
/// structure.
///
/// To insert elements in the queue use qli_enqueue() and it is equivalent to
/// inserting an element at the tail of a linked list. To remove an element use
/// qli_dequeue() and it is equivalent to removing and element at the head of
/// a linked list.
///
/// To delete a queue use qli_free(). This completely frees all elements and
/// sets the queue pointers to \c NULL. Note that if you haven't set a default
/// free function you won't be able to delete the queue or its elements. You
/// must set a free function that will be responsible for freeing from memory
/// all elements. You can also use qli_free_shallow() that will only free the
/// queue structure. If you simply want to erase all its contents use
/// qli_erase(). This will keep all default functions and all elements will be
/// removed from the queue and freed from memory.
///
/// The queue maintains a version id that keeps track of structural changes
/// done to the queue. This prevents any iterators from working the moment the
/// queue structure is changed. It works to prevent any undefined behaviour or
/// run-time errors.
///
/// \b Advantages over QueueArray_s
/// - Indefinitely grows
/// - No need to reallocate buffers or shift elements
///
/// \b Drawbacks
/// - No random access
/// - More memory usage as in every node there is a pointer to the next node
///
/// \par Functions
/// Located in the file QueueList.c
struct QueueList_s
{
    /// \brief Current amount of elements in the \c QueueList.
    ///
    /// QueueList current amount of elements linked between the \c front and
    /// \c rear pointers.
    integer_t length;

    /// \brief QueueList length limit.
    ///
    /// If it is set to 0 or a negative value then the queue has no limit to
    /// its length. Otherwise it won't be able to have more elements than the
    /// specified value. The queue is always initialized with no restrictions
    /// to its length, that is, \c limit equals 0. The user won't be able to
    /// limit the queue length if it already has more elements than the
    /// specified limit.
    integer_t limit;

    /// \brief The front of the queue.
    ///
    /// Where elements are removed from. The function \c qli_dequeue() operates
    /// relative to this pointer.
    struct QueueListNode_s *front;

    /// \brief The rear of the queue.
    ///
    /// Where elements are added. The function \c qli_enqueue() operates
    /// relative to this pointer.
    struct QueueListNode_s *rear;

    /// \brief Comparator function.
    ///
    /// A function that compares one element with another that returns an int
    /// with the following rules:
    ///
    /// - <code>[ > 0 ]</code> if first element is greater than the second;
    /// - <code>[ < 0 ]</code> if second element is greater than the first;
    /// - <code>[ 0 ]</code> if elements are equal.
    qli_compare_f v_compare;

    /// \brief Copy function.
    ///
    /// A function that returns an exact copy of an element.
    qli_copy_f v_copy;

    /// \brief Display function.
    ///
    /// A function that displays an element in the console. Useful for
    /// debugging.
    qli_display_f v_display;

    /// \brief Deallocator function.
    ///
    /// A function that completely frees an element from memory.
    qli_free_f v_free;

    /// \brief A version id to keep track of modifications.
    ///
    /// This version id is used by the iterator to check if the structure was
    /// modified. The iterator can only function if its version_id is the same
    /// as the structure's version id, that is, there have been no structural
    /// modifications (except for those done by the iterator itself).
    integer_t version_id;
};

/// \brief A QueueList_s node.
///
/// Implementation detail. This is a singly-linked node. It has one data member
/// and one pointer to the previous node or \c NULL if it is the last node
/// added to the queue.
struct QueueListNode_s
{
    /// \brief Data pointer.
    ///
    /// Points to node's data. The data needs to be dynamically allocated.
    void *data;

    /// \brief Previous element in the queue.
    ///
    /// Points to the previous element in the queue or \c NULL if this was the
    /// last one enqueued.
    struct QueueListNode_s *prev;
};

/// \brief A type for a queue node.
///
/// Defines a type to a <code> struct QueueListNode_s </code>.
typedef struct QueueListNode_s QueueListNode_t;

/// \brief A pointer type for a queue node.
///
/// Defines a pointer type to a <code> struct QueueListNode_s </code>.
typedef struct QueueListNode_s *QueueListNode;

///////////////////////////////////////////////////// NOT EXPOSED FUNCTIONS ///

static Status qli_make_node(QueueListNode *node, void *element);

static Status qli_free_node(QueueListNode *node, qli_free_f free_f);

static Status qli_free_node_shallow(QueueListNode *node);

////////////////////////////////////////////// END OF NOT EXPOSED FUNCTIONS ///

/// \brief Initializes a QueueList_s structure.
///
/// Initializes a new QueueList_s structure with initial length and limit as 0.
/// Note that it does not sets any default functions. If you don't set them
/// later you won't be able to do certain operations that depend on a user-
/// defined function.
///
/// \param[in,out] queue The queue to be initialized.
///
/// \return DS_ERR_ALLOC if queue allocation failed.
/// \return DS_OK if all operations were successful.
Status qli_init(QueueList *queue)
{
    *queue = malloc(sizeof(QueueList_t));

    if (!(*queue))
        return DS_ERR_ALLOC;

    (*queue)->length = 0;
    (*queue)->limit = 0;
    (*queue)->version_id = 0;

    (*queue)->front = NULL;
    (*queue)->rear = NULL;

    (*queue)->v_compare = NULL;
    (*queue)->v_copy = NULL;
    (*queue)->v_display = NULL;
    (*queue)->v_free = NULL;

    return DS_OK;
}

/// \brief Creates a QueueList_s.
///
/// This function completely creates a QueueList_s, setting all of its default
/// functions.
///
/// \param[in,out] queue QueueList_s to be allocated.
/// \param[in] compare_f A function that compares two elements.
/// \param[in] copy_f A function that makes an exact copy of an element.
/// \param[in] display_f A function that displays in the console an element.
/// \param[in] free_f A function that completely frees from memory an element.
///
/// \return DS_ERR_ALLOC if queue allocation failed.
/// \return DS_OK if all operations are successful.
Status qli_create(QueueList *queue, qli_compare_f compare_f, qli_copy_f copy_f,
        qli_display_f display_f, qli_free_f free_f)
{
    *queue = malloc(sizeof(QueueList_t));

    if (!(*queue))
        return DS_ERR_ALLOC;

    (*queue)->length = 0;
    (*queue)->limit = 0;
    (*queue)->version_id = 0;

    (*queue)->front = NULL;
    (*queue)->rear = NULL;

    (*queue)->v_compare = compare_f;
    (*queue)->v_copy = copy_f;
    (*queue)->v_display = display_f;
    (*queue)->v_free = free_f;

    return DS_OK;
}

/// \brief Frees from memory a QueueList_s and all its elements.
///
/// This function frees from memory all the queue's elements using its default
/// free function and then frees the queue's structure. The variable is then
/// set to \c NULL.
///
/// \param queue The QueueList_s to be freed from memory.
///
/// \return DS_ERR_INCOMPLETE_TYPE if a default free function is not set.
/// \return DS_ERR_NULL_POINTER if the queue references to \c NULL.
/// \return DS_OK if all operations were successful.
Status qli_free(QueueList *queue)
{
    if ((*queue) == NULL)
        return DS_ERR_NULL_POINTER;

    if ((*queue)->v_free == NULL)
        return DS_ERR_INCOMPLETE_TYPE;

    QueueListNode prev = (*queue)->front;

    Status st;

    while ((*queue)->front != NULL)
    {
        (*queue)->front = (*queue)->front->prev;

        st = qli_free_node(&prev, (*queue)->v_free);

        if (st != DS_OK)
            return st;

        prev = (*queue)->front;
    }

    free((*queue));

    (*queue) = NULL;

    return DS_OK;
}

/// \brief Frees from memory a QueueList_s.
///
/// This function frees from memory all the queue's nodes without freeing its
/// data and then frees the queue structure. The variable is then set to
/// \c NULL.
///
/// \param[in,out] queue QueueList_s to be freed from memory.
///
/// \return DS_ERR_NULL_POINTER if the queue references to \c NULL.
/// \return DS_OK if all operations are successful.
Status qli_free_shallow(QueueList *queue)
{
    if ((*queue) == NULL)
        return DS_ERR_NULL_POINTER;

    if ((*queue)->v_free == NULL)
        return DS_ERR_INCOMPLETE_TYPE;

    QueueListNode prev = (*queue)->front;

    Status st;

    while ((*queue)->front != NULL)
    {
        (*queue)->front = (*queue)->front->prev;

        st = qli_free_node_shallow(&prev);

        if (st != DS_OK)
            return st;

        prev = (*queue)->front;
    }

    free((*queue));

    (*queue) = NULL;

    return DS_OK;
}

/// \brief Erases a QueueList_s.
///
/// This function is equivalent to freeing a queue and the creating it again.
/// This will reset the queue to its initial state with no elements, but will
/// keep all of its default functions.
///
/// \param[in,out] queue QueueList_s to be erased.
///
/// \return DS_ERR_ALLOC if queue allocation failed.
/// \return DS_ERR_INCOMPLETE_TYPE if a default free function is not set.
/// \return DS_ERR_NULL_POINTER if the queue references to \c NULL.
/// \return DS_OK if all operations were successful.
Status qli_erase(QueueList *queue)
{
    if (*queue == NULL)
        return DS_ERR_NULL_POINTER;

    QueueList new_queue;

    Status st = qli_create(&new_queue, (*queue)->v_compare, (*queue)->v_copy,
            (*queue)->v_display, (*queue)->v_free);

    if (st !=  DS_OK)
        return st;

    st = qli_free(queue);

    // Probably didn't set the free function...
    if (st !=  DS_OK)
    {
        free(new_queue);

        return st;
    }

    *queue = new_queue;

    return DS_OK;
}

/// \brief Sets the default compare function.
///
/// Use this function to set a default compare function. It needs to comply
/// with the qli_compare_f specifications.
///
/// \param[in] queue QueueList_s to set the default compare function.
/// \param[in] function A qli_compare_f kind of function.
///
/// \return DS_ERR_NULL_POINTER if the queue references to \c NULL.
/// \return DS_OK if all operations are successful.
Status qli_set_v_compare(QueueList queue, qli_compare_f function)
{
    if (queue == NULL)
        return DS_ERR_NULL_POINTER;

    queue->v_compare = function;

    return DS_OK;
}

/// \brief Sets the default copy function.
///
/// Use this function to set a default compare function. It needs to comply
/// with the qli_copy_f specifications.
///
/// \param[in] queue QueueList_s to set the default copy function.
/// \param[in] function A qli_copy_f kind of function.
///
/// \return DS_ERR_NULL_POINTER if the queue references to \c NULL.
/// \return DS_OK if all operations are successful.
Status qli_set_v_copy(QueueList queue, qli_copy_f function)
{
    if (queue == NULL)
        return DS_ERR_NULL_POINTER;

    queue->v_copy = function;

    return DS_OK;
}

/// \brief Sets the default display function.
///
/// Use this function to set a default display function. It needs to comply
/// with the qli_display_f specifications. Useful for debugging.
///
/// \param[in] queue QueueList_s to set the default display function.
/// \param[in] function A qli_display_f kind of function.
///
/// \return DS_ERR_NULL_POINTER if the queue references to \c NULL.
/// \return DS_OK if all operations are successful.
Status qli_set_v_display(QueueList queue, qli_display_f function)
{
    if (queue == NULL)
        return DS_ERR_NULL_POINTER;

    queue->v_display = function;

    return DS_OK;
}

/// \brief Sets the default free function.
///
/// Use this function to set a default free function. It needs to comply
/// with the qli_free_f specifications.
///
/// \param[in] queue QueueList_s to set the default free function.
/// \param[in] function A qli_free_f kind of function.
///
/// \return DS_ERR_NULL_POINTER if the queue references to \c NULL.
/// \return DS_OK if all operations are successful.
Status qli_set_v_free(QueueList queue, qli_free_f function)
{
    if (queue == NULL)
        return DS_ERR_NULL_POINTER;

    queue->v_free = function;

    return DS_OK;
}

/// \brief Sets a limit to the specified QueueList_s's length.
///
/// Limit's the QueueList_s's length. You can only set a limit greater or equal to
/// the queue's current length and greater than 0. To remove this limitation
/// simply set the limit to 0 or less.
///
/// \param[in] queue QueueList_s reference.
/// \param[in] limit Maximum queue length.
///
/// \return DS_ERR_INVALID_OPERATION if the limitation is less than the queue's
/// current length.
/// \return DS_ERR_NULL_POINTER if the queue references to \c NULL.
/// \return DS_OK if all operations are successful.
Status qli_set_limit(QueueList queue, integer_t limit)
{
    if (queue == NULL)
        return DS_ERR_NULL_POINTER;

    // The new limit can't be lower than the queue's current length.
    if (queue->length > limit && limit > 0)
        return DS_ERR_INVALID_OPERATION;

    queue->limit = limit;

    return DS_OK;
}

integer_t qli_length(QueueList queue)
{
    if (queue == NULL)
        return -1;

    return queue->length;
}

integer_t qli_limit(QueueList queue)
{
    if (queue == NULL)
        return -1;

    return queue->limit;
}

/// Inserts an element into the specified queue. The element is added relative
/// to the \c rear pointer.
///
/// \param[in] queue The queue where the element is to be inserted.
/// \param[in] element The element to be inserted in the queue.
///
/// \return DS_ERR_ALLOC if node allocation failed.
/// \return DS_ERR_FULL if \c limit is set (different than 0) and the queue
/// length reached the specified limit.
/// \return DS_ERR_NULL_POINTER if the queue references to \c NULL.
/// \return DS_OK if all operations were successful.
Status qli_enqueue(QueueList queue, void *element)
{
    if (queue == NULL)
        return DS_ERR_NULL_POINTER;

    if (qli_full(queue))
        return DS_ERR_FULL;

    QueueListNode node;

    Status st = qli_make_node(&node, element);

    if (st != DS_OK)
        return st;

    if (qli_empty(queue))
    {
        queue->rear = node;
        queue->front = node;
    }
    else
    {
        queue->rear->prev = node;
        queue->rear = node;
    }

    queue->length++;
    queue->version_id++;

    return DS_OK;
}

/// Removes an element from the specified queue. The element is removed
/// relative to the \c front pointer.
///
/// \param[in] queue The queue where the element is to be removed from.
/// \param[out] result The resulting element removed from the queue.
///
/// \return DS_ERR_INVALID_OPERATION if the queue is empty.
/// \return DS_ERR_NULL_POINTER if the queue references to \c NULL.
/// \return DS_OK if all operations were successful.
Status qli_dequeue(QueueList queue, void **result)
{
    *result = NULL;

    if (queue == NULL)
        return DS_ERR_NULL_POINTER;

    if (qli_empty(queue))
        return DS_ERR_INVALID_OPERATION;

    QueueListNode node = queue->front;

    *result = node->data;

    queue->front = queue->front->prev;

    Status st = qli_free_node_shallow(&node);

    if (st != DS_OK)
        return st;

    queue->length--;
    queue->version_id++;

    if (qli_empty(queue))
    {
        queue->front = NULL;
        queue->rear = NULL;
    }

    return DS_OK;
}

bool qli_full(QueueList queue)
{
    return queue->limit > 0 && queue->length >= queue->limit;
}

bool qli_empty(QueueList queue)
{
    return queue->length == 0;
}

void *qli_peek_front(QueueList queue)
{
    if (queue == NULL)
        return NULL;

    if (qli_empty(queue))
        return NULL;

    return queue->front->data;
}

void *qli_peek_rear(QueueList queue)
{
    if (queue == NULL)
        return NULL;

    if (qli_empty(queue))
        return NULL;

    return queue->rear->data;
}

bool qli_contains(QueueList queue, void *key)
{
    // TODO
    return false;
}

Status qli_copy(QueueList queue, QueueList *result)
{
    *result = NULL;

    if (queue == NULL)
        return DS_ERR_NULL_POINTER;

    if (queue->v_copy == NULL || queue->v_free == NULL)
        return DS_ERR_INCOMPLETE_TYPE;

    Status st = qli_create(result, queue->v_compare, queue->v_copy,
                           queue->v_display, queue->v_free);

    if (st != DS_OK)
        return st;

    (*result)->limit = queue->limit;

    if (qli_empty(queue))
        return DS_OK;

    QueueListNode scan = queue->front;

    void *elem;

    while (scan != NULL)
    {
        elem = queue->v_copy(scan->data);

        st = qli_enqueue(*result, elem);

        if (st != DS_OK)
        {
            queue->v_free(elem);

            return st;
        }

        scan = scan->prev;
    }

    return DS_OK;
}

Status qli_append(QueueList queue, QueueList queue2)
{
    // TODO
    return DS_OK;
}

Status qli_to_array(QueueList queue,  void ***result, integer_t *length)
{
    // TODO
    return DS_ERR_INVALID_OPERATION;
}

/// \brief Displays a QueueList_s in the console.
///
/// Displays a QueueList_s in the console starting from \c front to \c rear.
///
/// \param[in] queue The QueueList_s to be displayed in the console.
///
/// \return DS_ERR_INCOMPLETE_TYPE if a default display function is not set.
/// \return DS_ERR_NULL_POINTER if the queue references to \c NULL.
/// \return DS_OK if all operations were successful.
Status qli_display(QueueList queue)
{
    if (queue == NULL)
        return DS_ERR_NULL_POINTER;

    if (queue->v_display == NULL)
        return DS_ERR_INCOMPLETE_TYPE;

    if (qli_empty(queue))
    {
        printf("\nQueueList\n[ empty ]\n");

        return DS_OK;
    }

    QueueListNode scan = queue->front;

    printf("\nQueueList\nfront <-");

    while (scan != NULL)
    {
        queue->v_display(scan->data);

        printf(" <- ");

        scan = scan->prev;
    }

    printf(" rear\n");

    return DS_OK;
}

/// \brief Displays a QueueList_s in the console like an array.
///
/// Displays a QueueList_s in the console starting from \c front to \c rear like an
/// array with its elements separated by commas, delimited with brackets.
///
/// \param[in] queue The QueueList_s to be displayed in the console.
///
/// \return DS_ERR_INCOMPLETE_TYPE if a default display function is not set.
/// \return DS_ERR_NULL_POINTER if the queue references to \c NULL.
/// \return DS_OK if all operations were successful.
Status qli_display_array(QueueList queue)
{
    if (queue == NULL)
        return DS_ERR_NULL_POINTER;

    if (queue->v_display == NULL)
        return DS_ERR_INCOMPLETE_TYPE;

    if (qli_empty(queue))
    {
        printf("\n[ empty ]\n");

        return DS_OK;
    }

    QueueListNode scan = queue->front;

    printf("\n[ ");

    while (scan->prev != NULL)
    {
        queue->v_display(scan->data);

        printf(", ");

        scan = scan->prev;
    }

    queue->v_display(scan->data);

    printf(" ]\n");

    return DS_OK;
}

/// \brief Displays a QueueList_s in the console.
///
/// Displays a QueueList_s in the console starting from \c front to \c rear with
/// its elements separated by spaces.
///
/// \param[in] queue The QueueList_s to be displayed in the console.
///
/// \return DS_ERR_INCOMPLETE_TYPE if a default display function is not set.
/// \return DS_ERR_NULL_POINTER if the queue references to \c NULL.
/// \return DS_OK if all operations were successful.
Status qli_display_raw(QueueList queue)
{
    if (queue == NULL)
        return DS_ERR_NULL_POINTER;

    if (queue->v_display == NULL)
        return DS_ERR_INCOMPLETE_TYPE;

    printf("\n");

    if (qli_empty(queue))
        return DS_OK;

    QueueListNode scan = queue->front;

    while (scan != NULL)
    {
        queue->v_display(scan->data);

        printf(" ");

        scan = scan->prev;
    }

    printf("\n");

    return DS_OK;
}

///////////////////////////////////////////////////// NOT EXPOSED FUNCTIONS ///

static Status qli_make_node(QueueListNode *node, void *element)
{
    (*node) = malloc(sizeof(QueueListNode_t));

    if (!(*node))
        return DS_ERR_ALLOC;

    (*node)->data = element;

    (*node)->prev = NULL;

    return DS_OK;
}

static Status qli_free_node(QueueListNode *node, qli_free_f free_f)
{
    if ((*node) == NULL)
        return DS_ERR_NULL_POINTER;

    free_f((*node)->data);

    free(*node);

    (*node) = NULL;

    return DS_OK;
}

static Status qli_free_node_shallow(QueueListNode *node)
{
    if (*node == NULL)
        return DS_ERR_NULL_POINTER;

    free(*node);

    *node = NULL;

    return DS_OK;
}

////////////////////////////////////////////// END OF NOT EXPOSED FUNCTIONS ///

///////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////// Iterator ///
///////////////////////////////////////////////////////////////////////////////

/// \brief An iterator for a QueueList_s.
///
/// This iterator is a forward-only iterator.
struct QueueListIterator_s
{
    /// \brief Target QueueList_s.
    ///
    /// Target QueueList_s. The iterator might need to use some information
    /// provided by the queue or change some of its data members.
    struct QueueList_s *target;

    /// \brief Current element.
    ///
    /// Points to the current node. The iterator is always initialized with the
    /// cursor pointing to the start (front) of the queue.
    struct QueueListNode_s *cursor;

    /// \brief Target version ID.
    ///
    /// When the iterator is initialized it stores the version_id of the target
    /// structure. This is kept to prevent iteration on the target structure
    /// that may have been modified and thus causing undefined behaviours or
    /// run-time crashes.
    integer_t target_id;
};

///////////////////////////////////////////////////// NOT EXPOSED FUNCTIONS ///

static bool qli_iter_target_modified(QueueListIterator iter);

static bool qli_iter_invalid_state(QueueListIterator iter);

////////////////////////////////////////////// END OF NOT EXPOSED FUNCTIONS ///

Status qli_iter_init(QueueListIterator *iter, QueueList target)
{
    *iter = malloc(sizeof(QueueListIterator_t));

    if (!(*iter))
        return DS_ERR_ALLOC;

    (*iter)->target = target;
    (*iter)->target_id = target->version_id;
    (*iter)->cursor = target->front;

    return DS_OK;
}

Status qli_iter_retarget(QueueListIterator *iter, QueueList target)
{
    Status st = qli_iter_free(iter);

    if (st != DS_OK)
        return st;

    st = qli_iter_init(iter, target);

    if (st != DS_OK)
        return st;

    return DS_OK;
}

Status qli_iter_free(QueueListIterator *iter)
{
    if (*iter == NULL)
        return DS_ERR_NULL_POINTER;

    free(*iter);

    *iter = NULL;

    return DS_OK;
}

Status qli_iter_next(QueueListIterator iter)
{
    if (iter == NULL)
        return DS_ERR_NULL_POINTER;

    if (qli_iter_invalid_state(iter))
        return DS_ERR_ITER_STATE;

    if (qli_iter_target_modified(iter))
        return DS_ERR_ITER_MODIFICATION;

    if (!qli_iter_has_next(iter))
        return DS_ERR_ITER;

    iter->cursor = iter->cursor->prev;

    return DS_OK;
}

Status qli_iter_to_front(QueueListIterator iter)
{
    if (iter == NULL)
        return DS_ERR_NULL_POINTER;

    if (qli_iter_invalid_state(iter))
        return DS_ERR_ITER_STATE;

    if (qli_iter_target_modified(iter))
        return DS_ERR_ITER_MODIFICATION;

    if (!qli_iter_has_next(iter))
        return DS_ERR_ITER;

    iter->cursor = iter->target->front;

    return DS_OK;
}

Status qli_iter_to_rear(QueueListIterator iter)
{
    if (iter == NULL)
        return DS_ERR_NULL_POINTER;

    if (qli_iter_invalid_state(iter))
        return DS_ERR_ITER_STATE;

    if (qli_iter_target_modified(iter))
        return DS_ERR_ITER_MODIFICATION;

    if (!qli_iter_has_next(iter))
        return DS_ERR_ITER;

    iter->cursor = iter->target->rear;

    return DS_OK;
}

bool qli_iter_has_next(QueueListIterator iter)
{
    return iter->cursor->prev != NULL;
}

Status qli_iter_get(QueueListIterator iter, void **result)
{
    if (iter == NULL)
        return DS_ERR_NULL_POINTER;

    if (qli_iter_invalid_state(iter))
        return DS_ERR_ITER_STATE;

    if (qli_iter_target_modified(iter))
        return DS_ERR_ITER_MODIFICATION;

    if (iter->target->v_copy == NULL || iter->target->v_free == NULL)
        return DS_ERR_INCOMPLETE_TYPE;

    *result = iter->target->v_copy(iter->cursor->data);

    return DS_OK;
}

Status qli_iter_set(QueueListIterator iter, void *element)
{
    if (iter == NULL)
        return DS_ERR_NULL_POINTER;

    if (qli_iter_invalid_state(iter))
        return DS_ERR_ITER_STATE;

    if (qli_iter_target_modified(iter))
        return DS_ERR_ITER_MODIFICATION;

    if (iter->target->v_free == NULL)
        return DS_ERR_INCOMPLETE_TYPE;

    iter->target->v_free(iter->cursor->data);

    iter->cursor->data = element;

    return DS_OK;
}

Status qli_iter_insert(QueueListIterator iter, void *element)
{
    if (iter == NULL)
        return DS_ERR_NULL_POINTER;

    if (qli_iter_invalid_state(iter))
        return DS_ERR_ITER_STATE;

    if (qli_iter_target_modified(iter))
        return DS_ERR_ITER_MODIFICATION;

    QueueListNode node;

    Status st = qli_make_node(&node, element);

    if (st != DS_OK)
        return st;

    if (qli_empty(iter->target))
    {
        iter->target->front = node;
        iter->target->rear = node;

        iter->cursor = node;
    }
    else
    {
        node->prev = iter->cursor->prev;

        iter->cursor->prev = node;
    }

    iter->target->length++;

    iter->target->version_id++;
    iter->target_id++;

    return DS_OK;
}

Status qli_iter_remove(QueueListIterator iter, void **result)
{
    if (iter == NULL)
        return DS_ERR_NULL_POINTER;

    if (qli_iter_invalid_state(iter))
        return DS_ERR_ITER_STATE;

    if (qli_iter_target_modified(iter))
        return DS_ERR_ITER_MODIFICATION;

    if (!qli_iter_has_next(iter))
        return DS_ERR_ITER;

    Status st;

    if (qli_length(iter->target) == 1)
    {
        *result = iter->cursor->data;

        st = qli_free_node_shallow(&(iter->cursor));

        if (st != DS_OK)
            return st;

        iter->cursor = NULL;

        iter->target->front = NULL;
        iter->target->rear = NULL;
    }
    else
    {
        QueueListNode node = iter->cursor->prev;

        *result = node->data;

        iter->cursor->prev = node->prev;

        st = qli_free_node_shallow(&node);

        if (st != DS_OK)
            return st;
    }

    iter->target->length--;

    iter->target->version_id++;
    iter->target_id++;

    return DS_OK;
}

void *qli_iter_peek_next(QueueListIterator iter)
{
    if (iter == NULL)
        return NULL;

    if (qli_iter_invalid_state(iter))
        return NULL;

    if (qli_iter_target_modified(iter))
        return NULL;

    if (!qli_iter_has_next(iter))
        return NULL;

    return iter->cursor->prev->data;
}

void *qli_iter_peek(QueueListIterator iter)
{
    if (iter == NULL)
        return NULL;

    if (qli_iter_invalid_state(iter))
        return NULL;

    if (qli_iter_target_modified(iter))
        return NULL;

    return iter->cursor->data;
}

///////////////////////////////////////////////////// NOT EXPOSED FUNCTIONS ///

static bool qli_iter_target_modified(QueueListIterator iter)
{
    return iter->target_id != iter->target->version_id;
}

static bool qli_iter_invalid_state(QueueListIterator iter)
{
    return iter->cursor == NULL || iter->target == NULL;
}

////////////////////////////////////////////// END OF NOT EXPOSED FUNCTIONS ///