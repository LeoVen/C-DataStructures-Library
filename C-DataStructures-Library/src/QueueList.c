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
    /// \brief Current amount of elements in the QueueList_s.
    ///
    /// Current amount of elements in the QueueList_s.
    integer_t count;

    /// \brief QueueList count limit.
    ///
    /// If it is set to 0 or a negative value then the queue has no limit to
    /// its count. Otherwise it won't be able to have more elements than the
    /// specified value. The queue is always initialized with no restrictions
    /// to its length, that is, \c limit equals 0. The user won't be able to
    /// limit the queue count if it already has more elements than the
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

    /// \brief QueueList_s interface.
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
const unsigned_t qli_size = sizeof(QueueList_t);

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

static QueueListNode_t *
qli_new_node(void *element);

static void
qli_free_node(QueueListNode_t *node, free_f function);

static void
qli_free_node_shallow(QueueListNode_t *node);

////////////////////////////////////////////// END OF NOT EXPOSED FUNCTIONS ///

/// Initializes a new QueueList_s with a given interface that contains
/// functions to handle any user-defined type.
/// \par Interface Requirements
/// - None
///
/// \param[in] interface An interface defining all necessary functions for the
/// queue to operate.
///
/// \return A new QueueList_s or NULL if allocation failed.
QueueList_t *
qli_new(Interface_t *interface)
{
    QueueList_t *queue = malloc(sizeof(QueueList_t));

    if (!queue)
        return NULL;

    queue->count = 0;
    queue->limit = 0;
    queue->version_id = 0;
    queue->front = NULL;
    queue->rear = NULL;

    queue->interface = interface;

    return queue;
}

/// Initializes a new QueueList_s allocated on the stack, given an interface
/// that contains functions to handle any user-defined type.
/// \par Interface Requirements
/// - None
///
/// \param[in,out] queue The queue allocated on the stack to be initialized.
/// \param[in] interface An interface defining all necessary functions for the
/// queue to operate.
///
/// \return True if all operations were successful.
bool
qli_init(QueueList_t *queue, Interface_t *interface)
{
    queue->count = 0;
    queue->limit = 0;
    queue->version_id = 0;
    queue->front = NULL;
    queue->rear = NULL;
    queue->interface = interface;

    return true;
}

/// Frees each element at the queue using its interface's \c free and then
/// frees the queue struct.
/// \par Interface Requirements
/// - free
///
/// \param[in] queue The queue to be freed from memory.
void
qli_free(QueueList_t *queue)
{
    QueueListNode_t *prev = queue->front;

    while (queue->front != NULL)
    {
        queue->front = queue->front->prev;

        qli_free_node(prev, queue->interface->free);

        prev = queue->front;
    }

    free(queue);
}

/// Frees the QueueList_s structure and its nodes, leaves all the elements
/// intact. Be careful as this might cause severe memory leaks. Only use this
/// if your queue elements are also handled by another structure or algorithm.
/// \par Interface Requirements
/// - None
///
/// \param[in] queue The queue to be freed from memory.
void
qli_free_shallow(QueueList_t *queue)
{
    QueueListNode_t *prev = queue->front;

    while (queue->front != NULL)
    {
        queue->front = queue->front->prev;

        qli_free_node_shallow(prev);

        prev = queue->front;
    }

    free(queue);
}

/// This function will free all the elements of the specified QueueList_s and
/// will keep the structure intact.
/// \par Interface Requirements
/// - free
///
/// \param[in] queue The queue to have its elements erased.
void
qli_erase(QueueList_t *queue)
{
    QueueListNode_t *prev = queue->front;

    while (queue->front != NULL)
    {
        queue->front = queue->front->prev;

        qli_free_node(prev, queue->interface->free);

        prev = queue->front;
    }

    queue->count = 0;
    queue->version_id++;

    queue->front = NULL;
    queue->rear = NULL;
}

