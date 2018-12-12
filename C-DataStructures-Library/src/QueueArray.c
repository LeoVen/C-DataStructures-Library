/**
 * @file QueueArray.c
 * 
 * @author Leonardo Vencovsky (https://github.com/LeoVen)
 *
 * @date 04/10/2018
 */

#include "QueueArray.h"

/// A QueueArray is a buffered Queue with FIFO (First-in First-out) or LILO
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
/// \par Advantages over Queue
/// - No need of pointers, only the data is allocated in memory
///
/// \par Drawbacks
/// - When the \c QueueArray is full the buffer needs to be reallocated
/// - When the buffer is reallocated some items might need to be shifted
///
/// \par Functions
///
/// Located in the file QueueArray.c
struct QueueArray_s
{
    /// \brief Data buffer.
    ///
    /// Buffer where elements are stored in.
    void **buffer;

    /// \brief Front of the queue.
    ///
    /// An index that represents the front of the queue. The \c qua_dequeue
    /// operates relative to this index. This is where the next element to be
    /// removed is located. This index represents the exact position of an the
    /// front element (unlike the \c rear index).
    integer_t front;

    /// \brief Back of the queue.
    ///
    /// An index that represents the back of the queue. The \c qua_enqueue
    /// operates relative to this index. This is where the last element was
    /// added. It does not represents the exact position of an element; it is
    /// always one position ahead (circularly) of the last inserted element.
    /// This might cause confusion; when this index is 0 the real rear element
    /// is at <code> capacity - 1 </code>.
    integer_t rear;

    /// \brief Current amount of elements in the \c QueueArray.
    ///
    /// Current amount of elements in the \c QueueArray.
    integer_t size;

    /// \brief \c QueueArray buffer maximum capacity.
    ///
    /// Buffer maximum capacity. When \c size reaches \c capacity the buffer is
    /// reallocated and increases according to \c growth_rate.
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

///////////////////////////////////////////////////// NOT EXPOSED FUNCTIONS ///

bool
static qua_grow(QueueArray_t *queue);

////////////////////////////////////////////// END OF NOT EXPOSED FUNCTIONS ///

/// Initializes a QueueArray_s with an initial capacity of 32 and a growth rate
/// of 200, that is, twice the size after each growth.
///
/// \param[in] interface The interface for your QueueArray_s.
///
/// \return A new QueueArray_s or NULL if allocation failed.
QueueArray_t *
qua_new(Interface_t *interface)
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

    queue->capacity = 32;
    queue->growth_rate = 200;
    queue->size = 0;
    queue->front = 0;
    queue->rear = 0;
    queue->locked = false;

    queue->interface = interface;

    return queue;
}

/// Initializes a QueueArray_s with a user defined \c initial_capacity and \c
/// growth_rate. This function only accepts an \c initial_capacity greater than
/// 0 and a \c growth_rate greater than 100; but keep in mind that in some
/// cases if the \c initial_capacity is too small and the \c growth_rate is too
/// close to 100 there won't be an increase in capacity and the minimum growth
/// will be triggered.
///
/// \param[in] initial_capacity
/// \param[in] growth_rate
/// \param[in] interface
///
/// \return
QueueArray_t *
qua_create(integer_t initial_capacity, integer_t growth_rate,
           Interface_t *interface)
{
    if (growth_rate <= 100 || initial_capacity == 0)
        return NULL;

    QueueArray_t *queue = malloc(sizeof(QueueArray_t));

    if (!queue)
        return NULL;

    queue->buffer = malloc(sizeof(void*) * initial_capacity);

    if (!(queue->buffer))
    {
        free(queue);
        return NULL;
    }

    queue->capacity = initial_capacity;
    queue->growth_rate = growth_rate;
    queue->size = 0;
    queue->front = 0;
    queue->rear = 0;
    queue->locked = false;

    queue->interface = interface;

    return queue;
}

