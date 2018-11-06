/**
 * @file DequeArray.h
 * 
 * @author Leonardo Vencovsky (https://github.com/LeoVen)
 *
 * @date 29/10/2018
 */

#ifndef C_DATASTRUCTURES_LIBRARY_DEQUEARRAY_H
#define C_DATASTRUCTURES_LIBRARY_DEQUEARRAY_H

#include "Core.h"

#ifdef __cplusplus
extern "C" {
#endif

/// A DequeArray is a buffered Deque with enqueue and dequeue operations on
/// both ends that are represented by indexes. It also has the ability to
/// increase in size when needed. This implementation also uses the QueueArray
/// implementation of resizing the buffer.
///
/// \b Advantages over \c Deque
/// - Fast insertion
/// - No need of pointers, only the data is allocated in memory
///
/// \b Drawbacks
/// - When the \c DequeArray is full the buffer needs to be reallocated and
/// elements need to be shifted
///
/// \b Functions
///
/// \c DequeArray.c
struct DequeArray_s
{
    /// \brief Data buffer.
    ///
    /// Buffer where elements are stored in.
    int *buffer;

    /// \brief Front of the deque.
    ///
    /// An index that represents the front of the deque.
    index_t front;

    /// \brief Back of the deque.
    ///
    /// An index that represents the back of the deque.
    index_t rear;

    /// \brief Current amount of elements in the \c QueueArray.
    ///
    /// Current amount of elements in the \c QueueArray.
    index_t size;

    /// \brief \c QueueArray buffer maximum capacity.
    ///
    /// Buffer maximum capacity. When \c size reaches \c capacity the buffer is
    /// reallocated and increases according to \c growth_rate.
    index_t capacity;

    /// \brief Buffer growth rate.
    ///
    /// Buffer growth rate. The new buffer capacity is calculated as:
    ///
    /// <code> capacity *= (growth_rate / 100.0) </code>
    index_t growth_rate;

    /// \brief Flag for locked capacity.
    ///
    /// If \c locked is set to true the buffer will not grow and if any
    /// elements are inserted with a full buffer \c DS_ERR_FULL will be
    /// returned.
    bool locked;
};

/// Defines a type for <code> struct DequeArray_s </code>.
///
/// Every deque is initialized by \c malloc with \c sizeof(DequeArray_t).
typedef struct DequeArray_s DequeArray_t;

/// Defines a type of pointer to <code> struct DequeArray_s </code>.
///
/// This typedef is used to avoid having to declare every deque as a pointer
/// type since they all must be dynamically allocated.
typedef struct DequeArray_s *DequeArray;

/// Initializes a \c DequeArray with an initial capacity of 32 and a growth
/// rate of 200, that is, twice the size after each growth.
///
/// \param[in,out] dqa The deque to be initialized.
///
/// \return DS_ERR_ALLOC if deque allocation failed.
/// \return DS_OK if all operations were successful.
///
/// \see dqa_create
Status dqa_init(DequeArray *dqa);

/// Initializes a \c DequeArray with a user defined \c initial_capacity and \c
/// growth_rate. This function only accepts an \c initial_capacity greater than
/// 8 and a \c growth_rate greater than 100; but keep in mind that in some
/// cases if the \c initial_capacity is too small and the \c growth_rate is too
/// close to 100 there won't be an increase in capacity and the minimum growth
/// will be triggered.
///
/// \param[in,out] dqa The deque to be initialized.
/// \param[in] initial_capacity Buffer initial capacity.
/// \param[in] growth_rate Buffer growth rate.
///
/// \return DS_ERR_ALLOC if deque allocation failed.
/// \return DS_ERR_INVALID_ARGUMENT if initial_capacity is less than 8 or
/// growth_rate is less than or equal to 100.
///
/// \see dqa_init
Status dqa_create(DequeArray *dqa, index_t initial_capacity, index_t growth_rate);

/// Inserts an element to the front of the specified deque.
///
/// \param[in] dqa The deque where the element is to be inserted.
/// \param[in] element The element to be inserted in the deque.
///
/// \return DS_ERR_ALLOC if the buffer reallocation failed.
/// \return DS_ERR_FULL if the buffer capacity is locked and the deque is full.
/// \return DS_ERR_NULL_POINTER if deque reference is \c NULL.
/// \return DS_OK if all operations were successful.
Status dqa_enqueue_front(DequeArray dqa, int element);

/// Inserts an element to the rear of the specified deque.
///
/// \param[in] dqa The deque where the element is to be inserted.
/// \param[in] element The element to be inserted in the deque.
///
/// \return DS_ERR_ALLOC if the buffer reallocation failed.
/// \return DS_ERR_FULL if the buffer capacity is locked and the deque is full.
/// \return DS_ERR_NULL_POINTER if deque reference is \c NULL.
/// \return DS_OK if all operations were successful.
Status dqa_enqueue_rear(DequeArray dqa, int element);

/// Removes an element at the front of the specified deque.
///
/// \param[in] dqa The deque where the element is to be removed from.
/// \param[out] result The resulting element removed from the deque.
///
/// \return DS_ERR_INVALID_OPERATION if the deque is empty.
/// \return DS_ERR_NULL_POINTER if deque reference is \c NULL.
/// \return DS_OK if all operations were successful.
Status dqa_dequeue_front(DequeArray dqa, int *result);

/// Removes an element at the rear of the specified deque.
///
/// \param[in] dqa The deque where the element is to be removed from.
/// \param[out] result The resulting element removed from the deque.
///
/// \return DS_ERR_INVALID_OPERATION if the deque is empty.
/// \return DS_ERR_NULL_POINTER if deque reference is \c NULL.
/// \return DS_OK if all operations were successful.
Status dqa_dequeue_rear(DequeArray dqa, int *result);

/// Displays a \c DequeArray in the console.
///
/// \param dqa The deque to be displayed in the console.
///
/// \return DS_ERR_NULL_POINTER if the deque reference is \c NULL.
/// \return DS_OK if all operations were successful.
Status dqa_display(DequeArray dqa);

/// Displays a \c DequeArray in the console like an array with its values
/// separated by commas, delimited with brackets.
///
/// \param dqa The deque to be displayed in the console.
///
/// \return DS_ERR_NULL_POINTER if the deque reference is \c NULL.
/// \return DS_OK if all operations were successful.
Status dqa_display_array(DequeArray dqa);

/// Displays a \c DequeArray in the console with its values separated by
/// spaces.
///
/// \param dqa The deque to be displayed in the console.
///
/// \return DS_ERR_NULL_POINTER if the deque reference is \c NULL.
/// \return DS_OK if all operations were successful.
Status dqa_display_raw(DequeArray dqa);

/// Frees the deque buffer and the DequeArray structure; the variable then is
/// set no \c NULL;
///
/// \param dqa The deque to be freed from memory.
///
/// \return DS_ERR_NULL_POINTER if the deque reference is \c NULL.
/// \return DS_OK if all operations were successful.
Status dqa_delete(DequeArray *dqa);

/// This function sets the deque to its initial state, erasing all of its data
/// and re-initializing the structure. It is equivalent to calling dqa_delete()
/// and then dqa_init().
///
/// \param dqa The deque to be erased.
///
/// \return DS_ERR_ALLOC if deque allocation failed.
/// \return DS_ERR_NULL_POINTER if the deque reference is \c NULL.
/// \return DS_OK if all operations were successful.
Status dqa_erase(DequeArray *dqa);

int dqa_peek_front(DequeArray dqa);

int dqa_peek_rear(DequeArray dqa);

index_t dqa_size(DequeArray dqa);

index_t dqa_capacity(DequeArray dqa);

bool dqa_empty(DequeArray dqa);

bool dqa_full(DequeArray dqa);

bool dqa_fits(DequeArray dqa, index_t size);

Status dqa_copy(DequeArray dqa, DequeArray *result);

Status dqa_cap_lock(DequeArray dqa);

Status dqa_cap_unlock(DequeArray dqa);

#ifdef __cplusplus
}
#endif

#endif //C_DATASTRUCTURES_LIBRARY_DEQUEARRAY_H