/// This function will reset the QueueList_s, freeing all of its nodes but not
/// its elements, keeping the structure intact including its original
/// interface.
/// \par Interface Requirements
/// - None
///
/// \param[in] queue The queue to have its nodes erased.
void
qli_erase_shallow(QueueList_t *queue)
{
    QueueListNode_t *prev = queue->front;

    while (queue->front != NULL)
    {
        queue->front = queue->front->prev;

        qli_free_node_shallow(prev);

        prev = queue->front;
    }

    queue->count = 0;
    queue->version_id++;

    queue->front = NULL;
    queue->rear = NULL;
}

/// Sets a new interface for the specified QueueList_s.
/// \par Interface Requirements
/// - None
///
/// \param[in] queue QueueList_s to change the interface.
/// \param[in] new_interface New interface for the specified structure.
void
qli_config(QueueList_t *queue, Interface_t *new_interface)
{
    queue->interface = new_interface;
}

/// Returns the current amount of elements in the specified queue.
/// \par Interface Requirements
/// - None
///
/// \param[in] queue QueueList_s reference.
///
/// \return The queue total amount of elements.
integer_t
qli_count(QueueList_t *queue)
{
    return queue->count;
}

/// Returns the current queue limit.
/// \par Interface Requirements
/// - None
///
/// \param[in] queue QueueList_s reference.
///
/// \return The current queue limit.
integer_t
qli_limit(QueueList_t *queue)
{
    return queue->limit;
}

/// Limit's the QueueList_s's length. You can only set a limit greater or equal
/// to the queue's current length and greater than 0. To remove this limitation
/// simply set the limit to 0 or less.
///
/// \param[in] queue QueueList_s reference.
/// \param[in] limit New queue limit.
///
/// \return True if a new limit was set. False if the new limit is lower than
/// the current amount of elements in the queue.
bool
qli_set_limit(QueueList_t *queue, integer_t limit)
{
    // The new limit can't be lower than the queue's current length.
    if (queue->count > limit && limit > 0)
        return false;

    queue->limit = limit;

    return true;
}

/// Inserts an element into the specified queue. The element is added relative
/// to the \c rear pointer.
/// \par Interface Requirements
/// - None
///
/// \param[in] queue The queue where the element is to be inserted.
/// \param[in] element The element to be inserted in the queue.
///
/// \return True if the element was successfully added to the queue or false if
/// the queue reached its limit size or node allocation failed.
bool
qli_enqueue(QueueList_t *queue, void *element)
{
    if (qli_full(queue))
        return false;

    QueueListNode_t *node = qli_new_node(element);

    if (!node)
        return false;

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

    queue->count++;
    queue->version_id++;

    return true;
}

/// Removes an element from the specified queue. The element is removed
/// relative to the \c front pointer.
/// \par Interface Requirements
/// - None
///
/// \param[in] queue The queue where the element is to be removed from.
/// \param[out] result The resulting element removed from the queue.
///
/// \return True if an element was removed from the queue or false if the queue
/// is empty.
bool
qli_dequeue(QueueList_t *queue, void **result)
{
    *result = NULL;

    if (qli_empty(queue))
        return false;

    QueueListNode_t *node = queue->front;

    *result = node->data;

    queue->front = queue->front->prev;

    qli_free_node_shallow(node);

    queue->count--;
    queue->version_id++;

    if (qli_empty(queue))
    {
        queue->front = NULL;
        queue->rear = NULL;
    }

    return true;
}

/// Returns the element at the front of the queue, that is, the oldest element
/// and the next one to be removed, or NULL if the queue is empty.
/// \par Interface Requirements
/// - None
///
/// \param[in] queue The target queue.
///
/// \return NULL if the queue is empty or the element at the front of the
/// queue.
void *
qli_peek_front(QueueList_t *queue)
{
    if (qli_empty(queue))
        return NULL;

    return queue->front->data;
}

/// Returns the element at the rear of the queue, that is, the newest element
/// and the last one to be removed, the or NULL if the queue is empty.
/// \par Interface Requirements
/// - None
///
/// \param[in] queue The target queue.
///
/// \return NULL if the queue is empty or the element at the rear of the queue.
void *
qli_peek_rear(QueueList_t *queue)
{
    if (qli_empty(queue))
        return NULL;

    return queue->rear->data;
}

