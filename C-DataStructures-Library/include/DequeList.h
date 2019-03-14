/**
 * @file DequeList.h
 *
 * @author Leonardo Vencovsky (https://github.com/LeoVen)
 *
 * @date 27/09/2018
 */

#ifndef C_DATASTRUCTURES_LIBRARY_DEQUELIST_H
#define C_DATASTRUCTURES_LIBRARY_DEQUELIST_H

#include "Core.h"
#include "Interface.h"

#ifdef __cplusplus
extern "C" {
#endif

/// \struct DequeList_s;
/// \brief A linked list implementation of a generic deque.
struct DequeList_s;

/// \ref DequeList_t
/// \brief A type for a doubly-linked list implementation of a deque.
///
/// A type for a <code> struct DequeList_s </code> so you don't have to always
/// write the full name of it.
typedef struct DequeList_s DequeList_t;

/// \ref DequeList
/// \brief A pointer type for a doubly-linked list implementation of a deque.
///
/// A pointer type to <code> struct DequeList_s </code>. This typedef is used to
/// avoid having to declare every deque as a pointer type since they all must
/// be dynamically allocated.
typedef struct DequeList_s *DequeList;

/// \ref dql_size
/// \brief The size of a DequeList_s in bytes.
extern const unsigned_t dql_size;

///////////////////////////////////// STRUCTURE INITIALIZATION AND DELETION ///

/// \ref dql_new
/// \brief Initializes a new DequeList_s on the heap.
DequeList_t *
dql_new(Interface_t *interface);

/// \ref dql_init
/// \brief Initializes a new DequeList_s on the stack.
bool
dql_init(DequeList_t *deque, Interface_t *interface);

/// \ref dql_free
/// \brief Frees from memory a DequeList_s and its elements.
void
dql_free(DequeList_t *deque);

/// \ref dql_free_shallow
/// \brief Frees from memory a DequeList_s leaving its elements intact.
void
dql_free_shallow(DequeList_t *deque);

/// \ref dql_erase
/// \brief Resets the DequeList_s freeing all its elements.
void
dql_erase(DequeList_t *deque);

/// \ref dql_erase_shallow
/// \brief Resets the DequeList_s without freeing its elements.
void
dql_erase_shallow(DequeList_t *deque);

//////////////////////////////////////////////////////////// CONFIGURATIONS ///

/// \ref dql_config
/// \brief Sets a new interface for a target deque.
void
dql_config(DequeList_t *deque, Interface_t *new_interface);

/////////////////////////////////////////////////////////////////// GETTERS ///

/// \ref dql_count
/// \brief Returns the amount of elements in the specified deque.
integer_t
dql_count(DequeList_t *deque);

/// \ref dql_limit
/// \brief Returns the current deque limit.
integer_t
dql_limit(DequeList_t *deque);

/////////////////////////////////////////////////////////////////// SETTERS ///

/// \ref dql_set_limit
/// \brief Sets a limit to the amount of elements to the specified deque.
bool
dql_set_limit(DequeList_t *deque, integer_t limit);

////////////////////////////////////////////////////////// INPUT AND OUTPUT ///

/// \ref dql_enqueue_front
/// \brief Inserts an element at the front of the specified deque.
bool
dql_enqueue_front(DequeList_t *deque, void *element);

/// \ref dql_enqueue_rear
/// \brief Inserts an element at the rear of the specified deque.
bool
dql_enqueue_rear(DequeList_t *deque, void *element);

/// \ref dql_dequeue_front
/// \brief Removes an element from the front of the specified deque.
bool
dql_dequeue_front(DequeList_t *deque, void **result);

/// \ref dql_dequeue_rear
/// \brief Removes an element from the rear of the specified deque.
bool
dql_dequeue_rear(DequeList_t *deque, void **result);

/// \ref dql_peek_front
/// \brief Returns the front element in the specified deque.
void *
dql_peek_front(DequeList_t *deque);

/// \ref dql_peek_rear
/// \brief Returns the rear element in the specified deque.
void *
dql_peek_rear(DequeList_t *deque);

/////////////////////////////////////////////////////////// STRUCTURE STATE ///

/// \ref dql_empty
/// \brief Returns true if the deque is full, false otherwise.
bool
dql_empty(DequeList_t *deque);

/// \ref dql_full
/// \brief Returns true if the deque is empty, false otherwise.
bool
dql_full(DequeList_t *deque);

/// \ref dql_fits
/// \brief Returns true if a given size will fit in the deque, assuming it has
/// a limit set.
bool
dql_fits(DequeList_t *deque, unsigned_t size);

/////////////////////////////////////////////////////////////////// UTILITY ///

/// \ref dql_contains
/// \brief Returns true if an elements is present in the specified deque.
bool
dql_contains(DequeList_t *deque, void *key);

/// \ref dql_copy
/// \brief Returns a copy of the specified deque.
DequeList_t *
dql_copy(DequeList_t *deque);

/// \ref dql_copy_shallow
/// \brief Creates a shallow copy of the specified deque.
DequeList_t *
dql_copy_shallow(DequeList_t *deque);

/// \ref dql_compare
/// \brief Compares two deques returning an int according to \ref compare_f.
int
dql_compare(DequeList_t *deque1, DequeList_t *deque2);

/// \ref dql_append
/// \brief Appends deque2 at the rear of deque1.
bool
dql_append(DequeList_t *deque1, DequeList_t *deque2);

/// \ref dql_append
/// \brief Prepends deque2 at the front of deque1.
bool
dql_prepend(DequeList_t *deque1, DequeList_t *deque2);

/// \ref dql_to_array
/// \brief Makes a copy of the deque as a C array.
void **
dql_to_array(DequeList_t *deque, integer_t *length);

/////////////////////////////////////////////////////////////////// DISPLAY ///

/// \ref dql_display
/// \brief Displays a DequeList_s in the console.
void
dql_display(DequeList_t *deque, int display_mode);

///////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////// Iterator ///
///////////////////////////////////////////////////////////////////////////////

/// \todo DequeListIterator

///////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////// Wrapper ///
///////////////////////////////////////////////////////////////////////////////

/// \todo DequeListWrapper

#ifdef __cplusplus
}
#endif

#endif //C_DATASTRUCTURES_LIBRARY_DEQUELIST_H
