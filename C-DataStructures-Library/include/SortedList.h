/**
 * @file SortedList.h
 * 
 * @author Leonardo Vencovsky (https://github.com/LeoVen)
 *
 * @date 08/11/2018
 */

#ifndef C_DATASTRUCTURES_LIBRARY_SORTEDLIST_H
#define C_DATASTRUCTURES_LIBRARY_SORTEDLIST_H

#include "Core.h"
#include "CoreSort.h"

/// A sorted doubly-linked list.
struct SortedList_s;

/// A type for a sorted doubly-linked list.
typedef struct SortedList_s SortedList_t;

/// A pointer type for a sorted doubly-linked list.
typedef struct SortedList_s *SortedList;

/// A type for a function that displays an element in the console.
typedef void(*sli_display_f)(void *);

/// A type for a function that compares two elements, returning:
/// - [ > 0] when the first element is greater than the second;
/// - [ < 0] when the first element is less than the second;
/// - 0 when both elements are equal.
typedef int(*sli_compare_f)(void *, void *);

/// A type for a function that takes an input (first parameter) and returns an
/// exact copy of that element.
typedef void *(*sli_copy_f)(void *);

/// A type for a function responsible for completely freeing an element from
/// memory.
typedef void(*sli_free_f)(void *);

///////////////////////////////////// STRUCTURE INITIALIZATION AND DELETION ///

Status sli_init(SortedList *list, SortOrder order);

Status sli_create(SortedList *list, SortOrder order, sli_compare_f compare_f,
                sli_copy_f copy_f, sli_display_f display_f, sli_free_f free_f);

Status sli_free(SortedList *list);

Status sli_erase(SortedList *list);

/////////////////////////////////////////////////////////////////// SETTERS ///

Status sli_set_func_compare(SortedList list, sli_compare_f function);

Status sli_set_func_copy(SortedList list, sli_copy_f function);

Status sli_set_func_display(SortedList list, sli_display_f function);

Status sli_set_func_free(SortedList list, sli_free_f function);

Status sli_set_limit(SortedList list, index_t limit);

Status sli_set_order(SortedList list, SortOrder order);

// No setter because the user might break the sorted property of the list.

/////////////////////////////////////////////////////////////////// GETTERS ///

index_t sli_length(SortedList list);

index_t sli_limit(SortedList list);

SortOrder sli_order(SortedList list);

Status sli_get(SortedList list, void **result, index_t index);

////////////////////////////////////////////////////////// INPUT AND OUTPUT ///

Status sli_insert(SortedList list, void *element);

Status sli_remove(SortedList list, void **result, index_t position);

Status sli_remove_max(SortedList list, void **result);

Status sli_remove_min(SortedList list, void **result);

/////////////////////////////////////////////////////////// STRUCTURE STATE ///

bool sli_full(SortedList list);

bool sli_empty(SortedList list);

/////////////////////////////////////////////////////////////////// UTILITY ///

void *sli_max(SortedList list);

void *sli_min(SortedList list);

index_t sli_index_first(SortedList list, void *key);

index_t sli_index_last(SortedList list, void *key);

bool sli_contains(SortedList list, void *key);

Status sli_reverse(SortedList list);

Status sli_copy(SortedList list, SortedList *result);

Status sli_to_array(SortedList list, void ***result, index_t *length);

/////////////////////////////////////////////////////////////////// LINKING ///

Status sli_merge(SortedList list1, SortedList list2);

Status sli_unlink(SortedList list, SortedList *result, index_t position);

Status sli_sublist(SortedList list, SortedList *result, index_t start,
        index_t end);

/////////////////////////////////////////////////////////////////// DISPLAY ///

Status sli_display(SortedList list);

Status sli_display_array(SortedList list);

Status sli_display_raw(SortedList list);

///////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////// Iterator ///
///////////////////////////////////////////////////////////////////////////////

/// A sorted list iterator.
struct SortedListIterator_s;

/// A type for a sorted list iterator.
typedef struct SortedListIterator_s SortedListIterator_t;

/// A pointer type for a sorted list iterator.
typedef struct SortedListIterator_s *SortedListIterator;

///////////////////////////////////// STRUCTURE INITIALIZATION AND DELETION ///

Status sli_iter_init(SortedListIterator *iter, SortedList target);

Status sli_iter_free(SortedListIterator *iter);

///////////////////////////////////////////////////////////////// ITERATION ///

Status sli_iter_next(SortedListIterator iter);

Status sli_iter_prev(SortedListIterator iter);

Status sli_iter_to_head(SortedListIterator iter);

Status sli_iter_to_tail(SortedListIterator iter);

/////////////////////////////////////////////////////////// STRUCTURE STATE ///

bool sli_iter_has_next(SortedListIterator iter);

bool sli_iter_has_prev(SortedListIterator iter);

////////////////////////////////////////////////////////// SETTER AND GETTER ///

/// Gets a copy of the element pointed by the cursor.
Status sli_iter_get(SortedListIterator iter, void **result);

// No setter because the user might break the sorted property of the list.

////////////////////////////////////////////////////////// INPUT AND OUTPUT ///

// No insert functions because the user might break the sorted property.

Status sli_iter_remove_next(SortedListIterator iter, void **result);

Status sli_iter_remove_curr(SortedListIterator iter, void **result);

Status sli_iter_remove_prev(SortedListIterator iter, void **result);

/////////////////////////////////////////////////////////////////// UTILITY ///

void *sli_iter_peek_next(SortedListIterator iter);

void *sli_iter_peek(SortedListIterator iter);

void *sli_iter_peek_prev(SortedListIterator iter);

#endif //C_DATASTRUCTURES_LIBRARY_SORTEDLIST_H
