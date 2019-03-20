/**
 * @file StackArray.h
 *
 * @author Leonardo Vencovsky (https://github.com/LeoVen)
 *
 * @date 03/10/2018
 */

#ifndef C_DATASTRUCTURES_LIBRARY_STACKARRAY_H
#define C_DATASTRUCTURES_LIBRARY_STACKARRAY_H

#include "Core.h"
#include "Interface.h"

#ifdef __cplusplus
extern "C" {
#endif

/// \struct StackArray_s
/// \brief An array-based generic stack.
struct StackArray_s;

/// \ref StackArray_t
/// \brief A type for an array-based implementation of a stack.
///
/// A type for a <code> struct StackArray_s </code> so you don't have to always
/// write the full name of it.
typedef struct StackArray_s StackArray_t;

/// \ref StackArray
/// \brief A pointer type for an array-based implementation of a stack.
///
/// A pointer type to <code> struct StackArray_s </code>. This typedef is used
/// to avoid having to declare every stack array as a pointer type since they
/// all must be dynamically allocated.
typedef struct StackArray_s *StackArray;

/// \ref sta_size
/// \brief The size of a StackArray_s in bytes.
extern const unsigned_t sta_size;

///////////////////////////////////// STRUCTURE INITIALIZATION AND DELETION ///

/// \ref sta_new
/// \brief Initializes a new StackArray_s.
StackArray_t *
sta_new(Interface_t *interface);

/// \ref sta_init
/// \brief Initializes a new StackArray_s on the stack.
bool
sta_init(StackArray_t *stack, Interface_t *interface,
         integer_t initial_capacity, integer_t growth_rate);

/// \ref sta_create
/// \brief Initializes a new StackArray_s with custom parameters.
StackArray_t *
sta_create(Interface_t *interface, integer_t initial_capacity,
           integer_t growth_rate);

/// \ref sta_free
/// \brief Frees from memory a StackArray_s and its elements.
void
sta_free(StackArray_t *stack);

/// \ref sta_free_shallow
/// \brief Frees from memory a StackArray_s without freeing its elements.
void
sta_free_shallow(StackArray_t *stack);

/// \ref sta_erase
/// \brief Resets the StackArray_s freeing all its elements.
void
sta_erase(StackArray_t *stack);

/// \ref sta_erase_shallow
/// \brief Resets the StackArray_s without freeing its elements.
void
sta_erase_shallow(StackArray_t *stack);

//////////////////////////////////////////////////////////// CONFIGURATIONS ///

/// \ref sta_config
/// \brief Sets a new interface for a target stack.
void
sta_config(StackArray_t *stack, Interface_t *new_interface);

/////////////////////////////////////////////////////////////////// GETTERS ///

/// \ref sta_count
/// \brief Returns the amount of elements in the specified stack.
integer_t
sta_count(StackArray_t *stack);

/// \ref sta_capacity
/// \brief Returns the total buffer capacity of the specified stack.
integer_t
sta_capacity(StackArray_t *stack);

/// \ref sta_growth
/// \brief Returns the growth rate of the specified stack.
integer_t
sta_growth(StackArray_t *stack);

/// \ref sta_locked
/// \brief Returns true if the stack's buffer is locked, false otherwise.
bool
sta_locked(StackArray_t *stack);

/////////////////////////////////////////////////////////////////// SETTERS ///

/// \ref sta_set_growth
/// \brief Sets a new growth rate for the stack's buffer.
bool
sta_set_growth(StackArray_t *stack, integer_t growth_rate);

/// \ref sta_capacity_lock
/// \brief Locks the buffer's growth for the specified stack.
void
sta_capacity_lock(StackArray_t *stack);

/// \ref sta_capacity_unlock
/// \brief Unlocks the buffer's growth for the specified stack.
void
sta_capacity_unlock(StackArray_t *stack);

////////////////////////////////////////////////////////// INPUT AND OUTPUT ///

/// \ref sta_push
/// \brief Inserts a new element in the specified stack.
bool
sta_push(StackArray_t *stack, void *element);

/// \ref sta_pop
/// \brief Removes and retrieves the top element in the specified stack.
bool
sta_pop(StackArray_t *stack, void **result);

/// \ref sta_peek
/// \brief Returns the top element in the specified stack.
void *
sta_peek(StackArray_t *stack);

/////////////////////////////////////////////////////////// STRUCTURE STATE ///

/// \ref sta_empty
/// \brief Returns true if the stack is empty, false otherwise.
bool
sta_empty(StackArray_t *stack);

/// \ref sta_full
/// \brief Returns true if the stack is full, false otherwise.
bool
sta_full(StackArray_t *stack);

/// \ref sta_fits
/// \brief Returns true if a given size fits inside the stack without
/// reallocating the buffer.
bool
sta_fits(StackArray_t *stack, integer_t size);

/////////////////////////////////////////////////////////////////// UTILITY ///

/// \ref sta_contains
/// \brief Returns true if an elements is present in the specified stack.
bool
sta_contains(StackArray_t *stack, void *key);

/// \ref sta_copy
/// \brief Returns a copy of the specified stack.
StackArray_t *
sta_copy(StackArray_t *stack);

/// \ref sta_copy_shallow
/// \brief Creates a shallow copy of the specified stack.
StackArray_t *
sta_copy_shallow(StackArray_t *stack);

/// \ref sta_compare
/// \brief Compares two stacks returning an int according to \ref compare_f.
int
sta_compare(StackArray_t *stack1, StackArray_t *stack2);

/// \ref sta_stack
/// \brief Stacks one stack at the top of the other.
bool
sta_stack(StackArray_t *stack1, StackArray_t *stack2);

/// \ref sta_to_array
/// \brief Makes a copy of the stack as a C array.
void **
sta_to_array(StackArray_t *stack, integer_t *length);

/////////////////////////////////////////////////////////////////// DISPLAY ///

/// \ref sta_display
/// \brief Displays a StackArray_s in the console.
void
sta_display(StackArray_t *stack, int display_mode);

///////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////// Iterator ///
///////////////////////////////////////////////////////////////////////////////

/// \struct StackArrayIterator_s.
/// \brief A StackArray_s iterator.
struct StackArrayIterator_s;

/// \brief A type for a stack iterator.
///
/// A type for a <code> struct StackArrayIterator_s </code>.
typedef struct StackArrayIterator_s StackArrayIterator_t;

/// \brief A pointer type for a stack iterator.
///
/// A pointer type for a <code> struct StackArrayIterator_s </code>.
typedef struct StackArrayIterator_s *StackArrayIterator;

///////////////////////////////////// STRUCTURE INITIALIZATION AND DELETION ///

/// \ref sta_iter_new
/// \brief Creates a new stack iterator given a target stack.
StackArrayIterator_t *
sta_iter_new(StackArray_t *target);

/// \ref sta_iter_retarget
/// \brief Retargets an existing iterator.
void
sta_iter_retarget(StackArrayIterator_t *iter, StackArray_t *target);

/// \ref sta_iter_free
/// \brief Frees from memory an existing iterator.
void
sta_iter_free(StackArrayIterator_t *iter);

///////////////////////////////////////////////////////////////// ITERATION ///

/// \ref sta_iter_next
/// \brief Iterates to the next element if available.
bool
sta_iter_next(StackArrayIterator_t *iter);

/// \ref sta_iter_prev
/// \brief Iterates to the previous element if available.
bool
sta_iter_prev(StackArrayIterator_t *iter);

/// \ref sta_iter_to_top
/// \brief Moves the cursor to the top element in the stack.
bool
sta_iter_to_top(StackArrayIterator_t *iter);

/// \ref sta_iter_to_bottom
/// \brief Moves the cursor to the last element in the stack.
bool
sta_iter_to_bottom(StackArrayIterator_t *iter);

/////////////////////////////////////////////////////////// STRUCTURE STATE ///

/// \ref sta_iter_has_next
/// \brief Returns true if there is another element next in the iteration.
bool
sta_iter_has_next(StackArrayIterator_t *iter);

/// \ref sta_iter_has_prev
/// \brief Returns true if there is a previous element in the iteration.
bool
sta_iter_has_prev(StackArrayIterator_t *iter);

////////////////////////////////////////////////////////// INPUT AND OUTPUT ///

/// \ref sta_iter_get
/// \brief Gets the element pointed by the iterator.
bool
sta_iter_get(StackArrayIterator_t *iter, void **result);

/// \ref sta_iter_set
/// \brief Sets the element pointed by the iterator to a new element.
bool
sta_iter_set(StackArrayIterator_t *iter, void *element);

/////////////////////////////////////////////////////////////////// UTILITY ///

/// \ref sta_iter_peek_next
/// \brief Returns the next element in the iteration if available.
void *
sta_iter_peek_next(StackArrayIterator_t *iter);

/// \ref sta_iter_peek
/// \brief Returns the current element in the iteration if available.
void *
sta_iter_peek(StackArrayIterator_t *iter);

/// \ref sta_iter_peek_prev
/// \brief Returns the previous element in the iteration if available.
void *
sta_iter_peek_prev(StackArrayIterator_t *iter);

#define STA_FOR_EACH(target, body)                          \
    do {                                                    \
        StackArrayIterator_t *iter_ = sta_iter_new(target); \
        while (sta_iter_has_next(iter_)) {                  \
            void *var = sta_iter_peek(iter_);               \
            body;                                           \
            sta_iter_next(iter_);                           \
        }                                                   \
        sta_iter_free(iter_);                               \
    } while (0);                                            \

#define STA_DECL(name)                                        \
    char name##_storage__[sta_size];                          \
    StackArray_t *name = (StackArray_t*)&name##_storage__[0]; \

///////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////// Wrapper ///
///////////////////////////////////////////////////////////////////////////////

/// \todo StackArrayWrapper

#ifdef __cplusplus
}
#endif

#endif //C_DATASTRUCTURES_LIBRARY_STACKARRAY_H
