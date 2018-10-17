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

/**
 * A \c QueueArray is a buffered \c Queue with FIFO operations, (First-int
 * First-out), so the first item added is the first one to be removed. It
 * also has the ability to increase in size.
 *
 * \b Advantages over \c Queue
 * - Fast insertion
 * - No need of pointers, only the data is allocated in memory
 *
 * \b Drawbacks
 * - Slow removal as all items sometimes must be shifted
 * - When the @c QueueArray is full the buffer needs to be reallocated
 *
 * \b Functions
 *
 * \c Located in file QueueArray.c
 *
 */
struct QueueArray_s
{
    /** \c QueueArray data buffer where elements are stored in. */
    int *buffer;

    /** An index that represents the front of the queue. The \c qua_dequeue
     * operates relative to this index.
     */
    size_t front;

    /** An index that represents the back of the queue. The \c qua_enqueue
     * operates relative to this index.
     */
    size_t rear;

    /** Current amount of elements in the \c QueueArray. */
    size_t size;

    /** \c QueueArray buffer maximum capacity */
    size_t capacity;

    /** \c QueueArray buffer growth rate. The new buffer capacity is calculated
     * as:
     * \code
     * capacity *= (growth_rate / 100.0)
     * \endcode
     */
    size_t growth_rate;
};

/** Defines a type for <code> struct QueueArray_s </code>. */
typedef struct QueueArray_s QueueArray_t;

/** Defines a type of pointer to <code> struct QueueArray_s </code>. */
typedef struct QueueArray_s *QueueArray;

/// Initializes a \c QueueArray with an initial capacity of 32 and a growth
/// rate of 200, that is, twice the size after each growth.
///
/// \param qua \c QueueArray to be initialized.
///
/// \return DS_ERR_ALLOC if allocation failed.
/// \return DS_OK if all operations were successful.
///
/// \see qua_create
Status qua_init(QueueArray *qua);

/// Initializes a \c QueueArray with a user defined \c initial_capacity and \c
/// growth_rate. This function only accepts an \c initial_capacity greater than
/// 0 and a \c growth_rate greater than 100; but keep in mind that in some
/// cases if the \c initial_capacity is too small and the \c growth_rate is too
///  close to 100 there won't be an increase in capacity and the minimum growth
/// will be triggered.
///
/// \param qua \c QueueArray to be initialized
/// \param initial_capacity Buffer initial capacity
/// \param growth_rate Buffer growth rate
///
/// \return DS_ERR_ALLOC if allocation failed
/// \return DS_ERR_INVALID_ARGUMENT if initial_capacity is 0 or growth_rate is
/// less than or equal to 100.
///
/// \see qua_init
/// \see qua_grow
Status qua_create(QueueArray *qua, size_t initial_capacity, size_t growth_rate);

Status qua_enqueue(QueueArray qua, int value);

Status qua_dequeue(QueueArray qua, int *value);

Status qua_display(QueueArray qua);

Status qua_display_array(QueueArray qua);

Status qua_display_raw(QueueArray qua);

Status qua_delete(QueueArray *qua);

Status qua_erase(QueueArray *qua);

int qua_peek_front(QueueArray qua);

int qua_peek_rear(QueueArray qua);

size_t qua_size(QueueArray qua);

size_t qua_capacity(QueueArray qua);

bool qua_empty(QueueArray qua);

bool qua_full(QueueArray qua);

bool qua_fits(QueueArray qua, size_t size);

Status qua_copy(QueueArray qua, QueueArray *result);

#endif //C_DATASTRUCTURES_LIBRARY_QUEUEARRAY_H
