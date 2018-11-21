/**
 * @file Queue.c
 * 
 * @author Leonardo Vencovsky (https://github.com/LeoVen)
 *
 * @date 26/09/2018
 */

#include "Queue.h"

/// \brief A linked list implementation of a generic queue.
///
/// This is a linked list implementation of a Queue with FIFO (First-in
/// First-out) or LILO (Last-in Last-out) operations, so the first item added
/// is the first one to be removed. It is implemented as a SinglyLinkedList_s
/// but with restricted operations to preserve the FIFO (LILO) order of
/// elements. The function \c que_enqueue() is equivalent to \c
/// sll_insert_tail() and the function \c que_dequeue() is equivalent to
/// \c sll_remove_head(). This is done in such a way that removal and
/// insertions are O(1) without the need of a second pointer to the previous
///  element (like a Deque implemented with a DoublyLinkedList).
///
/// To initialize the queue use que_init(). This only initializes the structure.
/// If you don't set the default functions later you won't be able to do
/// certain operations. If you want to initialize it completely, use instead
/// que_create(). Here you must pass in default functions (compare, copy,
/// display and free) according with the specifications of each type of
/// function. You can also use que_free_shallow() that will only free the queue
/// structure.
///
/// To insert elements in the queue use que_enqueue() and it is equivalent to
/// inserting an element at the tail of a linked list. To remove an element use
/// que_dequeue() and it is equivalent to removing and element at the head of
/// a linked list.
///
/// To delete a queue use que_free(). This completely frees all elements and
/// sets the queue pointers to \c NULL. Note that if you haven't set a default
/// free function you won't be able to delete the queue or its elements. You
/// must set a free function that will be responsible for freeing from memory
/// all elements. You can also use que_free_shallow() that will only free the
/// queue structure. If you simply want to erase all its contents use
/// que_erase(). This will keep all default functions and all elements will be
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
/// \b Functions
///
/// Located in file Queue.c
struct Queue_s
{
    /// \brief Current amount of elements in the \c Queue.
    ///
    /// Queue current amount of elements linked between the \c front and
    /// \c rear pointers.
    index_t length;

    /// \brief Queue length limit.
    ///
    /// If it is set to 0 or a negative value then the queue has no limit to
    /// its length. Otherwise it won't be able to have more elements than the
    /// specified value. The queue is always initialized with no restrictions
    /// to its length, that is, \c limit equals 0. The user won't be able to
    /// limit the queue length if it already has more elements than the
    /// specified limit.
    index_t limit;

    /// \brief The front of the queue.
    ///
    /// Where elements are removed from. The function \c que_dequeue() operates
    /// relative to this pointer.
    struct QueueNode_s *front;

    /// \brief The rear of the queue.
    ///
    /// Where elements are added. The function \c que_enqueue() operates
    /// relative to this pointer.
    struct QueueNode_s *rear;

    /// \brief Comparator function.
    ///
    /// A function that compares one element with another that returns an int
    /// with the following rules:
    ///
    /// - <code>[ > 0 ]</code> if first element is greater than the second;
    /// - <code>[ < 0 ]</code> if second element is greater than the first;
    /// - <code>[ 0 ]</code> if elements are equal.
    que_compare_f d_compare;

    /// \brief Copy function.
    ///
    /// A function that returns an exact copy of an element.
    que_copy_f d_copy;

    /// \brief Display function.
    ///
    /// A function that displays an element in the console. Useful for
    /// debugging.
    que_display_f d_display;

    /// \brief Deallocator function.
    ///
    /// A function that completely frees an element from memory.
    que_free_f d_free;

    /// \brief A version id to keep track of modifications.
    ///
    /// This version id is used by the iterator to check if the structure was
    /// modified. The iterator can only function if its version_id is the same
    /// as the structure's version id, that is, there have been no structural
    /// modifications (except for those done by the iterator itself).
    index_t version_id;
};

/// \brief A Queue_s node.
///
/// Implementation detail. This is a singly-linked node. It has one data member
/// and one pointer to the previous node or \c NULL if it is the last node
/// added to the queue.
struct QueueNode_s
{
    /// \brief Data pointer.
    ///
    /// Points to node's data. The data needs to be dynamically allocated.
    void *data;

    /// \brief Previous element in the queue.
    ///
    /// Points to the previous element in the queue or \c NULL if this was the
    /// last one enqueued.
    struct QueueNode_s *prev;
};

