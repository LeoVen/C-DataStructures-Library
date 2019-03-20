/**
 * @file DequeArray.c
 *
 * @author Leonardo Vencovsky (https://github.com/LeoVen)
 *
 * @date 29/10/2018
 */

#include "DequeArray.h"

/// A DequeArray_s is a buffered Deque_s with enqueue and dequeue operations on
/// both ends that are represented by indexes. The deque is implemented as a
/// circular buffer. Its indexes can wrap around the buffer if they reach
/// either ends. The buffer can also expand. The grow function will first check
/// if there are any items that wrapped around the buffer. If so, it will
/// calculate which portion (left or right) has the least amount of elements.
/// If left is chosen, it will append its contents to the right of the right
/// portion; otherwise it will shift the right portion to the end of the
/// buffer. This effectively decreases the amount of shifts needed.
///
/// \par Advantages over DequeList_s
/// - Fast insertion
/// - No need of pointers, only the data is allocated in memory
///
/// \par Drawbacks
/// - When the DequeArray_s is full the buffer needs to be reallocated
/// - When the buffer is reallocated some items might need to be shifted
///
/// \par Functions
/// Located in the file DequeArray.c
struct DequeArray_s
{
    /// \brief Data buffer.
    ///
    /// Buffer where elements are stored in.
    void **buffer;

    /// \brief Front of the deque.
    ///
    /// An index that represents the front of the deque.
    integer_t front;

    /// \brief Back of the deque.
    ///
    /// An index that represents the back of the deque.
    integer_t rear;

    /// \brief Current amount of elements in the DequeArray_s.
    ///
    /// Current amount of elements in the DequeArray_s.
    integer_t count;

    /// \brief \c QueueArray buffer maximum capacity.
    ///
    /// Buffer maximum capacity. When \c count reaches \c capacity the buffer is
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

