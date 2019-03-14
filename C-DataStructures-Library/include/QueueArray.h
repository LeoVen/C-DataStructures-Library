/**
 * @file QueueArray.h
 *
 * @author Leonardo Vencovsky (https://github.com/LeoVen)
 *
 * @date 04/10/2018
 */

#ifndef C_DATASTRUCTURES_LIBRARY_QUEUEARRAY_H
#define C_DATASTRUCTURES_LIBRARY_QUEUEARRAY_H

#include "Core.h"
#include "Interface.h"

#ifdef __cplusplus
extern "C" {
#endif

/// \struct QueueArray_s
/// \brief An array-based generic queue.
struct QueueArray_s;

/// \ref QueueArray_t
/// \brief A type for an array-based implementation of a queue.
///
/// A type for a <code> struct QueueArray_s </code> so you don't have to always
/// write the full name of it.
typedef struct QueueArray_s QueueArray_t;

/// \ref QueueArray
/// \brief A pointer type for an array-based implementation of a queue.
///
/// A pointer type to <code> struct QueueArray_s </code>. This typedef is used
/// to avoid having to declare every queue array as a pointer type since they
/// all must be dynamically allocated.
typedef struct QueueArray_s *QueueArray;

/// \ref qar_size
/// \brief The size of a QueueArray_s in bytes.
extern const unsigned_t qar_size;

///////////////////////////////////// STRUCTURE INITIALIZATION AND DELETION ///

/// \ref qar_new
/// \brief Initializes a new QueueArray_s.
QueueArray_t *
qar_new(Interface_t *interface);

/// \ref qar_init
/// \brief Initializes a new QueueArray_s on the stack.
bool
qar_init(QueueArray_t *queue, Interface_t *interface,
         integer_t initial_capacity, integer_t growth_rate);

/// \ref qar_create
/// \brief Initializes a new QueueArray_s with custom parameters.
QueueArray_t *
qar_create(Interface_t *interface, integer_t initial_capacity,
           integer_t growth_rate);

/// \ref qar_free
/// \brief Frees from memory a QueueArray_s and its elements.
void
qar_free(QueueArray_t *queue);

/// \ref qar_free_shallow
/// \brief Frees from memory a QueueArray_s leaving its elements intact.
void
qar_free_shallow(QueueArray_t *queue);

/// \ref qar_erase
/// \brief Resets the QueueArray_s freeing all its elements.
void
qar_erase(QueueArray_t *queue);

/// \ref qar_erase_shallow
/// \brief Resets the QueueArray_s without freeing its elements.
void
qar_erase_shallow(QueueArray_t *queue);

//////////////////////////////////////////////////////////// CONFIGURATIONS ///

/// \ref qar_config
/// \brief Sets a new interface for a target queue.
void
qar_config(QueueArray_t *queue, Interface_t *new_interface);

/////////////////////////////////////////////////////////////////// GETTERS ///

/// \ref qar_count
/// \brief Returns the amount of elements in the specified queue.
integer_t
qar_count(QueueArray_t *queue);

/// \ref qar_capacity
/// \brief Returns the total buffer capacity of the specified queue.
integer_t
qar_capacity(QueueArray_t *queue);

/// \ref qar_growth
/// \brief Returns the growth rate of the specified queue.
integer_t
qar_growth(QueueArray_t *queue);

/// \ref qar_locked
/// \brief Returns true if the queue's buffer is locked, false otherwise.
bool
qar_locked(QueueArray_t *queue);

/////////////////////////////////////////////////////////////////// SETTERS ///

/// \ref qar_set_growth
/// \brief Sets a new growth rate for the queue's buffer.
bool
qar_set_growth(QueueArray_t *queue, integer_t growth_rate);

/// \ref qar_capacity_lock
/// \brief Locks the buffer's growth for the specified queue.
void
qar_capacity_lock(QueueArray_t *queue);

/// \ref qar_capacity_unlock
/// \brief Unlocks the buffer's growth for the specified queue.
void
qar_capacity_unlock(QueueArray_t *queue);

////////////////////////////////////////////////////////// INPUT AND OUTPUT ///

/// \ref qar_enqueue
/// \brief Adds an element to the specified queue.
bool
qar_enqueue(QueueArray_t *queue, void *element);

/// \ref qar_dequeue
/// \brief Removes and element from the specified queue.
bool
qar_dequeue(QueueArray_t *queue, void **result);

/// \ref qar_peek_front
/// \brief Returns the oldest element in the specified queue.
void *
qar_peek_front(QueueArray_t *queue);

/// \ref qar_peek_rear
/// \brief Returns the last item added in the queue.
void *
qar_peek_rear(QueueArray_t *queue);

/////////////////////////////////////////////////////////// STRUCTURE STATE ///

/// \ref qar_empty
/// \brief Returns true if the queue is empty, false otherwise.
bool
qar_empty(QueueArray_t *queue);

/// \ref qar_full
/// \brief Returns true if the queue is full, false otherwise.
bool
qar_full(QueueArray_t *queue);

/// \ref qar_fits
/// \brief Returns true if a given size fits inside the queue without
/// reallocating the buffer.
bool
qar_fits(QueueArray_t *queue, unsigned_t size);

/////////////////////////////////////////////////////////////////// UTILITY ///

/// \ref qar_contains
/// \brief Returns true if an elements is present in the specified queue.
bool
qar_contains(QueueArray_t *queue, void *key);

/// \ref qar_copy
/// \brief Returns a copy of the specified queue.
QueueArray_t *
qar_copy(QueueArray_t *queue);

/// \ref qar_copy_shallow
/// \brief Creates a shallow copy of the specified queue.
QueueArray_t *
qar_copy_shallow(QueueArray_t *queue);

/// \ref qar_compare
/// \brief Compares two queues returning an int according to \ref compare_f.
int
qar_compare(QueueArray_t *queue1, QueueArray_t *queue2);

/// \ref qar_append
/// \brief Appends one queue at the back of the other.
bool
qar_append(QueueArray_t *queue1, QueueArray_t *queue2);

/// \ref qar_to_array
/// \brief Makes a copy of the queue as a C array.
void **
qar_to_array(QueueArray_t *queue, integer_t *length);

/////////////////////////////////////////////////////////////////// DISPLAY ///

/// \ref qar_display
/// \brief Displays a QueueArray_s in the console.
void
qar_display(QueueArray_t *queue, int display_mode);

///////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////// Iterator ///
///////////////////////////////////////////////////////////////////////////////

/// \todo QueueArrayIterator

///////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////// Wrapper ///
///////////////////////////////////////////////////////////////////////////////

/// \todo QueueArrayWrapper

#ifdef __cplusplus
}
#endif

#endif //C_DATASTRUCTURES_LIBRARY_QUEUEARRAY_H