/// \brief A type for a queue node.
///
/// Defines a type to a <code> struct QueueNode_s </code>.
typedef struct QueueNode_s QueueNode_t;

/// \brief A pointer type for a queue node.
///
/// Defines a pointer type to a <code> struct QueueNode_s </code>.
typedef struct QueueNode_s *QueueNode;

///////////////////////////////////////////////////// NOT EXPOSED FUNCTIONS ///

static Status que_make_node(QueueNode *node, void *element);

static Status que_free_node(QueueNode *node, que_free_f free_f);

static Status que_free_node_shallow(QueueNode *node);

////////////////////////////////////////////// END OF NOT EXPOSED FUNCTIONS ///

/// \brief Initializes the Queue_s structure.
///
/// Initializes a new Queue_s structure with initial length and limit as 0.
/// Note that it does not sets any default functions. If you don't set them
/// later you won't be able to do certain operations that depend on a user-
/// defined function.
///
/// \param[in,out] queue The queue to be initialized.
///
/// \return DS_ERR_ALLOC if queue allocation failed.
/// \return DS_OK if all operations were successful.
Status que_init(Queue *queue)
{
    *queue = malloc(sizeof(Queue_t));

    if (!(*queue))
        return DS_ERR_ALLOC;

    (*queue)->length = 0;
    (*queue)->limit = 0;
    (*queue)->version_id = 0;

    (*queue)->front = NULL;
    (*queue)->rear = NULL;

    (*queue)->d_compare = NULL;
    (*queue)->d_copy = NULL;
    (*queue)->d_display = NULL;
    (*queue)->d_free = NULL;

    return DS_OK;
}

/// \brief Creates a Queue_s.
///
/// This function completely creates a Queue_s, setting all of its default
/// functions.
///
/// \param[in,out] queue Queue_s to be allocated.
/// \param[in] compare_f A function that compares two elements.
/// \param[in] copy_f A function that makes an exact copy of an element.
/// \param[in] display_f A function that displays in the console an element.
/// \param[in] free_f A function that completely frees from memory an element.
///
/// \return DS_ERR_ALLOC if queue allocation failed.
/// \return DS_OK if all operations are successful.
Status que_create(Queue *queue, que_compare_f compare_f, que_copy_f copy_f,
        que_display_f display_f, que_free_f free_f)
{
    *queue = malloc(sizeof(Queue_t));

    if (!(*queue))
        return DS_ERR_ALLOC;

    (*queue)->length = 0;
    (*queue)->limit = 0;
    (*queue)->version_id = 0;

    (*queue)->front = NULL;
    (*queue)->rear = NULL;

    (*queue)->d_compare = compare_f;
    (*queue)->d_copy = copy_f;
    (*queue)->d_display = display_f;
    (*queue)->d_free = free_f;

    return DS_OK;
}

/// \brief Frees from memory a Queue_s and all its elements.
///
/// This function frees from memory all the queue's elements using its default
/// free function and then frees the queue's structure. The variable is then
/// set to \c NULL.
///
/// \param queue The Queue_s to be freed from memory.
///
/// \return DS_ERR_INCOMPLETE_TYPE if a default free function is not set.
/// \return DS_ERR_NULL_POINTER if the queue reference is \c NULL.
/// \return DS_OK if all operations were successful.
Status que_free(Queue *queue)
{
    if ((*queue) == NULL)
        return DS_ERR_NULL_POINTER;

    if ((*queue)->d_free == NULL)
        return DS_ERR_INCOMPLETE_TYPE;

    QueueNode prev = (*queue)->front;

    Status st;

    while ((*queue)->front != NULL)
    {
        (*queue)->front = (*queue)->front->prev;

        st = que_free_node(&prev, (*queue)->d_free);

        if (st != DS_OK)
            return st;

        prev = (*queue)->front;
    }

    free((*queue));

    (*queue) = NULL;

    return DS_OK;
}

/// \brief Frees from memory a Queue_s.
///
/// This function frees from memory all the queue's nodes without freeing its
/// data and then frees the queue structure. The variable is then set to
/// \c NULL.
///
/// \param[in,out] queue Queue_s to be freed from memory.
///
/// \return DS_ERR_NULL_POINTER if the queue references to \c NULL.
/// \return DS_OK if all operations are successful.
Status que_free_shallow(Queue *queue)
{
    if ((*queue) == NULL)
        return DS_ERR_NULL_POINTER;

    if ((*queue)->d_free == NULL)
        return DS_ERR_INCOMPLETE_TYPE;

    QueueNode prev = (*queue)->front;

    Status st;

    while ((*queue)->front != NULL)
    {
        (*queue)->front = (*queue)->front->prev;

        st = que_free_node_shallow(&prev);

        if (st != DS_OK)
            return st;

        prev = (*queue)->front;
    }

    free((*queue));

    (*queue) = NULL;

    return DS_OK;
}

