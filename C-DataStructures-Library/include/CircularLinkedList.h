/**
 * @file CircularLinkedList.h
 * 
 * @author Leonardo Vencovsky (https://github.com/LeoVen)
 *
 * @date 09/10/2018
 */

#ifndef C_DATASTRUCTURES_LIBRARY_CIRCULARLINKEDLIST_H
#define C_DATASTRUCTURES_LIBRARY_CIRCULARLINKEDLIST_H

#include "Core.h"

#ifdef __cplusplus
extern "C" {
#endif

// A circular doubly-linked list. See the source file for the full documentation.
struct CircularLinkedList_s;

/// \brief A type for a circular doubly-linked list.
///
/// A type for a <code> struct CircularLinkedList_s </code> so you don't have
/// to always write the full name of it.
typedef struct CircularLinkedList_s CircularLinkedList_t;

/// \brief A pointer type for a circular doubly-linked list.
///
/// A pointer type to <code> struct CircularLinkedList_s </code>. This typedef
/// is used to avoid having to declare every circular lists as a pointer type
/// since they all must be dynamically allocated.
typedef struct CircularLinkedList_s *CircularLinkedList;

/// \brief Comparator function type.
///
/// A type for a function that compares two elements, returning:
/// - [ > 0] when the first element is greater than the second;
/// - [ < 0] when the first element is less than the second;
/// - 0 when both elements are equal.
typedef int(*cll_compare_f)(void *, void *);

/// \brief A Copy function type.
///
/// A type for a function that takes an input (first parameter) and returns an
/// exact copy of that element.
typedef void *(*cll_copy_f)(void *);

/// \brief Display function type.
///
/// A type for a function that displays an element in the console. Please do
/// not prvoid *a newline character.
typedef void(*cll_display_f)(void *);

/// \brief A Free function type.
///
/// A type for a function responsible for completely freeing an element from
/// memory.
typedef void(*cll_free_f)(void *);

///////////////////////////////////// STRUCTURE INITIALIZATION AND DELETION ///

Status cll_init(CircularLinkedList *cll);

Status cll_create(CircularLinkedList *list, cll_compare_f compare_f,
                cll_copy_f copy_f, cll_display_f display_f, cll_free_f free_f);

Status cll_free(CircularLinkedList *cll);

Status cll_free_shallow(CircularLinkedList *list);

Status cll_erase(CircularLinkedList *cll);

/////////////////////////////////////////////////////////////////// SETTERS ///

Status cll_set_v_compare(CircularLinkedList list, cll_compare_f function);

Status cll_set_v_copy(CircularLinkedList list, cll_copy_f function);

Status cll_set_v_display(CircularLinkedList list, cll_display_f function);

Status cll_set_v_free(CircularLinkedList list, cll_free_f function);

Status cll_set_limit(CircularLinkedList list, integer_t limit);

/////////////////////////////////////////////////////////////////// GETTERS ///

integer_t cll_length(CircularLinkedList list);

integer_t cll_limit(CircularLinkedList list);

////////////////////////////////////////////////////////// INPUT AND OUTPUT ///

Status cll_insert_after(CircularLinkedList cll, void *element);

Status cll_insert_before(CircularLinkedList cll, void *element);

Status cll_remove_after(CircularLinkedList cll, void **result);

Status cll_remove_current(CircularLinkedList cll, void **result);

Status cll_remove_before(CircularLinkedList cll, void **result);

/////////////////////////////////////////////////////////// STRUCTURE STATE ///

bool cll_full(CircularLinkedList cll);

bool cll_empty(CircularLinkedList cll);

/////////////////////////////////////////////////////////////////// UTILITY ///

void *cll_max(CircularLinkedList cll);

void *cll_min(CircularLinkedList cll);

bool cll_contains(CircularLinkedList cll, void *key);

void *cll_peek_next(CircularLinkedList cll);

void *cll_peek(CircularLinkedList cll);

void *cll_peek_prev(CircularLinkedList cll);

Status cll_copy(CircularLinkedList cll, CircularLinkedList *result);

///////////////////////////////////////////////////////////////// ITERATION ///

Status cll_iter_next(CircularLinkedList cll, integer_t positions);

Status cll_iter(CircularLinkedList cll, integer_t positions);

Status cll_iter_prev(CircularLinkedList cll, integer_t positions);

/////////////////////////////////////////////////////////////////// DISPLAY ///

Status cll_display(CircularLinkedList cll);

Status cll_display_array(CircularLinkedList cll);

Status cll_display_raw(CircularLinkedList cll);

#ifdef __cplusplus
}
#endif

#endif //C_DATASTRUCTURES_LIBRARY_CIRCULARLINKEDLIST_H
