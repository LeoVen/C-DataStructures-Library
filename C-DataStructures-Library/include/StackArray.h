/**
 * @file StackArray.h
 * 
 * @author Leonardo Vencovsky (https://github.com/LeoVen)
 *
 * @date 03/10/2018
 */

#ifndef C_DATASTRUCTURES_LIBRARY_STACKARRAY_H
#define C_DATASTRUCTURES_LIBRARY_STACKARRAY_H

#include "Core.h"

/// A \c StackArray is a buffered \c Stack with FILO (First-in Last-out) or
/// LIFO (Last-in First-out) operations, so the first item added is the last
/// one to be removed. The stack is implemented as a normal buffer that only
/// grows on one side. This simplifies the structure as no indexes will ever
/// need to wrap around the buffer. The height of the stack, or the amount of
/// elements in the stack can be used as an index for push and pop operations.
/// The buffer can also expand according to the \c growth_rate.
///
/// \b Advantages over \c Stack
/// - No need of pointers, only the data is allocated in memory
///
/// \b Drawbacks
/// - When the @c StackArray is full the buffer needs to be reallocated
///
/// \b Functions
///
/// Located in file StackArray.c
struct StackArray_s
{
    /// \brief Data buffer.
    ///
    /// Buffer where elements are stored in.
    int *buffer;

    /// \brief Current amount of elements in the \c StackArray.
    ///
    /// Current amount of elements in the \c StackArray.
    size_t height;

    /// \brief \c StackArray buffer maximum capacity.
    ///
    /// Buffer maximum capacity. When \c height reaches \c capacity the buffer
    /// is reallocated and increases according to \c growth_rate.
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

/// Defines a type for <code> struct StackArray_s </code>.
///
/// Every stack is initialized by \c malloc with \c sizeof(StackArray_t)
typedef struct StackArray_s StackArray_t;

/// Defines a type of pointer to <code> struct StackArray_s </code>.
///
/// This typedef is used to avoid having to declare every stack as a pointer
/// type since they all must be dynamically allocated.
typedef struct StackArray_s *StackArray;

/// Initializes a \c StackArray with an initial capacity of 32 and a growth
/// rate of 200, that is, twice the size after each growth.
///
/// \param[in,out] sta The stack to be initialized.
///
/// \return DS_ERR_ALLOC if stack allocation failed.
/// \return DS_OK if all operations were successful.
///
/// \see sta_create
Status sta_init(StackArray *sta);

/// Initializes a \c StackArray with a user defined \c initial_capacity and \c
/// growth_rate. This function only accepts an \c initial_capacity greater than
/// 8 and a \c growth_rate greater than 100; but keep in mind that in some
/// cases if the \c initial_capacity is too small and the \c growth_rate is too
/// close to 100 there won't be an increase in capacity and the minimum growth
/// will be triggered.
///
/// \param[in,out] sta The stack to be initialized.
/// \param[in] initial_capacity Buffer initial capacity
/// \param[in] growth_rate Buffer growth rate
///
/// \return DS_ERR_ALLOC if allocation failed
/// \return DS_ERR_INVALID_ARGUMENT if initial_capacity is 0 or growth_rate is
/// less than or equal to 100.
///
/// \see sta_init
Status sta_create(StackArray *sta, size_t initial_capacity, size_t growth_rate);

/// Inserts an element at the top of the specified stack.
///
/// \param[in] sta The stack where the element is to be inserted.
/// \param[in] element The element to be inserted in the stack.
///
/// \return DS_ERR_ALLOC if the buffer reallocation failed.
/// \return DS_ERR_FULL if the buffer capacity is locked and the stack is full.
/// \return DS_ERR_NULL_POINTER if stack reference is \c NULL.
/// \return DS_OK if all operations were successful.
Status sta_push(StackArray sta, int element);

/// Removes an element from the top of the specified stack.
///
/// \param[in] sta The stack where the element is to be removed from.
/// \param[out] result The resulting element removed from the stack.
///
/// \return DS_ERR_INVALID_OPERATION if the stack is empty.
/// \return DS_ERR_NULL_POINTER if stack reference is \c NULL.
/// \return DS_OK if all operations were successful.
Status sta_pop(StackArray sta, int *result);

/// Alias to sta_push().
///
/// \param[in] sta The stack where the element is to be inserted.
/// \param[in] element The element to be inserted in the stack.
///
/// \return DS_ERR_ALLOC if the buffer reallocation failed.
/// \return DS_ERR_FULL if the buffer capacity is locked and the stack is full.
/// \return DS_ERR_NULL_POINTER if stack reference is \c NULL.
/// \return DS_OK if all operations were successful.
///
/// \see sta_push()
Status sta_insert(StackArray sta, int element);

/// Alias to sta_pop().
///
/// \param[in] sta The stack where the element is to be removed from.
/// \param[out] result The resulting element removed from the stack.
///
/// \return DS_ERR_INVALID_OPERATION if the stack is empty.
/// \return DS_ERR_NULL_POINTER if stack reference is \c NULL.
/// \return DS_OK if all operations were successful.
///
/// \see sta_pop()
Status sta_remove(StackArray sta, int *result);

/// Displays a \c StackArray in the console.
///
/// \param sta The stack to be displayed in the console.
///
/// \return DS_ERR_NULL_POINTER if the stack reference is \c NULL.
/// \return DS_OK if all operations were successful.
Status sta_display(StackArray sta);

/// Displays a \c StackArray in the console like an array with its values
/// separated by commas, delimited with brackets.
///
/// \param sta The stack to be displayed in the console.
///
/// \return DS_ERR_NULL_POINTER if the stack reference is \c NULL.
/// \return DS_OK if all operations were successful.
Status sta_display_array(StackArray sta);

/// Displays a \c StackArray in the console with its values separated by
/// spaces.
///
/// \param sta The stack to be displayed in the console.
///
/// \return DS_ERR_NULL_POINTER if the stack reference is \c NULL.
/// \return DS_OK if all operations were successful.
Status sta_display_raw(StackArray sta);

Status sta_delete(StackArray *sta);

Status sta_erase(StackArray *sta);

int sta_peek(StackArray sta);

size_t sta_height(StackArray sta);

size_t sta_capacity(StackArray sta);

bool sta_empty(StackArray sta);

bool sta_full(StackArray sta);

bool sta_fits(StackArray sta, size_t size);

Status sta_copy(StackArray sta, StackArray *result);

Status sta_cap_lock(StackArray sta);

Status sta_cap_unlock(StackArray sta);

#endif //C_DATASTRUCTURES_LIBRARY_STACKARRAY_H
