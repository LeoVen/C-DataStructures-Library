/**
 * @file GQueue.h
 * 
 * @author Leonardo Vencovsky (https://github.com/LeoVen)
 *
 * @date 01/11/2018
 */

#ifndef C_DATASTRUCTURES_LIBRARY_GQUEUE_H
#define C_DATASTRUCTURES_LIBRARY_GQUEUE_H

#include "Core.h"
#include "GCore.h"

struct GQueue_s
{
    /// \brief Current amount of elements in the \c GQueue.
    ///
    /// GQueue current amount of elements linked between the \c front and
    /// \c rear pointers.
    index_t length;

    /// \brief GQueue length limit.
    ///
    /// If it is set to 0 or a negative value then the queue has no limit to
    /// its length. Otherwise it won't be able to have more elements than the
    /// specified value. The queue is always initialized with no restrictions
    /// to its length, that is, \c limit equals 0. The user won't be able to
    /// limit the list length if it already has more elements than the
    /// specified limit.
    index_t limit;

    /// \brief The front of the queue.
    ///
    /// Where elements are removed from. The function \c gque_dequeue() operates
    /// relative to this pointer.
    struct GQueueNode_s *front;

    /// \brief The rear of the queue.
    ///
    /// Where elements are added. The function \c gque_enqueue() operates
    /// relative to this pointer.
    struct GQueueNode_s *rear;

    /// Default function that frees an element from memory.
    free_f d_free;

    /// Default function that makes a copy of an element.
    ///
    /// A function that returns a copy of an element from the queue.
    copy_f d_copy;

    /// Default function that compares one element with another.
    ///
    /// A function that compares one element with another. Returns an int with
    /// the following rules:
    ///
    /// - [ > 0 ] if first element is greater than the second;
    /// - [ < 0 ] if second element is greater than the first;
    /// - [  0  ] if elements are equal.
    compare_f d_compare;

    /// Default function that displays elements to the console.
    display_f d_display;
};

/// Defines a type for <code> struct GQueue_s </code>.
///
/// Every queue is initialized by \c malloc with \c sizeof(GQueue_t).
typedef struct GQueue_s GQueue_t;

/// Defines a type of pointer to <code> struct GQueue_s </code>.
///
/// This typedef is used to avoid having to declare every queue as a pointer
/// type since they all must be dynamically allocated.
typedef struct GQueue_s *GQueue;

/// Initializes a new \c GQueue with initial length and limit as 0 and its
/// pointer members pointing to \c NULL.
///
/// \param[in,out] que The queue to be initialized.
///
/// \return DS_ERR_ALLOC if queue allocation failed.
/// \return DS_OK if all operations were successful.
Status gque_init(GQueue *que);

/// Initializes a new GQueue with its default functions.
///
/// \param[in,out] que The queue to be initialized.
/// \param[in] free_func Default function to free every element.
/// \param[in] copy_func Default function to copy an element.
/// \param[in] compare_func Default function that compares one element with
/// another.
/// \param[in] display_func Default function that displays an element in the
/// console.
///
/// \return DS_ERR_ALLOC if queue allocation failed.
/// \return DS_OK if all operations were successful.
Status gque_create(GQueue *que, free_f free_func, copy_f copy_func, compare_f compare_func, display_f display_func);

/// Inserts an element into the specified queue. The element is added relative
/// to the \c rear pointer.
///
/// \param[in] que The queue where the element is to be inserted.
/// \param[in] element The element to be inserted in the queue.
/// \param[in] flag A user-defined flag that denotes the element type that is
/// being added to the queue.
///
/// \return DS_ERR_ALLOC if node allocation failed.
/// \return DS_ERR_FULL if \c limit is set (different than 0) and the queue
/// length reached the specified limit.
/// \return DS_ERR_NULL_POINTER if queue reference is \c NULL.
/// \return DS_OK if all operations were successful.
Status gque_enqueue(GQueue que, void *element, signed char flag);

/// Removes an element from the specified queue. The element is removed
/// relative to the \c front pointer.
///
/// \param[in] que The queue where the element is to be removed from.
/// \param[out] result The resulting element removed from the queue.
/// \param[out] flag User defined flag that denotes the element type that is
/// being removed from the queue.
///
/// \return DS_ERR_INVALID_OPERATION if the queue is empty.
/// \return DS_ERR_NULL_POINTER if queue reference is \c NULL.
/// \return DS_OK if all operations were successful.
Status gque_dequeue(GQueue que, void **result, signed char *flag);

/// Displays a \c GQueue in the console using its default display function.
///
/// \param que The queue to be displayed in the console.
///
/// \return DS_ERR_NULL_POINTER if the queue reference is \c NULL.
/// \return DS_OK if all operations were successful.
Status gque_display(GQueue que);

/// Displays a \c GQueue in the console using its default display function. It
/// is printed like an array with its elements separated by commas, delimited
/// with brackets.
///
/// \param que The queue to be displayed in the console.
///
/// \return DS_ERR_NULL_POINTER if the queue reference is \c NULL.
/// \return DS_OK if all operations were successful.
Status gque_display_array(GQueue que);

/// Displays a \c GQueue in the console with its values separated by spaces
/// using its default display function.
///
/// \param que The queue to be displayed in the console.
///
/// \return DS_ERR_NULL_POINTER if the queue reference is \c NULL.
/// \return DS_OK if all operations were successful.
Status gque_display_raw(GQueue que);

/// Iterates through every node of the queue and frees them from memory along
/// with its data. Then the GQueue structure is deallocated and set to \c NULL.
///
/// \param que The queue to be freed from memory.
///
/// \return DS_ERR_NULL_POINTER if the queue reference is \c NULL.
/// \return DS_OK if all operations were successful.
Status gque_delete(GQueue *que);

/// This function sets the queue to its initial state, erasing all of its data
/// and re-initializing the structure. It is equivalent to calling gque_delete()
/// and then gque_init().
///
/// \param que The queue to be erased.
///
/// \return DS_ERR_ALLOC if queue allocation failed.
/// \return DS_ERR_NULL_POINTER if the queue reference is \c NULL.
/// \return DS_OK if all operations were successful.
Status gque_erase(GQueue *que);

void* gque_peek_front(GQueue que);

void* gque_peek_rear(GQueue que);

bool gque_complete(GQueue que);

bool gque_full(GQueue que);

bool gque_empty(GQueue que);

index_t gque_length(GQueue que);

Status gque_limit(GQueue que, index_t limit);

Status gque_copy(GQueue que, GQueue *result);

Status gque_copy_shallow(GQueue que, GQueue *result);

#endif //C_DATASTRUCTURES_LIBRARY_GQUEUE_H
