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

#ifdef __cplusplus
extern "C" {
#endif

// A sorted doubly-linked list. See the source file for the full documentation.
struct SortedList_s;

/// \brief A type for a sorted doubly-linked list.
///
/// A type for a <code> struct SortedList_s </code> so you don't have to always
/// write the full name of it.
typedef struct SortedList_s SortedList_t;

/// \brief A pointer type for a sorted doubly-linked list.
///
/// Useful for not having to declare every variable as pointer type. This
/// typedef does that for you.
typedef struct SortedList_s *SortedList;

/// \brief Comparator function type.
///
/// A type for a function that compares two elements, returning:
/// - [ > 0] when the first element is greater than the second;
/// - [ < 0] when the first element is less than the second;
/// - 0 when both elements are equal.
typedef int(*sli_compare_f)(void *, void *);

/// \brief A Copy function type.
///
/// A type for a function that takes an input (first parameter) and returns an
/// exact copy of that element.
typedef void *(*sli_copy_f)(void *);

/// \brief Display function type.
///
/// A type for a function that displays an element in the console. Please do
/// not print a newline character.
typedef void(*sli_display_f)(void *);

/// \brief A Free function type.
///
/// A type for a function responsible for completely freeing an element from
/// memory.
typedef void(*sli_free_f)(void *);

///////////////////////////////////// STRUCTURE INITIALIZATION AND DELETION ///

Status sli_init(SortedList *list);

Status sli_create(SortedList *list, SortOrder order, sli_compare_f compare_f,
                sli_copy_f copy_f, sli_display_f display_f, sli_free_f free_f);

Status sli_free(SortedList *list);

Status sli_free_shallow(SortedList *list);

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

Status sli_insert_all(SortedList list, void **elements, index_t count);

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

// A sorted list iterator. See the source file for the full documentation.
struct SortedListIterator_s;

/// \brief A type for a sorted list iterator.
///
/// A type for a <code> struct SortedListIterator_s </code>.
typedef struct SortedListIterator_s SortedListIterator_t;

/// \brief A pointer type for a sorted list iterator.
///
/// A pointer type for a <code> struct SortedListIterator_s </code>.
typedef struct SortedListIterator_s *SortedListIterator;

///////////////////////////////////// STRUCTURE INITIALIZATION AND DELETION ///

Status sli_iter_init(SortedListIterator *iter, SortedList target);

Status sli_iter_retarget(SortedListIterator *iter, SortedList target);

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

///////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////// Wrapper ///
///////////////////////////////////////////////////////////////////////////////

/// \brief A SortedList_s wrapper.
///
/// A SortedListWrapper_s can only be declared once. All of its operations are
/// wrapping a global and static variable declared in the source file. This
/// allows you to not have to always pass a reference to a sorted list in every
/// function. This emulates a way of writing C code in an object-oriented
/// manner.
struct SortedListWrapper_s
{
    /// \brief Wrapper for sli_set_func_compare().
    ///
    /// Wrapper for sli_set_func_compare().
    Status(*set_compare)(sli_compare_f);

    /// \brief Wrapper for sli_set_func_copy().
    ///
    /// Wrapper for sli_set_func_copy().
    Status(*set_copy)(sli_copy_f);

    /// \brief Wrapper for sli_set_func_display().
    ///
    /// Wrapper for sli_set_func_display().
    Status(*set_display)(sli_display_f);

    /// \brief Wrapper for sli_set_func_free()
    ///
    /// Wrapper for sli_set_func_free().
    Status(*set_free)(sli_free_f);

    /// \brief Wrapper for sli_set_limit().
    ///
    /// Wrapper for sli_set_limit().
    Status(*set_limit)(index_t);

    /// \brief Wrapper for sli_set_order().
    ///
    /// Wrapper for sli_set_order().
    Status(*set_order)(SortOrder);

    /// \brief Wrapper for sli_length().
    ///
    /// Wrapper for sli_length().
    index_t(*length)(void);

    /// \brief Wrapper for sli_limit().
    ///
    /// Wrapper for sli_limit().
    index_t(*limit)(void);

    /// \brief Wrapper for sli_order().
    ///
    /// Wrapper for sli_order().
    SortOrder(*order)(void);

    /// \brief Wrapper for sli_get().
    ///
    /// Wrapper for sli_get().
    Status(*get_copy)(void**,index_t);

    /// \brief Wrapper for sli_insert().
    ///
    /// Wrapper for sli_insert().
    Status(*insert)(void*);

    /// \brief Wrapper for sli_insert_all().
    ///
    /// Wrapper for sli_insert_all().
    Status(*insert_all)(void**,index_t);

    /// \brief Wrapper for sli_remove().
    ///
    /// Wrapper for sli_remove().
    Status(*remove)(void**,index_t);

    /// \brief Wrapper for sli_remove_max().
    ///
    /// Wrapper for sli_remove_max().
    Status(*remove_max)(void**);

    /// \brief Wrapper for sli_remove_min().
    ///
    /// Wrapper for sli_remove_min().
    Status(*remove_min)(void**);

    /// \brief Wrapper for sli_full().
    ///
    /// Wrapper for sli_full().
    bool(*full)(void);

    /// \brief Wrapper for sli_empty().
    ///
    /// Wrapper for sli_empty().
    bool(*empty)(void);

    /// \brief Wrapper for sli_max().
    ///
    /// Wrapper for sli_max().
    void*(*max)(void);

    /// \brief Wrapper for sli_min().
    ///
    /// Wrapper for sli_remove_min().
    void*(*min)(void);

    /// \brief Wrapper for sli_index_first().
    ///
    /// Wrapper for sli_index_first().
    index_t(*index_first)(void*);

    /// \brief Wrapper for sli_index_last().
    ///
    /// Wrapper for sli_index_last().
    index_t(*index_last)(void*);

    /// \brief Wrapper for sli_contains().
    ///
    /// Wrapper for sli_contains().
    bool(*contains)(void*);

    /// \brief Wrapper for sli_reverse().
    ///
    /// Wrapper for sli_reverse().
    Status(*reverse)(void);

    /// \brief Wrapper for sli_copy().
    ///
    /// Wrapper for sli_copy().
    Status(*copy)(SortedList*);

    /// \brief Wrapper for sli_to_array().
    ///
    /// Wrapper for sli_to_array().
    Status(*to_array)(void***,index_t*);

    /// \brief Wrapper for sli_merge().
    ///
    /// Wrapper for sli_merge().
    Status(*merge)(SortedList);

    /// \brief Wrapper for sli_unlink().
    ///
    /// Wrapper for sli_unlink().
    Status(*unlink)(SortedList*,index_t);

    /// \brief Wrapper for sli_sublist().
    ///
    /// Wrapper for sli_sublist().
    Status(*sublist)(SortedList*,index_t,index_t);

    /// \brief Wrapper for sli_display().
    ///
    /// Wrapper for sli_display().
    Status(*display)(void);

    /// \brief Wrapper for sli_display_array().
    ///
    /// Wrapper for sli_display_array().
    Status(*display_array)(void);

    /// \brief Wrapper for sli_display_raw().
    ///
    /// Wrapper for sli_display_raw().
    Status(*display_raw)(void);
};

/// \brief A type for a sorted list wrapper.
///
/// A type for a <code> struct SortedListWrapper_s </code>.
typedef struct SortedListWrapper_s SortedListWrapper_t;

/// \brief A pointer type for a sorted list wrapper.
///
/// A pointer type for a <code> struct SortedListWrapper_s </code>.
typedef struct SortedListWrapper_s *SortedListWrapper;

Status sli_wrap_init(SortedListWrapper *wrapper, SortedList target);

Status sli_wrap_retarget(SortedListWrapper wrapper, SortedList target);

Status sli_wrap_free(SortedListWrapper *wrapper);

#ifdef __cplusplus
}
#endif

#endif //C_DATASTRUCTURES_LIBRARY_SORTEDLIST_H
