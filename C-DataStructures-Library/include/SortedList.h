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

Status sli_init(SortedList *list, SORT_ORDER order);

Status sli_create(SortedList *list, SORT_ORDER order, sli_compare_f compare_f,
                sli_copy_f copy_f, sli_display_f display_f, sli_free_f free_f);

Status sli_free(SortedList *list);

Status sli_erase(SortedList *list);

/////////////////////////////////////////////////////////////////// SETTERS ///

Status sli_set_func_compare(SortedList list, sli_compare_f function);

Status sli_set_func_copy(SortedList list, sli_copy_f function);

Status sli_set_func_display(SortedList list, sli_display_f function);

Status sli_set_func_free(SortedList list, sli_free_f function);

Status sli_set_order(SortedList list, SORT_ORDER order);

Status sli_set_limit(SortedList list, index_t limit);

/////////////////////////////////////////////////////////////////// GETTERS ///

index_t sli_length(SortedList list);

index_t sli_limit(SortedList list);

SORT_ORDER sli_order(SortedList list);

////////////////////////////////////////////////////////// INPUT AND OUTPUT ///

Status sli_insert(SortedList list, void *element);

//Status sli_remove(SortedList list, void **result, index_t position);
//
//Status sli_remove_max(SortedList list, void **result);
//
//Status sli_remove_min(SortedList list, void **result);

/////////////////////////////////////////////////////////// STRUCTURE STATE ///

bool sli_full(SortedList list);

bool sli_empty(SortedList list);

/////////////////////////////////////////////////////////////////// UTILITY ///

void *sli_max(SortedList list);

void *sli_min(SortedList list);

index_t sli_index_first(SortedList list, void *key);

index_t sli_index_last(SortedList list, void *key);

//Status sli_reverse(SortedList list);

/////////////////////////////////////////////////////////////////// LINKING ///

//Status sli_merge(SortedList list1, SortedList list2);
//
//Status sli_unlink(SortedList list, SortedList *result, index_t position);
//
//Status sli_unlink_at(SortedList list, SortedList *result, index_t start, index_t end);

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

Status sli_iter_init(SortedListIterator *iter, SortedList target);

Status sli_iter_free(SortedListIterator *iter);

#endif //C_DATASTRUCTURES_LIBRARY_SORTEDLIST_H
