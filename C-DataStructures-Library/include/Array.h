/**
 * @file Array.h
 * 
 * @author Leonardo Vencovsky (https://github.com/LeoVen)
 *
 * @date 07/10/2018
 */

#ifndef C_DATASTRUCTURES_LIBRARY_ARRAY_H
#define C_DATASTRUCTURES_LIBRARY_ARRAY_H

#include "Core.h"

#ifdef __cplusplus
extern "C" {
#endif

// A C array wrapper. See the source file for the documentation.
struct Array_s;

/// \brief A type for a an Array_s.
///
/// A type for a <code> struct Array_s </code> so you don't have to always
/// write the full name of it.
typedef struct Array_s Array_t;

/// \brief A pointer type for an Array_s.
///
/// Useful for not having to declare every variable as pointer type. This
/// typedef does that for you.
typedef struct Array_s *Array;

/// \brief Display function type.
///
/// A type for a function that displays an element in the console.
typedef void(*arr_display_f)(void *);

/// \brief Comparator function type.
///
/// A type for a function that compares two elements, returning:
/// - [ > 0] when the first element is greater than the second;
/// - [ < 0] when the first element is less than the second;
/// - 0 when both elements are equal.
///
/// Used when sorting the array.
typedef int(*arr_compare_f)(void *, void *);

/// \brief A Copy function type.
///
/// A type for a function that takes an input (first parameter) and returns an
/// exact copy of that element.
typedef void *(*arr_copy_f)(void *);

/// \brief A Free function type.
///
/// A type for a function responsible for completely freeing an element from
/// memory.
typedef void(*arr_free_f)(void *);

///////////////////////////////////// STRUCTURE INITIALIZATION AND DELETION ///

Status arr_init(Array *array, index_t length);

Status arr_create(Array *array, index_t length, arr_compare_f compare_f,
        arr_copy_f copy_f, arr_display_f display_f, arr_free_f free_f);

Status arr_free(Array *array);

Status arr_free_shallow(Array *array);

Status arr_erase(Array *array);

/////////////////////////////////////////////////////////////////// SETTERS ///

Status arr_set_func_compare(Array array, arr_compare_f function);

Status arr_set_func_copy(Array array, arr_copy_f function);

Status arr_set_func_display(Array array, arr_display_f function);

Status arr_set_func_free(Array array, arr_free_f function);

/////////////////////////////////////////////////////////////////// GETTERS ///

index_t arr_length(Array array);

index_t arr_count(Array array);

////////////////////////////////////////////////////////// INPUT AND OUTPUT ///

index_t arr_set_next(Array array, void *element);

Status arr_set(Array array, index_t index, void *element);

index_t arr_set_last(Array array, void *element);

void *arr_get_next(Array array, index_t *index);

void *arr_get(Array array, index_t index);

void *arr_get_last(Array array, index_t *index);

void *arr_pop_next(Array array, index_t *index);

void *arr_pop(Array array, index_t index);

void *arr_pop_last(Array array, index_t *index);

/////////////////////////////////////////////////////////// STRUCTURE STATE ///

bool arr_full(Array array);

bool arr_empty(Array array);

/////////////////////////////////////////////////////////////////// UTILITY ///

Status arr_sort(Array array);

Status arr_copy(Array array, Array *result);

Status arr_switch(Array array, index_t pos1, index_t pos2);

Status arr_reverse(Array array);

/////////////////////////////////////////////////////////////////// DISPLAY ///

Status arr_display(Array array);

Status arr_display_raw(Array array);

///////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////// Iterator ///
///////////////////////////////////////////////////////////////////////////////

// An array iterator. See the source file for the full documentation.
struct ArrayIterator_s;

/// \brief A type for an array iterator.
///
/// A type for a <code> struct ArrayIterator_s </code>.
typedef struct ArrayIterator_s ArrayIterator_t;

/// \brief A pointer type for an array iterator.
///
/// A pointer type for a <code> struct ArrayIterator_s </code>.
typedef struct ArrayIterator_s *ArrayIterator;

///////////////////////////////////// STRUCTURE INITIALIZATION AND DELETION ///

Status arr_iter_init(ArrayIterator *iter, Array target);

Status arr_iter_retarget(ArrayIterator *iter, Array target);

Status arr_iter_free(ArrayIterator *iter);

///////////////////////////////////////////////////////////////// ITERATION ///

Status arr_iter_next(ArrayIterator iter);

Status arr_iter_prev(ArrayIterator iter);

Status arr_iter_to_start(ArrayIterator iter);

Status arr_iter_to_end(ArrayIterator iter);

/////////////////////////////////////////////////////////// STRUCTURE STATE ///

bool arr_iter_has_next(ArrayIterator iter);

bool arr_iter_has_prev(ArrayIterator iter);

////////////////////////////////////////////////////////// INPUT AND OUTPUT ///

Status arr_iter_set(ArrayIterator iter, void *element);

Status arr_iter_get(ArrayIterator iter, void **result);

Status arr_iter_pop(ArrayIterator iter, void **result);

/////////////////////////////////////////////////////////////////// UTILITY ///

void *arr_iter_peek_next(ArrayIterator iter);

void *arr_iter_peek(ArrayIterator iter);

void *arr_iter_peek_prev(ArrayIterator iter);

#ifdef __cplusplus
}
#endif

#endif //C_DATASTRUCTURES_LIBRARY_ARRAY_H
