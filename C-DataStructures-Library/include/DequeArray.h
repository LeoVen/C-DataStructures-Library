/**
 * @file DequeArray.h
 *
 * @author Leonardo Vencovsky (https://github.com/LeoVen)
 *
 * @date 29/10/2018
 */

#ifndef C_DATASTRUCTURES_LIBRARY_DEQUEARRAY_H
#define C_DATASTRUCTURES_LIBRARY_DEQUEARRAY_H

#include "Core.h"
#include "Interface.h"

#ifdef __cplusplus
extern "C" {
#endif

/// \struct DequeArray_s
/// \brief A generic array-based double-ended queue.
struct DequeArray_s;

/// \ref DequeArray_t
/// \brief A type for a deque array.
///
/// A type for a <code> struct DequeArray_s </code> so you don't have to always
/// write the full name of it.
typedef struct DequeArray_s DequeArray_t;

/// \ref DequeArray
/// \brief A pointer type for an array-based implementation of a deque.
///
/// A pointer type to <code> struct DequeArray_s </code>. This typedef is used
/// to avoid having to declare every deque array as a pointer type since they
/// all must be dynamically allocated.
typedef struct DequeArray_s *DequeArray;

/// \ref dqa_size
/// \brief The size of a DequeArray_s in bytes.
extern const unsigned_t dqa_size;

///////////////////////////////////// STRUCTURE INITIALIZATION AND DELETION ///

/// \ref dqa_new
/// \brief Initializes a new DequeArray_s.
DequeArray_t *
dqa_new(Interface_t *interface);

/// \ref dqa_init
/// \brief Initializes a new DequeArray_s on the stack.
bool
dqa_init(DequeArray_t *deque, Interface_t *interface,
         integer_t initial_capacity, integer_t growth_rate);

/// \ref dqa_create
/// \brief Initializes a new DequeArray_s with custom parameters.
DequeArray_t *
dqa_create(Interface_t *interface, integer_t initial_capacity,
           integer_t growth_rate);

/// \ref dqa_free
/// \brief Frees from memory a DequeArray_s and its elements.
void
dqa_free(DequeArray_t *deque);

/// \ref dqa_free_shallow
/// \brief Frees from memory a DequeArray_s leaving its elements intact.
void
dqa_free_shallow(DequeArray_t *deque);

/// \ref dqa_erase
/// \brief Resets the DequeArray_s freeing all its elements.
void
dqa_erase(DequeArray_t *deque);

/// \ref dqa_erase_shallow
/// \brief Resets the DequeArray_s without freeing its elements.
void
dqa_erase_shallow(DequeArray_t *deque);

//////////////////////////////////////////////////////////// CONFIGURATIONS ///

/// \ref dqa_config
/// \brief Sets a new interface for a target deque.
void
dqa_config(DequeArray_t *deque, Interface_t *new_interface);

/////////////////////////////////////////////////////////////////// GETTERS ///

/// \ref dqa_count
/// \brief Returns the amount of elements in the specified deque.
integer_t
dqa_count(DequeArray_t *deque);

/// \ref dqa_capacity
/// \brief Returns the total buffer capacity of the specified deque.
integer_t
dqa_capacity(DequeArray_t *deque);

/// \ref dqa_growth
/// \brief Returns the growth rate of the specified deque.
integer_t
dqa_growth(DequeArray_t *deque);

/// \ref dqa_locked
/// \brief Returns true if the deque's buffer is locked, false otherwise.
bool
dqa_locked(DequeArray_t *deque);

/////////////////////////////////////////////////////////////////// SETTERS ///

/// \ref dqa_set_growth
/// \brief Sets a new growth rate for the deque's buffer.
bool
dqa_set_growth(DequeArray_t *deque, integer_t growth_rate);

/// \ref dqa_capacity_lock
/// \brief Locks the buffer's growth for the specified deque.
void
dqa_capacity_lock(DequeArray_t *deque);

/// \ref dqa_capacity_unlock
/// \brief Unlocks the buffer's growth for the specified deque.
void
dqa_capacity_unlock(DequeArray_t *deque);

////////////////////////////////////////////////////////// INPUT AND OUTPUT ///

/// \ref dqa_enqueue_front
/// \brief Inserts an element at the front of the specified deque.
bool
dqa_enqueue_front(DequeArray_t *deque, void *element);

/// \ref dqa_enqueue_rear
/// \brief Inserts an element at the rear of the specified deque.
bool
dqa_enqueue_rear(DequeArray_t *deque, void *element);

/// \ref dqa_dequeue_front
/// \brief Removes an element from the front of the specified deque.
bool
dqa_dequeue_front(DequeArray_t *deque, void **result);

/// \ref dqa_dequeue_rear
/// \brief Removes an element from the rear of the specified deque.
bool
dqa_dequeue_rear(DequeArray_t *deque, void **result);

/// \ref dqa_peek_front
/// \brief Returns the front element in the specified deque.
void *
dqa_peek_front(DequeArray_t *deque);

/// \ref dqa_peek_rear
/// \brief Returns the rear element in the specified deque.
void *
dqa_peek_rear(DequeArray_t *deque);

/////////////////////////////////////////////////////////// STRUCTURE STATE ///

/// \ref dqa_empty
/// \brief Returns true if the deque is empty, false otherwise.
bool dqa_empty(DequeArray_t *deque);

/// \ref dqa_full
/// \brief Returns true if the deque is full, false otherwise.
bool dqa_full(DequeArray_t *deque);

/// \ref dqa_fits
/// \brief Returns true if a given size fits inside the deque without
/// reallocating the buffer.
bool dqa_fits(DequeArray_t *deque, unsigned_t size);

/////////////////////////////////////////////////////////////////// UTILITY ///

/// \ref dqa_contains
/// \brief Returns true if an elements is present in the specified deque.
bool
dqa_contains(DequeArray_t *deque, void *key);

/// \ref dqa_copy
/// \brief Returns a copy of the specified deque.
DequeArray_t *
dqa_copy(DequeArray_t *deque);

/// \ref dqa_copy_shallow
/// \brief Creates a shallow copy of the specified deque.
DequeArray_t *
dqa_copy_shallow(DequeArray_t *deque);

/// \ref dqa_compare
/// \brief Compares two deques returning an int according to \ref compare_f.
int
dqa_compare(DequeArray_t *deque1, DequeArray_t *deque2);

/// \ref dqa_append
/// \brief Appends deque2 at the rear of deque1.
bool
dqa_append(DequeArray_t *deque1, DequeArray_t *deque2);

/// \ref dqa_prepend
/// \brief Prepends deque2 at the front of deque1.
bool
dqa_prepend(DequeArray_t *deque1, DequeArray_t *deque2);

/// \ref dqa_to_array
/// \brief Makes a copy of the deque as a C array.
void **
dqa_to_array(DequeArray_t *deque, integer_t *length);

/////////////////////////////////////////////////////////////////// DISPLAY ///

/// \ref dqa_display
/// \brief Displays a DequeArray_s in the console.
void
dqa_display(DequeArray_t *deque, int display_mode);

///////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////// Iterator ///
///////////////////////////////////////////////////////////////////////////////

/// \todo DequeArrayIterator

///////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////// Wrapper ///
///////////////////////////////////////////////////////////////////////////////

/// \todo DequeArrayWrapper

#ifdef __cplusplus
}
#endif

#endif //C_DATASTRUCTURES_LIBRARY_DEQUEARRAY_H
