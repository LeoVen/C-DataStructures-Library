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
/// TODO Implement as a DoublyLinkedList
struct CircularLinkedList_s
{
    /// \brief List length.
    ///
    /// List current amount of nodes.
    size_t length;

    /// Pointer to current node.
    ///
    /// Pointer to current node. Functions operate relative to this node.
    struct CircularLinkedNode_s *curr;
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

Status cll_init(CircularLinkedList *cli);

Status cll_insert_after(CircularLinkedList cll, int value);

Status cll_insert_before(CircularLinkedList cll, int value);

Status cll_remove_after(CircularLinkedList cll, int *result);

Status cll_remove_current(CircularLinkedList cll, int *result);

Status cll_remove_before(CircularLinkedList cll, int *result);

Status cll_iter_next(CircularLinkedList cll);

Status cll_iter(CircularLinkedList cll, size_t positions);

Status cll_iter_before(CircularLinkedList cll);

Status cll_display(CircularLinkedList cll);

Status cll_display_array(CircularLinkedList cll);

Status cll_display_raw(CircularLinkedList cll);

Status cll_delete(CircularLinkedList *cll);

Status cll_erase(CircularLinkedList *cll);

size_t cll_length(CircularLinkedList cll);

bool cll_contains(CircularLinkedList cll, int key);

bool cll_empty(CircularLinkedList cll);

int cll_max(CircularLinkedList cll);

int cll_min(CircularLinkedList cll);

int cll_current(CircularLinkedList cll);

Status cll_copy(CircularLinkedList cll, CircularLinkedList *result);

#endif //C_DATASTRUCTURES_LIBRARY_CIRCULARLINKEDLIST_H
