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

typedef struct CircularLinkedList_s
{
    size_t length;                     /*!< List length */
    struct CircularLinkedNode_s *curr; /*!< Pointer to current node. Functions operate relative to this node */
} CircularLinkedList_t, *CircularLinkedList;

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
