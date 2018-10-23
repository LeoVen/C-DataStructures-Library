/**
 * @file QueueArray.h
 * 
 * @author Leonardo Vencovsky (https://github.com/LeoVen)
 *
 * @date 04/10/2018
 */

#ifndef C_DATASTRUCTURES_LIBRARY_QUEUEARRAY_H
#define C_DATASTRUCTURES_LIBRARY_QUEUEARRAY_H

#include "Core.h"

/// A QueueArray is a buffered Queue with FIFO (First-in First-out) or LILO
/// (Last-in Last-out) operations, so the first item added is the first one to
/// be removed. The queue is implemented as a circular buffer. Its indexes can
/// wrap around the buffer if they reach the end. The buffer can also expand.
/// The grow function will first check if there are any items that wrapped
/// around the buffer. If so, it will calculate which portion (left or right)
/// has the least amount of elements. If left is chosen, it will append its
/// contents to the right of the right portion; otherwise it will shift the
/// right portion to the end of the buffer. This effectively decreases the
/// amount of shifts needed.
///
/// \b Advantages over \c Queue
/// - No need of pointers, only the data is allocated in memory
///
/// \b Drawbacks
/// - When the \c QueueArray is full the buffer needs to be reallocated
/// - When the buffer is reallocated some items might need to be shifted
///
/// \b Functions
///
/// Located in file QueueArray.c
struct QueueArray_s
{
    /// \brief Data buffer.
    ///
    /// Buffer where elements are stored in.
    int *buffer;

    /// \brief Front of the queue.
    ///
    /// An index that represents the front of the queue. The \c qua_dequeue
    /// operates relative to this index. This is where the next element to be
    /// removed is located. This index represents the exact position of an the
    /// front element (unlike the \c rear index).
    index_t front;

    /// \brief Back of the queue.
    ///
    /// An index that represents the back of the queue. The \c qua_enqueue
    /// operates relative to this index. This is where the last element was
    /// added. It does not represents the exact position of an element; it is
    /// always one position ahead (circularly) of the last inserted element.
    /// This might cause confusion; when this index is 0 the real rear element
    /// is at <code> capacity - 1 </code>.
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

/// Defines a type for <code> struct QueueArray_s </code>.
///
/// Every queue is initialized by \c malloc with \c sizeof(QueueArray_t).
typedef struct QueueArray_s QueueArray_t;

/// Defines a type of pointer to <code> struct QueueArray_s </code>.
///
/// This typedef is used to avoid having to declare every queue as a pointer
/// type since they all must be dynamically allocated.
typedef struct QueueArray_s *QueueArray;

/// Initializes a \c QueueArray with an initial capacity of 32 and a growth
/// rate of 200, that is, twice the size after each growth.
///
/// \param[in,out] qua The queue to be initialized.
///
/// \return DS_ERR_ALLOC if queue allocation failed.
/// \return DS_OK if all operations were successful.
///
/// \see qua_create
Status qua_init(QueueArray *qua);

/// Initializes a \c QueueArray with a user defined \c initial_capacity and \c
/// growth_rate. This function only accepts an \c initial_capacity greater than
/// 8 and a \c growth_rate greater than 100; but keep in mind that in some
/// cases if the \c initial_capacity is too small and the \c growth_rate is too
/// close to 100 there won't be an increase in capacity and the minimum growth
/// will be triggered.
///
/// \param[in,out] qua The queue to be initialized.
/// \param[in] initial_capacity Buffer initial capacity.
/// \param[in] growth_rate Buffer growth rate.
///
/// \return DS_ERR_ALLOC if queue allocation failed.
/// \return DS_ERR_INVALID_ARGUMENT if initial_capacity is less than 8 or
/// growth_rate is less than or equal to 100.
///
/// \see qua_init
Status qua_create(QueueArray *qua, index_t initial_capacity, index_t growth_rate);

/// Inserts an element into the specified queue. The element is added at the
/// \c rear index.
///
/// \param[in] qua The queue where the element is to be inserted.
/// \param[in] element The element to be inserted in the queue.
///
/// \return DS_ERR_ALLOC if the buffer reallocation failed.
/// \return DS_ERR_FULL if the buffer capacity is locked and the queue is full.
/// \return DS_ERR_NULL_POINTER if queue reference is \c NULL.
/// \return DS_OK if all operations were successful.
Status qua_enqueue(QueueArray qua, int element);

/// Removes an element from the specified queue. The element is removed from
/// the \c front index.
///
/// \param[in] qua The queue where the element is to be removed from.
/// \param[out] result The resulting element removed from the queue.
///
/// \return DS_ERR_INVALID_OPERATION if the queue is empty.
/// \return DS_ERR_NULL_POINTER if queue reference is \c NULL.
/// \return DS_OK if all operations were successful.
Status qua_dequeue(QueueArray qua, int *result);

/// Displays a \c QueueArray in the console.
///
/// \param qua The queue to be displayed in the console.
///
/// \return DS_ERR_NULL_POINTER if the queue reference is \c NULL.
/// \return DS_OK if all operations were successful.
Status qua_display(QueueArray qua);

/// Displays a \c QueueArray in the console like an array with its values
/// separated by commas, delimited with brackets.
///
/// \param qua The queue to be displayed in the console.
///
/// \return DS_ERR_NULL_POINTER if the queue reference is \c NULL.
/// \return DS_OK if all operations were successful.
Status qua_display_array(QueueArray qua);

/// Displays a \c QueueArray in the console with its values separated by
/// spaces.
///
/// \param qua The queue to be displayed in the console.
///
/// \return DS_ERR_NULL_POINTER if the queue reference is \c NULL.
/// \return DS_OK if all operations were successful.
Status qua_display_raw(QueueArray qua);

/// Frees the queue buffer and the QueueArray structure; the variable then is
/// set no \c NULL;
///
/// \param qua The queue to be freed from memory.
///
/// \return DS_ERR_NULL_POINTER if the queue reference is \c NULL.
/// \return DS_OK if all operations were successful.
Status qua_delete(QueueArray *qua);

Status qua_erase(QueueArray *qua);

int qua_peek_front(QueueArray qua);

int qua_peek_rear(QueueArray qua);

index_t qua_size(QueueArray qua);

index_t qua_capacity(QueueArray qua);

bool qua_empty(QueueArray qua);

bool qua_full(QueueArray qua);

bool qua_fits(QueueArray qua, index_t size);

Status qua_copy(QueueArray qua, QueueArray *result);

Status qua_cap_lock(QueueArray qua);

Status qua_cap_unlock(QueueArray qua);

#endif //C_DATASTRUCTURES_LIBRARY_QUEUEARRAY_H
