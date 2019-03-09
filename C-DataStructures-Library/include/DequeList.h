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

#ifdef __cplusplus
extern "C" {
#endif

// A doubly-linked list implementation of a Deque. See the source file for the
// full documentation.
struct DequeList_s;

/// \brief A type for a doubly-linked list implementation of a deque.
///
/// A type for a <code> struct DequeList_s </code> so you don't have to always
/// write the full name of it.
typedef struct DequeList_s DequeList_t;

/// \brief A pointer type for a doubly-linked list implementation of a deque.
///
/// A pointer type to <code> struct DequeList_s </code>. This typedef is used to
/// avoid having to declare every deque as a pointer type since they all must
/// be dynamically allocated.
typedef struct DequeList_s *DequeList;

/// \brief Comparator function type.
///
/// A type for a function that compares two elements, returning:
/// - [ > 0] when the first element is greater than the second;
/// - [ < 0] when the first element is less than the second;
/// - 0 when both elements are equal.
typedef int(*dql_compare_f)(void *, void *);

/// \brief A Copy function type.
///
/// A type for a function that takes an input (first parameter) and returns an
/// exact copy of that element.
typedef void *(*dql_copy_f)(void *);

/// \brief Display function type.
///
/// A type for a function that displays an element in the console. Please do
/// not print a newline character.
typedef void(*dql_display_f)(void *);

/// \brief A Free function type.
///
/// A type for a function responsible for completely freeing an element from
/// memory.
typedef void(*dql_free_f)(void *);

///////////////////////////////////// STRUCTURE INITIALIZATION AND DELETION ///

Status dql_init(DequeList *deque);

Status dql_create(DequeList *deque, dql_compare_f compare_f, dql_copy_f copy_f,
                  dql_display_f display_f, dql_free_f free_f);

Status dql_free(DequeList *deque);

Status dql_free_shallow(DequeList *deque);

Status dql_erase(DequeList *deque);

/////////////////////////////////////////////////////////////////// SETTERS ///

Status dql_set_v_compare(DequeList deque, dql_compare_f function);

Status dql_set_v_copy(DequeList deque, dql_copy_f function);

Status dql_set_v_display(DequeList deque, dql_display_f function);

Status dql_set_v_free(DequeList deque, dql_free_f function);

Status dql_set_limit(DequeList deque, integer_t limit);

/////////////////////////////////////////////////////////////////// GETTERS ///

integer_t dql_length(DequeList deque);

integer_t dql_limit(DequeList deque);

////////////////////////////////////////////////////////// INPUT AND OUTPUT ///

Status dql_enqueue_front(DequeList deque, void *element);

Status dql_enqueue_rear(DequeList deque, void *element);

Status dql_dequeue_front(DequeList deque, void **result);

Status dql_dequeue_rear(DequeList deque, void **result);

/////////////////////////////////////////////////////////// STRUCTURE STATE ///

bool dql_full(DequeList deque);

bool dql_empty(DequeList deque);

/////////////////////////////////////////////////////////////////// UTILITY ///

void *dql_peek_front(DequeList deque);

void *dql_peek_rear(DequeList deque);

bool dql_contains(DequeList deque, void *key);

Status dql_copy(DequeList deque, DequeList *result);

Status dql_append(DequeList deque1, DequeList deque2);

Status dql_prepend(DequeList deque1, DequeList deque2);

Status dql_to_array(DequeList deque,  void ***result, integer_t *length);

/////////////////////////////////////////////////////////////////// DISPLAY ///

Status dql_display(DequeList deque);

Status dql_display_array(DequeList deque);

Status dql_display_raw(DequeList deque);

#ifdef __cplusplus
}
#endif

#endif //C_DATASTRUCTURES_LIBRARY_DEQUELIST_H
