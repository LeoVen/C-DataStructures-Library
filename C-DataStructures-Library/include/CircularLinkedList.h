/**
 * @file CircularLinkedList.h
 * 
 * @author Leonardo Vencovsky (https://github.com/LeoVen)
 *
 * @date 09/10/2018
 */

#ifndef C_DATASTRUCTURES_LIBRARY_CIRCULARLINKEDLIST_H
#define C_DATASTRUCTURES_LIBRARY_CIRCULARLINKEDLIST_H

#include "Core.h"

/// A CircularLinkedList is a linked list where all nodes are connected to form
/// a circular structure. There are no \c head or \c tail pointers; the only
/// reference is the current node where all operations work relatively to this
/// pointer. This implementation allows insertions before or after the current
/// node. Elements can be removed from before or after the current node or the
/// current node itself.
///
/// This circular linked list is implemented as a DoublyLinkedList. This allows
/// much faster insertions and removals, where they all practically made in
/// O(1) time.
///
/// \b Advantages over \c DoublyLinkedList
/// - Inserting, removing and accessing nodes takes half the time on average
///
/// \b Drawbacks
/// - Extra memory for yet another pointer on each element
///
/// \b Functions
///
/// Located in file DoublyLinkedList.c
struct CircularLinkedList_s
{
    /// \brief List length.
    ///
    /// List current amount of nodes.
    size_t length;

    /// \brief List length limit.
    ///
    /// If it is set to 0 then the list has no limit to its length. Otherwise
    /// it won't be able to have more elements than the specified value. The
    /// list is always initialized with no restrictions to its length, that is,
    /// \c limit equals 0. The user won't be able to limit the list length if
    /// it already has more elements than the specified limit.
    size_t limit;

    /// Pointer to current node.
    ///
    /// Pointer to current node. Functions operate relative to this node.
    struct CircularLinkedNode_s *cursor;
};

/// Defines a type for <code> struct CircularLinkedList_s </code>.
///
/// Every list is initialized by \c malloc with \c sizeof(CircularLinkedList_t).
typedef struct CircularLinkedList_s CircularLinkedList_t;

/// Defines a type of pointer to <code> struct CircularLinkedList_s </code>.
///
/// This typedef is used to avoid having to declare every list as a pointer
/// type since they all must be dynamically allocated.
typedef struct CircularLinkedList_s *CircularLinkedList;

/// Initializes a new \c CircularLinkedList with initial length and limit as 0
/// and its pointer member pointing to \c NULL.
///
/// \param[in,out] cll The list to be initialized.
///
/// \return DS_ERR_ALLOC if list allocation failed.
/// \return DS_OK if all operations were successful.
Status cll_init(CircularLinkedList *cll);

/// Inserts a new element after the current node pointed by the list. If it is
/// the first element to be inserted, then it becomes the current node pointed
/// by \c curr.
///
/// \param cll The list where the element is to be inserted.
/// \param element The element to be inserted in the list.
///
/// \return DS_ERR_ALLOC if node allocation failed.
/// \return DS_ERR_FULL if \c limit is set (different than 0) and the list
/// length reached the specified limit.
/// \return DS_ERR_NULL_POINTER if the list reference is \c NULL.
/// \return DS_OK if all operations were successful.
Status cll_insert_after(CircularLinkedList cll, int element);

/// Inserts a new element before the current node pointed by the list cursor.
/// If it is the first element to be inserted, then it becomes the current node
/// pointed by \c curr.
///
/// \param cll The list where the element is to be inserted.
/// \param element The element to be inserted in the list.
///
/// \return DS_ERR_ALLOC if node allocation failed.
/// \return DS_ERR_FULL if \c limit is set (different than 0) and the list
/// length reached the specified limit.
/// \return DS_ERR_NULL_POINTER if the list reference is \c NULL.
/// \return DS_OK if all operations were successful.
Status cll_insert_before(CircularLinkedList cll, int element);

/// Removes and retrieves the element located after the current node pointed
/// by the list cursor. If it is the last element, then the cursor's node is
/// retrieved.
///
/// \param[in] cll The list where the element is to be removed from.
/// \param[out] result The resulting element.
///
/// \return DS_ERR_INVALID_OPERATION if the list is empty.
/// \return DS_ERR_NULL_POINTER if the list reference is \c NULL.
/// \return DS_OK if all operations were successful.
Status cll_remove_after(CircularLinkedList cll, int *result);

/// Removes and retrieves the current element pointed by the \c cursor pointer.
/// At this point the \c cursor pointer can either move to the next element or
/// the previous one. Both ways are written but one must remain commented.
///
/// \param[in] cll The list where the element is to be removed from.
/// \param[out] result The resulting element.
///
/// \return DS_ERR_INVALID_OPERATION if the list is empty.
/// \return DS_ERR_NULL_POINTER if the list reference is \c NULL.
/// \return DS_OK if all operations were successful.
Status cll_remove_current(CircularLinkedList cll, int *result);

/// Removes and retrieves the element located before the current node pointed
/// by the list cursor. If it is the last element, then the cursor's node is
/// retrieved.
///
/// \param[in] cll The list where the element is to be removed from.
/// \param[out] result The resulting element.
///
/// \return DS_ERR_INVALID_OPERATION if the list is empty.
/// \return DS_ERR_NULL_POINTER if the list reference is \c NULL.
/// \return DS_OK if all operations were successful.
Status cll_remove_before(CircularLinkedList cll, int *result);

Status cll_iter_next(CircularLinkedList cll, size_t positions);

Status cll_iter_prev(CircularLinkedList cll, size_t positions);

Status cll_display(CircularLinkedList cll);

Status cll_display_array(CircularLinkedList cll);

Status cll_display_raw(CircularLinkedList cll);

Status cll_delete(CircularLinkedList *cll);

Status cll_erase(CircularLinkedList *cll);

bool cll_contains(CircularLinkedList cll, int key);

bool cll_empty(CircularLinkedList cll);

size_t cll_length(CircularLinkedList cll);

Status cll_limit(CircularLinkedList cll, size_t limit);

int cll_max(CircularLinkedList cll);

int cll_min(CircularLinkedList cll);

int cll_peek_next(CircularLinkedList cll);

int cll_peek(CircularLinkedList cll);

int cll_peek_prev(CircularLinkedList cll);

Status cll_copy(CircularLinkedList cll, CircularLinkedList *result);

#endif //C_DATASTRUCTURES_LIBRARY_CIRCULARLINKEDLIST_H
