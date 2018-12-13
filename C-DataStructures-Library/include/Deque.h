/**
 * @file Deque.h
 *
 * @author Leonardo Vencovsky (https://github.com/LeoVen)
 *
 * @date 27/09/2018
 */

#ifndef C_DATASTRUCTURES_LIBRARY_DEQUE_H
#define C_DATASTRUCTURES_LIBRARY_DEQUE_H

#include "Core.h"

#ifdef __cplusplus
extern "C" {
#endif

// A doubly-linked list implementation of a Deque. See the source file for the
// full documentation.
struct Deque_s;

/// \brief A type for a doubly-linked list implementation of a deque.
///
/// A type for a <code> struct Deque_s </code> so you don't have to always
/// write the full name of it.
typedef struct Deque_s Deque_t;

/// \brief A pointer type for a doubly-linked list implementation of a deque.
///
/// A pointer type to <code> struct Deque_s </code>. This typedef is used to
/// avoid having to declare every deque as a pointer type since they all must
/// be dynamically allocated.
typedef struct Deque_s *Deque;

/// \brief Comparator function type.
///
/// A type for a function that compares two elements, returning:
/// - [ > 0] when the first element is greater than the second;
/// - [ < 0] when the first element is less than the second;
/// - 0 when both elements are equal.
typedef int(*deq_compare_f)(void *, void *);

/// \brief A Copy function type.
///
/// A type for a function that takes an input (first parameter) and returns an
/// exact copy of that element.
typedef void *(*deq_copy_f)(void *);

/// \brief Display function type.
///
/// A type for a function that displays an element in the console. Please do
/// not print a newline character.
typedef void(*deq_display_f)(void *);

/// \brief A Free function type.
///
/// A type for a function responsible for completely freeing an element from
/// memory.
typedef void(*deq_free_f)(void *);

///////////////////////////////////// STRUCTURE INITIALIZATION AND DELETION ///

Status deq_init(Deque *deque);

Status deq_create(Deque *deque, deq_compare_f compare_f, deq_copy_f copy_f,
                  deq_display_f display_f, deq_free_f free_f);

Status deq_free(Deque *deque);

Status deq_free_shallow(Deque *deque);

Status deq_erase(Deque *deque);

/////////////////////////////////////////////////////////////////// SETTERS ///

Status deq_set_v_compare(Deque deque, deq_compare_f function);

Status deq_set_v_copy(Deque deque, deq_copy_f function);

Status deq_set_v_display(Deque deque, deq_display_f function);

Status deq_set_v_free(Deque deque, deq_free_f function);

Status deq_set_limit(Deque deque, integer_t limit);

/////////////////////////////////////////////////////////////////// GETTERS ///

integer_t deq_length(Deque deque);

integer_t deq_limit(Deque deque);

////////////////////////////////////////////////////////// INPUT AND OUTPUT ///

Status deq_enqueue_front(Deque deque, void *element);

Status deq_enqueue_rear(Deque deque, void *element);

Status deq_dequeue_front(Deque deque, void **result);

Status deq_dequeue_rear(Deque deque, void **result);

/////////////////////////////////////////////////////////// STRUCTURE STATE ///

bool deq_full(Deque deque);

bool deq_empty(Deque deque);

/////////////////////////////////////////////////////////////////// UTILITY ///

void *deq_peek_front(Deque deque);

void *deq_peek_rear(Deque deque);

bool deq_contains(Deque deque, void *key);

Status deq_copy(Deque deque, Deque *result);

Status deq_append(Deque deque1, Deque deque2);

Status deq_prepend(Deque deque1, Deque deque2);

Status deq_to_array(Deque deque,  void ***result, integer_t *length);

/////////////////////////////////////////////////////////////////// DISPLAY ///

Status deq_display(Deque deque);

Status deq_display_array(Deque deque);

Status deq_display_raw(Deque deque);

#ifdef __cplusplus
}
#endif

#endif //C_DATASTRUCTURES_LIBRARY_DEQUE_H
