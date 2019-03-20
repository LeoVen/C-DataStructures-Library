/**
 * @file StackArray.c
 *
 * @author Leonardo Vencovsky (https://github.com/LeoVen)
 *
 * @date 03/10/2018
 */

#include "StackArray.h"

/// A StackArray_s is a buffered StackList_s with FILO (First-in Last-out) or
/// LIFO (Last-in First-out) operations, so the first item added is the last
/// one to be removed. The stack is implemented as a normal buffer that only
/// grows on one side. This simplifies the structure as no indexes will ever
/// need to wrap around the buffer. The size of the stack, or the amount of
/// elements in the stack can be used as an index for push and pop operations.
/// The buffer can also expand according to the \c growth_rate.
///
/// \par Advantages over StackList_s
/// - No need of pointers, only the data is allocated in memory
///
/// \par Drawbacks
/// - When the StackArray_s is full the buffer needs to be reallocated
///
/// \par Functions
/// Located in the file StackArray.c
struct StackArray_s
{
    /// \brief Data buffer.
    ///
    /// Buffer where elements are stored in.
    void **buffer;

    /// \brief Current amount of elements in the StackArray_s.
    ///
    /// Current amount of elements in the StackArray_S.
    integer_t count;

    /// \brief StackArray_s buffer maximum capacity.
    ///
    /// Buffer maximum capacity. When \c size reaches \c capacity the buffer
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

