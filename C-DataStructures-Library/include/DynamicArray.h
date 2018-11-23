/**
 * @file DynamicArray.h
 * 
 * @author Leonardo Vencovsky (https://github.com/LeoVen)
 *
 * @date 01/10/2018
 */

#ifndef C_DATASTRUCTURES_LIBRARY_DYNAMICARRAY_H
#define C_DATASTRUCTURES_LIBRARY_DYNAMICARRAY_H

#include "Core.h"

#ifdef __cplusplus
extern "C" {
#endif

// A dynamic C array wrapper. See the source file for the documentation.
struct DynamicArray_s;

/// \brief A type for a an DynamicArray_s.
///
/// A type for a <code> struct DynamicArray_s </code> so you don't have to
/// always write the full name of it.
typedef struct DynamicArray_s DynamicArray_t;

/// \brief A pointer type for an DynamicArray_s.
///
/// Useful for not having to declare every variable as pointer type. This
/// typedef does that for you.
typedef struct DynamicArray_s *DynamicArray;

/// \brief Display function type.
///
/// A type for a function that displays an element in the console.
typedef void(*dar_display_f)(void *);

/// \brief Comparator function type.
///
/// A type for a function that compares two elements, returning:
/// - [ > 0] when the first element is greater than the second;
/// - [ < 0] when the first element is less than the second;
/// - 0 when both elements are equal.
///
/// Used when sorting the array.
typedef int(*dar_compare_f)(void *, void *);

/// \brief A Copy function type.
///
/// A type for a function that takes an input (first parameter) and returns an
/// exact copy of that element.
typedef void *(*dar_copy_f)(void *);

/// \brief A Free function type.
///
/// A type for a function responsible for completely freeing an element from
/// memory.
typedef void(*dar_free_f)(void *);

///////////////////////////////////// STRUCTURE INITIALIZATION AND DELETION ///

Status dar_init(DynamicArray *d_array);

Status dar_create(DynamicArray *d_array, index_t initial_capacity,
        index_t growth_rate, dar_compare_f compare_f, dar_copy_f copy_f,
        dar_display_f display_f, dar_free_f free_f);

Status dar_free(DynamicArray *d_array);

Status dar_free_shallow(DynamicArray *d_array);

Status dar_erase(DynamicArray *d_array);

/////////////////////////////////////////////////////////////////// SETTERS ///

Status dar_set_func_compare(DynamicArray d_array, dar_compare_f function);

Status dar_set_func_copy(DynamicArray d_array, dar_copy_f function);

Status dar_set_func_display(DynamicArray d_array, dar_display_f function);

Status dar_set_func_free(DynamicArray d_array, dar_free_f function);

Status dar_capacity_lock(DynamicArray d_array);

Status dar_capacity_unlock(DynamicArray d_array);

/////////////////////////////////////////////////////////////////// GETTERS ///

index_t dar_capacity(DynamicArray d_array);

index_t dar_size(DynamicArray d_array);

index_t dar_growth_rate(DynamicArray d_array);

bool dar_is_locked(DynamicArray d_array);

void *dar_get(DynamicArray d_array, index_t index);

////////////////////////////////////////////////////////// INPUT AND OUTPUT ///

Status dar_insert(DynamicArray d_array, void **array, index_t arr_size,
        index_t index);

Status dar_insert_front(DynamicArray d_array, void *element);

Status dar_insert_at(DynamicArray d_array, void *element, index_t index);

Status dar_insert_back(DynamicArray d_array, void *element);

Status dar_remove(DynamicArray d_array, index_t from, index_t to,
        void ***result, index_t *size);

Status dar_remove_front(DynamicArray d_array, void **result);

Status dar_remove_at(DynamicArray d_array, void **result, index_t index);

Status dar_remove_back(DynamicArray d_array, void **result);

Status dar_delete(DynamicArray d_array, index_t from, index_t to);

Status dar_prepend(DynamicArray d_array1, DynamicArray d_array2);

Status dar_add(DynamicArray d_array1, DynamicArray d_array2, index_t index);

Status dar_append(DynamicArray d_array1, DynamicArray d_array2);

Status dar_replace(DynamicArray d_array, void *element, index_t index);

/////////////////////////////////////////////////////////// STRUCTURE STATE ///

bool dar_empty(DynamicArray d_array);

bool dar_full(DynamicArray d_array);

bool dar_fits(DynamicArray d_array, index_t size);

/////////////////////////////////////////////////////////////////// UTILITY ///

void *dar_max(DynamicArray d_array);

void *dar_min(DynamicArray d_array);

index_t dar_index_first(DynamicArray d_array, void *key);

index_t dar_index_last(DynamicArray d_array, void *key);

bool dar_contains(DynamicArray d_array, void *element);

Status dar_switch(DynamicArray d_array, index_t pos1, index_t pos2);

Status dar_reverse(DynamicArray d_array);

Status dar_copy(DynamicArray d_array, DynamicArray *result);

Status dar_to_array(DynamicArray d_array, void ***result, index_t *length);

Status dar_sort(DynamicArray d_array);

/////////////////////////////////////////////////////////////////// DISPLAY ///

Status dar_display(DynamicArray d_array);

Status dar_display_array(DynamicArray d_array);

Status dar_display_raw(DynamicArray d_array);

#ifdef __cplusplus
}
#endif

#endif //C_DATASTRUCTURES_LIBRARY_DYNAMICARRAY_H
