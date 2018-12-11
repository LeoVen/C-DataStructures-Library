/**
 * @file StackArray.c
 * 
 * @author Leonardo Vencovsky (https://github.com/LeoVen)
 *
 * @date 03/10/2018
 */

#include "StackArray.h"

/// A StackArray_s is a buffered Stack with FILO (First-in Last-out) or LIFO
/// (Last-in First-out) operations, so the first item added is the last one to
/// be removed. The stack is implemented as a normal buffer that only grows on
/// one side. This simplifies the structure as no indexes will ever need to
/// wrap around the buffer. The size of the stack, or the amount of elements
/// in the stack can be used as an index for push and pop operations. The
/// buffer can also expand according to the \c growth_rate.
///
/// \par Advantages over Stack_s
/// - No need of pointers, only the data is allocated in memory
///
/// \par Drawbacks
/// - When the \c StackArray is full the buffer needs to be reallocated
///
/// \par Functions
///
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
    unsigned_t size;

    /// \brief StackArray_s buffer maximum capacity.
    ///
    /// Buffer maximum capacity. When \c size reaches \c capacity the buffer
    /// is reallocated and increases according to \c growth_rate.
    unsigned_t capacity;

    /// \brief Buffer growth rate.
    ///
    /// Buffer growth rate. The new buffer capacity is calculated as:
    ///
    /// <code> capacity *= (growth_rate / 100.0) </code>
    unsigned_t growth_rate;

    /// \brief Flag for locked capacity.
    ///
    /// If \c locked is set to true the buffer will not grow and if any
    /// elements are inserted with a full buffer \c DS_ERR_FULL will be
    /// returned.
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

///////////////////////////////////////////////////// NOT EXPOSED FUNCTIONS ///

bool
static sta_grow(StackArray stack);

////////////////////////////////////////////// END OF NOT EXPOSED FUNCTIONS ///

/// Initializes a StackArray_s with an initial capacity of 32 and a growth rate
/// of 200, that is, twice the size after each growth.
///
/// \param[in] interface The interface for your StackArray_s.
///
/// \return A new StackArray_s or NULL if allocation failed.
StackArray_t *
sta_new(Interface_t *interface)
{
    StackArray stack = malloc(sizeof(StackArray_t));

    if (!stack)
        return NULL;

    stack->buffer = malloc(sizeof(void*) * 32);

    if (!(stack->buffer))
    {
        free(stack);

        return NULL;
    }

    stack->capacity = 32;
    stack->growth_rate = 200;
    stack->size = 0;
    stack->version_id = 0;
    stack->locked = false;

    stack->interface = interface;

    return stack;
}

/// Initializes a \c StackArray with a user defined \c initial_capacity and \c
/// growth_rate. This function only accepts an \c initial_capacity greater than
/// 8 and a \c growth_rate greater than 100; but keep in mind that in some
/// cases if the \c initial_capacity is too small and the \c growth_rate is too
/// close to 100 there won't be an increase in capacity and the minimum growth
/// will be triggered.
///
/// \param[in] initial_capacity Buffer initial capacity.
/// \param[in] growth_rate Buffer growth rate.
/// \param[in] interface An interface defining all necessary functions for the
/// stack to operate.
///
/// \return NULL if the growth rate is less than 101, if the initial capacity
/// is 0 or if allocation failed.
/// \return A new StackArray_s dynamically allocated.
StackArray_t *
sta_create(unsigned_t initial_capacity, unsigned_t growth_rate,
           Interface_t *interface)
{
    if (growth_rate <= 100 || initial_capacity == 0)
        return NULL;

    StackArray stack = malloc(sizeof(StackArray_t));

    if (!stack)
        return NULL;

    stack->buffer = malloc(sizeof(void*) * initial_capacity);

    if (!(stack->buffer))
    {
        free(stack);

        return NULL;
    }

    stack->capacity = initial_capacity;
    stack->growth_rate = growth_rate;
    stack->size = 0;
    stack->version_id = 0;
    stack->locked = false;

    stack->interface = interface;

    return stack;
}

