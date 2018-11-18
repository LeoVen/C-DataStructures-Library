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

// A doubly-linked list implementation of a Queue. See the source file for the
// full documentation.
struct Queue;

/// Defines a type for <code> struct Queue_s </code>.
///
/// Every queue is initialized by \c malloc with \c sizeof(Queue_t).
typedef struct Queue_s Queue_t;

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

/// Defines a type of pointer to <code> struct Queue_s </code>.
///
/// This typedef is used to avoid having to declare every queue as a pointer
/// type since they all must be dynamically allocated.
typedef struct Queue_s *Queue;

///////////////////////////////////// STRUCTURE INITIALIZATION AND DELETION ///

Status que_init(Queue *queue);

Status que_create(Queue *queue, que_compare_f compare_f, que_copy_f copy_f,
                  que_display_f display_f, que_free_f free_f);

Status que_free(Queue *queue);

Status que_erase(Queue *queue);

/////////////////////////////////////////////////////////////////// SETTERS ///

Status que_set_func_compare(Queue queue, que_compare_f function);

Status que_set_func_copy(Queue queue, que_copy_f function);

Status que_set_func_display(Queue queue, que_display_f function);

Status que_set_func_free(Queue queue, que_free_f function);

Status que_set_limit(Queue queue, index_t limit);

/////////////////////////////////////////////////////////////////// GETTERS ///

index_t que_length(Queue queue);

index_t que_limit(Queue queue);

////////////////////////////////////////////////////////// INPUT AND OUTPUT ///

Status que_enqueue(Queue que, void *element);

Status que_dequeue(Queue que, void **result);

/////////////////////////////////////////////////////////// STRUCTURE STATE ///

bool que_full(Queue que);

bool que_empty(Queue que);

/////////////////////////////////////////////////////////////////// UTILITY ///

void *que_peek_front(Queue que);

void *que_peek_rear(Queue que);

Status que_copy(Queue que, Queue *result);

/////////////////////////////////////////////////////////////////// DISPLAY ///

Status que_display(Queue que);

Status que_display_array(Queue que);

Status que_display_raw(Queue que);

///////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////// Iterator ///
///////////////////////////////////////////////////////////////////////////////

// TODO

#ifdef __cplusplus
}
#endif

#endif //C_DATASTRUCTURES_LIBRARY_QUEUE_H
