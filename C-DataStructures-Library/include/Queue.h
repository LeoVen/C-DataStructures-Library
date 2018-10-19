/**
 * @file Queue.h
 * 
 * @author Leonardo Vencovsky (https://github.com/LeoVen)
 *
 * @date 26/09/2018
 */

#ifndef C_DATASTRUCTURES_LIBRARY_QUEUE_H
#define C_DATASTRUCTURES_LIBRARY_QUEUE_H

#include "Core.h"

/// This is a linked list implementation of a \c Queue with FIFO (First-in
/// First-out) or LILO (Last-in Last-out) operations, so the first item added
/// is the first one to be removed. It is implemented as a SinglyLinkedList but
///  with restricted operations to preserve the FIFO (LILO) order of elements.
/// The function \c que_enqueue() is equivalent to \c sll_insert_tail() and the
/// function \c que_dequeue() is equivalent to \c sll_remove_head(). This is
/// done in such a way that removal and insertions are O(1) without the need of
/// a second pointer to the previous element (like a Deque implemented with a
/// DoublyLinkedList).
///
/// \b Advantages over \c QueueArray
/// - Indefinitely grows
/// - No need to reallocate buffers or shift elements
///
/// \b Drawbacks
/// - No random access
/// - More memory usage as in every node there is a pointer to the next node
///
/// \b Functions
/// Located in file Queue.c
struct Queue_s
{
    /// \brief Current amount of elements in the \c Queue.
    ///
    /// Queue current amount of nodes linked between the \c front and \c rear
    /// pointers.
    size_t length;

    /// \brief The front of the queue.
    ///
    /// Where elements are removed from. The function \c que_dequeue() operates
    /// relative to this pointer.
    struct QueueNode_s *front;

    /// \brief The rear of the queue.
    ///
    /// Where elements are added. The function \c que_enqueue() operates
    /// relative to this pointer.
    struct QueueNode_s *rear;
};

/// Defines a type for <code> struct Queue_s </code>.
///
/// Every queue is initialized by \c malloc with \c sizeof(Queue_t).
typedef struct Queue_s Queue_t;

/// Defines a type of pointer to <code> struct Queue_s </code>.
///
/// This typedef is used to avoid having to declare every queue as a pointer
/// type since they all must be dynamically allocated.
typedef struct Queue_s *Queue;

/// Initializes a new \c Queue with initial length 0 and its pointer members
/// pointing to \c NULL.
///
/// \param[in,out] que The queue to be initialized.
///
/// \return DS_ERR_ALLOC if queue allocation failed.
/// \return DS_OK if all operations were successful.
Status que_init(Queue *que);

/// Inserts an element into the specified queue. The element is added relative
/// to the \c rear pointer.
///
/// \param[in] que The queue where the element is to be inserted.
/// \param[in] element The element to be inserted in the queue.
///
/// \return DS_ERR_ALLOC if node allocation failed.
/// \return DS_ERR_NULL_POINTER if queue reference is \c NULL.
/// \return DS_OK if all operations were successful.
Status que_enqueue(Queue que, int element);

/// Removes an element from the specified queue. The element is removed
/// relative to the \c front pointer.
///
/// \param[in] que The queue where the element is to be removed from.
/// \param[out] result The resulting element removed from the queue.
///
/// \return DS_ERR_INVALID_OPERATION if the queue is empty.
/// \return DS_ERR_NULL_POINTER if queue reference is \c NULL.
/// \return DS_OK if all operations were successful.
Status que_dequeue(Queue que, int *result);

/// Displays a \c Queue in the console.
///
/// \param que The queue to be displayed in the console.
///
/// \return DS_ERR_NULL_POINTER if the queue reference is \c NULL.
/// \return DS_OK if all operations were successful.
Status que_display(Queue que);

/// Displays a \c Queue in the console like an array with its values separated
/// by commas, delimited with brackets.
///
/// \param que The queue to be displayed in the console.
///
/// \return DS_ERR_NULL_POINTER if the queue reference is \c NULL.
/// \return DS_OK if all operations were successful.
Status que_display_array(Queue que);

/// Displays a \c Queue in the console with its values separated by spaces.
///
/// \param que The queue to be displayed in the console.
///
/// \return DS_ERR_NULL_POINTER if the queue reference is \c NULL.
/// \return DS_OK if all operations were successful.
Status que_display_raw(Queue que);

Status que_delete(Queue *que);

Status que_erase(Queue *que);

int que_peek_front(Queue que);

int que_peek_rear(Queue que);

size_t que_length(Queue que);

bool que_empty(Queue que);

Status que_copy(Queue que, Queue *result);

#endif //C_DATASTRUCTURES_LIBRARY_QUEUE_H