/// Returns true if the queue is empty, or false if there are elements in the
/// queue.
/// \par Interface Requirements
/// - None
///
/// \param[in] queue The target queue.
///
/// \return True if the queue is empty, otherwise false.
bool
qli_empty(QueueList_t *queue)
{
    return queue->count == 0;
}

/// Returns true if the queue is full or false otherwise. The queue can only be
/// full if its limit is set to a value higher than 0 and respecting all the
/// rules from qli_set_limit().
/// \par Interface Requirements
/// - None
///
/// \param[in] queue The target queue.
///
/// \return True if the amount of elements in the queue have reached a limit.
bool
qli_full(QueueList_t *queue)
{
    return queue->limit > 0 && queue->count >= queue->limit;
}

/// Returns true if the specified size will fit in the queue before it reaches
/// its limit (if the limit is set).
/// \par Interface Requirements
/// - None
///
/// \param[in] queue The target queue.
/// \param[in] size The specified size.
///
/// \return True if a given size fits inside the queue if it has a limit.
bool
qli_fits(QueueList_t *queue, unsigned_t size)
{
    if (queue->limit <= 0)
        return true;

    return (queue->count + size) <= queue->limit;
}

/// Returns true if the element is present in the queue, otherwise false.
/// \par Interface Requirements
/// - compare
///
/// \param[in] queue QueueList_s reference.
/// \param[in] key Key to be matched.
///
/// \return True if the element is present in the queue, otherwise false.
bool
qli_contains(QueueList_t *queue, void *key)
{
    QueueListNode_t *scan = queue->front;

    while (scan != NULL)
    {
        if (queue->interface->compare(scan->data, key) == 0)
            return true;

        scan = scan->prev;
    }

    return false;
}

/// Returns a copy of the specified QueueList_s with the same interface. All
/// elements are copied using the queue interface's copy function.
/// \par Interface Requirements
/// - copy
/// - free
///
/// \param[in] queue The queue to be copied.
///
/// \return NULL if allocation failed or a copy of the specified queue.
QueueList_t *
qli_copy(QueueList_t *queue)
{
    QueueList_t *result = qli_new(queue->interface);

    if (!result)
        return NULL;

    result->limit = queue->limit;

    // scan -> goes through the original queue
    // copy -> current element being copied
    // prev -> copy's previous node to make the link
    QueueListNode_t *prev = NULL, *copy = NULL, *scan = queue->front;

    while (scan != NULL)
    {
        void *element = queue->interface->copy(scan->data);
        copy = qli_new_node(element);

        if (!copy)
        {
            queue->interface->free(element);
            return false;
        }

        if (prev == NULL)
        {
            prev = copy;
            result->front = prev;
        }
        else
        {
            prev->prev = copy;
            prev = prev->prev;
        }

        scan = scan->prev;
    }

    result->rear = copy;

    result->count = queue->count;

    return result;
}

/// Creates a shallow copy of all elements in the queue, that is, only the
/// pointers addresses are copied to the new queue.
/// \par Interface Requirements
/// - None
///
/// \param[in] queue The queue to be copied.
///
/// \return NULL if allocation failed or a shallow copy of the specified queue.
QueueList_t *
qli_copy_shallow(QueueList_t *queue)
{
    QueueList_t *result = qli_new(queue->interface);

    if (!result)
        return NULL;

    result->limit = queue->limit;

    // scan -> goes through the original queue
    // copy -> current element being copied
    // prev -> copy's previous node to make the link
    QueueListNode_t *prev = NULL, *copy = NULL, *scan = queue->front;

    while (scan != NULL)
    {
        copy = qli_new_node(scan->data);

        if (!copy)
        {
            qli_free_node_shallow(copy);
            return false;
        }

        if (prev == NULL)
        {
            prev = copy;
            result->front = prev;
        }
        else
        {
            prev->prev = copy;
            prev = prev->prev;
        }

        scan = scan->prev;
    }

    result->rear = copy;

    result->count = queue->count;

    return result;
}

