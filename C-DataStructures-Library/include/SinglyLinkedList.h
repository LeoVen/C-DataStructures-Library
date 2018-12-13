/**
 * @file SinglyLinkedList.h
 *
 * @author Leonardo Vencovsky (https://github.com/LeoVen)
 *
 * @date 25/09/2018
 */

#ifndef C_DATASTRUCTURES_LIBRARY_SINGLYLINKEDLIST_H
#define C_DATASTRUCTURES_LIBRARY_SINGLYLINKEDLIST_H

#include "Core.h"

#ifdef __cplusplus
extern "C" {
#endif

// A singly-linked list. See the source file for the full documentation.
struct SinglyLinkedList_s;

/// \brief A type for a singly-linked list.
///
/// A type for a <code> struct SinglyLinkedList_s </code> so you don't have to
/// always write the full name of it.
typedef struct SinglyLinkedList_s SinglyLinkedList_t;

/// \brief A pointer type for a singly-linked list.
///
/// A pointer type to <code> struct SinglyLinkedList_s </code>. This typedef is
/// used to avoid having to declare every singly-linked lists as a pointer type
/// since they all must be dynamically allocated.
typedef struct SinglyLinkedList_s *SinglyLinkedList;

/// \brief Comparator function type.
///
/// A type for a function that compares two elements, returning:
/// - [ > 0] when the first element is greater than the second;
/// - [ < 0] when the first element is less than the second;
/// - 0 when both elements are equal.
typedef int(*sll_compare_f)(void *, void *);

/// \brief A Copy function type.
///
/// A type for a function that takes an input (first parameter) and returns an
/// exact copy of that element.
typedef void *(*sll_copy_f)(void *);

/// \brief Display function type.
///
/// A type for a function that displays an element in the console. Please do
/// not print a newline character.
typedef void(*sll_display_f)(void *);

/// \brief A Free function type.
///
/// A type for a function responsible for completely freeing an element from
/// memory.
typedef void(*sll_free_f)(void *);

///////////////////////////////////// STRUCTURE INITIALIZATION AND DELETION ///

Status sll_init(SinglyLinkedList *list);

Status sll_create(SinglyLinkedList *list, sll_compare_f compare_f,
        sll_copy_f copy_f, sll_display_f display_f, sll_free_f free_f);

Status sll_free(SinglyLinkedList *list);

Status sll_free_shallow(SinglyLinkedList *list);

Status sll_erase(SinglyLinkedList *list);

/////////////////////////////////////////////////////////////////// SETTERS ///

Status sll_set_v_compare(SinglyLinkedList list, sll_compare_f function);

Status sll_set_v_copy(SinglyLinkedList list, sll_copy_f function);

Status sll_set_v_display(SinglyLinkedList list, sll_display_f function);

Status sll_set_v_free(SinglyLinkedList list, sll_free_f function);

Status sll_set_limit(SinglyLinkedList list, integer_t limit);

Status sll_set(SinglyLinkedList list, void *element, integer_t position);

/////////////////////////////////////////////////////////////////// GETTERS ///

integer_t sll_length(SinglyLinkedList list);

integer_t sll_limit(SinglyLinkedList list);

Status sll_get(SinglyLinkedList list, void **result, integer_t position);

////////////////////////////////////////////////////////// INPUT AND OUTPUT ///

Status sll_insert_head(SinglyLinkedList list, void *element);

Status sll_insert_at(SinglyLinkedList list, void *element, integer_t position);

Status sll_insert_tail(SinglyLinkedList list, void *element);

Status sll_remove_head(SinglyLinkedList list, void **result);

Status sll_remove_at(SinglyLinkedList list, void **result, integer_t position);

Status sll_remove_tail(SinglyLinkedList list, void **result);

/////////////////////////////////////////////////////////// STRUCTURE STATE ///

bool sll_full(SinglyLinkedList list);

bool sll_empty(SinglyLinkedList list);

/////////////////////////////////////////////////////////////////// UTILITY ///

void *sll_max(SinglyLinkedList list);

void *sll_min(SinglyLinkedList list);

integer_t sll_index_first(SinglyLinkedList list, void *key);

integer_t sll_index_last(SinglyLinkedList list, void *key);

bool sll_contains(SinglyLinkedList list, void *key);

Status sll_reverse(SinglyLinkedList list);

Status sll_copy(SinglyLinkedList list, SinglyLinkedList *result);

Status sll_to_array(SinglyLinkedList list, void ***result, integer_t *length);

/////////////////////////////////////////////////////////////////// LINKING ///

Status sll_link(SinglyLinkedList list1, SinglyLinkedList list2);

Status sll_link_at(SinglyLinkedList list1, SinglyLinkedList list2,
        integer_t position);

Status sll_unlink(SinglyLinkedList list, SinglyLinkedList result,
        integer_t position);

Status sll_unlink_at(SinglyLinkedList list, SinglyLinkedList result,
                     integer_t position1, integer_t position2);

/////////////////////////////////////////////////////////////////// DISPLAY ///

Status sll_display(SinglyLinkedList list);

Status sll_display_array(SinglyLinkedList list);

Status sll_display_raw(SinglyLinkedList list);

///////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////// Iterator ///
///////////////////////////////////////////////////////////////////////////////

/// \todo SinglyLinkedListIterator

///////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////// Wrapper ///
///////////////////////////////////////////////////////////////////////////////

/// \todo SinglyLinkedListWrapper

#ifdef __cplusplus
}
#endif

#endif //C_DATASTRUCTURES_LIBRARY_SINGLYLINKEDLIST_H