    /// \brief StackArray_s interface.
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
const unsigned_t sta_size = sizeof(StackArray_t);

///////////////////////////////////////////////////// NOT EXPOSED FUNCTIONS ///

bool
static sta_grow(StackArray_t *stack, integer_t required_size);

integer_t
sta_calculate_growth(integer_t required, integer_t current,
                     integer_t multiplier);

////////////////////////////////////////////// END OF NOT EXPOSED FUNCTIONS ///

/// Initializes a StackArray_s with an initial capacity of 32 and a growth rate
/// of 200, that is, twice the size after each growth.
/// \par Interface Requirements
/// - None
///
/// \param[in] interface An interface defining all necessary functions for the
/// stack to operate.
///
/// \return A new StackArray_s or NULL if allocation failed.
StackArray_t *
sta_new(Interface_t *interface)
{
    StackArray_t *stack = malloc(sizeof(StackArray_t));

    if (!stack)
        return NULL;

    stack->buffer = malloc(sizeof(void*) * 32);

    if (!(stack->buffer))
    {
        free(stack);

        return NULL;
    }

    for (integer_t i = 0; i < 32; i++)
        stack->buffer[i] = NULL;

    stack->capacity = 32;
    stack->growth_rate = 200;
    stack->version_id = 0;
    stack->count = 0;
    stack->locked = false;

    stack->interface = interface;

    return stack;
}

/// Initializes a stack allocated on the stack with a given interface, initial
/// capacity and growth rate.
///
/// \param[in] stack The stack to be initialized.
/// \param[in] initial_capacity Buffer initial capacity.
/// \param[in] growth_rate Buffer growth rate.
/// \param[in] interface An interface defining all necessary functions for the
/// deque to operate.
///
/// \return True if the stack was successfully initialized or false if the
/// growth rate is less than 101, if the initial capacity is 0 or if allocation
/// failed.
bool
sta_init(StackArray_t *stack, Interface_t *interface,
         integer_t initial_capacity, integer_t growth_rate)
{
    if (growth_rate <= 100 || initial_capacity <= 0)
        return false;

    stack->buffer = malloc(sizeof(void*) * (size_t)initial_capacity);

    if (!(stack->buffer))
        return false;

    for (integer_t i = 0; i < initial_capacity; i++)
        stack->buffer[i] = NULL;

    stack->capacity = initial_capacity;
    stack->growth_rate = growth_rate;
    stack->version_id = 0;
    stack->count = 0;
    stack->locked = false;

    stack->interface = interface;

    return true;
}

/// Initializes a StackArray_s with a user defined \c initial_capacity and \c
/// growth_rate. This function only accepts an \c initial_capacity greater than
/// 0 and a \c growth_rate greater than 100; but keep in mind that in some
/// cases if the \c initial_capacity is too small and the \c growth_rate is too
/// close to 100 there won't be an increase in capacity and the minimum growth
/// will be triggered.
/// \par Interface Requirements
/// - None
///
/// \param[in] interface An interface defining all necessary functions for the
/// stack to operate.
/// \param[in] initial_capacity Buffer initial capacity.
/// \param[in] growth_rate Buffer growth rate.
///
/// \return NULL if the growth rate is less than 101, if the initial capacity
/// is 0 or if allocation failed.
/// \return A new StackArray_s dynamically allocated.
StackArray_t *
sta_create(Interface_t *interface, integer_t initial_capacity,
           integer_t growth_rate)
{
    if (growth_rate <= 100 || initial_capacity <= 0)
        return NULL;

    StackArray_t *stack = malloc(sizeof(StackArray_t));

    if (!stack)
        return NULL;

    stack->buffer = malloc(sizeof(void*) * (size_t)initial_capacity);

    if (!(stack->buffer))
    {
        free(stack);

        return NULL;
    }

    for (integer_t i = 0; i < initial_capacity; i++)
        stack->buffer[i] = NULL;

    stack->capacity = initial_capacity;
    stack->growth_rate = growth_rate;
    stack->version_id = 0;
    stack->count = 0;
    stack->locked = false;

    stack->interface = interface;

    return stack;
}

/// Frees each element at the stack using its interface's \c free and then
/// frees the stack struct.
/// \par Interface Requirements
/// - free
///
/// \param[in] stack The stack to be freed from memory.
void
sta_free(StackArray_t *stack)
{
    for (integer_t i = 0; i < stack->count; i++)
        stack->interface->free(stack->buffer[i]);

    free(stack->buffer);

    free(stack);
}

/// Frees the StackArray_s structure and leaves all the elements intact. Be
/// careful as this might cause severe memory leaks. Only use this if your
/// stack elements are also handled by another structure or algorithm.
/// \par Interface Requirements
/// - None
///
/// \param[in] stack The stack to be freed from memory.
void
sta_free_shallow(StackArray_t *stack)
{
    free(stack->buffer);

    free(stack);
}

/// This function will reset the StackArray_s, freeing all its elements,
/// keeping the structure intact including its original interface and its
/// buffer.
/// \par Interface Requirements
/// - free
///
/// \param[in] stack The stack to have its elements erased.
void
sta_erase(StackArray_t *stack)
{
    for (integer_t i = 0; i < stack->count; i++)
    {
        stack->interface->free(stack->buffer[i]);

        stack->buffer[i] = NULL;
    }

    stack->version_id++;
    stack->count = 0;
}

/// This functions will reset the StackArray_s without freeing its elements.
/// This will keep its original interface and its original buffer size.
/// \par Interface Requirements
/// - None
///
/// \param[in] stack The stack to be reset.
void
sta_erase_shallow(StackArray_t *stack)
{
    for (integer_t i = 0; i < stack->count; i++)
    {
        stack->buffer[i] = NULL;
    }

    stack->version_id++;
    stack->count = 0;
}

/// Sets a new interface for the specified StackArray_s.
/// \par Interface Requirements
/// - None
///
/// \param[in] stack StackArray_s to change the interface.
/// \param[in] new_interface New interface for the specified structure.
void
sta_config(StackArray_t *stack, Interface_t *new_interface)
{
    stack->interface = new_interface;
}

/// Returns the current amount of elements in the specified stack.
/// \par Interface Requirements
/// - None
///
/// \param[in] stack The target stack.
///
/// \return The amount of elements in the stack.
integer_t
sta_count(StackArray_t *stack)
{
    return stack->count;
}

/// Returns the current buffer's size of the specified stack.
/// \par Interface Requirements
/// - None
///
/// \param[in] stack The target stack.
///
/// \return The stack's buffer's size.
integer_t
sta_capacity(StackArray_t *stack)
{
    return stack->capacity;
}

/// Returns the buffer's growth rate of the specified stack.
/// \par Interface Requirements
/// - None
///
/// \param[in] stack The target stack.
///
/// \return The buffer's growth rate.
integer_t
sta_growth(StackArray_t *stack)
{
    return stack->growth_rate;
}

/// Returns the boolean state of \c locked member.
/// \par Interface Requirements
/// - None
///
/// \param[in] stack The target stack.
///
/// \return True if the buffer's growth is locked, false otherwise.
bool
sta_locked(StackArray_t *stack)
{
    return stack->locked;
}

/// Sets a new growth_rate to the target stack.
/// \par Interface Requirements
/// - None
///
/// \param[in] stack The target stack.
/// \param[in] growth_rate Stack's new growth_rate.
///
/// \return True if the growth rate was change or false if the parameter is
/// less than 101.
bool
sta_set_growth(StackArray_t *stack, integer_t growth_rate)
{
    if (growth_rate <= 100)
        return false;

    stack->growth_rate = growth_rate;

    return true;
}

/// Locks the the target's buffer growth. If the buffer is full no more
/// elements will be added to the stack until its capacity is unlocked or
/// another element is removed.
/// \par Interface Requirements
/// - None
///
/// \param[in] stack The stack to have its buffer's growth locked.
void
sta_capacity_lock(StackArray_t *stack)
{
    stack->locked = true;
}

/// Unlocks the buffer's capacity allowing it to be reallocated once full.
/// \par Interface Requirements
/// - None
///
/// \param[in] stack The stack to have its buffer's growth unlocked.
void
sta_capacity_unlock(StackArray_t *stack)
{
    stack->locked = false;
}

/// Inserts an element at the top of the specified stack.
/// \par Interface Requirements
/// - None
///
/// \param[in] stack The stack where the element is to be inserted.
/// \param[in] element The element to be inserted onto the stack.
///
/// \return True if the element was successfully added to the stack or false if
/// the buffer reallocation failed or the stack buffer capacity is locked.
bool
sta_push(StackArray_t *stack, void *element)
{
    if (sta_full(stack))
    {
        if (!sta_grow(stack, stack->count + 1))
            return false;
    }

    stack->buffer[stack->count] = element;

    stack->count++;
    stack->version_id++;

    return true;
}

/// Removes an element from the top of the specified stack.
/// \par Interface Requirements
/// - None
///
/// \param[in] stack The stack where the element is to be removed from.
/// \param[out] result The resulting element removed from the stack.
///
/// \return True if an element was removed from the stack or false if the stack
/// is empty.
bool
sta_pop(StackArray_t *stack, void **result)
{
    *result = NULL;

    if (sta_empty(stack))
        return false;

    *result = stack->buffer[stack->count - 1];

    stack->buffer[stack->count - 1] = NULL;

    stack->count--;
    stack->version_id++;

    return true;
}

/// Returns the element at the top of the stack or NULL if the stack is empty.
/// \par Interface Requirements
/// - None
///
/// \param[in] stack The target stack.
///
/// \return NULL if the stack is empty or the element at the top of the stack.
void *
sta_peek(StackArray_t *stack)
{
    if (sta_empty(stack))
        return NULL;

    return stack->buffer[stack->count - 1];
}

/// Returns true if the stack is empty, or false if there are elements in the
/// stack.
/// \par Interface Requirements
/// - None
///
/// \param[in] stack The target stack.
///
/// \return True if the stack is empty, otherwise false.
bool
sta_empty(StackArray_t *stack)
{
    return stack->count == 0;
}

/// Returns true if the current amount of elements in the stack is the same as
/// the buffer's capacity, that is, the next element to be added to the stack
/// will cause the buffer to be reallocated.
/// \par Interface Requirements
/// - None
///
/// \param[in] stack The target stack.
///
/// \return True if the amount of elements is the same as the buffer's
/// capacity, otherwise false.
bool
sta_full(StackArray_t *stack)
{
    return stack->count == stack->capacity;
}

/// Returns true if the specified size will fit in the stack's buffer without
/// it being reallocated.
/// \par Interface Requirements
/// - None
///
/// \param[in] stack The target stack.
/// \param[in] size The specified size.
///
/// \return True if a given size fits inside the stack without reallocating the
/// buffer.
bool
sta_fits(StackArray_t *stack, integer_t size)
{
    return (stack->count + size) <= stack->capacity;
}

/// Returns true if the element is present in the stack, otherwise false.
/// \par Interface Requirements
/// - compare
///
/// \param[in] stack StackArray_s reference.
/// \param[in] key Key to be matched.
///
/// \return True if the element is present in the stack, otherwise false.
bool
sta_contains(StackArray_t *stack, void *key)
{
    for (integer_t i = 0; i < stack->count; i++)
    {
        if (stack->interface->compare(stack->buffer[i], key) == 0)
            return true;
    }

    return false;
}

/// Returns a copy of the specified StackArray_s with the same interface. All
/// elements are copied using the stack interface's copy function.
/// \par Interface Requirements
/// - copy
///
/// \param[in] stack The stack to be copied.
///
/// \return NULL if allocation failed or a copy of the specified stack.
StackArray_t *
sta_copy(StackArray_t *stack)
{
    StackArray_t *new_stack = sta_create(stack->interface, stack->capacity,
                                         stack->growth_rate);

    if (!new_stack)
        return NULL;

    for (integer_t i = 0; i < stack->count; i++)
    {
        new_stack->buffer[i] = stack->interface->copy(stack->buffer[i]);
    }

    new_stack->count = stack->count;
    new_stack->locked = stack->locked;

    return new_stack;
}

/// Creates a shallow copy of all elements in the stack, that is, only the
/// pointers addresses are copied to the new stack.
/// \par Interface Requirements
/// - None
///
/// \param[in] stack The stack to be copied.
///
/// \return NULL if allocation failed or a shallow copy of the specified stack.
StackArray_t *
sta_copy_shallow(StackArray_t *stack)
{
    StackArray_t *new_stack = sta_create(stack->interface, stack->capacity,
                                         stack->growth_rate);

    if (!new_stack)
        return NULL;

    for (integer_t i = 0; i < stack->count; i++)
    {
        new_stack->buffer[i] = stack->buffer[i];
    }

    new_stack->count = stack->count;

    new_stack->locked = stack->locked;

    return new_stack;
}

/// Makes a comparison between two stacks element by element. If one stack has
/// less elements than the other, the comparison of elements will go up until
/// one stack reaches its limit. If all elements are the same until then, the
/// tie breaker goes to their size. If it is also the same, then both stacks
/// are equal.
/// \par Interface Requirements
/// - compare
///
/// \param[in] stack1 A target stack to be compared.
/// \param[in] stack2 A target stack to be compared.
///
/// \return An int according to \ref compare_f.
int
sta_compare(StackArray_t *stack1, StackArray_t *stack2)
{
    integer_t max_size = stack1->count < stack2->count
            ? stack1->count
            : stack2->count;

    int comparison = 0;
    for (integer_t i = 0; i < max_size; i++)
    {
        comparison = stack1->interface->compare(stack1->buffer[i],
                stack2->buffer[i]);
        if (comparison > 0)
            return 1;
        else if (comparison < 0)
            return -1;
    }

    // So far all elements were the same
    if (stack1->count > stack2->count)
        return 1;
    else if (stack1->count < stack2->count)
        return -1;

    return 0;
}

/// Stacks the \c stack2 on top of the \c stack1, emptying the \c stack2. Both
/// stacks need to have been initialized. If both stacks are empty nothing will
/// happen.
/// \par Interface Requirements
/// - None
///
/// \param[in] stack1 Stack to receive elements.
/// \param[in] stack2 Stack where the elements are going to be taken from.
///
/// \return True if all operations were successful, otherwise false.
bool
sta_stack(StackArray_t *stack1, StackArray_t *stack2)
{
    if (sta_empty(stack2))
        return true;

    if (!sta_fits(stack1, stack2->count))
    {
        if (!sta_grow(stack1, stack2->count))
            return false;
    }

    for (integer_t i = stack1->count, j = 0; j < stack2->count; i++, j++)
    {
        stack1->buffer[i] = stack2->buffer[j];
    }

    for (integer_t i = 0; i < stack2->count; i++)
        stack2->buffer[i] = NULL;

    stack1->count += stack2->count;
    stack2->count = 0;

    return true;
}

/// Makes a copy of all the elements in the stack to a C array starting from
/// the top element.
/// \par Interface Requirements
/// - copy
///
/// \param[in] stack The stack to be copied to the array.
/// \param[out] length The resulting array's length.
///
/// \return The resulting array or NULL if the stack is empty or the array
/// allocation failed.
void **
sta_to_array(StackArray_t *stack, integer_t *length)
{
    *length = 0;

    if (sta_empty(stack))
        return NULL;

    void **array = malloc(sizeof(void*) * (size_t)stack->count);

    if (!array)
        return NULL;

    for (integer_t i = stack->count - 1; i > 0; i--)
        array[i] = stack->interface->copy(stack->buffer[i]);

    *length = stack->count;

    return array;
}

/// Displays a StackArray_s in the console starting from the top element. There
/// are currently four modes:
/// - -1 Displays each element separated by newline;
/// -  0 Displays each element like a linked list;
/// -  1 Displays each element separated by a space;
/// - Any other number defaults to the array representation.
/// \par Interface Requirements
/// - display
///
/// \param[in] stack The stack to be displayed in the console.
/// \param[in] display_mode The way the stack is to be displayed in the console.
void
sta_display(StackArray_t *stack, int display_mode)
{
    if (sta_empty(stack))
    {
        printf("\nStackArray\n[ empty ]\n");
        return;
    }

    switch (display_mode)
    {
        case -1:
            printf("\nStackArray\n");
            for (integer_t i = 0; i < stack->count; i++)
            {
                stack->interface->display(stack->buffer[i]);
                printf("\n");
            }
            break;
        case 0:
            printf("\nStackArray\nTop -> ");
            for (integer_t i = 0; i < stack->count - 1; i++)
            {
                stack->interface->display(stack->buffer[i]);
                printf(" -> ");
            }
            stack->interface->display(stack->buffer[stack->count - 1]);
            printf(" NULL\n");
            break;
        case 1:
            printf("\nStackArray\n");
            for (integer_t i = 0; i < stack->count; i++)
            {
                stack->interface->display(stack->buffer[i]);
                printf(" ");
            }
            printf("\n");
            break;
        default:
            printf("\nStackArray\n[ ");
            for (integer_t i = 0; i < stack->count - 1; i++)
            {
                stack->interface->display(stack->buffer[i]);
                printf(", ");
            }
            stack->interface->display(stack->buffer[stack->count - 1]);
            printf(" ]\n");
            break;
    }
}

///////////////////////////////////////////////////// NOT EXPOSED FUNCTIONS ///

// This function reallocates the data buffer increasing its capacity
bool
static sta_grow(StackArray_t *stack, integer_t required_size)
{
    if (stack->locked)
        return false;

    integer_t old_capacity = stack->capacity;

    // capacity = capacity * (growth_rate / 100)
    stack->capacity = (integer_t) ((double) (stack->capacity) *
            ((double) (stack->growth_rate) / 100.0));

    // 4 is the minimum growth
    if (stack->capacity - old_capacity < 4)
        stack->capacity = old_capacity + 4;

    // If not enough...
    if (stack->capacity < required_size)
        stack->capacity = required_size;

    void **new_buffer = realloc(stack->buffer,
            sizeof(void*) * (size_t)stack->capacity);

    // Reallocation failed
    if (!new_buffer)
    {
        stack->capacity = old_capacity;
        return false;
    }

    stack->buffer = new_buffer;

    return true;
}

////////////////////////////////////////////// END OF NOT EXPOSED FUNCTIONS ///

///////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////// Iterator ///
///////////////////////////////////////////////////////////////////////////////

/// This is a stack array iterator and it is represented by the current
/// element's index. Is is also possible to traverse this stack both ways due
/// to the array's nature.
struct StackArrayIterator_s
{
    /// \brief Target StackArray_s.
    ///
    /// Target StackArray_s. The iterator might need to use some information
    /// provided by the stack or change some of its data members.
    struct StackArray_s *target;

