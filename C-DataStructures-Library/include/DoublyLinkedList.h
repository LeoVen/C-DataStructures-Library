/**
 * @file DoublyLinkedList.h
 *
 * @author Leonardo Vencovsky (https://github.com/LeoVen)
 *
 * @date 25/09/2018
 */

#ifndef C_DATASTRUCTURES_LIBRARY_DOUBLYLINKEDLIST_H
#define C_DATASTRUCTURES_LIBRARY_DOUBLYLINKEDLIST_H

#include "Core.h"

#ifdef __cplusplus
extern "C" {
#endif

// A doubly-linked list. See the source file for the full documentation.
struct DoublyLinkedList_s;

/// \brief A type for a doubly-linked list.
///
/// A type for a <code> struct DoublyLinkedList_s </code> so you don't have to
/// always write the full name of it.
typedef struct DoublyLinkedList_s DoublyLinkedList_t;

/// \brief A pointer type for a doubly-linked list.
///
/// A pointer type to <code> struct DoublyLinkedList_s </code>. This typedef is
/// used to avoid having to declare every doubly-linked lists as a pointer type
/// since they all must be dynamically allocated.
typedef struct DoublyLinkedList_s *DoublyLinkedList;

/// \brief Comparator function type.
///
/// A type for a function that compares two elements, returning:
/// - [ > 0] when the first element is greater than the second;
/// - [ < 0] when the first element is less than the second;
/// - 0 when both elements are equal.
typedef int(*dll_compare_f)(void *, void *);

/// \brief A Copy function type.
///
/// A type for a function that takes an input (first parameter) and returns an
/// exact copy of that element.
typedef void *(*dll_copy_f)(void *);

/// \brief Display function type.
///
/// A type for a function that displays an element in the console. Please do
/// not print a newline character.
typedef void(*dll_display_f)(void *);

/// \brief A Free function type.
///
/// A type for a function responsible for completely freeing an element from
/// memory.
typedef void(*dll_free_f)(void *);

///////////////////////////////////// STRUCTURE INITIALIZATION AND DELETION ///

Status dll_init(DoublyLinkedList *list);

Status dll_create(DoublyLinkedList *list, dll_compare_f compare_f,
        dll_copy_f copy_f, dll_display_f display_f, dll_free_f free_f);

Status dll_free(DoublyLinkedList *list);

Status dll_free_shallow(DoublyLinkedList *list);

Status dll_erase(DoublyLinkedList *list);

/////////////////////////////////////////////////////////////////// SETTERS ///

Status dll_set_v_compare(DoublyLinkedList list, dll_compare_f function);

Status dll_set_v_copy(DoublyLinkedList list, dll_copy_f function);

Status dll_set_v_display(DoublyLinkedList list, dll_display_f function);

Status dll_set_v_free(DoublyLinkedList list, dll_free_f function);

Status dll_set_limit(DoublyLinkedList list, integer_t limit);

Status dll_set(DoublyLinkedList list, void *element, integer_t position);

/////////////////////////////////////////////////////////////////// GETTERS ///

integer_t dll_length(DoublyLinkedList list);

integer_t dll_limit(DoublyLinkedList list);

Status dll_get(DoublyLinkedList list, void **result, integer_t position);

////////////////////////////////////////////////////////// INPUT AND OUTPUT ///

Status dll_insert_head(DoublyLinkedList list, void *element);

Status dll_insert_at(DoublyLinkedList list, void *element, integer_t position);

Status dll_insert_tail(DoublyLinkedList list, void *element);

Status dll_remove_head(DoublyLinkedList list, void **result);

Status dll_remove_at(DoublyLinkedList list, void **result, integer_t position);

Status dll_remove_tail(DoublyLinkedList list, void **result);

/////////////////////////////////////////////////////////// STRUCTURE STATE ///

bool dll_full(DoublyLinkedList list);

bool dll_empty(DoublyLinkedList list);

/////////////////////////////////////////////////////////////////// UTILITY ///

void *dll_max(DoublyLinkedList list);

void *dll_min(DoublyLinkedList list);

integer_t dll_index_first(DoublyLinkedList list, void *key);

integer_t dll_index_last(DoublyLinkedList list, void *key);

bool dll_contains(DoublyLinkedList list, void *key);

Status dll_reverse(DoublyLinkedList list);

Status dll_copy(DoublyLinkedList list, DoublyLinkedList *result);

Status dll_to_array(DoublyLinkedList list, void ***result, integer_t *length);

/////////////////////////////////////////////////////////////////// LINKING ///

Status dll_link(DoublyLinkedList list1, DoublyLinkedList list2);

Status dll_link_at(DoublyLinkedList list1, DoublyLinkedList list2,
                   integer_t position);

Status dll_unlink(DoublyLinkedList list, DoublyLinkedList result,
                  integer_t position);

Status dll_unlink_at(DoublyLinkedList list, DoublyLinkedList result,
                     integer_t position1, integer_t position2);

/////////////////////////////////////////////////////////////////// DISPLAY ///

Status dll_display(DoublyLinkedList list);

Status dll_display_array(DoublyLinkedList list);

Status dll_display_raw(DoublyLinkedList list);

///////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////// Iterator ///
///////////////////////////////////////////////////////////////////////////////

/// \todo DoublyLinkedListIterator

///////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////// Wrapper ///
///////////////////////////////////////////////////////////////////////////////

/// \todo DoublyLinkedListWrapper

#ifdef __cplusplus
}
#endif

#endif //C_DATASTRUCTURES_LIBRARY_DOUBLYLINKEDLIST_H
