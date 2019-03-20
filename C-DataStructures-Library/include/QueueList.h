/**
 * @file QueueList.h
 * 
 * @author Leonardo Vencovsky (https://github.com/LeoVen)
 *
 * @date 26/09/2018
 */

#ifndef C_DATASTRUCTURES_LIBRARY_QUEUELIST_H
#define C_DATASTRUCTURES_LIBRARY_QUEUELIST_H

#include "Core.h"
#include "Interface.h"

#ifdef __cplusplus
extern "C" {
#endif

/// \struct QueueList_s
/// \brief A linked list implementation of a generic queue.
struct QueueList_s;

/// \ref QueueList_t
/// \brief A type for a singly-linked list implementation of a queue.
///
/// A type for a <code> struct QueueList_s </code> so you don't have to always
/// write the full name of it.
typedef struct QueueList_s QueueList_t;

/// \ref QueueList
/// \brief A pointer type for a singly-linked list implementation of a queue.
///
/// A pointer type to <code> struct QueueList_s </code>. This typedef is used
/// to avoid having to declare every queue as a pointer type since they all
/// must be dynamically allocated.
typedef struct QueueList_s *QueueList;

/// \ref qli_size
/// \brief The size of a QueueList_s in bytes.
extern const unsigned_t qli_size;

///////////////////////////////////// STRUCTURE INITIALIZATION AND DELETION ///

/// \ref qli_new
/// \brief Initializes a new QueueList_s on the heap.
QueueList_t *
qli_new(Interface_t *interface);

/// \ref qli_init
/// \brief Initializes a new QueueList_s on the stack.
bool
qli_init(QueueList_t *queue, Interface_t *interface);

/// \ref qli_free
/// \brief Frees from memory a QueueList_s and its elements.
void
qli_free(QueueList_t *queue);

/// \ref qli_free_shallow
/// \brief Frees from memory a QueueList_s leaving its elements intact.
void
qli_free_shallow(QueueList_t *queue);

/// \ref qli_erase
/// \brief Resets the QueueList_s freeing all its elements.
void
qli_erase(QueueList_t *queue);

/// \ref qli_erase_shallow
/// \brief Resets the QueueList_s without freeing its elements.
void
qli_erase_shallow(QueueList_t *queue);

//////////////////////////////////////////////////////////// CONFIGURATIONS ///

/// \ref qli_config
/// \brief Sets a new interface for a target queue.
void
qli_config(QueueList_t *queue, Interface_t *new_interface);

/////////////////////////////////////////////////////////////////// GETTERS ///

/// \ref qli_count
/// \brief Returns the amount of elements in the specified queue.
integer_t
qli_count(QueueList_t *queue);

/// \ref qli_limit
/// \brief Returns the current queue limit.
integer_t
qli_limit(QueueList_t *queue);

/////////////////////////////////////////////////////////////////// SETTERS ///

/// \ref qli_set_limit
/// \brief Sets a limit to the amount of elements to the specified queue.
bool
qli_set_limit(QueueList_t *queue, integer_t limit);

////////////////////////////////////////////////////////// INPUT AND OUTPUT ///

/// \ref qli_enqueue
/// \brief Adds an element to the specified queue.
bool
qli_enqueue(QueueList_t *queue, void *element);

/// \ref qli_dequeue
/// \brief Removes and element from the specified queue.
bool
qli_dequeue(QueueList_t *queue, void **result);

/// \ref qli_peek_front
/// \brief Returns the oldest element in the specified queue.
void *
qli_peek_front(QueueList_t *queue);

/// \ref qli_peek_rear
/// \brief Returns the last item added in the queue.
void *
qli_peek_rear(QueueList_t *queue);

/////////////////////////////////////////////////////////// STRUCTURE STATE ///

/// \ref qli_empty
/// \brief Returns true if the queue is empty, false otherwise.
bool
qli_empty(QueueList_t *queue);

/// \ref qli_full
/// \brief Returns true if the queue is full, false otherwise.
bool
qli_full(QueueList_t *queue);

/// \ref qli_fits
/// \brief Returns true if a given size will fit in the queue, assuming it has
/// a limit set.
bool
qli_fits(QueueList_t *queue, unsigned_t size);

/////////////////////////////////////////////////////////////////// UTILITY ///

/// \ref qli_contains
/// \brief Returns true if an elements is present in the specified queue.
bool
qli_contains(QueueList_t *queue, void *key);

/// \ref qli_copy
/// \brief Returns a copy of the specified queue.
QueueList_t *
qli_copy(QueueList_t *queue);

/// \ref qli_copy_shallow
/// \brief Creates a shallow copy of the specified queue.
QueueList_t *
qli_copy_shallow(QueueList_t *queue);

/// \ref qli_compare
/// \brief Compares two queues returning an int according to \ref compare_f.
int
qli_compare(QueueList_t *queue1, QueueList_t *queue2);

/// \ref qli_append
/// \brief Appends queue2 at the back of queue1.
bool
qli_append(QueueList_t *queue1, QueueList_t *queue2);

/// \ref qli_to_array
/// \brief Makes a copy of the queue as a C array.
void **
qli_to_array(QueueList_t *queue, integer_t *length);

/////////////////////////////////////////////////////////////////// DISPLAY ///

/// \ref qli_display
/// \brief Displays a QueueList_s in the console.
void
qli_display(QueueList_t *queue, int display_mode);

///////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////// Iterator ///
///////////////////////////////////////////////////////////////////////////////

/// \struct QueueListIterator_s.
/// \brief A QueueList_s iterator.
struct QueueListIterator_s;

/// \brief A type for a queue iterator.
///
/// A type for a <code> struct QueueListIterator_s </code>.
typedef struct QueueListIterator_s QueueListIterator_t;

/// \brief A pointer type for a queue iterator.
///
/// A pointer type for a <code> struct QueueListIterator_s </code>.
typedef struct QueueListIterator_s *QueueListIterator;

///////////////////////////////////// STRUCTURE INITIALIZATION AND DELETION ///

/// \ref stl_iter_new
/// \brief Creates a new queue iterator given a target queue.
QueueListIterator_t *
qli_iter_new(QueueList_t *target);

/// \ref qli_iter_init
/// \brief Initializes a new iterator allocated on the stack.
bool
qli_iter_init(QueueListIterator_t *iter, QueueList_t *target);

/// \ref qli_iter_retarget
/// \brief Retargets an existing iterator.
void
qli_iter_retarget(QueueListIterator_t *iter, QueueList_t *target);

/// \ref qli_iter_free
/// \brief Frees from memory an existing iterator.
void
qli_iter_free(QueueListIterator_t *iter);

///////////////////////////////////////////////////////////////// ITERATION ///

/// \ref qli_iter_next
/// \brief Iterates to the next element if available.
bool
qli_iter_next(QueueListIterator_t *iter);

/// \ref qli_iter_to_front
/// \brief Iterates to the front element in the queue.
bool
qli_iter_to_front(QueueListIterator_t *iter);

/// \ref qli_iter_to_rear
/// \brief Iterates to the rear element in the queue.
bool
qli_iter_to_rear(QueueListIterator_t *iter);

/////////////////////////////////////////////////////////// STRUCTURE STATE ///

/// \ref qli_iter_has_next
/// \brief Returns true if there is another element next in the iteration.
bool
qli_iter_has_next(QueueListIterator_t *iter);

////////////////////////////////////////////////////////// INPUT AND OUTPUT ///

/// \ref qli_iter_get
/// \brief Gets the element pointed by the iterator.
bool
qli_iter_get(QueueListIterator_t *iter, void **result);

/// \ref qli_iter_set
/// \brief Sets the element pointed by the iterator to a new element.
bool
qli_iter_set(QueueListIterator_t *iter, void *element);

/////////////////////////////////////////////////////////////////// UTILITY ///

/// \ref qli_iter_peek_next
/// \brief Returns the next element in the iteration if available.
void *
qli_iter_peek_next(QueueListIterator_t *iter);

/// \ref qli_iter_peek
/// \brief Returns the current element in the iteration if available.
void *
qli_iter_peek(QueueListIterator_t *iter);

#define QLI_FOR_EACH(target, body)                         \
    do {                                                   \
        QueueListIterator_t *iter_ = qli_iter_new(target); \
        while (qli_iter_has_next(iter_)) {                 \
            void *var = qli_iter_peek(iter_);              \
            body;                                          \
            qli_iter_next(iter_);                          \
        }                                                  \
        qli_iter_free(iter_);                              \
    } while (0);                                           \

#define QLI_DECL(name)                                      \
    char name##_storage__[qli_size];                        \
    QueueList_t *name = (QueueList_t*)&name##_storage__[0]; \

///////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////// Wrapper ///
///////////////////////////////////////////////////////////////////////////////

/// \todo QueueListWrapper

#ifdef __cplusplus
}
#endif

#endif //C_DATASTRUCTURES_LIBRARY_QUEUELIST_H
