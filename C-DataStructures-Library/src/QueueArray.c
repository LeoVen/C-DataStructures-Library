/**
 * @file QueueArray.c
 *
 * @author Leonardo Vencovsky (https://github.com/LeoVen)
 *
 * @date 04/10/2018
 */

#include "QueueArray.h"

/// A QueueArray_s is a buffered Queue_s with FIFO (First-in First-out) or LILO
/// (Last-in Last-out) operations, so the first item added is the first one to
/// be removed. The queue is implemented as a circular buffer. Its indexes can
/// wrap around the buffer if they reach the end. The buffer can also expand.
/// The grow function will first check if there are any items that wrapped
/// around the buffer. If so, it will calculate which portion (left or right)
/// has the least amount of elements. If left is chosen, it will append its
/// contents to the right of the right portion; otherwise it will shift the
/// right portion to the end of the buffer. This effectively decreases the
/// amount of shifts needed.
///
/// \par Advantages over QueueList_s
/// - No need of pointers, only the data is allocated in memory
///
/// \par Drawbacks
/// - When the QueueArray_s is full the buffer needs to be reallocated
/// - When the buffer is reallocated some items might need to be shifted
///
/// \par Functions
/// Located in the file QueueArray.c
struct QueueArray_s
{
    /// \brief Data buffer.
    ///
    /// Buffer where elements are stored in.
    void **buffer;

    /// \brief Front of the queue.
    ///
    /// An index that represents the front of the queue. The \c qar_dequeue
    /// operates relative to this index. This is where the next element to be
    /// removed is located. This index represents the exact position of an the
    /// front element (unlike the \c rear index).
    integer_t front;

    /// \brief Back of the queue.
    ///
    /// An index that represents the back of the queue. The \c qar_enqueue
    /// operates relative to this index. This is where the last element was
    /// added. It does not represents the exact position of an element; it is
    /// always one position ahead (circularly) of the last inserted element.
    /// This might cause confusion; when this index is 0 the real rear element
    /// is at <code> capacity - 1 </code>.
    integer_t rear;

    /// \brief Current amount of elements in the \c QueueArray.
    ///
    /// Current amount of elements in the \c QueueArray.
    integer_t count;

    /// \brief \c QueueArray buffer maximum capacity.
    ///
    /// Buffer maximum capacity. When \c count reaches \c capacity the buffer
    /// is reallocated and increases according to \c growth_rate.
    integer_t capacity;

    /// \brief Buffer growth rate.
    ///
    /// Buffer growth rate. The new buffer capacity is calculated as:
    ///
    /// <code> capacity *= (growth_rate / 100.0) </code>
    integer_t growth_rate;

    /// \brief Flag for locked capacity.
    ///
    /// If \c locked is set to true the buffer will not grow and insertions
    /// won't be successful once the buffer gets filled up.
    bool locked;

