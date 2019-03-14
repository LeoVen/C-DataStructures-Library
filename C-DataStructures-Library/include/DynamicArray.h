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
#include "Interface.h"

#ifdef __cplusplus
extern "C" {
#endif

/// \struct DynamicArray_s
/// \brief A C generic array wrapper with dynamic size.
struct DynamicArray_s;

/// \brief A type for a an DynamicArray_s.
///
/// A type for a <code> struct DynamicArray_s </code> so you don't have to
/// always write the full name of it.
typedef struct DynamicArray_s DynamicArray_t;

/// \brief A pointer type for an DynamicArray_s.
///
/// A pointer type to <code> struct DynamicArray_s </code>. This typedef is
/// used to avoid having to declare every dynamic array as a pointer type since
/// they all must be dynamically allocated.
typedef struct DynamicArray_s *DynamicArray;

///////////////////////////////////// STRUCTURE INITIALIZATION AND DELETION ///

/// \ref dar_new
/// \brief Initializes a dynamic array structure with default size.
DynamicArray_t *
dar_new(Interface_t *interface);

/// \ref dar_create
/// \brief Creates a dynamic array with a custom initial capacity.
DynamicArray_t *
dar_create(Interface_t *interface, integer_t initial_capacity,
           integer_t growth_rate);

/// \ref dar_free
/// \brief Frees from memory a dynamic array and its elements.
void
dar_free(DynamicArray_t *array);

/// \ref dar_free_shallow
/// \brief Frees from memory a dynamic array leaving its elements intact.
void
dar_free_shallow(DynamicArray_t *array);

/// \ref dar_erase
/// \brief Resets the DynamicArray_t freeing all its elements.
void
dar_erase(DynamicArray_t *array);

/// \ref dar_erase_shallow
/// \brief Resets the DynamicArray_t without freeing its elements.
void
dar_erase_shallow(DynamicArray_t *array);

//////////////////////////////////////////////////////////// CONFIGURATIONS ///

/// \ref dar_config
/// \brief Sets a new interface for a target dynamic array.
void
dar_config(DynamicArray_t *array, Interface_t *new_interface);

/////////////////////////////////////////////////////////////////// SETTERS ///

/// \ref dar_capacity_lock
/// \brief Locks the growth of the dynamic array.
void
dar_capacity_lock(DynamicArray_t *array);

/// \ref dar_capacity_unlock
/// \brief Unlocks the growth of the dynamic array.
void
dar_capacity_unlock(DynamicArray_t *array);

/////////////////////////////////////////////////////////////////// GETTERS ///

/// \ref dar_capacity
/// \brief Returns the current buffer's capacity.
integer_t
dar_capacity(DynamicArray_t *array);

/// \ref dar_size
/// \brief Returns the current amount of elements in the dynamic array.
integer_t
dar_size(DynamicArray_t *array);

/// \ref dar_growth_rate
/// \brief Returns the current set growth_rate.
integer_t
dar_growth_rate(DynamicArray_t *array);

/// \ref dar_is_locked
/// \brief Returns true if the dynamic array's growth is locked.
bool
dar_is_locked(DynamicArray_t *array);

/// \ref dar_get
/// \brief Returns an element at the specified position of the array.
void *
dar_get(DynamicArray_t *array, integer_t index);

////////////////////////////////////////////////////////// INPUT AND OUTPUT ///

/// \ref dar_insert
/// \brief Inserts an array of elements at a given index.
bool
dar_insert(DynamicArray_t *array, void **elements, integer_t array_size,
           integer_t index);

/// \ref dar_insert_front
/// \brief Inserts an element at the start of the array.
bool
dar_insert_front(DynamicArray_t *array, void *element);

/// \ref dar_insert_at
/// \brief Inserts an element at the specified index.
bool
dar_insert_at(DynamicArray_t *array, void *element, integer_t index);

/// \ref dar_insert_back
/// \brief Inserts and element at the end of the array.
bool
dar_insert_back(DynamicArray_t *array, void *element);

/// \ref dar_remove
/// \brief Extracts a sub-array from a given range.
bool
dar_remove(DynamicArray_t *array, integer_t from, integer_t to, void ***result,
           integer_t *size);

/// \ref dar_remove_front
/// \brief Removes the first element from the array.
bool
dar_remove_front(DynamicArray_t *array, void **result);

/// \ref dar_remove_at
/// \brief Removes an element located at a given index.
bool
dar_remove_at(DynamicArray_t *array, void **result, integer_t index);

/// \ref dar_remove_back
/// \brief Removes the last element from the array.
bool
dar_remove_back(DynamicArray_t *array, void **result);

/// \ref dar_delete
/// \brief Deletes a given range using the interface's free function.
bool
dar_delete(DynamicArray_t *array, integer_t from, integer_t to);

/// \ref dar_prepend
/// \brief Prepends the second array into the first, emptying it.
bool
dar_prepend(DynamicArray_t *array1, DynamicArray_t *array2);

/// \ref dar_add
/// \brief Adds the second array into the first at a given index, emptying it.
bool
dar_add(DynamicArray_t *array1, DynamicArray_t *array2, integer_t index);

/// \ref dar_append
/// \brief Appends the second array into the first, emptying it.
bool
dar_append(DynamicArray_t *array1, DynamicArray_t *array2);

/// \ref dar_replace
/// \brief Frees the element and replaces it by a new one at a given index.
bool
dar_replace(DynamicArray_t *array, void *element, integer_t index);

/////////////////////////////////////////////////////////// STRUCTURE STATE ///

/// \ref dar_empty
/// \brief Returns true if the array is empty, otherwise false.
bool
dar_empty(DynamicArray_t *array);

/// \ref dar_full
/// \brief Returns true if the array is full, otherwise false.
bool
dar_full(DynamicArray_t *array);

/// \ref dar_fits
/// \brief Returns true if the given size fits in the array without resizing.
bool
dar_fits(DynamicArray_t *array, integer_t size);

/////////////////////////////////////////////////////////////////// UTILITY ///

/// \ref dar_max
/// \brief Returns the maximum element in the array.
void *
dar_max(DynamicArray_t *array);

/// \ref dar_min
/// \brief Returns the minimum element in the array.
void *
dar_min(DynamicArray_t *array);

/// \ref dar_index_first
/// \brief Returns the first index of an element that matches a given key.
integer_t
dar_index_first(DynamicArray_t *array, void *key);

/// \ref dar_index_last
/// \brief Returns the last index of an element that matches a given key.
integer_t
dar_index_last(DynamicArray_t *array, void *key);

/// \ref dar_contains
/// \brief Returns true if a given element is present in the array.
bool
dar_contains(DynamicArray_t *array, void *element);

/// \ref dar_switch
/// \brief Switches two elements within the array.
bool
dar_switch(DynamicArray_t *array, integer_t pos1, integer_t pos2);

/// \ref dar_reverse
/// \brief Reverses the order of the array's elements.
bool
dar_reverse(DynamicArray_t *array);

/// \ref dar_copy
/// \brief Returns a copy of the specified dynamic array.
DynamicArray_t *
dar_copy(DynamicArray_t *array);

/// \ref dar_copy_shallow
/// \brief Returns a shallow copy of the specified dynamic array.
DynamicArray_t *
dar_copy_shallow(DynamicArray_t *array);

/// \ref dar_to_array
/// \brief Makes a C array from an existing DynamicArray_s.
void **
dar_to_array(DynamicArray_t *array, integer_t *length);

/// \ref dar_from_array
/// \brief Makes a new DynamicArray_s from an existing C array.
DynamicArray_t *
dar_from_array(Interface_t *interface, void **buffer, integer_t length,
               integer_t growth_rate);

void
dar_sort(DynamicArray_t *array);

/////////////////////////////////////////////////////////////////// DISPLAY ///

/// \ref dar_display
/// \brief Displays a DynamicArray_s in the console.
void
dar_display(DynamicArray_t *array, int display_mode);

///////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////// Iterator ///
///////////////////////////////////////////////////////////////////////////////

// An array iterator. See the source file for the full documentation.
struct DynamicArrayIterator_s;

/// \brief A type for a dynamic array iterator.
///
/// A type for a <code> struct DynamicArrayIterator_s </code>.
typedef struct DynamicArrayIterator_s DynamicArrayIterator_t;

/// \brief A pointer type for a dynamic array iterator.
///
/// A pointer type for a <code> struct DynamicArrayIterator_s </code>.
typedef struct DynamicArrayIterator_s *DynamicArrayIterator;

///////////////////////////////////// STRUCTURE INITIALIZATION AND DELETION ///

/// \ref dar_iter_new
/// \brief Creates a new dynamic array iterator given a target.
DynamicArrayIterator_t *
dar_iter_new(DynamicArray_t *target);

/// \ref dar_iter_retarget
/// \brief Retargets an existing iterator.
void
dar_iter_retarget(DynamicArrayIterator_t *iter, DynamicArray_t *target);

/// \ref dar_iter_free
/// \brief Frees from memory an existing iterator.
void
dar_iter_free(DynamicArrayIterator_t *iter);

///////////////////////////////////////////////////////////////// ITERATION ///

/// \ref dar_iter_next
/// \brief Iterates to the next element if available.
bool
dar_iter_next(DynamicArrayIterator_t *iter);

/// \ref dar_iter_prev
/// \brief Iterates to the previous element if available.
bool
dar_iter_prev(DynamicArrayIterator_t *iter);

/// \ref dar_iter_to_start
/// \brief Iterates to the first element in the array.
bool
dar_iter_to_start(DynamicArrayIterator_t *iter);

/// \ref dar_iter_to_end
/// \brief Iterates to the last element in the array.
bool
dar_iter_to_end(DynamicArrayIterator_t *iter);

/////////////////////////////////////////////////////////// STRUCTURE STATE ///

/// \ref dar_iter_has_next
/// \brief Returns true if there is another element next in the iteration.
bool
dar_iter_has_next(DynamicArrayIterator_t *iter);

/// \ref dar_iter_has_prev
/// \brief Returns true if it is possible to iterate to a previous element.
bool
dar_iter_has_prev(DynamicArrayIterator_t *iter);

////////////////////////////////////////////////////////// INPUT AND OUTPUT ///

/// \ref dar_iter_set
/// \brief Sets the element pointed by the iterator to a new element.
bool
dar_iter_set(DynamicArrayIterator_t *iter, void *element);

/// \ref dar_iter_get
/// \brief Gets the element pointed by the iterator.
bool
dar_iter_get(DynamicArrayIterator_t *iter, void **result);

/////////////////////////////////////////////////////////////////// UTILITY ///

/// \ref dar_iter_peek_next
/// \brief Returns the next element in the iteration if available.
void *
dar_iter_peek_next(DynamicArrayIterator_t *iter);

/// \ref dar_iter_peek
/// \brief Returns the current element in the iteration if available.
void *
dar_iter_peek(DynamicArrayIterator_t *iter);

/// \ref dar_iter_peek_prev
/// \brief Returns the previous element in the iteration if available.
void *
dar_iter_peek_prev(DynamicArrayIterator_t *iter);

#ifdef __cplusplus
}
#endif

#endif //C_DATASTRUCTURES_LIBRARY_DYNAMICARRAY_H
