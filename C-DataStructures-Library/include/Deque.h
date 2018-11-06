/**
 * @file Deque.h
 *
 * @author Leonardo Vencovsky (https://github.com/LeoVen)
 *
 * @date 27/09/2018
 */

#ifndef C_DATASTRUCTURES_LIBRARY_DEQUE_H
#define C_DATASTRUCTURES_LIBRARY_DEQUE_H

#include "Core.h"

/// This is a linked list implementation of a Deque. A Deque is a double-ended
/// queue, so you can insert and remove elements from both ends of the queue.
/// It is implemented as a \c DoublyLinkedList but with restricted operations
/// which can only insert or remove elements at the edges of the linked list.
/// With this implementation every insertion and removal operations take
/// constant time O(1). This simplifies things a lot but every node needs to
/// maintain two pointers, one to its left and another to its right.
///
/// \b Advantages over \c DequeArray
/// - Indefinitely grows
/// - No need to reallocate buffers or shift elements
///
/// \b Drawbacks
/// - No random access
/// - More memory usage as in every node there are two pointers to the
/// neighbouring nodes
///
/// \b Functions
/// Located in file Deque.c
struct Deque_s
{
    /// \brief Current amount of elements in the \c Deque.
    ///
    /// Current amount of nodes linked between the \c front and \c rear
    /// pointers.
    index_t length;

    /// \brief Queue length limit.
    ///
    /// If it is set to 0 or a negative value then the deque has no limit to
    /// its length. Otherwise it won't be able to have more elements than the
    /// specified value. The deque is always initialized with no restrictions
    /// to its length, that is, \c limit equals 0. The user won't be able to
    /// limit the list length if it already has more elements than the
    /// specified limit.
    index_t limit;

    /// \brief Points to the first Node on the deque.
    ///
    /// Points to the first Node on the deque or \c NULL if the deque is empty.
    struct DequeNode_s *front;

    /// \brief Points to the last Node on the deque.
    ///
    /// Points to the first Node on the deque or \c NULL if the deque is empty.
    struct DequeNode_s *rear;
};

/// Defines a type for <code> struct Deque_s </code>.
///
/// Every deque is initialized by \c malloc with \c sizeof(Deque_t).
typedef struct Deque_s Deque_t;

/// Defines a type of pointer to <code> struct Deque_s </code>.
///
/// This typedef is used to avoid having to declare every deque as a pointer
/// type since they all must be dynamically allocated.
typedef struct Deque_s *Deque;

/// Initializes a new \c Deque with initial length and limit as 0 and its
/// pointer members pointing to \c NULL.
///
/// \param[in,out] deq The deque to be initialized.
///
/// \return DS_ERR_ALLOC if deque allocation failed.
/// \return DS_OK if all operations were successful.
Status deq_init(Deque *deq);

/// Inserts an element at the front of the specified deque.
///
/// \param[in] deq The deque where the element is to be inserted.
/// \param[in] element The element to be inserted in the deque.
///
/// \return DS_ERR_ALLOC if node allocation failed.
/// \return DS_ERR_FULL if \c limit is set (greater than 0) and the deque
/// length reached the specified limit.
/// \return DS_ERR_NULL_POINTER if deque reference is \c NULL.
/// \return DS_OK if all operations were successful.
Status deq_enqueue_front(Deque deq, int element);

/// Inserts an element at the rear of the specified deque.
///
/// \param[in] deq The deque where the element is to be inserted.
/// \param[in] element The element to be inserted in the deque.
///
/// \return DS_ERR_ALLOC if node allocation failed.
/// \return DS_ERR_FULL if \c limit is set (greater than 0) and the deque
/// length reached the specified limit.
/// \return DS_ERR_NULL_POINTER if deque reference is \c NULL.
/// \return DS_OK if all operations were successful.
Status deq_enqueue_rear(Deque deq, int element);

/// Removes an element from the front of the specified deque.
///
/// \param[in] deq The deque where the element is to be removed from.
/// \param[out] result The resulting element removed from the deque.
///
/// \return DS_ERR_INVALID_OPERATION if the deque is empty.
/// \return DS_ERR_NULL_POINTER if deque reference is \c NULL.
/// \return DS_OK if all operations were successful.
Status deq_dequeue_front(Deque deq, int *result);

/// Removes an element from the rear of the specified deque.
///
/// \param[in] deq The deque where the element is to be removed from.
/// \param[out] result The resulting element removed from the deque.
///
/// \return DS_ERR_INVALID_OPERATION if the deque is empty.
/// \return DS_ERR_NULL_POINTER if deque reference is \c NULL.
/// \return DS_OK if all operations were successful.
Status deq_dequeue_rear(Deque deq, int *result);

/// Displays a \c Deque in the console.
///
/// \param deq The deque to be displayed in the console.
///
/// \return DS_ERR_NULL_POINTER if the deque reference is \c NULL.
/// \return DS_OK if all operations were successful.
Status deq_display(Deque deq);

/// Displays a \c Deque in the console like an array with its values separated
/// by commas, delimited with brackets.
///
/// \param deq The deque to be displayed in the console.
///
/// \return DS_ERR_NULL_POINTER if the deque reference is \c NULL.
/// \return DS_OK if all operations were successful.
Status deq_display_array(Deque deq);

/// Displays a \c Deque in the console with its values separated by spaces.
///
/// \param deq The deque to be displayed in the console.
///
/// \return DS_ERR_NULL_POINTER if the deque reference is \c NULL.
/// \return DS_OK if all operations were successful.
Status deq_display_raw(Deque deq);

/// Iterates through every node of the deque and frees them from memory along
/// with its data. Then the Deque structure is deallocated and set to \c NULL.
///
/// \param deq The deque to be freed from memory.
///
/// \return DS_ERR_NULL_POINTER if the deque reference is \c NULL.
/// \return DS_OK if all operations were successful.
Status deq_delete(Deque *deq);

/// This function sets the deque to its initial state, erasing all of its data
/// and re-initializing the structure. It is equivalent to calling deq_delete()
/// and then deq_init().
///
/// \param deq The deque to be erased.
///
/// \return DS_ERR_ALLOC if deque allocation failed.
/// \return DS_ERR_NULL_POINTER if the deque reference is \c NULL.
/// \return DS_OK if all operations were successful.
Status deq_erase(Deque *deq);

int deq_peek_front(Deque deq);

int deq_peek_rear(Deque deq);

bool deq_full(Deque deq);

bool deq_empty(Deque deq);

index_t deq_length(Deque deq);

Status deq_limit(Deque deq, index_t limit);

Status deq_copy(Deque deq, Deque *result);

#endif //C_DATASTRUCTURES_LIBRARY_DEQUE_H