/// Frees each element at the stack using its interface's @c v_free and then
/// frees the stack struct.
///
/// \param stack The stack to be freed from memory.
///
/// \return DS_ERR_NULL_POINTER if the stack reference is \c NULL.
/// \return DS_OK if all operations were successful.
void sta_free(StackArray_t *stack)
{
    for (integer_t i = 0; i < stack->size; i++)
        stack->interface->free(stack->buffer[i]);

    free(stack->buffer);

    free(stack);
}

void sta_free_shallow(StackArray_t *stack)
{
    free(stack->buffer);

    free(stack);
}

/// This function will free all the elements of the specified StackArray_s and
/// will keep the structure intact.
///
/// \param[in] stack The stack to be erased.
///
/// \return DS_ERR_ALLOC if stack allocation failed.
/// \return DS_ERR_NULL_POINTER if the stack reference is \c NULL.
/// \return DS_OK if all operations were successful.
bool
sta_erase(StackArray_t *stack)
{
    for (integer_t i = 0; i < stack->size; i++)
    {
        stack->interface->free(stack->buffer[i]);

        stack->buffer[i] = NULL;
    }

    return true;
}

/// Sets a new interface for the specified StackArray_s.
///
/// \param[in] stack StackArray_s to change the interface.
/// \param[in] new_interface New interface for the specified structure.
void
sta_config(StackArray_t *stack, Interface_t *new_interface)
{
    stack->interface = new_interface;
}

unsigned_t
sta_size(StackArray_t *stack)
{
    return stack->size;
}

unsigned_t
sta_capacity(StackArray_t *stack)
{
    return stack->capacity;
}

unsigned_t
sta_growth(StackArray_t *stack)
{
    return stack->growth_rate;
}

bool
sta_set_growth(StackArray_t *stack, unsigned_t growth_rate)
{
    if (growth_rate <= 100)
        return false;

    stack->growth_rate = growth_rate;

    return true;
}

/// Inserts an element at the top of the specified stack.
///
/// \param[in] stack The StackArray_s where the element is to be inserted.
/// \param[in] element The element to be inserted in the stack.
///
/// \return true if the operation was successful, otherwise false if the buffer
/// reallocation failed.
bool
sta_push(StackArray_t *stack, void *element)
{
    if (sta_full(stack))
    {
        if (!sta_grow(stack))
            return false;
    }

    stack->buffer[stack->size] = element;

    stack->size++;
    stack->version_id++;

    return true;
}

/// Removes an element from the top of the specified stack.
///
/// \param[in] stack The stack where the element is to be removed from.
/// \param[out] result The resulting element removed from the stack.
///
/// \return DS_ERR_INVALID_OPERATION if the stack is empty.
/// \return DS_ERR_NULL_POINTER if stack reference is \c NULL.
/// \return DS_OK if all operations were successful.
bool
sta_pop(StackArray_t *stack, void **result)
{
    *result = NULL;

    if (sta_empty(stack))
        return false;

    *result = stack->buffer[stack->size - 1];

    stack->buffer[stack->size - 1] = NULL;

    stack->size--;
    stack->version_id++;

    return DS_OK;
}

void *
sta_peek(StackArray_t *stack)
{
    if (sta_empty(stack))
        return NULL;

    return stack->buffer[stack->size - 1];
}

bool
sta_empty(StackArray_t *stack)
{
    return stack->size == 0;
}

bool
sta_full(StackArray_t *stack)
{
    return stack->size == stack->capacity;
}

bool
sta_fits(StackArray_t *stack, unsigned_t size)
{
    return (stack->size + size) <= stack->capacity;
}

void
sta_capacity_lock(StackArray stack)
{
    stack->locked = true;
}

void
sta_capacity_unlock(StackArray stack)
{
    stack->locked = false;
}