/// Makes a comparison between two queues element by element. If one queue has
/// less elements than the other the comparison of elements will go up until
/// one queue reaches its limit. If all elements are the same until then, the
/// tie breaker goes to their element count. If it is also the same, then both
/// queues are equal.
/// \par Interface Requirements
/// - compare
///
/// \param[in] queue1 A target queue to be compared.
/// \param[in] queue2 A target queue to be compared.
///
/// \return An int according to \ref compare_f.
int
qli_compare(QueueList_t *queue1, QueueList_t *queue2)
{
    QueueListNode_t *scan1 = queue1->front, *scan2 = queue2->front;

    int comparison = 0;
    while (scan1 != NULL && scan2 != NULL)
    {
        comparison = queue1->interface->compare(scan1->data, scan2->data);
        if (comparison > 0)
            return 1;
        else if (comparison < 0)
            return -1;

        scan1 = scan1->prev;
        scan2 = scan2->prev;
    }

    // So far all elements were the same
    if (queue1->count > queue2->count)
        return 1;
    else if (queue1->count < queue2->count)
        return -1;

    return 0;
}

/// Appends \c queue2 at the rear of \c queue1, emptying queue2. Both queues
/// need to have been initialized.
/// \par Interface Requirements
/// - None
///
/// \param[in] queue1 Queue to receive elements.
/// \param[in] queue2 Queue where the elements are going to be taken from.
///
/// \return True if all operations were successful, otherwise false.
bool
qli_append(QueueList_t *queue1, QueueList_t *queue2)
{
    /// \todo qli_append
    return true;
}

/// Makes a copy of all the elements in the queue to a C array starting from
/// the front element to the rear element.
/// \par Interface Requirements
/// - copy
///
/// \param[in] queue The queue to be copied to the array.
/// \param[out] length The resulting array's length.
///
/// \return The resulting array or NULL if the queue is empty or the array
/// allocation failed.
void **
qli_to_array(QueueList_t *queue, integer_t *length)
{
    *length = 0;

    if (qli_empty(queue))
        return NULL;

    void **array = malloc(sizeof(void*) * (size_t)queue->count);

    if (!array)
        return NULL;

    QueueListNode_t *scan = queue->front;

    integer_t i = 0;
    while (scan != NULL)
    {
        array[i++] = queue->interface->copy(scan->data);
        scan = scan->prev;
    }

    *length = queue->count;

    return array;
}

/// Displays a QueueList_s in the console starting from the front element to
/// the rear element. There are currently four modes:
/// - -1 Displays each element separated by newline;
/// -  0 Displays each element like a linked list;
/// -  1 Displays each element separated by a space;
/// - Any other number defaults to the array representation.
/// \par Interface Requirements
/// - display
///
/// \param[in] queue The queue to be displayed in the console.
/// \param[in] display_mode How the queue is to be displayed in the console.
void
qli_display(QueueList_t *queue, int display_mode)
{
    if (qli_empty(queue))
    {
        printf("\nQueueList\n[ empty ]\n");
        return;
    }

    QueueListNode_t *scan = queue->front;

    switch (display_mode)
    {
        case -1:
            printf("\nQueueList\n");
            while (scan != NULL)
            {
                queue->interface->display(scan->data);
                printf("\n");
                scan = scan->prev;
            }
            break;
        case 0:
            printf("\nQueueList\nFront -> ");
            while (scan->prev != NULL)
            {
                queue->interface->display(scan->data);
                printf(" -> ");
                scan = scan->prev;
            }
            queue->interface->display(scan->data);
            printf(" Rear\n");
            break;
        case 1:
            printf("\nQueueList\n");
            while (scan != NULL)
            {
                queue->interface->display(scan->data);
                printf(" ");
                scan = scan->prev;
            }
            printf("\n");
            break;
        default:
            printf("\nQueueList\n[ ");
            while (scan->prev != NULL)
            {
                queue->interface->display(scan->data);
                printf(", ");
                scan = scan->prev;
            }
            queue->interface->display(scan->data);
            printf(" ]\n");
            break;
    }
}

///////////////////////////////////////////////////// NOT EXPOSED FUNCTIONS ///

