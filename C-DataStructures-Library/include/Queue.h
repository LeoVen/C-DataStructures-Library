/**
 * @file Queue.h
 * 
 * @author Leonardo Vencovsky (https://github.com/LeoVen)
 *
 * @date 26/09/2018
 */

#ifndef C_DATASTRUCTURES_LIBRARY_QUEUE_H
#define C_DATASTRUCTURES_LIBRARY_QUEUE_H

#include "Core.h"

#ifdef __cplusplus
extern "C" {
#endif

// A singly-linked list implementation of a Queue. See the source file for the
// full documentation.
struct Queue_s;

/// \brief A type for a singly-linked list implementation of a queue.
///
/// A type for a <code> struct Queue_s </code> so you don't have to always
/// write the full name of it.
typedef struct Queue_s Queue_t;

/// \brief A pointer type for a singly-linked list implementation of a queue.
///
/// A pointer type to <code> struct Queue_s </code>. This typedef is used to
/// avoid having to declare every queue as a pointer type since they all must
/// be dynamically allocated.
typedef struct Queue_s *Queue;

/// \brief Comparator function type.
///
/// A type for a function that compares two elements, returning:
/// - [ > 0] when the first element is greater than the second;
/// - [ < 0] when the first element is less than the second;
/// - 0 when both elements are equal.
typedef int(*que_compare_f)(void *, void *);

/// \brief A Copy function type.
///
/// A type for a function that takes an input (first parameter) and returns an
/// exact copy of that element.
typedef void *(*que_copy_f)(void *);

/// \brief Display function type.
///
/// A type for a function that displays an element in the console. Please do
/// not print a newline character.
typedef void(*que_display_f)(void *);

/// \brief A Free function type.
///
/// A type for a function responsible for completely freeing an element from
/// memory.
typedef void(*que_free_f)(void *);

///////////////////////////////////// STRUCTURE INITIALIZATION AND DELETION ///

Status que_init(Queue *queue);

Status que_create(Queue *queue, que_compare_f compare_f, que_copy_f copy_f,
                  que_display_f display_f, que_free_f free_f);

Status que_free(Queue *queue);

Status que_free_shallow(Queue *queue);

Status que_erase(Queue *queue);

/////////////////////////////////////////////////////////////////// SETTERS ///

Status que_set_v_compare(Queue queue, que_compare_f function);

Status que_set_v_copy(Queue queue, que_copy_f function);

Status que_set_v_display(Queue queue, que_display_f function);

Status que_set_v_free(Queue queue, que_free_f function);

Status que_set_limit(Queue queue, integer_t limit);

/////////////////////////////////////////////////////////////////// GETTERS ///

integer_t que_length(Queue queue);

integer_t que_limit(Queue queue);

////////////////////////////////////////////////////////// INPUT AND OUTPUT ///

Status que_enqueue(Queue queue, void *element);

Status que_dequeue(Queue queue, void **result);

/////////////////////////////////////////////////////////// STRUCTURE STATE ///

bool que_full(Queue queue);

bool que_empty(Queue queue);

/////////////////////////////////////////////////////////////////// UTILITY ///

void *que_peek_front(Queue queue);

void *que_peek_rear(Queue queue);

bool que_contains(Queue queue, void *key);

Status que_copy(Queue queue, Queue *result);

Status que_append(Queue queue, Queue queue2);

Status que_to_array(Queue queue,  void ***result, integer_t *length);

/////////////////////////////////////////////////////////////////// DISPLAY ///

Status que_display(Queue queue);

Status que_display_array(Queue queue);

Status que_display_raw(Queue queue);

///////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////// Iterator ///
///////////////////////////////////////////////////////////////////////////////

// A queue iterator. See the source file for the full documentation.
struct QueueIterator_s;

/// \brief A type for a queue iterator.
///
/// A type for a <code> struct QueueIterator_s </code>.
typedef struct QueueIterator_s QueueIterator_t;

/// \brief A pointer type for a queue iterator.
///
/// A pointer type for a <code> struct QueueIterator_s </code>.
typedef struct QueueIterator_s *QueueIterator;

///////////////////////////////////// STRUCTURE INITIALIZATION AND DELETION ///

Status que_iter_init(QueueIterator *iter, Queue target);

Status que_iter_retarget(QueueIterator *iter, Queue target);

Status que_iter_free(QueueIterator *iter);

///////////////////////////////////////////////////////////////// ITERATION ///

Status que_iter_next(QueueIterator iter);

Status que_iter_to_front(QueueIterator iter);

Status que_iter_to_rear(QueueIterator iter);

/////////////////////////////////////////////////////////// STRUCTURE STATE ///

bool que_iter_has_next(QueueIterator iter);

////////////////////////////////////////////////////////// SETTER AND GETTER ///

Status que_iter_get(QueueIterator iter, void **result);

Status que_iter_set(QueueIterator iter, void *element);

////////////////////////////////////////////////////////// INPUT AND OUTPUT ///

Status que_iter_insert(QueueIterator iter, void *element);

Status que_iter_remove(QueueIterator iter, void **result);

/////////////////////////////////////////////////////////////////// UTILITY ///

void *que_iter_peek_next(QueueIterator iter);

void *que_iter_peek(QueueIterator iter);

#ifdef __cplusplus
}
#endif

#endif //C_DATASTRUCTURES_LIBRARY_QUEUE_H