/// Returns a copy of the specified StackArray_s.
///
/// \param stack The stack to be copied.
///
/// \return \c NULL if allocation failed or a copy of the specified stack.
StackArray_t *
sta_copy(StackArray_t *stack)
{
    StackArray_t *new_stack = sta_create(stack->capacity, stack->growth_rate,
            stack->interface);

    if (!new_stack)
        return NULL;

    for (integer_t i = 0; i < stack->size; i++)
    {
        new_stack->buffer[i] = stack->interface->copy(stack->buffer[i]);
    }

    new_stack->size = stack->size;

    new_stack->locked = stack->locked;

    return new_stack;
}

/// Compares two stacks.
///
/// \param stack1
/// \param stack2
///
/// \return
int
sta_compare(StackArray_t *stack1, StackArray_t *stack2)
{
    unsigned_t max_size = stack1->size < stack2->size
            ? stack1->size
            : stack2->size;

    int comparison = 0;
    for (unsigned_t i = 0; i < max_size; i++)
    {
        comparison = stack1->interface->compare(stack1->buffer[i],
                stack2->buffer[i]);
        if (comparison > 0)
            return 1;
        else if (comparison < 0)
            return -1;
    }

    // So far all elements were the same
    if (stack1->size > stack2->size)
        return 1;
    else if (stack1->size < stack2->size)
        return -1;

    return 0;
}

/// Displays a \c StackArray in the console.
///
/// \param stack The stack to be displayed in the console.
///
/// \return DS_ERR_NULL_POINTER if the stack reference is \c NULL.
/// \return DS_OK if all operations were successful.
void sta_display(StackArray_t *stack)
{
    if (sta_empty(stack))
    {
        printf("\nStack\n[ empty]\n");
        return;
    }

    printf("\nStack");

    for (integer_t i = 0; i < stack->size; i++)
    {
        stack->interface->display(stack->buffer[i]);
        printf("\n");
    }

    printf("\n");
}

/// Displays a \c StackArray in the console like an array with its values
/// separated by commas, delimited with brackets.
///
/// \param stack The stack to be displayed in the console.
///
/// \return DS_ERR_NULL_POINTER if the stack reference is \c NULL.
/// \return DS_OK if all operations were successful.
void sta_display_array(StackArray_t *stack)
{
    if (sta_empty(stack))
    {
        printf("\n[ empty ] \n");
        return;
    }

    printf("\n[ ");

    for (integer_t i = 0; i < stack->size - 1; i++)
    {
        stack->interface->display(stack->buffer[i]);
        printf(", ");
    }

    stack->interface->display(stack->buffer[stack->size - 1]);

    printf(" ]\n");
}

/// Displays a \c StackArray in the console with its values separated by
/// spaces.
///
/// \par Requirements
/// - An interface with a display function.
///
/// \param stack The stack to be displayed in the console.
///
/// \return DS_ERR_NULL_POINTER if the stack reference is \c NULL.
/// \return DS_OK if all operations were successful.
void sta_display_raw(StackArray_t *stack)
{
    printf("\n");

    if (sta_empty(stack))
        return;

    for (integer_t i = 0; i < stack->size; i++)
    {
        stack->interface->display(stack->buffer[i]);
        printf(" ");
    }

    printf("\n");
}

///////////////////////////////////////////////////// NOT EXPOSED FUNCTIONS ///

// This function reallocates the data buffer increasing its capacity
bool
static sta_grow(StackArray stack)
{
    if (stack->locked)
        return false;

    unsigned_t old_capacity = stack->capacity;

    // capacity = capacity * (growth_rate / 100)
    stack->capacity = (unsigned_t) ((double) (stack->capacity) *
            ((double) (stack->growth_rate) / 100.0));

    // 4 is the minimum growth
    if (stack->capacity - old_capacity < 4)
        stack->capacity = old_capacity + 4;

    void **new_buffer = realloc(stack->buffer,sizeof(void*) * stack->capacity);

    // Reallocation failed
    if (!new_buffer)
    {
        stack->capacity = old_capacity;

        return false;
    }

    stack->buffer = new_buffer;

    return true;
}

///////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////// Iterator ///
///////////////////////////////////////////////////////////////////////////////

/// \todo StackArrayIterator

///////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////// Wrapper ///
///////////////////////////////////////////////////////////////////////////////

/// \todo StackArrayWrapper
