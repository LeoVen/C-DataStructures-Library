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

/// A DequeArray is a buffered Deque with enqueue and dequeue operations on
/// both ends that are represented by indexes. It also has the ability to
/// increase in size when needed.
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

    /// \brief Front of the queue.
    ///
    /// An index that represents the front of the queue.
    index_t front;

    /// \brief Back of the queue.
    ///
    /// An index that represents the back of the queue.
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
/// Every queue is initialized by \c malloc with \c sizeof(DequeArray_t).
typedef struct DequeArray_s DequeArray_t;

/// Defines a type of pointer to <code> struct DequeArray_s </code>.
///
/// This typedef is used to avoid having to declare every queue as a pointer
/// type since they all must be dynamically allocated.
typedef struct DequeArray_s *DequeArray;

Status dqa_init(DequeArray *dqa);

Status dqa_create(DequeArray *dqa, index_t initial_capacity, index_t growth_rate);

Status dqa_enqueue_front(DequeArray dqa, int value);

Status dqa_enqueue_rear(DequeArray dqa, int value);

Status dqa_dequeue_front(DequeArray dqa, int *value);

Status dqa_dequeue_rear(DequeArray dqa, int *value);

Status dqa_display(DequeArray dqa);

Status dqa_display_array(DequeArray dqa);

Status dqa_display_raw(DequeArray dqa);

Status dqa_delete(DequeArray *dqa);

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

#endif //C_DATASTRUCTURES_LIBRARY_DEQUEARRAY_H