    /// \brief Current element.
    ///
    /// Current element's index. The iterator is always initialized with 0,
    /// that is, the start (top) of the stack.
    integer_t cursor;

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
sta_iter_target_modified(StackArrayIterator_t *iter);

////////////////////////////////////////////// END OF NOT EXPOSED FUNCTIONS ///

///
/// \param[in] target
///
/// \return
StackArrayIterator_t *
sta_iter_new(StackArray_t *target)
{
    if (sta_empty(target))
        return NULL;

    StackArrayIterator_t *iter = malloc(sizeof(StackArrayIterator_t));

    if (!iter)
        return NULL;

    iter->target = target;
    iter->target_id = target->version_id;
    iter->cursor = 0;

    return iter;
}

///
/// \param[in] iter
/// \param[in] target
void
sta_iter_retarget(StackArrayIterator_t *iter, StackArray_t *target)
{
    iter->target = target;
    iter->target_id = target->version_id;
}

///
/// \param[in] iter
void
sta_iter_free(StackArrayIterator_t *iter)
{
    free(iter);
}

///
/// \param[in] iter
///
/// \return
bool
sta_iter_next(StackArrayIterator_t *iter)
{
    if (sta_iter_target_modified(iter))
        return false;

    if (!sta_iter_has_next(iter))
        return false;

    iter->cursor++;

    return true;
}

///
/// \param[in] iter
///
/// \return
bool
sta_iter_prev(StackArrayIterator_t *iter)
{
    if (sta_iter_target_modified(iter))
        return false;

    if (!sta_iter_has_prev(iter))
        return false;

    iter->cursor--;

    return true;
}

///
/// \param[in] iter
///
/// \return
bool
sta_iter_to_top(StackArrayIterator_t *iter)
{
    if (sta_iter_target_modified(iter))
        return false;

    iter->cursor = 0;

    return true;
}

///
/// \param[in] iter
///
/// \return
bool
sta_iter_to_bottom(StackArrayIterator_t *iter)
{
    if (sta_iter_target_modified(iter))
        return false;

    iter->cursor = iter->target->count == 0 ? 0 : iter->target->count - 1;

    return true;
}

///
/// \param[in] iter
///
/// \return
bool
sta_iter_has_next(StackArrayIterator_t *iter)
{
    return iter->cursor < iter->target->count;
}

///
/// \param[in] iter
///
/// \return
bool
sta_iter_has_prev(StackArrayIterator_t *iter)
{
    return iter->cursor > 0;
}

///
/// \param[in] iter
/// \param[in] result
///
/// \return
bool
sta_iter_get(StackArrayIterator_t *iter, void **result)
{
    if (sta_iter_target_modified(iter))
        return false;

    *result = iter->target->buffer[iter->cursor];

    return true;
}

///
/// \param[in] iter
/// \param[in] element
///
/// \return
bool
sta_iter_set(StackArrayIterator_t *iter, void *element)
{
    if (sta_iter_target_modified(iter))
        return false;

    iter->target->interface->free(iter->target->buffer[iter->cursor]);

    iter->target->buffer[iter->cursor] = element;

    return true;
}

///
/// \param[in] iter
///
/// \return
void *
sta_iter_peek_next(StackArrayIterator_t *iter)
{
    if (sta_iter_target_modified(iter))
        return NULL;

    if (!sta_iter_has_next(iter))
        return NULL;

    return iter->target->buffer[iter->cursor + 1];
}

///
/// \param[in] iter
///
/// \return
void *
sta_iter_peek(StackArrayIterator_t *iter)
{
    if (sta_iter_target_modified(iter))
        return NULL;

    return iter->target->buffer[iter->cursor];
}

///
/// \param[in] iter
///
/// \return
void *
sta_iter_peek_prev(StackArrayIterator_t *iter)
{
    if (sta_iter_target_modified(iter))
        return NULL;

    if (!sta_iter_has_prev(iter))
        return NULL;

    return iter->target->buffer[iter->cursor - 1];
}

///////////////////////////////////////////////////// NOT EXPOSED FUNCTIONS ///

static bool
sta_iter_target_modified(StackArrayIterator_t *iter)
{
    return iter->target_id != iter->target->version_id;
}

////////////////////////////////////////////// END OF NOT EXPOSED FUNCTIONS ///

///////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////// Wrapper ///
///////////////////////////////////////////////////////////////////////////////

/// \todo StackArrayWrapper