    /// \brief QueueArray_s interface.
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
const unsigned_t qar_size = sizeof(QueueArray_t);

///////////////////////////////////////////////////// NOT EXPOSED FUNCTIONS ///

bool
static qar_grow(QueueArray_t *queue);

////////////////////////////////////////////// END OF NOT EXPOSED FUNCTIONS ///

/// Initializes a QueueArray_s with an initial capacity of 32 and a growth rate
/// of 200, that is, twice the size after each growth.
/// \par Interface Requirements
/// - None
///
/// \param[in] interface An interface defining all necessary functions for the
/// queue to operate.
///
/// \return A new QueueArray_s or NULL if allocation failed.
QueueArray_t *
qar_new(Interface_t *interface)
{
    QueueArray_t *queue = malloc(sizeof(QueueArray_t));

    if (!queue)
        return NULL;

    queue->buffer = malloc(sizeof(void*) * 32);

    if (!(queue->buffer))
    {
        free(queue);
        return NULL;
    }

    for (integer_t i = 0; i < 32; i++)
        queue->buffer[i] = NULL;

    queue->capacity = 32;
    queue->growth_rate = 200;
    queue->version_id = 0;
    queue->count = 0;
    queue->front = 0;
    queue->rear = 0;
    queue->locked = false;

    queue->interface = interface;

    return queue;
}

/// Initializes a queue allocated on the stack with a given interface, initial
/// capacity and growth rate.
///
/// \param[in] queue The queue to be initialized.
/// \param[in] initial_capacity Buffer initial capacity.
/// \param[in] growth_rate Buffer growth rate.
/// \param[in] interface An interface defining all necessary functions for the
/// deque to operate.
///
/// \return True if the queue was successfully initialized or false if the
/// growth rate is less than 101, if the initial capacity is 0 or if allocation
/// failed.
bool
qar_init(QueueArray_t *queue, Interface_t *interface,
         integer_t initial_capacity, integer_t growth_rate)
{
    if (growth_rate <= 100 || initial_capacity <= 0)
        return false;

    queue->buffer = malloc(sizeof(void*) * (size_t)initial_capacity);

    if (!(queue->buffer))
        return false;

    for (integer_t i = 0; i < initial_capacity; i++)
        queue->buffer[i] = NULL;

    queue->capacity = initial_capacity;
    queue->growth_rate = growth_rate;
    queue->version_id = 0;
    queue->count = 0;
    queue->front = 0;
    queue->rear = 0;
    queue->locked = false;

    queue->interface = interface;

    return true;
}

/// Initializes a QueueArray_s with a user defined \c initial_capacity and \c
/// growth_rate. This function only accepts an \c initial_capacity greater than
/// 0 and a \c growth_rate greater than 100; but keep in mind that in some
/// cases if the \c initial_capacity is too small and the \c growth_rate is too
/// close to 100 there won't be an increase in capacity and the minimum growth
/// will be triggered.
/// \par Interface Requirements
/// - None
///
/// \param[in] interface An interface defining all necessary functions for the
/// queue to operate.
/// \param[in] initial_capacity Buffer initial capacity.
/// \param[in] growth_rate Buffer growth rate.
///
/// \return NULL if the growth rate is less than 101, if the initial capacity
/// is 0 or if allocation failed.
/// \return A new QueueArray_s dynamically allocated.
QueueArray_t *
qar_create(Interface_t *interface, integer_t initial_capacity,
           integer_t growth_rate)
{
    if (growth_rate <= 100 || initial_capacity <= 0)
        return NULL;

    QueueArray_t *queue = malloc(sizeof(QueueArray_t));

    if (!queue)
        return NULL;

    queue->buffer = malloc(sizeof(void*) * (size_t)initial_capacity);

    if (!(queue->buffer))
    {
        free(queue);
        return NULL;
    }

    for (integer_t i = 0; i < initial_capacity; i++)
        queue->buffer[i] = NULL;

    queue->capacity = initial_capacity;
    queue->growth_rate = growth_rate;
    queue->version_id = 0;
    queue->count = 0;
    queue->front = 0;
    queue->rear = 0;
    queue->locked = false;

    queue->interface = interface;

    return queue;
}

/// Frees each element at the queue using its interface's \c free and then
/// frees the queue struct.
/// \par Interface Requirements
/// - free
///
/// \param[in] queue The queue to be freed from memory.
void
qar_free(QueueArray_t *queue)
{
    for (integer_t i = queue->front, j = 0;
            j < queue->count;
            i = (i + 1) % queue->capacity, j++)
    {
        queue->interface->free(queue->buffer[i]);
    }

    free(queue->buffer);

    free(queue);
}

/// Frees the QueueArray_s structure and leaves all the elements intact. Be
/// careful as this might cause severe memory leaks. Only use this if your
/// queue elements are also handled by another structure or algorithm.
/// \par Interface Requirements
/// - None
///
/// \param[in] queue The queue to be freed from memory.
void
qar_free_shallow(QueueArray_t *queue)
{
    free(queue->buffer);

    free(queue);
}

/// This function will reset the QueueArray_s, freeing all of its elements,
/// keeping the structure intact including its original interface.
/// \par Interface Requirements
/// - free
///
/// \param[in] queue The queue to have its elements erased.
void
qar_erase(QueueArray_t *queue)
{
    for (integer_t i = queue->front, j = 0;
         j < queue->count;
         i = (i + 1) % queue->capacity, j++)
    {
        queue->interface->free(queue->buffer[i]);

        queue->buffer[i] = NULL;
    }

    queue->count = 0;
    queue->version_id++;
    queue->front = 0;
    queue->rear = 0;
}

/// This functions will reset the QueueArray_s without freeing its elements.
/// This will keep its original interface and its original buffer size.
///
/// \param[in] queue The queue to be reset.
void
qar_erase_shallow(QueueArray_t *queue)
{
    for (integer_t i = queue->front, j = 0;
         j < queue->count;
         i = (i + 1) % queue->capacity, j++)
    {
        queue->buffer[i] = NULL;
    }

    queue->count = 0;
    queue->version_id++;
    queue->front = 0;
    queue->rear = 0;
}

/// Sets a new interface for the specified QueueArray_s.
/// \par Interface Requirements
/// - None
///
/// \param[in] queue QueueArray_s to change the interface.
/// \param[in] new_interface New interface for the specified structure.
void
qar_config(QueueArray_t *queue, Interface_t *new_interface)
{
    queue->interface = new_interface;
}

/// Returns the current amount of elements in the specified queue.
/// \par Interface Requirements
/// - None
///
/// \param[in] queue The target queue.
///
/// \return The amount of elements in the queue.
integer_t
qar_count(QueueArray_t *queue)
{
    return queue->count;
}

/// Returns the current buffer's size of the specified queue.
/// \par Interface Requirements
/// - None
///
/// \param[in] queue The target queue.
///
/// \return The queue's buffer's size.
integer_t
qar_capacity(QueueArray_t *queue)
{
    return queue->capacity;
}

/// Returns the buffer's growth rate of the specified queue.
/// \par Interface Requirements
/// - None
///
/// \param[in] queue The target queue.
///
/// \return The buffer's growth rate.
integer_t
qar_growth(QueueArray_t *queue)
{
    return queue->growth_rate;
}

/// Returns the boolean state of \c locked member.
/// \par Interface Requirements
/// - None
///
/// \param[in] queue The target queue.
///
/// \return True if the buffer's growth is locked, false otherwise.
bool
qar_locked(QueueArray_t *queue)
{
    return queue->locked;
}

/// Sets a new growth_rate to the target queue.
/// \par Interface Requirements
/// - None
///
/// \param[in] queue The target queue.
/// \param[in] growth_rate Queue's new growth_rate.
///
/// \return True if the growth rate was change or false if the parameter is
/// less than 101.
bool
qar_set_growth(QueueArray_t *queue, integer_t growth_rate)
{
    if (growth_rate <= 100)
        return false;

    queue->growth_rate = growth_rate;

    return true;
}

/// Locks the the target's buffer growth. If the buffer is full no more
/// elements will be added to the queue until its capacity is unlocked or
/// another element is removed.
///
/// \param[in] queue The queue to have its buffer's growth locked.
void
qar_capacity_lock(QueueArray_t *queue)
{
    queue->locked = true;
}

/// Unlocks the buffer's capacity allowing it to be reallocated once full.
///
/// \param[in] queue The queue to have its buffer's growth unlocked.
void
qar_capacity_unlock(QueueArray_t *queue)
{
    queue->locked = false;
}

/// Inserts an element into the specified queue. The element is added at the
/// \c rear index.
/// \par Interface Requirements
/// - None
///
/// \param[in] queue The queue where the element is to be inserted.
/// \param[in] element The element to be inserted in the queue.
///
/// \return True if the element was successfully added to the queue or false if
/// the buffer reallocation failed or the queue buffer capacity is locked.
bool
qar_enqueue(QueueArray_t *queue, void *element)
{
    if (qar_full(queue))
    {
        if (!qar_grow(queue))
            return false;
    }

    queue->buffer[queue->rear] = element;

    queue->rear = (queue->rear == queue->capacity - 1) ? 0 : queue->rear + 1;

    queue->count++;
    queue->version_id++;

    return true;
}

/// Removes an element from the specified queue. The element is removed from
/// the \c front index.
/// \par Interface Requirements
/// - None
///
/// \param[in] queue The queue where the element is to be removed from.
/// \param[out] result The resulting element removed from the queue.
///
/// \return True if an element was removed from the queue or false if the queue
/// is empty.
bool
qar_dequeue(QueueArray_t *queue, void **result)
{
    *result = NULL;

    if (qar_empty(queue))
        return false;

    *result = queue->buffer[queue->front];

    queue->buffer[queue->front] = NULL;

    queue->front = (queue->front == queue->capacity - 1) ? 0 : queue->front +1;

    queue->count--;
    queue->version_id++;

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
qar_peek_front(QueueArray_t *queue)
{
    if (qar_empty(queue))
        return NULL;

    return queue->buffer[queue->front];
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
qar_peek_rear(QueueArray_t *queue)
{
    if (qar_empty(queue))
        return NULL;

    integer_t i = (queue->rear == 0) ? queue->capacity - 1 : queue->rear - 1;

    return queue->buffer[i];
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
qar_empty(QueueArray_t *queue)
{
    return queue->count == 0;
}

/// Returns true if the current amount of elements in the queue is the same as
/// the buffer's capacity, that is, the next element to be added to the queue
/// will cause the buffer to be reallocated.
/// \par Interface Requirements
/// - None
///
/// \param[in] queue The target queue.
///
/// \return True if the amount of elements is the same as the buffer's
/// capacity, otherwise false.
bool
qar_full(QueueArray_t *queue)
{
    return queue->count == queue->capacity;
}

/// Returns true if the specified size will fit in the queue's buffer without
/// it being reallocated.
///
/// \param[in] queue The target queue.
/// \param[in] size The specified size.
///
/// \return True if a given size fits inside the queue without reallocating the
/// buffer.
bool
qar_fits(QueueArray_t *queue, unsigned_t size)
{
    return (queue->count + size) <= queue->capacity;
}

/// Returns true if the element is present in the queue, otherwise false.
/// \par Interface Requirements
/// - compare
///
/// \param[in] queue QueueArray_s reference.
/// \param[in] key Key to be matched.
///
/// \return True if the element is present in the queue, otherwise false.
bool
qar_contains(QueueArray_t *queue, void *key)
{
    for (integer_t i = queue->front, j = 0;
         j < queue->count;
         i = (i + 1) % queue->capacity, j++)
    {
        if (queue->interface->compare(queue->buffer[i], key) == 0)
            return true;
    }

    return false;
}

/// Returns a copy of the specified QueueArray_s with the same interface. All
/// elements are copied using the queue's interface's copy function.
/// \par Interface Requirements
/// - copy
///
/// \param[in] queue The queue to be copied.
///
/// \return NULL if allocation failed or a copy of the specified queue.
QueueArray_t *
qar_copy(QueueArray_t *queue)
{
    QueueArray_t *new_queue = qar_create(queue->interface, queue->capacity,
                                         queue->growth_rate);

    if (!new_queue)
        return NULL;

    for (integer_t i = queue->front, j = 0;
         j < queue->count - 1;
         i = (i + 1) % queue->capacity, j++)
    {
        new_queue->buffer[i] = queue->interface->copy(queue->buffer[i]);
    }

    new_queue->front = queue->front;
    new_queue->rear = queue->rear;
    new_queue->count = queue->count;
    new_queue->locked = queue->locked;

    return new_queue;
}

/// Creates a shallow copy of all elements in the queue, that is, only the
/// pointers addresses are copied to the new queue.
/// \par Interface Requirements
/// - None
///
/// \param[in] queue The queue to be copied.
///
/// \return NULL if allocation failed or a shallow copy of the specified queue.
QueueArray_t *
qar_copy_shallow(QueueArray_t *queue)
{
    QueueArray_t *new_queue = qar_create(queue->interface, queue->capacity,
                                         queue->growth_rate);

    if (!new_queue)
        return NULL;

    for (integer_t i = queue->front, j = 0;
         j < queue->count - 1;
         i = (i + 1) % queue->capacity, j++)
    {
        new_queue->buffer[i] = queue->buffer[i];
    }

    new_queue->front = queue->front;
    new_queue->rear = queue->rear;
    new_queue->count = queue->count;
    new_queue->locked = queue->locked;

    return new_queue;
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
qar_compare(QueueArray_t *queue1, QueueArray_t *queue2)
{
    integer_t max_count = queue1->count < queue2->count
                         ? queue1->count
                         : queue2->count;

    int comparison = 0;
    for (integer_t i = 0; i < max_count; i++)
    {
        // Since its a circular buffer we need to calculate where the ith
        // element of each queue is.
        comparison = queue1->interface->compare(
                queue1->buffer[(i + queue1->front) % queue1->capacity],
                queue2->buffer[(i + queue2->front) % queue2->capacity]);
        if (comparison > 0)
            return 1;
        else if (comparison < 0)
            return -1;
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
qar_append(QueueArray_t *queue1, QueueArray_t *queue2)
{
    /// \todo qar_append
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
qar_to_array(QueueArray_t *queue, integer_t *length)
{
    if (qar_empty(queue))
        return NULL;

    void **array = malloc(sizeof(void*) * (size_t)queue->count);

    if (!array)
        return NULL;

    for (integer_t i = queue->front, j = 0;
         j < queue->count;
         i = (i + 1) % queue->capacity, j++)
    {
        array[i] = queue->interface->copy(queue->buffer[i]);
    }

    *length = queue->count;

    return array;
}

/// Displays a QueueArray_s in the console starting from the front element to
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
qar_display(QueueArray_t *queue, int display_mode)
{
    if (qar_empty(queue))
    {
        printf("\nQueueArray\n[ empty ]\n");
        return;
    }

    switch (display_mode)
    {
        case -1:
            printf("\nQueueArray\n");
            for (integer_t i = queue->front, j = 0;
                 j < queue->count;
                 i = (i + 1) % queue->capacity, j++)
            {
                queue->interface->display(queue->buffer[i]);
                printf("\n");
            }
            break;
        case 0:
            printf("\nQueueArray\nFront -> ");
            for (integer_t i = queue->front, j = 0;
                 j < queue->count - 1;
                 i = (i + 1) % queue->capacity, j++)
            {
                queue->interface->display(queue->buffer[i]);
                printf(" -> ");
            }
            queue->interface->display(queue->buffer[
                    (queue->rear == 0) ? queue->capacity - 1 : queue->rear - 1
                    ]);
            printf(" -> Rear\n");
            break;
        case 1:
            printf("\nQueueArray\n");
            for (integer_t i = queue->front, j = 0;
                 j < queue->count;
                 i = (i + 1) % queue->capacity, j++)
            {
                queue->interface->display(queue->buffer[i]);
                printf(" ");
            }
            printf("\n");
            break;
        default:
            printf("\nQueueArray\n[ ");
            for (integer_t i = queue->front, j = 0;
                 j < queue->count - 1;
                 i = (i + 1) % queue->capacity, j++)
            {
                queue->interface->display(queue->buffer[i]);
                printf(", ");
            }
            queue->interface->display(queue->buffer[
                    (queue->rear == 0) ? queue->capacity - 1 : queue->rear - 1
                    ]);
            printf(" ]\n");
            break;
    }
}

///////////////////////////////////////////////////// NOT EXPOSED FUNCTIONS ///

// This function reallocates the data buffer effectively increasing its
// capacity
bool
static qar_grow(QueueArray_t *queue)
{
    if (queue->locked)
        return false;

    integer_t old_capacity = queue->capacity;

    // capacity = capacity * (growth_rate / 100)
    queue->capacity = (integer_t) ((double) (queue->capacity) *
            ((double) (queue->growth_rate) / 100.0));

    // 4 is the minimum growth
    if (queue->capacity - old_capacity < 4)
        queue->capacity = old_capacity + 4;

    void **new_buffer = realloc(queue->buffer,
            sizeof(void*) * (size_t)queue->capacity);

    // Reallocation failed
    if (!new_buffer)
    {
        queue->capacity = old_capacity;
        return false;
    }

    queue->buffer = new_buffer;

    integer_t real_rear = (queue->rear == 0)
            ? old_capacity - 1
            : queue->rear - 1;

    // Shift elements if the rear index wrapped around the buffer
    if (real_rear < queue->front)
    {
        // When the buffer is full the rear index equals the front index. In
        // order to shift elements effectively this calculates which portion of
        // the buffer has the least amount of elements.
        // Shift the right portion to the end of the buffer if the total
        // elements are less than the left portion; otherwise append the left
        // portion to the right portion.
        if (old_capacity - queue->front < queue->rear)
        {
            integer_t distance = old_capacity - queue->front;

            for (integer_t i = old_capacity - 1, j = queue->capacity - 1;
                    i >= queue->front; i--, j--)
            {
                queue->buffer[j] = queue->buffer[i];
            }

            queue->front = queue->capacity - distance;
        }
        // If the growth rate is less than 150 the rear index might wrap around
        // the buffer again
        else
        {
            for (integer_t i = 0, j = old_capacity; i < queue->rear;
                    i++, j = (j + 1) % queue->capacity)
            {
                queue->buffer[j] = queue->buffer[i];
            }

            queue->rear = (old_capacity + queue->rear) % queue->capacity;
        }
    }
    // This case only happens when qua->front == 0 and qua->rear == 0
    // The rear index has wrapped around but the buffer increased in size
    // allowing the rear index to keep increasing instead of wrapping around.
    // This can be achieved by enqueueing elements without ever dequeueing any.
    else if (queue->rear == 0)
    {
        queue->rear = old_capacity;
    }
    // This should never happen. This function should never be called when the
    // buffer is not full (front == rear and count == capacity).
    else
        return false;

    return true;
}

////////////////////////////////////////////// END OF NOT EXPOSED FUNCTIONS ///

///////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////// Iterator ///
///////////////////////////////////////////////////////////////////////////////

/// \todo QueueArrayIterator

///////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////// Wrapper ///
///////////////////////////////////////////////////////////////////////////////

/// \todo QueueArrayWrapper
