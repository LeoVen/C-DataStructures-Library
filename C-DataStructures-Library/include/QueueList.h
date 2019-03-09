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

#ifdef __cplusplus
extern "C" {
#endif

/// \struct QueueList_s
/// \brief A linked list implementation of a generic queue.
struct QueueList_s;

/// \brief A type for a singly-linked list implementation of a queue.
///
/// A type for a <code> struct QueueList_s </code> so you don't have to always
/// write the full name of it.
typedef struct QueueList_s QueueList_t;

/// \brief A pointer type for a singly-linked list implementation of a queue.
///
/// A pointer type to <code> struct QueueList_s </code>. This typedef is used
/// to avoid having to declare every queue as a pointer type since they all
/// must be dynamically allocated.
typedef struct QueueList_s *QueueList;

/// \brief Comparator function type.
///
/// A type for a function that compares two elements, returning:
/// - [ > 0] when the first element is greater than the second;
/// - [ < 0] when the first element is less than the second;
/// - 0 when both elements are equal.
typedef int(*qli_compare_f)(void *, void *);

/// \brief A Copy function type.
///
/// A type for a function that takes an input (first parameter) and returns an
/// exact copy of that element.
typedef void *(*qli_copy_f)(void *);

/// \brief Display function type.
///
/// A type for a function that displays an element in the console. Please do
/// not print a newline character.
typedef void(*qli_display_f)(void *);

/// \brief A Free function type.
///
/// A type for a function responsible for completely freeing an element from
/// memory.
typedef void(*qli_free_f)(void *);

///////////////////////////////////// STRUCTURE INITIALIZATION AND DELETION ///

Status qli_init(QueueList *queue);

Status qli_create(QueueList *queue, qli_compare_f compare_f, qli_copy_f copy_f,
                  qli_display_f display_f, qli_free_f free_f);

Status qli_free(QueueList *queue);

Status qli_free_shallow(QueueList *queue);

Status qli_erase(QueueList *queue);

/////////////////////////////////////////////////////////////////// SETTERS ///

Status qli_set_v_compare(QueueList queue, qli_compare_f function);

Status qli_set_v_copy(QueueList queue, qli_copy_f function);

Status qli_set_v_display(QueueList queue, qli_display_f function);

Status qli_set_v_free(QueueList queue, qli_free_f function);

Status qli_set_limit(QueueList queue, integer_t limit);

/////////////////////////////////////////////////////////////////// GETTERS ///

integer_t qli_length(QueueList queue);

integer_t qli_limit(QueueList queue);

////////////////////////////////////////////////////////// INPUT AND OUTPUT ///

Status qli_enqueue(QueueList queue, void *element);

Status qli_dequeue(QueueList queue, void **result);

/////////////////////////////////////////////////////////// STRUCTURE STATE ///

bool qli_full(QueueList queue);

bool qli_empty(QueueList queue);

/////////////////////////////////////////////////////////////////// UTILITY ///

void *qli_peek_front(QueueList queue);

void *qli_peek_rear(QueueList queue);

bool qli_contains(QueueList queue, void *key);

Status qli_copy(QueueList queue, QueueList *result);

Status qli_append(QueueList queue, QueueList queue2);

Status qli_to_array(QueueList queue,  void ***result, integer_t *length);

/////////////////////////////////////////////////////////////////// DISPLAY ///

Status qli_display(QueueList queue);

Status qli_display_array(QueueList queue);

Status qli_display_raw(QueueList queue);

///////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////// Iterator ///
///////////////////////////////////////////////////////////////////////////////

// A queue iterator. See the source file for the full documentation.
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

Status qli_iter_init(QueueListIterator *iter, QueueList target);

Status qli_iter_retarget(QueueListIterator *iter, QueueList target);

Status qli_iter_free(QueueListIterator *iter);

///////////////////////////////////////////////////////////////// ITERATION ///

Status qli_iter_next(QueueListIterator iter);

Status qli_iter_to_front(QueueListIterator iter);

Status qli_iter_to_rear(QueueListIterator iter);

/////////////////////////////////////////////////////////// STRUCTURE STATE ///

bool qli_iter_has_next(QueueListIterator iter);

///////////////////////////////////////////////////////// SETTER AND GETTER ///

Status qli_iter_get(QueueListIterator iter, void **result);

Status qli_iter_set(QueueListIterator iter, void *element);

////////////////////////////////////////////////////////// INPUT AND OUTPUT ///

Status qli_iter_insert(QueueListIterator iter, void *element);

Status qli_iter_remove(QueueListIterator iter, void **result);

/////////////////////////////////////////////////////////////////// UTILITY ///

void *qli_iter_peek_next(QueueListIterator iter);

void *qli_iter_peek(QueueListIterator iter);

#ifdef __cplusplus
}
#endif

#endif //C_DATASTRUCTURES_LIBRARY_QUEUELIST_H
