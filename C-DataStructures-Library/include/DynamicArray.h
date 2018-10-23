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

/// A DynamicArray is an Array that grows in size when needed. It has a
/// \c capacity that grows according to \c growth_rate. Both parameters can be
/// set by the user using the function dar_create() when creating a new array.
/// It can also be locked, disabling the growth functionality, or unlocked,
/// enabling it back.
///
struct DynamicArray_s
{
    /// \brief Data buffer.
    ///
    /// Buffer where elements are stored in.
    int *buffer;

    /// \brief Current amount of elements in the \c DynamicArray.
    ///
    /// Current amount of elements in the \c DynamicArray.
    size_t size;

    /// \brief \c DynamicArray buffer maximum capacity.
    ///
    /// Buffer maximum capacity. When \c size reaches \c capacity the buffer is
    /// reallocated and increases according to \c growth_rate.
    size_t capacity;

    /// \brief Buffer growth rate.
    ///
    /// Buffer growth rate. The new buffer capacity is calculated as:
    ///
    /// <code> capacity *= (growth_rate / 100.0) </code>
    size_t growth_rate;

    /// \brief Flag for locked capacity.
    ///
    /// If \c locked is set to true the buffer will not grow and if any
    /// elements are inserted with a full buffer \c DS_ERR_FULL will be
    /// returned.
    bool locked;
};

/// Defines a type for <code> struct DynamicArray_s </code>.
///
/// Every dynamic array is initialized by \c malloc with
/// \c sizeof(DynamicArray_t).
typedef struct DynamicArray_s DynamicArray_t;

/// Defines a type of pointer to <code> struct DynamicArray_s </code>.
///
/// This typedef is used to avoid having to declare every array as a pointer
/// type since they all must be dynamically allocated.
typedef struct DynamicArray_s *DynamicArray;

/// Initializes a \c DynamicArray with an initial capacity of 32 and a growth
/// rate of 200, that is, twice the size after each growth.
///
/// \param[in,out] dar The dynamic array to be initialized.
///
/// \return DS_ERR_ALLOC if dynamic array allocation failed.
/// \return DS_OK if all operations were successful.
///
/// \see dar_create
Status dar_init(DynamicArray *dar);

/// Initializes a \c DynamicArray with a user defined \c initial_capacity and
/// \c growth_rate. This function only accepts an \c initial_capacity greater
/// than 8 and a \c growth_rate greater than 100; but keep in mind that in some
/// cases if the \c initial_capacity is too small and the \c growth_rate is too
/// close to 100 there won't be an increase in capacity and the minimum growth
/// will be triggered.
///
/// \param[in,out] dar The dynamic array to be initialized.
/// \param[in] initial_capacity Buffer initial capacity.
/// \param[in] growth_rate Buffer growth rate.
///
/// \return DS_ERR_ALLOC if dynamic array allocation failed.
/// \return DS_ERR_INVALID_ARGUMENT if initial_capacity is less than 8 or
/// growth_rate is less than or equal to 100.
///
/// \see qua_init
Status dar_create(DynamicArray *dar, size_t initial_capacity, size_t growth_rate);

Status dar_make(DynamicArray *dar, int *array, size_t arr_size);

Status dar_insert(DynamicArray dar, int *array, size_t arr_size, size_t index);

Status dar_insert_front(DynamicArray dar, int value);

Status dar_insert_at(DynamicArray dar, int value, size_t index);

Status dar_insert_back(DynamicArray dar, int value);

Status dar_remove(DynamicArray dar, size_t from, size_t to);

Status dar_remove_front(DynamicArray dar, int *result);

Status dar_remove_at(DynamicArray dar, int *result, size_t index);

Status dar_remove_back(DynamicArray dar, int *result);

Status dar_update(DynamicArray dar, int value, size_t index);

Status dar_get(DynamicArray dar, int *result, size_t index);

Status dar_display(DynamicArray dar);

Status dar_display_array(DynamicArray dar);

Status dar_display_raw(DynamicArray dar);

Status dar_delete(DynamicArray *dar);

Status dar_erase(DynamicArray *dar);

size_t dar_cap(DynamicArray dar);

size_t dar_size(DynamicArray dar);

bool dar_empty(DynamicArray dar);

bool dar_full(DynamicArray dar);

bool dar_fits(DynamicArray dar, size_t size);

bool dar_contains(DynamicArray dar, int value);

Status dar_copy(DynamicArray dar, DynamicArray *result);

Status dar_prepend(DynamicArray dar1, DynamicArray dar2);

Status dar_add(DynamicArray dar1, DynamicArray dar2, size_t index);

Status dar_append(DynamicArray dar1, DynamicArray dar2);

Status dar_cap_lock(DynamicArray dar);

Status dar_cap_unlock(DynamicArray dar);

#endif //C_DATASTRUCTURES_LIBRARY_DYNAMICARRAY_H