static QueueListNode_t *
qli_new_node(void *element)
{
    QueueListNode_t *node = malloc(sizeof(QueueListNode_t));

    if (!node)
        return NULL;

    node->data = element;
    node->prev = NULL;

    return node;
}

static void
qli_free_node(QueueListNode_t *node, free_f function)
{
    function(node->data);
    free(node);
}

static void
qli_free_node_shallow(QueueListNode_t *node)
{
    free(node);
}

////////////////////////////////////////////// END OF NOT EXPOSED FUNCTIONS ///

///////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////// Iterator ///
///////////////////////////////////////////////////////////////////////////////

/// This iterator is a forward-only iterator. Its cursor is represented by a
/// pointer to one of the queue's node.
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

static bool
qli_iter_target_modified(QueueListIterator_t *iter);

////////////////////////////////////////////// END OF NOT EXPOSED FUNCTIONS ///

///
/// \param[in] target
///
/// \return
QueueListIterator_t *
qli_iter_new(QueueList_t *target)
{
    if (qli_empty(target))
        return NULL;

    QueueListIterator_t *iter = malloc(sizeof(QueueListIterator_t));

    if (!iter)
        return NULL;

    iter->target = target;
    iter->target_id = target->version_id;
    iter->cursor = target->front;

    return iter;
}

///
/// \param[in,out] iter
/// \param[in] target
///
/// \return
bool
qli_iter_init(QueueListIterator_t *iter, QueueList_t *target)
{
    if (qli_empty(target))
        return false;

    iter->target = target;
    iter->target_id = target->version_id;
    iter->cursor = target->front;

    return true;
}

///
/// \param[in] iter
/// \param[in] target
void
qli_iter_retarget(QueueListIterator_t *iter, QueueList_t *target)
{
    iter->target = target;
    iter->target_id = target->version_id;
}

///
/// \param[in] iter
void
qli_iter_free(QueueListIterator_t *iter)
{
    free(iter);
}

///
/// \param[in] iter
///
/// \return
bool
qli_iter_next(QueueListIterator_t *iter)
{
    if (qli_iter_target_modified(iter))
        return false;

    if (!qli_iter_has_next(iter))
        return false;

    iter->cursor = iter->cursor->prev;

    return true;
}

///
/// \param[in] iter
///
/// \return
bool
qli_iter_to_front(QueueListIterator_t *iter)
{
    if (qli_iter_target_modified(iter))
        return false;

    iter->cursor = iter->target->front;

    return true;
}

///
/// \param[in] iter
///
/// \return
bool
qli_iter_to_rear(QueueListIterator_t *iter)
{
    if (qli_iter_target_modified(iter))
        return false;

    iter->cursor = iter->target->rear;

    return true;
}

///
/// \param[in] iter
///
/// \return
bool
qli_iter_has_next(QueueListIterator_t *iter)
{
    return iter->cursor->prev != NULL;
}

///
/// \param[in] iter
/// \param[in] result
///
/// \return
bool
qli_iter_get(QueueListIterator_t *iter, void **result)
{
    if (qli_iter_target_modified(iter))
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
qli_iter_set(QueueListIterator_t *iter, void *element)
{
    if (qli_iter_target_modified(iter))
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
qli_iter_peek_next(QueueListIterator_t *iter)
{
    if (qli_iter_target_modified(iter))
        return NULL;

    if (!qli_iter_has_next(iter))
        return NULL;

    return iter->cursor->prev->data;
}

///
/// \param[in] iter
///
/// \return
void *
qli_iter_peek(QueueListIterator_t *iter)
{
    if (qli_iter_target_modified(iter))
        return NULL;

    return iter->cursor->data;
}

///////////////////////////////////////////////////// NOT EXPOSED FUNCTIONS ///

static bool
qli_iter_target_modified(QueueListIterator_t *iter)
{
    return iter->target_id != iter->target->version_id;
}

////////////////////////////////////////////// END OF NOT EXPOSED FUNCTIONS ///

///////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////// Wrapper ///
///////////////////////////////////////////////////////////////////////////////

/// \todo QueueListWrapper
