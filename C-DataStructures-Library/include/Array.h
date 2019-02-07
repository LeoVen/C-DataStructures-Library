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
#include "Interface.h"

#ifdef __cplusplus
extern "C" {
#endif

/// \struct Array_s
/// \brief A C generic array wrapper.
struct Array_s;

/// \brief A type for a an Array_s.
///
/// A type for a <code> struct Array_s </code> so you don't have to always
/// write the full name of it.
typedef struct Array_s Array_t;

/// \brief A pointer type for an Array_s.
///
/// A pointer type to <code> struct Array_s </code>. This typedef is used to
/// avoid having to declare every array as a pointer type since they all must
/// be dynamically allocated.
typedef struct Array_s *Array;

///////////////////////////////////// STRUCTURE INITIALIZATION AND DELETION ///

/// \ref arr_new
/// \brief Initializes a new Array_s.
Array_t *
arr_new(Interface_t *interface, integer_t length);

/// \ref arr_free
/// \brief Frees from memory an Array_s and all of its elements.
void
arr_free(Array_t *array);

/// \ref arr_free_shallow
/// \brief Frees from memory an Array_s leaving its elements intact.
void
arr_free_shallow(Array_t *array);

/// \ref arr_erase
/// \brief Frees from memory all elements of an Array_s.
void
arr_erase(Array_t *array);

/// \ref arr_erase_shallow
/// \brief Sets all array pointers to NULL (shallow erase).
void
arr_erase_shallow(Array_t *array);

//////////////////////////////////////////////////////////// CONFIGURATIONS ///

/// \ref arr_config
/// \brief Sets a new interface for the array.
void
arr_config(Array_t *array, Interface_t *interface);

/////////////////////////////////////////////////////////////////// SETTERS ///

/////////////////////////////////////////////////////////////////// GETTERS ///

/// \ref arr_length
/// \brief Returns the length of the specified array.
integer_t
arr_length(Array_t *array);

/// \ref arr_count
/// \brief Returns how many non-null elements are in the array.
integer_t
arr_count(Array_t *array);

////////////////////////////////////////////////////////// INPUT AND OUTPUT ///

/// \ref arr_set_first
/// \brief Sets the first available position of the array to a given element.
integer_t
arr_set_first(Array_t *array, void *element);

/// \ref arr_set
/// \brief Sets the specified index to a specified element.
integer_t
arr_set(Array_t *array, void *element, integer_t index);

/// \ref arr_set_last
/// \brief Sets the last available position of the array to a given element.
integer_t
arr_set_last(Array_t *array, void *element);

/// \ref arr_get_first
/// \brief Gets the first available element in the array.
integer_t
arr_get_first(Array_t *array, void **result);

/// \ref arr_get
/// \brief Gets an element at the specified index.
integer_t
arr_get(Array_t *array, void **result, integer_t index);

/// \ref arr_get_last
/// \brief Gets the last available element in the array.
integer_t
arr_get_last(Array_t *array, void **result);

/// \ref arr_remove_first
/// \brief Removes the first occurrence of an element in the array.
integer_t
arr_remove_first(Array_t *array, void **result);

/// \ref arr_remove
/// \brief Removes an element at the specified index.
integer_t
arr_remove(Array_t *array, void **result, integer_t index);

/// \ref arr_remove_last
/// \brief Removes the last occurrence of an element in the array.
integer_t
arr_remove_last(Array_t *array, void **result);

/// \ref arr_update_first
/// \brief Updates the first occurrence of an element in the array.
integer_t
arr_update_first(Array_t *array, void *element);

/// \ref arr_update
/// \brief Updates an element at the specified index.
integer_t
arr_update(Array_t *array, void *element, integer_t index);

/// \ref arr_update_last
/// \brief Updates the last occurrence of an element in the array.
integer_t
arr_update_last(Array_t *array, void *element);

/////////////////////////////////////////////////////////// STRUCTURE STATE ///

/// \ref arr_full
/// \brief Returns true if the specified array is full.
bool
arr_full(Array_t *array);

/// \ref arr_empty
/// \brief Returns true if the specified array is empty.
bool
arr_empty(Array_t *array);

/////////////////////////////////////////////////////////////////// UTILITY ///

/// \ref arr_max
/// \brief Returns the highest element in the array.
void *
arr_max(Array_t *array);

/// \ref arr_min
/// \brief Returns the lowest element in the array.
void *
arr_min(Array_t *array);

/// \ref arr_index_first
/// \brief Returns the index of the first element that matches a given key.
integer_t
arr_index_first(Array_t *array, void *key);

/// \ref arr_index_last
/// \brief Returns the index of the last element that matches a given key.
integer_t
arr_index_last(Array_t *array, void *key);

/// \ref arr_contains
/// \brief Returns true if a given key is present in the array.
bool
arr_contains(Array_t *array, void *key);

/// \ref arr_switch
/// \brief Switches two elements from two specified indexes.
integer_t
arr_switch(Array_t *array, integer_t index1, integer_t index2);

/// \ref arr_reverse
/// \brief Reverses the order of its elements.
integer_t
arr_reverse(Array_t *array);

/// \ref arr_copy
/// \brief Makes a deep copy of the specified array.
Array_t *
arr_copy(Array_t *array);

/// \ref arr_copy_shallow
/// \brief Makes a shallow copy of the specified array.
Array_t *
arr_copy_shallow(Array_t *array);

/// \ref arr_sort
/// \brief Sorts the specified array with its interface's compare function.
void
arr_sort(Array_t *array);

/// \ref arr_sortby
/// \brief Sorts the specified array with a given comparator function.
void
arr_sortby(Array_t *array, compare_f comparator);

/// \ref arr_to_array
/// \brief Makes a copy to a C array.
void **
arr_to_array(Array_t *array, integer_t *length, bool shallow);

/// \ref arr_from_array
/// \brief Makes a new Array_s from an existing C array.
Array_t *
arr_from_array(Interface_t *interface, void **buffer, integer_t length);

/////////////////////////////////////////////////////////////////// DISPLAY ///

/// \ref arr_display
/// \brief Displays an Array_s in the console.
void
arr_display(Array_t *array, int display_mode);

///////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////// Iterator ///
///////////////////////////////////////////////////////////////////////////////

/// \struct ArrayIterator_s
/// \brief An iterator for an Array_s.
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

/// \ref arr_iter_new
/// \brief Initializes an ArrayIterator_s with a target Array_s.
ArrayIterator_t *
arr_iter_new(Array_t *target);

/// \ref arr_iter_retarget
/// \brief Retarget or resets an existing iterator.
bool
arr_iter_retarget(ArrayIterator_t *iter, Array target);

/// \ref arr_iter_free
/// \brief Frees from memory an existing iterator.
void
arr_iter_free(ArrayIterator_t *iter);

///////////////////////////////////////////////////////////////// ITERATION ///

/// \ref arr_iter_next
/// \brief Moves the iterator to the next element.
bool
arr_iter_next(ArrayIterator_t *iter);

/// \ref arr_iter_prev
/// \brief Moves the iterator to the previous element.
bool
arr_iter_prev(ArrayIterator_t *iter);

/// \ref arr_iter_to_start
/// \brief Moves the iterator to the start of the array.
bool
arr_iter_to_start(ArrayIterator_t *iter);

/// \ref arr_iter_to_end
/// \brief Moves the iterator to the end of the array.
bool
arr_iter_to_end(ArrayIterator_t *iter);

/////////////////////////////////////////////////////////// STRUCTURE STATE ///

/// \ref arr_iter_has_next
/// \brief Returns true if the iterator has a next element to iterate.
bool
arr_iter_has_next(ArrayIterator_t *iter);

/// \ref arr_iter_has_prev
/// \brief Returns true if the iterator has a previous element to iterate.
bool
arr_iter_has_prev(ArrayIterator_t *iter);

/// \ref arr_iter_test
/// \brief Returns true if the iterator's current position is a valid element.
bool
arr_iter_test(ArrayIterator_t *iter);

////////////////////////////////////////////////////////// INPUT AND OUTPUT ///

/// \ref arr_iter_set
/// \brief Sets a new element at the iterator's current position.
bool
arr_iter_set(ArrayIterator_t *iter, void *element);

/// \ref arr_iter_get
/// \brief Gets a copy of the iterator's current element.
bool
arr_iter_get(ArrayIterator_t *iter, void **result);

/// \ref arr_iter_pop
/// \brief Removes and returns the iterator's current element.
bool
arr_iter_pop(ArrayIterator_t *iter, void **result);

/////////////////////////////////////////////////////////////////// UTILITY ///

/// \ref arr_iter_peek_next
/// \brief Returns the next element if any.
void *
arr_iter_peek_next(ArrayIterator_t *iter);

/// \ref arr_iter_peek
/// \brief Returns the current element.
void *
arr_iter_peek(ArrayIterator_t *iter);

/// \ref arr_iter_peek_prev
/// \brief Returns the previous element if any.
void *
arr_iter_peek_prev(ArrayIterator_t *iter);

///////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////// Wrapper ///
///////////////////////////////////////////////////////////////////////////////

/// \todo ArrayWrapper

#ifdef __cplusplus
}
#endif

#endif //C_DATASTRUCTURES_LIBRARY_ARRAY_H