/// Frees each element at the queue using its interface's \c free and then
/// frees the queue struct.
///
/// \param[in] queue The queue to be freed from memory.
void
qua_free(QueueArray_t *queue)
{
    for (integer_t i = queue->front, j = 0;
            j < queue->size;
            i = (i + 1) % queue->capacity, j++)
    {
        queue->interface->free(queue->buffer[i]);
    }

    free(queue->buffer);

    free(queue);
}

void
qua_free_shallow(QueueArray_t *queue)
{
    free(queue->buffer);

    free(queue);
}

/// This function will free all the elements of the specified QueueArray_s and
/// will keep the structure intact.
///
/// \param[in] queue queue queue to be erased.
///
/// \return True if all operations were successful.
bool
qua_erase(QueueArray_t *queue)
{
    for (integer_t i = queue->front, j = 0;
         j < queue->size - 1;
         i = (i + 1) % queue->capacity, j++)
    {
        queue->interface->free(queue->buffer[i]);

        queue->buffer[i] = NULL;
    }

    return true;
}

void
qua_config(QueueArray_t *queue, Interface_t *new_interface)
{
    queue->interface = new_interface;
}

integer_t
qua_size(QueueArray_t *queue)
{
    return queue->size;
}

integer_t
qua_capacity(QueueArray_t *queue)
{
    return queue->capacity;
}

integer_t
qua_growth(QueueArray_t *queue)
{
    return queue->growth_rate;
}

bool
qua_set_growth(QueueArray_t *queue, integer_t growth_rate)
{
    if (growth_rate <= 100)
        return false;

    queue->growth_rate = growth_rate;

    return true;
}

/// Inserts an element into the specified queue. The element is added at the
/// \c rear index.
///
/// \param[in] queue The queue where the element is to be inserted.
/// \param[in] element The element to be inserted in the queue.
///
/// \return True if the element was successfully added to the queue or false if
/// the buffer reallocation failed or the queue buffer capacity is locked.
bool
qua_enqueue(QueueArray_t *queue, void *element)
{
    if (qua_full(queue))
    {
        if (!qua_grow(queue))
            return false;
    }

    queue->buffer[queue->rear] = element;

    queue->rear = (queue->rear == queue->capacity - 1) ? 0 : queue->rear + 1;

    queue->size++;
    queue->version_id++;

    return true;
}

/// Removes an element from the specified queue. The element is removed from
/// the \c front index.
///
/// \param[in] queue The queue where the element is to be removed from.
/// \param[out] result The resulting element removed from the queue.
///
/// \return True if an element was removed from the queue or false if the queue
/// is empty.
bool
qua_dequeue(QueueArray_t *queue, void **result)
{
    *result = NULL;

    if (qua_empty(queue))
        return false;

    *result = queue->buffer[queue->front];

    queue->buffer[queue->front] = NULL;

    queue->front = (queue->front == queue->capacity - 1) ? 0 : queue->front +1;

    queue->size--;
    queue->version_id++;

    return true;
}

void *
qua_peek_front(QueueArray_t *queue)
{
    if (qua_empty(queue))
        return NULL;

    return queue->buffer[queue->front];
}

void *
qua_peek_rear(QueueArray_t *queue)
{
    if (qua_empty(queue))
        return NULL;

    integer_t i = (queue->rear == 0) ? queue->capacity - 1 : queue->rear - 1;

    return queue->buffer[i];
}

bool
qua_empty(QueueArray_t *queue)
{
    return queue->size == 0;
}

bool
qua_full(QueueArray_t *queue)
{
    return queue->size == queue->capacity;
}

bool
qua_fits(QueueArray_t *queue, unsigned_t size)
{
    return (queue->size + size) <= queue->capacity;
}

void
qua_capacity_lock(QueueArray_t *queue)
{
    queue->locked = true;
}

void
qua_capacity_unlock(QueueArray_t *queue)
{
    queue->locked = false;
}