/// This function sets the queue to its initial state, erasing all of its data
/// and re-initializing the structure with the same default functions.
///
/// \param queue The queue to be erased.
///
/// \return DS_ERR_ALLOC if queue allocation failed.
/// \return DS_ERR_INCOMPLETE_TYPE if a default free function is not set.
/// \return DS_ERR_NULL_POINTER if the queue reference is \c NULL.
/// \return DS_OK if all operations were successful.
Status que_erase(Queue *queue)
{
    if (*queue == NULL)
        return DS_ERR_NULL_POINTER;

    Queue new_queue;

    Status st = que_create(&new_queue, (*queue)->d_compare, (*queue)->d_copy,
            (*queue)->d_display, (*queue)->d_free);

    if (st !=  DS_OK)
        return st;

    st = que_free(queue);

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
/// with the que_compare_f specifications.
///
/// \param[in] queue Queue_s to set the default compare function.
/// \param[in] function A que_compare_f kind of function.
///
/// \return DS_ERR_NULL_POINTER if the queue references to \c NULL.
/// \return DS_OK if all operations are successful.
Status que_set_func_compare(Queue queue, que_compare_f function)
{
    if (queue == NULL)
        return DS_ERR_NULL_POINTER;

    queue->d_compare = function;

    return DS_OK;
}

/// \brief Sets the default copy function.
///
/// Use this function to set a default compare function. It needs to comply
/// with the que_copy_f specifications.
///
/// \param[in] queue Queue_s to set the default copy function.
/// \param[in] function A que_copy_f kind of function.
///
/// \return DS_ERR_NULL_POINTER if the queue references to \c NULL.
/// \return DS_OK if all operations are successful.
Status que_set_func_copy(Queue queue, que_copy_f function)
{
    if (queue == NULL)
        return DS_ERR_NULL_POINTER;

    queue->d_copy = function;

    return DS_OK;
}

/// \brief Sets the default display function.
///
/// Use this function to set a default display function. It needs to comply
/// with the que_display_f specifications. Useful for debugging.
///
/// \param[in] queue Queue_s to set the default display function.
/// \param[in] function A que_display_f kind of function.
///
/// \return DS_ERR_NULL_POINTER if the queue references to \c NULL.
/// \return DS_OK if all operations are successful.
Status que_set_func_display(Queue queue, que_display_f function)
{
    if (queue == NULL)
        return DS_ERR_NULL_POINTER;

    queue->d_display = function;

    return DS_OK;
}

/// \brief Sets the default free function.
///
/// Use this function to set a default free function. It needs to comply
/// with the que_free_f specifications.
///
/// \param[in] queue Queue_s to set the default free function.
/// \param[in] function A que_free_f kind of function.
///
/// \return DS_ERR_NULL_POINTER if the queue references to \c NULL.
/// \return DS_OK if all operations are successful.
Status que_set_func_free(Queue queue, que_free_f function)
{
    if (queue == NULL)
        return DS_ERR_NULL_POINTER;

    queue->d_free = function;

    return DS_OK;
}

/// \brief Sets a limit to the specified Queue_s's length.
///
/// Limit's the Queue_s's length. You can only set a limit greater or equal to
/// the queue's current length and greater than 0. To remove this limitation
/// simply set the limit to 0 or less.
///
/// \param[in] queue Queue_s reference.
/// \param[in] limit Maximum queue length.
///
/// \return DS_ERR_INVALID_OPERATION if the limitation is less than the queue's
/// current length.
/// \return DS_ERR_NULL_POINTER if the queue references to \c NULL.
/// \return DS_OK if all operations are successful.
Status que_set_limit(Queue queue, index_t limit)
{
    if (queue == NULL)
        return DS_ERR_NULL_POINTER;

    // The new limit can't be lower than the queue's current length.
    if (queue->length > limit && limit > 0)
        return DS_ERR_INVALID_OPERATION;

    queue->limit = limit;

    return DS_OK;
}

index_t que_length(Queue queue)
{
    if (queue == NULL)
        return -1;

    return queue->length;
}

index_t que_limit(Queue queue)
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
/// \return DS_ERR_NULL_POINTER if queue reference is \c NULL.
/// \return DS_OK if all operations were successful.
Status que_enqueue(Queue queue, void *element)
{
    if (queue == NULL)
        return DS_ERR_NULL_POINTER;

    if (que_full(queue))
        return DS_ERR_FULL;

    QueueNode node;

    Status st = que_make_node(&node, element);

    if (st != DS_OK)
        return st;

    if (que_empty(queue))
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
/// \return DS_ERR_NULL_POINTER if queue reference is \c NULL.
/// \return DS_OK if all operations were successful.
Status que_dequeue(Queue queue, void **result)
{
    *result = NULL;

    if (queue == NULL)
        return DS_ERR_NULL_POINTER;

    if (que_empty(queue))
        return DS_ERR_INVALID_OPERATION;

    QueueNode node = queue->front;

    *result = node->data;

    queue->front = queue->front->prev;

    Status st = que_free_node_shallow(&node);

    if (st != DS_OK)
        return st;

    queue->length--;
    queue->version_id++;

    if (que_empty(queue))
    {
        queue->front = NULL;
        queue->rear = NULL;
    }

    return DS_OK;
}

bool que_full(Queue queue)
{
    return queue->limit > 0 && queue->length >= queue->limit;
}

bool que_empty(Queue queue)
{
    return queue->length == 0;
}

void *que_peek_front(Queue queue)
{
    if (queue == NULL)
        return NULL;

    if (que_empty(queue))
        return NULL;

    return queue->front->data;
}

void *que_peek_rear(Queue queue)
{
    if (queue == NULL)
        return NULL;

    if (que_empty(queue))
        return NULL;

    return queue->rear->data;
}

Status que_copy(Queue queue, Queue *result)
{
    *result = NULL;

    if (queue == NULL)
        return DS_ERR_NULL_POINTER;

    if (queue->d_copy == NULL || queue->d_free == NULL)
        return DS_ERR_INCOMPLETE_TYPE;

    Status st = que_create(result, queue->d_compare, queue->d_copy,
                           queue->d_display, queue->d_free);

    if (st != DS_OK)
        return st;

    (*result)->limit = queue->limit;

    if (que_empty(queue))
        return DS_OK;

    QueueNode scan = queue->front;

    void *elem;

    while (scan != NULL)
    {
        elem = queue->d_copy(scan->data);

        st = que_enqueue(*result, elem);

        if (st != DS_OK)
        {
            queue->d_free(elem);

            return st;
        }

        scan = scan->prev;
    }

    return DS_OK;
}

/// \brief Displays a Queue_s in the console.
///
/// Displays a Queue_s in the console starting from \c front to \c tail.
///
/// \param[in] queue The queue to be displayed in the console.
///
/// \return DS_ERR_INCOMPLETE_TYPE if a default display function is not set.
/// \return DS_ERR_NULL_POINTER if the queue reference is \c NULL.
/// \return DS_OK if all operations were successful.
Status que_display(Queue queue)
{
    if (queue == NULL)
        return DS_ERR_NULL_POINTER;

    if (queue->d_display == NULL)
        return DS_ERR_INCOMPLETE_TYPE;

    if (que_empty(queue))
    {
        printf("\nQueue\n[ empty ]\n");

        return DS_OK;
    }

    QueueNode scan = queue->front;

    printf("\nQueue\nfront <-");

    while (scan != NULL)
    {
        queue->d_display(scan->data);

        printf(" <- ");

        scan = scan->prev;
    }

    printf(" rear\n");

    return DS_OK;
}

/// \brief Displays a Queue_s in the console like an array.
///
/// Displays a Queue_s in the console starting from \c front to \c tail like an
/// array with its elements separated by commas, delimited with brackets.
///
/// \param[in] queue The queue to be displayed in the console.
///
/// \return DS_ERR_INCOMPLETE_TYPE if a default display function is not set.
/// \return DS_ERR_NULL_POINTER if the queue reference is \c NULL.
/// \return DS_OK if all operations were successful.
Status que_display_array(Queue queue)
{
    if (queue == NULL)
        return DS_ERR_NULL_POINTER;

    if (queue->d_display == NULL)
        return DS_ERR_INCOMPLETE_TYPE;

    if (que_empty(queue))
    {
        printf("\n[ empty ]\n");

        return DS_OK;
    }

    QueueNode scan = queue->front;

    printf("\n[ ");

    while (scan->prev != NULL)
    {
        queue->d_display(scan->data);

        printf(", ");

        scan = scan->prev;
    }

    queue->d_display(scan->data);

    printf(" ]\n");

    return DS_OK;
}

/// \brief Displays a Queue_s in the console.
///
/// Displays a Queue_s in the console starting from \c front to \c tail with
/// its elements separated by spaces.
///
/// \param[in] queue The queue to be displayed in the console.
///
/// \return DS_ERR_INCOMPLETE_TYPE if a default display function is not set.
/// \return DS_ERR_NULL_POINTER if the queue reference is \c NULL.
/// \return DS_OK if all operations were successful.
Status que_display_raw(Queue queue)
{
    if (queue == NULL)
        return DS_ERR_NULL_POINTER;

    if (queue->d_display == NULL)
        return DS_ERR_INCOMPLETE_TYPE;

    printf("\n");

    if (que_empty(queue))
        return DS_OK;

    QueueNode scan = queue->front;

    while (scan != NULL)
    {
        queue->d_display(scan->data);

        printf(" ");

        scan = scan->prev;
    }

    printf("\n");

    return DS_OK;
}

///////////////////////////////////////////////////// NOT EXPOSED FUNCTIONS ///

static Status que_make_node(QueueNode *node, void *element)
{
    (*node) = malloc(sizeof(QueueNode_t));

    if (!(*node))
        return DS_ERR_ALLOC;

    (*node)->data = element;

    (*node)->prev = NULL;

    return DS_OK;
}

static Status que_free_node(QueueNode *node, que_free_f free_f)
{
    if ((*node) == NULL)
        return DS_ERR_NULL_POINTER;

    free_f((*node)->data);

    free(*node);

    (*node) = NULL;

    return DS_OK;
}

static Status que_free_node_shallow(QueueNode *node)
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

/// \brief An iterator for a Queue_s.
///
/// This iterator is a forward-only iterator.
struct QueueIterator_s
{
    /// \brief Target Queue_s.
    ///
    /// Target Queue_s. The iterator might need to use some information
    /// provided by the queue or change some of its data members.
    struct Queue_s *target;

    /// \brief Current element.
    ///
    /// Points to the current node. The iterator is always initialized with the
    /// cursor pointing to the start (front) of the queue.
    struct QueueNode_s *cursor;

    /// \brief Target version ID.
    ///
    /// When the iterator is initialized it stores the version_id of the target
    /// structure. This is kept to prevent iteration on the target structure
    /// that may have been modified and thus causing undefined behaviours or
    /// run-time crashes.
    index_t target_id;
};

///////////////////////////////////////////////////// NOT EXPOSED FUNCTIONS ///

static bool que_iter_target_modified(QueueIterator iter);

static bool que_iter_invalid_state(QueueIterator iter);

////////////////////////////////////////////// END OF NOT EXPOSED FUNCTIONS ///

Status que_iter_init(QueueIterator *iter, Queue target)
{
    *iter = malloc(sizeof(QueueIterator_t));

    if (!(*iter))
        return DS_ERR_ALLOC;

    (*iter)->target = target;
    (*iter)->target_id = target->version_id;
    (*iter)->cursor = target->front;

    return DS_OK;
}

Status que_iter_retarget(QueueIterator *iter, Queue target)
{
    Status st = que_iter_free(iter);

    if (st != DS_OK)
        return st;

    st = que_iter_init(iter, target);

    if (st != DS_OK)
        return st;

    return DS_OK;
}

Status que_iter_free(QueueIterator *iter)
{
    if (*iter == NULL)
        return DS_ERR_NULL_POINTER;

    free(*iter);

    *iter = NULL;

    return DS_OK;
}

Status que_iter_next(QueueIterator iter)
{
    if (iter == NULL)
        return DS_ERR_NULL_POINTER;

    if (que_iter_invalid_state(iter))
        return DS_ERR_ITER_STATE;

    if (que_iter_target_modified(iter))
        return DS_ERR_ITER_MODIFICATION;

    if (!que_iter_has_next(iter))
        return DS_ERR_ITER;

    iter->cursor = iter->cursor->prev;

    return DS_OK;
}

Status que_iter_to_front(QueueIterator iter)
{
    if (iter == NULL)
        return DS_ERR_NULL_POINTER;

    if (que_iter_invalid_state(iter))
        return DS_ERR_ITER_STATE;

    if (que_iter_target_modified(iter))
        return DS_ERR_ITER_MODIFICATION;

    if (!que_iter_has_next(iter))
        return DS_ERR_ITER;

    iter->cursor = iter->target->front;

    return DS_OK;
}

Status que_iter_to_rear(QueueIterator iter)
{
    if (iter == NULL)
        return DS_ERR_NULL_POINTER;

    if (que_iter_invalid_state(iter))
        return DS_ERR_ITER_STATE;

    if (que_iter_target_modified(iter))
        return DS_ERR_ITER_MODIFICATION;

    if (!que_iter_has_next(iter))
        return DS_ERR_ITER;

    iter->cursor = iter->target->rear;

    return DS_OK;
}

bool que_iter_has_next(QueueIterator iter)
{
    return iter->cursor->prev != NULL;
}

Status que_iter_get(QueueIterator iter, void **result)
{
    if (iter == NULL)
        return DS_ERR_NULL_POINTER;

    if (que_iter_invalid_state(iter))
        return DS_ERR_ITER_STATE;

    if (que_iter_target_modified(iter))
        return DS_ERR_ITER_MODIFICATION;

    if (iter->target->d_copy == NULL || iter->target->d_free == NULL)
        return DS_ERR_INCOMPLETE_TYPE;

    *result = iter->target->d_copy(iter->cursor->data);

    return DS_OK;
}

Status que_iter_set(QueueIterator iter, void *element)
{
    if (iter == NULL)
        return DS_ERR_NULL_POINTER;

    if (que_iter_invalid_state(iter))
        return DS_ERR_ITER_STATE;

    if (que_iter_target_modified(iter))
        return DS_ERR_ITER_MODIFICATION;

    if (iter->target->d_free == NULL)
        return DS_ERR_INCOMPLETE_TYPE;

    iter->target->d_free(iter->cursor->data);

    iter->cursor->data = element;

    return DS_OK;
}

Status que_iter_insert(QueueIterator iter, void *element)
{
    if (iter == NULL)
        return DS_ERR_NULL_POINTER;

    if (que_iter_invalid_state(iter))
        return DS_ERR_ITER_STATE;

    if (que_iter_target_modified(iter))
        return DS_ERR_ITER_MODIFICATION;

    QueueNode node;

    Status st = que_make_node(&node, element);

    if (st != DS_OK)
        return st;

    if (que_empty(iter->target))
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

Status que_iter_remove(QueueIterator iter, void **result)
{
    if (iter == NULL)
        return DS_ERR_NULL_POINTER;

    if (que_iter_invalid_state(iter))
        return DS_ERR_ITER_STATE;

    if (que_iter_target_modified(iter))
        return DS_ERR_ITER_MODIFICATION;

    if (!que_iter_has_next(iter))
        return DS_ERR_ITER;

    Status st;

    if (que_length(iter->target) == 1)
    {
        *result = iter->cursor->data;

        st = que_free_node_shallow(&(iter->cursor));

        if (st != DS_OK)
            return st;

        iter->cursor = NULL;

        iter->target->front = NULL;
        iter->target->rear = NULL;
    }
    else
    {
        QueueNode node = iter->cursor->prev;

        *result = node->data;

        iter->cursor->prev = node->prev;

        st = que_free_node_shallow(&node);

        if (st != DS_OK)
            return st;
    }

    iter->target->length--;

    iter->target->version_id++;
    iter->target_id++;

    return DS_OK;
}

void *que_iter_peek_next(QueueIterator iter)
{
    if (iter == NULL)
        return NULL;

    if (que_iter_invalid_state(iter))
        return NULL;

    if (que_iter_target_modified(iter))
        return NULL;

    if (!que_iter_has_next(iter))
        return NULL;

    return iter->cursor->prev->data;
}

void *que_iter_peek(QueueIterator iter)
{
    if (iter == NULL)
        return NULL;

    if (que_iter_invalid_state(iter))
        return NULL;

    if (que_iter_target_modified(iter))
        return NULL;

    return iter->cursor->data;
}

///////////////////////////////////////////////////// NOT EXPOSED FUNCTIONS ///

static bool que_iter_target_modified(QueueIterator iter)
{
    return iter->target_id != iter->target->version_id;
}

static bool que_iter_invalid_state(QueueIterator iter)
{
    return iter->cursor == NULL || iter->target == NULL;
}

////////////////////////////////////////////// END OF NOT EXPOSED FUNCTIONS ///