    /// \brief DequeArray_s interface.
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
const unsigned_t dqa_size = sizeof(DequeArray_t);

///////////////////////////////////////////////////// NOT EXPOSED FUNCTIONS ///

bool
static dqa_grow(DequeArray_t *deque);

////////////////////////////////////////////// END OF NOT EXPOSED FUNCTIONS ///

/// Initializes a DequeArray_s with an initial capacity of 32 and a growth rate
/// of 200, that is, twice the count after each growth.
/// \par Interface Requirements
/// - None
///
/// \param[in] interface An interface defining all necessary functions for the
/// deque to operate.
///
/// \return A new DequeArray_s or NULL if allocation failed.
DequeArray_t *
dqa_new(Interface_t *interface)
{
    DequeArray_t *deque = malloc(sizeof(DequeArray_t));

    if (!deque)
        return NULL;

    deque->buffer = malloc(sizeof(void*) * 32);

    if (!(deque->buffer))
    {
        free(deque);
        return NULL;
    }

    for (integer_t i = 0; i < 32; i++)
        deque->buffer[i] = NULL;

    deque->capacity = 32;
    deque->growth_rate = 200;
    deque->version_id = 0;
    deque->count = 0;
    deque->front = 0;
    deque->rear = 0;
    deque->locked = false;

    deque->interface = interface;

    return deque;
}

/// Initializes a deque allocated on the stack with a given interface, initial
/// capacity and growth rate.
///
/// \param[in] deque The deque to be initialized.
/// \param[in] initial_capacity Buffer initial capacity.
/// \param[in] growth_rate Buffer growth rate.
/// \param[in] interface An interface defining all necessary functions for the
/// deque to operate.
///
/// \return True if the deque was successfully initialized or false if the
/// growth rate is less than 101, if the initial capacity is 0 or if allocation
/// failed.
bool
dqa_init(DequeArray_t *deque, Interface_t *interface,
         integer_t initial_capacity, integer_t growth_rate)
{
    if (growth_rate <= 100 || initial_capacity <= 0)
        return false;

    deque->buffer = malloc(sizeof(void*) * (size_t)initial_capacity);

    if (!deque->buffer)
        return false;

    for (integer_t i = 0; i < initial_capacity; i++)
        deque->buffer[i] = NULL;

    deque->capacity = initial_capacity;
    deque->growth_rate = growth_rate;
    deque->version_id = 0;
    deque->count = 0;
    deque->front = 0;
    deque->rear = 0;
    deque->locked = false;
    deque->interface = interface;

    return true;
}

/// Initializes a DequeArray_s with a user defined \c initial_capacity and \c
/// growth_rate. This function only accepts an \c initial_capacity greater than
/// 0 and a \c growth_rate greater than 100; but keep in mind that in some
/// cases if the \c initial_capacity is too small and the \c growth_rate is too
/// close to 100 there won't be an increase in capacity and the minimum growth
/// will be triggered.
/// \par Interface Requirements
/// - None
///
/// \param[in] initial_capacity Buffer initial capacity.
/// \param[in] growth_rate Buffer growth rate.
/// \param[in] interface An interface defining all necessary functions for the
/// deque to operate.
///
/// \return NULL if the growth rate is less than 101, if the initial capacity
/// is 0 or if allocation failed.
/// \return A new DequeArray_s dynamically allocated.
DequeArray_t *
dqa_create(Interface_t *interface, integer_t initial_capacity,
           integer_t growth_rate)
{
    if (growth_rate <= 100 || initial_capacity <= 0)
        return NULL;

    DequeArray_t *deque = malloc(sizeof(DequeArray_t));

    if (!deque)
        return NULL;

    deque->buffer = malloc(sizeof(void*) * (size_t)initial_capacity);

    if (!(deque->buffer))
    {
        free(deque);
        return NULL;
    }

    for (integer_t i = 0; i < initial_capacity; i++)
        deque->buffer[i] = NULL;

    deque->capacity = initial_capacity;
    deque->growth_rate = growth_rate;
    deque->version_id = 0;
    deque->count = 0;
    deque->front = 0;
    deque->rear = 0;
    deque->locked = false;

    deque->interface = interface;

    return deque;
}

/// Frees each element at the deque using its interface's \c free and then
/// frees the deque struct.
/// \par Interface Requirements
/// - free
///
/// \param[in] deque The deque to be freed from memory.
void
dqa_free(DequeArray_t *deque)
{
    for (integer_t i = deque->front, j = 0;
         j < deque->count;
         i = (i + 1) % deque->capacity, j++)
    {
        deque->interface->free(deque->buffer[i]);
    }

    free(deque->buffer);

    free(deque);
}

/// Frees the DequeArray_s structure and leaves all the elements intact. Be
/// careful as this might cause severe memory leaks. Only use this if your
/// deque elements are also handled by another structure or algorithm.
/// \par Interface Requirements
/// - None
///
/// \param[in] deque The deque to be freed from memory.
void
dqa_free_shallow(DequeArray_t *deque)
{
    free(deque->buffer);

    free(deque);
}

/// This function will reset the DequeArray_s, freeing all of its nodes along
/// with its elements, keeping the structure intact including its original
/// interface.
/// \par Interface Requirements
/// - free
///
/// \param[in] deque The deque to have its elements erased.
void
dqa_erase(DequeArray_t *deque)
{
    for (integer_t i = deque->front, j = 0;
         j < deque->count;
         i = (i + 1) % deque->capacity, j++)
    {
        deque->interface->free(deque->buffer[i]);

        deque->buffer[i] = NULL;
    }

    deque->count = 0;
    deque->front = 0;
    deque->rear = 0;
    deque->version_id++;
}

/// This function will reset the DequeArray_s, freeing all of its nodes but not
/// its elements, keeping the structure intact including its original
/// interface.
/// \par Interface Requirements
/// - None
///
/// \param[in] deque The deque to have its nodes erased.
void
dqa_erase_shallow(DequeArray_t *deque)
{
    for (integer_t i = deque->front, j = 0;
         j < deque->count;
         i = (i + 1) % deque->capacity, j++)
    {
        deque->buffer[i] = NULL;
    }

    deque->count = 0;
    deque->front = 0;
    deque->rear = 0;
    deque->version_id++;
}

/// Sets a new interface for the specified DequeArray_s.
/// \par Interface Requirements
/// - None
///
/// \param[in] deque DequeArray_s to change the interface.
/// \param[in] new_interface New interface for the specified structure.
void
dqa_config(DequeArray_t *deque, Interface_t *new_interface)
{
    deque->interface = new_interface;
}

/// Returns the current amount of elements in the specified deque.
/// \par Interface Requirements
/// - None
///
/// \param[in] deque The target deque.
///
/// \return The amount of elements in the deque.
integer_t
dqa_count(DequeArray_t *deque)
{
    return deque->count;
}

/// Returns the current buffer's size of the specified deque.
/// \par Interface Requirements
/// - None
///
/// \param[in] deque The target deque.
///
/// \return The deque's buffer's size.
integer_t
dqa_capacity(DequeArray_t *deque)
{
    return deque->capacity;
}

/// Returns the buffer's growth rate of the specified deque.
/// \par Interface Requirements
/// - None
///
/// \param[in] deque The target deque.
///
/// \return The buffer's growth rate.
integer_t
dqa_growth(DequeArray_t *deque)
{
    return deque->growth_rate;
}

/// Returns the boolean state of \c locked member.
/// \par Interface Requirements
/// - None
///
/// \param[in] deque The target deque.
///
/// \return True if the buffer's growth is locked, false otherwise.
bool
dqa_locked(DequeArray_t *deque)
{
    return deque->locked;
}

/// Sets a new growth_rate to the target deque.
/// \par Interface Requirements
/// - None
///
/// \param[in] deque The target deque.
/// \param[in] growth_rate Deque's new growth_rate.
///
/// \return True if the growth rate was change or false if the parameter is
/// less than 101.
bool
dqa_set_growth(DequeArray_t *deque, integer_t growth_rate)
{
    if (growth_rate <= 100)
        return false;

    deque->growth_rate = growth_rate;

    return true;
}

/// Locks the the target's buffer growth. If the buffer is full no more
/// elements will be added to the deque until its capacity is unlocked or
/// another element is removed.
///
/// \param[in] deque The deque to have its buffer's growth locked.
void
dqa_capacity_lock(DequeArray_t *deque)
{
    deque->locked = true;
}

/// Unlocks the buffer's capacity allowing it to be reallocated once full.
///
/// \param[in] deque The deque to have its buffer's growth unlocked.
void
dqa_capacity_unlock(DequeArray_t *deque)
{
    deque->locked = false;
}

/// Inserts an element to the front of the specified deque.
/// \par Interface Requirements
/// - None
///
/// \param[in] deque The deque where the element is to be inserted.
/// \param[in] element The element to be inserted in the deque.
///
/// \return True if the element was successfully added to the deque or false if
/// the buffer reallocation failed or the deque buffer capacity is locked.
bool
dqa_enqueue_front(DequeArray_t *deque, void *element)
{
    if (dqa_full(deque))
    {
        if (!dqa_grow(deque))
            return false;
    }

    deque->front = (deque->front == 0) ? deque->capacity - 1 : deque->front -1;

    deque->buffer[deque->front] = element;

    deque->count++;
    deque->version_id++;

    return true;
}

/// Inserts an element to the rear of the specified deque.
/// \par Interface Requirements
/// - None
///
/// \param[in] deque The deque where the element is to be inserted.
/// \param[in] element The element to be inserted in the deque.
///
/// \return True if the element was successfully added to the deque or false if
/// the buffer reallocation failed or the deque buffer capacity is locked.
bool
dqa_enqueue_rear(DequeArray_t *deque, void *element)
{
    if (dqa_full(deque))
    {
        if (!dqa_grow(deque))
            return false;
    }

    deque->buffer[deque->rear] = element;

    deque->rear = (deque->rear == deque->capacity - 1) ? 0 : deque->rear + 1;

    deque->count++;
    deque->version_id++;

    return true;
}

/// Removes an element at the front of the specified deque.
/// \par Interface Requirements
/// - None
///
/// \param[in] deque The deque where the element is to be removed from.
/// \param[out] result The resulting element removed from the deque.
///
/// \return True if an element was removed from the deque or false if the deque
/// is empty.
bool
dqa_dequeue_front(DequeArray_t *deque, void **result)
{
    *result = NULL;

    if (dqa_empty(deque))
        return false;

    *result = deque->buffer[deque->front];

    deque->buffer[deque->front] = NULL;

    deque->front = (deque->front == deque->capacity - 1) ? 0 : deque->front +1;

    deque->count--;
    deque->version_id++;

    return true;
}

/// Removes an element at the rear of the specified deque.
/// \par Interface Requirements
/// - None
///
/// \param[in] deque The deque where the element is to be removed from.
/// \param[out] result The resulting element removed from the deque.
///
/// \return True if an element was removed from the deque or false if the deque
/// is empty.
bool
dqa_dequeue_rear(DequeArray_t *deque, void **result)
{
    *result = NULL;

    if (dqa_empty(deque))
        return false;

    deque->rear = (deque->rear == 0) ? deque->capacity - 1 : deque->rear - 1;

    *result = deque->buffer[deque->rear];

    deque->buffer[deque->rear] = NULL;

    deque->count--;
    deque->version_id++;

    return true;
}

/// Returns the element at the front of the deque or NULL if the deque is
/// empty.
///
/// \param[in] deque The target deque.
///
/// \return NULL if the deque is empty or the element at the front of the
/// deque.
void *
dqa_peek_front(DequeArray_t *deque)
{
    if (dqa_empty(deque))
        return NULL;

    return deque->buffer[deque->front];
}

/// Returns the element at the rear of the deque or NULL if the deque is empty.
///
/// \param[in] deque The target deque.
///
/// \return NULL if the deque is empty or the element at the rear of the deque.
void *
dqa_peek_rear(DequeArray_t *deque)
{
    if (dqa_empty(deque))
        return NULL;

    integer_t i = (deque->rear == 0) ? deque->capacity - 1 : deque->rear - 1;

    return deque->buffer[i];
}

/// Returns true if the deque is empty, or false if there are elements in the
/// deque.
///
/// \param[in] deque The target deque.
///
/// \return True if the deque is empty, otherwise false.
bool
dqa_empty(DequeArray_t *deque)
{
    return deque->count == 0;
}

/// Returns true if the current amount of elements in the deque is the same as
/// the buffer's capacity, that is, the next element to be added to the deque
/// will cause the buffer to be reallocated.
///
/// \param[in] deque The target deque.
///
/// \return True if the amount of elements is the same as the buffer's
/// capacity, otherwise false.
bool
dqa_full(DequeArray_t *deque)
{
    return deque->count == deque->capacity;
}

/// Returns true if the specified size will fit in the deque's buffer without
/// it being reallocated.
///
/// \param[in] deque The target deque.
/// \param[in] size The specified size.
///
/// \return True if a given size fits inside the deque without reallocating the
/// buffer.
bool
dqa_fits(DequeArray_t *deque, unsigned_t size)
{
    return (deque->count + size) <= deque->capacity;
}

/// Returns true if the element is present in the deque, otherwise false.
/// \par Interface Requirements
/// - compare
///
/// \param[in] deque DequeArray_s reference.
/// \param[in] key Key to be matched.
///
/// \return True if the element is present in the deque, otherwise false.
bool
dqa_contains(DequeArray_t *deque, void *key)
{
    for (integer_t i = deque->front, j = 0;
         j < deque->count;
         i = (i + 1) % deque->capacity, j++)
    {
        if (deque->interface->compare(deque->buffer[i], key) == 0)
            return true;
    }

    return false;
}

/// Returns a copy of the specified DequeArray_s with the same interface. All
/// elements are copied using the deque's interface's copy function.
/// \par Interface Requirements
/// - copy
///
/// \param[in] deque The deque to be copied.
///
/// \return NULL if allocation failed or a copy of the specified deque.
DequeArray_t *
dqa_copy(DequeArray_t *deque)
{
    DequeArray_t *new_deque = dqa_create(deque->interface, deque->capacity,
                                         deque->growth_rate);

    if (!new_deque)
        return NULL;

    for (integer_t i = deque->front, j = 0;
         j < deque->count;
         i = (i + 1) % deque->capacity, j++)
    {
        new_deque->buffer[i] = deque->interface->copy(deque->buffer[i]);
    }

    new_deque->front = deque->front;
    new_deque->rear = deque->rear;
    new_deque->count = deque->count;
    new_deque->locked = deque->locked;

    return new_deque;
}

/// Creates a shallow copy of all elements in the deque, that is, only the
/// pointers addresses are copied to the new deque.
/// \par Interface Requirements
/// - None
///
/// \param[in] deque The deque to be copied.
///
/// \return NULL if allocation failed or a shallow copy of the specified deque.
DequeArray_t *
dqa_copy_shallow(DequeArray_t *deque)
{
    DequeArray_t *new_deque = dqa_create(deque->interface, deque->capacity,
                                         deque->growth_rate);

    if (!new_deque)
        return NULL;

    for (integer_t i = deque->front, j = 0;
         j < deque->count;
         i = (i + 1) % deque->capacity, j++)
    {
        new_deque->buffer[i] = deque->buffer[i];
    }

    new_deque->front = deque->front;
    new_deque->rear = deque->rear;
    new_deque->count = deque->count;
    new_deque->locked = deque->locked;

    return new_deque;
}

/// Makes a comparison between two deques element by element. If one deque has
/// less elements than the other the comparison of elements will go up until
/// one deque reaches its limit. If all elements are the same until then, the
/// tie breaker goes to their count. If it is also the same, then both deques
/// are equal.
/// \par Interface Requirements
/// - compare
///
/// \param[in] deque1 A target deque to be compared.
/// \param[in] deque2 A target deque to be compared.
///
/// \return An int according to \ref compare_f.
int
dqa_compare(DequeArray_t *deque1, DequeArray_t *deque2)
{
    integer_t max_count = deque1->count < deque2->count
                         ? deque1->count
                         : deque2->count;

    int comparison = 0;
    for (integer_t i = 0; i < max_count; i++)
    {
        // Since its a circular buffer we need to calculate where the ith
        // element of each queue is.
        comparison = deque1->interface->compare(
                deque1->buffer[(i + deque1->front) % deque1->capacity],
                deque2->buffer[(i + deque2->front) % deque2->capacity]);
        if (comparison > 0)
            return 1;
        else if (comparison < 0)
            return -1;
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
dqa_append(DequeArray_t *deque1, DequeArray_t *deque2)
{
    /// \todo dqa_append
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
dqa_prepend(DequeArray_t *deque1, DequeArray_t *deque2)
{
    /// \todo dqa_prepend
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
dqa_to_array(DequeArray_t *deque, integer_t *length)
{
    if (dqa_empty(deque))
        return NULL;

    void **array = malloc(sizeof(void*) * (size_t)deque->count);

    if (!array)
        return NULL;

    for (integer_t i = deque->front, j = 0;
         j < deque->count;
         i = (i + 1) % deque->capacity, j++)
    {
        array[i] = deque->interface->copy(deque->buffer[i]);
    }

    *length = deque->count;

    return array;
}

/// Displays a DequeArray_s in the console starting from the front element to
/// the rear element. There are currently four modes:
/// - -1 Displays each element separated by newline;
/// -  0 Displays each element like a linked list;
/// -  1 Displays each element separated by a space;
/// - Any other number defaults to the array representation.
/// \par Interface Requirements
/// - display
///
/// \param[in] deque The deque to be displayed in the console.
/// \param[in] display_mode The way the deque is to be displayed in the console.
void
dqa_display(DequeArray_t *deque, int display_mode)
{
    if (dqa_empty(deque))
    {
        printf("\nDequeArray\n[ empty ]\n");
        return;
    }

    switch (display_mode)
    {
        case -1:
            printf("\nDequeArray\n");
            for (integer_t i = deque->front, j = 0;
                 j < deque->count;
                 i = (i + 1) % deque->capacity, j++)
            {
                deque->interface->display(deque->buffer[i]);
                printf("\n");
            }
            break;
        case 0:
            printf("\nDequeArray\nFront <-> ");
            for (integer_t i = deque->front, j = 0;
                 j < deque->count - 1;
                 i = (i + 1) % deque->capacity, j++)
            {
                deque->interface->display(deque->buffer[i]);
                printf(" <-> ");
            }
            deque->interface->display(deque->buffer[
                    (deque->rear == 0) ? deque->capacity - 1 : deque->rear - 1
                    ]);
            printf(" <-> Rear\n");
            break;
        case 1:
            printf("\nDequeArray\n");
            for (integer_t i = deque->front, j = 0;
                 j < deque->count;
                 i = (i + 1) % deque->capacity, j++)
            {
                deque->interface->display(deque->buffer[i]);
                printf(" ");
            }
            printf("\n");
            break;
        default:
            printf("\nDequeArray\n[ ");
            for (integer_t i = deque->front, j = 0;
                 j < deque->count - 1;
                 i = (i + 1) % deque->capacity, j++)
            {
                deque->interface->display(deque->buffer[i]);
                printf(", ");
            }
            deque->interface->display(deque->buffer[
                    (deque->rear == 0) ? deque->capacity - 1 : deque->rear - 1
                    ]);
            printf(" ]\n");
            break;
    }
}

///////////////////////////////////////////////////// NOT EXPOSED FUNCTIONS ///

// This function reallocates the data buffer effectively increasing its
// capacity
bool
static dqa_grow(DequeArray_t *deque)
{
    if (deque->locked)
        return false;

    integer_t old_capacity = deque->capacity;

    // capacity = capacity * (growth_rate / 100)
    deque->capacity = (integer_t) ((double) (deque->capacity)
            * ((double) (deque->growth_rate) / 100.0));

    // 4 is the minimum growth
    if (deque->capacity - old_capacity < 4)
        deque->capacity = old_capacity + 4;

    void **new_buffer = realloc(deque->buffer,
            sizeof(void*) * (size_t)deque->capacity);

    // Reallocation failed
    if (!new_buffer)
    {
        deque->capacity = old_capacity;
        return false;
    }

    deque->buffer = new_buffer;

    integer_t real_rear = (deque->rear == 0)
            ? old_capacity - 1
            : deque->rear - 1;

    // Shift elements if the rear index or the front index wrapped around the
    // buffer.
    if (real_rear < deque->front)
    {
        // When the buffer is full the rear index equals the front index. In
        // order to shift elements effectively this calculates which portion of
        // the buffer has the least amount of elements.
        // Shift the right portion to the end of the buffer if the total
        // elements are less than the left portion; otherwise append the left
        // portion to the right portion.
        if (old_capacity - deque->front < deque->rear)
        {
            integer_t distance = old_capacity - deque->front;

            for (integer_t i = old_capacity - 1, j = deque->capacity - 1;
                    i >= deque->front; i--, j--)
            {
                deque->buffer[j] = deque->buffer[i];
            }

            deque->front = deque->capacity - distance;
        }
        // If the growth rate is less than 150 the rear index might wrap around
        // the buffer again
        else
        {
            for (integer_t i = 0, j = old_capacity; i < deque->rear;
                    i++, j = (j + 1) % deque->capacity)
            {
                deque->buffer[j] = deque->buffer[i];
            }

            deque->rear = (old_capacity + deque->rear) % deque->capacity;
        }
    }
    // This case only happens when dqa->front == 0 and dqa->rear == 0
    // The rear index has wrapped around but the buffer increased in size
    // allowing the rear index to keep increasing instead of wrapping around.
    // This can be achieved by inserting elements only at one end and never
    // removing elements from the other end.
    else if (deque->rear == 0)
    {
        deque->rear = old_capacity;
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

/// \todo DequeArrayIterator

///////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////// Wrapper ///
///////////////////////////////////////////////////////////////////////////////

/// \todo DequeArrayWrapper
