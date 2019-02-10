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

///////////////////////////////////// STRUCTURE INITIALIZATION AND DELETION ///

/// \ref sta_new
/// \brief Initializes a new StackArray_s.
StackArray_t *
sta_new(Interface_t *interface);

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
/// \brief Frees from memory a StackArray_s leaving its elements intact.
void
sta_free_shallow(StackArray_t *stack);

/// \ref sta_erase
/// \brief Frees from memory all elements of a StackArray_s.
bool
sta_erase(StackArray_t *stack);

/// \ref sta_erase_shallow
/// \brief Resets the StackArray_s without freeing its elements.
bool
sta_erase_shallow(StackArray_t *stack);

//////////////////////////////////////////////////////////// CONFIGURATIONS ///

/// \ref sta_config
/// \brief Sets a new interface for a target stack.
void
sta_config(StackArray_t *stack, Interface_t *new_interface);

/////////////////////////////////////////////////////////////////// GETTERS ///

/// \ref sta_size
/// \brief Returns the amount of elements in the specified stack.
integer_t
sta_size(StackArray_t *stack);

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

////////////////////////////////////////////////////////// INPUT AND OUTPUT ///

/// \ref sta_push
/// \brief Inserts a new element in the specified stack.
bool
sta_push(StackArray_t *stack, void *element);

/// \ref sta_pop
/// \brief Removes and retrieves a new element in the specified stack.
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
sta_fits(StackArray_t *stack, unsigned_t size);

/// \ref sta_capacity_lock
/// \brief Locks the buffer's growth for the specified stack.
void
sta_capacity_lock(StackArray stack);

/// \ref sta_capacity_unlock
/// \brief Unlocks the buffer's growth for the specified stack.
void
sta_capacity_unlock(StackArray stack);

/////////////////////////////////////////////////////////////////// UTILITY ///

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

/// \todo StackArrayIterator

///////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////// Wrapper ///
///////////////////////////////////////////////////////////////////////////////

/// \todo StackArrayWrapper

#ifdef __cplusplus
}
#endif

#endif //C_DATASTRUCTURES_LIBRARY_STACKARRAY_H
