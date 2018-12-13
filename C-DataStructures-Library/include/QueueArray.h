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
/// \brief A type for a queue array.
///
/// A type for a <code> struct QueueArray_s </code> so you don't have to always
/// write the full name of it.
typedef struct QueueArray_s QueueArray_t;

/// \ref QueueArray
/// \brief Defines a type of pointer to <code> struct QueueArray_s </code>.
///
/// This typedef is used to avoid having to declare every queue as a pointer
/// type since they all must be dynamically allocated.
typedef struct QueueArray_s *QueueArray;

///////////////////////////////////// STRUCTURE INITIALIZATION AND DELETION ///

/// \ref qua_new
/// \brief Initializes a new QueueArray_s.
QueueArray_t *
qua_new(Interface_t *interface);

/// \ref qua_create
/// \brief Initializes a new QueueArray_s with custom parameters.
QueueArray_t *
qua_create(integer_t initial_capacity, integer_t growth_rate,
           Interface_t *interface);

/// \ref qua_free
/// \brief Frees from memory a QueueArray_s and its elements.
void
qua_free(QueueArray_t *queue);

/// \ref qua_free_shallow
/// \brief Frees from memory a QueueArray_s leaving its elements intact.
void
qua_free_shallow(QueueArray_t *queue);

/// \ref qua_erase
/// \brief Frees from memory all elements of a QueueArray_s.
bool
qua_erase(QueueArray_t *queue);

//////////////////////////////////////////////////////////// CONFIGURATIONS ///

/// \ref qua_config
/// \brief Sets a new interface for a target queue.
void
qua_config(QueueArray_t *queue, Interface_t *new_interface);

/////////////////////////////////////////////////////////////////// GETTERS ///

/// \ref qua_size
/// \brief Returns the amount of elements in the specified queue.
integer_t
qua_size(QueueArray_t *queue);

/// \ref qua_capacity
/// \brief Returns the total buffer capacity of the specified queue.
integer_t
qua_capacity(QueueArray_t *queue);

/// \ref qua_growth
/// \brief Returns the growth rate of the specified queue.
integer_t
qua_growth(QueueArray_t *queue);

/// \ref qua_locked
/// \brief Returns true if the queue's buffer is locked, false otherwise.
bool
qua_locked(QueueArray_t *queue);

/////////////////////////////////////////////////////////////////// SETTERS ///

/// \ref qua_set_growth
/// \brief Sets a new growth rate for the queue's buffer.
bool
qua_set_growth(QueueArray_t *queue, integer_t growth_rate);

////////////////////////////////////////////////////////// INPUT AND OUTPUT ///

/// \ref qua_enqueue
/// \brief Adds an element to the specified queue.
bool
qua_enqueue(QueueArray_t *queue, void *element);

/// \ref qua_dequeue
/// \brief Removes and element from the specified queue.
bool
qua_dequeue(QueueArray_t *queue, void **result);

/// \ref qua_peek_front
/// \brief Returns oldest element in the specified queue.
void *
qua_peek_front(QueueArray_t *queue);

/// \ref qua_peek_rear
/// \brief Returns the last item added in the queue.
void *
qua_peek_rear(QueueArray_t *queue);

/////////////////////////////////////////////////////////// STRUCTURE STATE ///

/// \ref qua_empty
/// \brief Returns true if the queue is empty, false otherwise.
bool
qua_empty(QueueArray_t *queue);

/// \ref qua_full
/// \brief Returns true if the queue is full, false otherwise.
bool
qua_full(QueueArray_t *queue);

/// \ref qua_fits
/// \brief Returns true if a given size fits inside the queue without
/// reallocating the buffer.
bool
qua_fits(QueueArray_t *queue, unsigned_t size);

/// \ref qua_capacity_lock
/// \brief Locks the buffer's growth for the specified queue.
void
qua_capacity_lock(QueueArray_t *queue);

/// \ref qua_capacity_unlock
/// \brief Unlocks the buffer's growth for the specified queue.
void
qua_capacity_unlock(QueueArray_t *queue);

/////////////////////////////////////////////////////////////////// UTILITY ///

/// \ref qua_copy
/// \brief Returns a copy of the specified queue.
QueueArray_t *
qua_copy(QueueArray_t *queue);

/// \ref qua_copy_shallow
/// \brief Creates a shallow copy of the specified queue.
QueueArray_t *
qua_copy_shallow(QueueArray_t *queue);

/// \ref qua_compare
/// \brief Compares two queues returning an int according to \ref compare_f.
int
qua_compare(QueueArray_t *queue1, QueueArray_t *queue2);

/// \ref qua_to_array
/// \brief Makes a copy of the queue as a C array.
void **
qua_to_array(QueueArray_t *queue, integer_t *length);

/////////////////////////////////////////////////////////////////// DISPLAY ///

/// \ref qua_display
/// \brief Displays a QueueArray_s in the console.
void
qua_display(QueueArray_t *queue, int display_mode);


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