/// Returns a copy of the specified QueueArray_s with the same interface. All
/// elements are copied using the queue's interface's copy function.
/// \par Interface Requirements
/// - copy
///
/// \param queue The queue to be copied.
///
/// \return \c NULL if allocation failed or a copy of the specified queue.
QueueArray_t *
qua_copy(QueueArray_t *queue)
{
    QueueArray_t *new_queue = qua_create(queue->capacity, queue->growth_rate,
                                         queue->interface);

    if (!new_queue)
        return NULL;

    for (integer_t i = queue->front, j = 0; j < queue->size - 1;
            i = (i + 1) % queue->capacity, j++)
    {
        new_queue->buffer[i] = queue->interface->copy(queue->buffer[i]);
    }

    new_queue->front = queue->front;
    new_queue->rear = queue->rear;
    new_queue->size = queue->size;
    new_queue->locked = queue->locked;

    return new_queue;
}

/// Creates a shallow copy of all elements in the queue, that is, only the
/// pointers addresses are copied to the new queue.
/// \par Interface Requirements
/// - None
///
/// \param queue The queue to be copied.
///
/// \return \c NULL if allocation failed or a shallow copy of the specified
/// queue.
QueueArray_t *
qua_copy_shallow(QueueArray_t *queue)
{
    QueueArray_t *new_queue = qua_create(queue->capacity, queue->growth_rate,
                                         queue->interface);

    if (!new_queue)
        return NULL;

    for (integer_t i = queue->front, j = 0; j < queue->size - 1;
         i = (i + 1) % queue->capacity, j++)
    {
        new_queue->buffer[i] = queue->buffer[i];
    }

    new_queue->front = queue->front;
    new_queue->rear = queue->rear;
    new_queue->size = queue->size;
    new_queue->locked = queue->locked;

    return new_queue;
}

/// Compares two queues.
/// \par Interface Requirements
/// - compare
///
/// \param queue1
/// \param queue2
///
/// \return
int
qua_compare(QueueArray_t *queue1, QueueArray_t *queue2)
{
    integer_t max_size = queue1->size < queue2->size
                         ? queue1->size
                         : queue2->size;

    int comparison = 0;
    for (integer_t i = 0; i < max_size; i++)
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
    if (queue1->size > queue2->size)
        return 1;
    else if (queue1->size < queue2->size)
        return -1;

    return 0;
}

void **
qua_to_array(QueueArray_t *queue, integer_t *size)
{
    /// \todo qua_to_array
    return NULL;
}

/// Displays a QueueArray_s in the console. There are currently four modes:
/// - -1 Displays each element separated by newline;
/// -  0 Displays each element like a linked list;
/// -  1 Displays each element separated by a space;
/// - Any other number defaults to the array representation.
///
/// \param queue The queue to be displayed in the console.
/// \param display_mode The way the queue is to be displayed in the console.
void
qua_display(QueueArray_t *queue, int display_mode)
{
    if (qua_empty(queue))
    {
        printf("\nQueueArray\n[ empty ]\n");
        return;
    }

    switch (display_mode)
    {
        case -1:
            printf("\nStackArray\n");
            for (integer_t i = queue->front, j = 0; j < queue->size - 1;
                 i = (i + 1) % queue->capacity, j++)
            {
                queue->interface->display(queue->buffer[i]);
                printf("\n");
            }
            break;
        case 0:
            printf("\nStackArray\nFront -> ");
            for (integer_t i = queue->front, j = 0; j < queue->size - 1;
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
            printf("\nStackArray\n");
            for (integer_t i = queue->front, j = 0; j < queue->size - 1;
                 i = (i + 1) % queue->capacity, j++)
            {
                queue->interface->display(queue->buffer[i]);
                printf(" ");
            }
            printf("\n");
            break;
        default:
            printf("\nStackArray\n[ ");
            for (integer_t i = queue->front, j = 0; j < queue->size - 1;
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
static qua_grow(QueueArray_t *queue)
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

    void **new_buffer = realloc(queue->buffer,sizeof(void*) * queue->capacity);

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
    // buffer is not full (front == rear and size == capacity).
    else
        return false;

    return true;
}

////////////////////////////////////////////// END OF NOT EXPOSED FUNCTIONS ///

///////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////// Iterator ///
///////////////////////////////////////////////////////////////////////////////

/// \todo StackArrayIterator

///////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////// Wrapper ///
///////////////////////////////////////////////////////////////////////////////

/// \todo StackArrayWrapper