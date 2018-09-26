/**
 * @file SinglyLinkedList.h
 *
 * @author Leonardo Vencovsky (https://github.com/LeoVen)
 *
 * @date 25/09/2018
 */

#ifndef C_DATASTRUCTURES_LIBRARY_SINGLYLINKEDLIST_H
#define C_DATASTRUCTURES_LIBRARY_SINGLYLINKEDLIST_H

#include "Core.h"

typedef struct SinglyLinkedList
{
    size_t length;                 /*!< List length */
    struct SinglyLinkedNode *head; /*!< Pointer to the first Node on the list */
    struct SinglyLinkedNode *tail; /*!< Pointer to the last Node on the list */
} SinglyLinkedList;

Status sll_init(SinglyLinkedList **sll);

Status sll_insert_head(SinglyLinkedList *sll, int value);

Status sll_insert_at(SinglyLinkedList *sll, int value, size_t position);

Status sll_insert_tail(SinglyLinkedList *sll, int value);

Status sll_remove_head(SinglyLinkedList *sll, int *result);

Status sll_remove_at(SinglyLinkedList *sll, int *result, size_t position);

Status sll_remove_tail(SinglyLinkedList *sll, int *result);

Status sll_update(SinglyLinkedList *sll, int value, size_t position);

Status sll_get(SinglyLinkedList *sll, int *result, size_t position);

Status sll_display(SinglyLinkedList *sll);

Status sll_display_array(SinglyLinkedList *sll);

Status sll_display_raw(SinglyLinkedList *sll);

Status sll_delete(SinglyLinkedList **sll);

Status sll_erase(SinglyLinkedList **sll);

bool sll_contains(SinglyLinkedList *sll, int key);

bool sll_empty(SinglyLinkedList *sll);

size_t sll_length(SinglyLinkedList *sll);

Status sll_link(SinglyLinkedList *sll1, SinglyLinkedList *sll2);

Status sll_link_at(SinglyLinkedList *sll1, SinglyLinkedList *sll2, size_t position);

Status sll_unlink(SinglyLinkedList *sll, SinglyLinkedList *result, size_t position);

//Status sll_unlink_at(SinglyLinkedList *sll, SinglyLinkedList *result, size_t position1, size_t position2);

Status sll_copy(SinglyLinkedList *sll, SinglyLinkedList **result);

Status sll_reverse(SinglyLinkedList *sll);

#endif //C_DATASTRUCTURES_LIBRARY_SINGLYLINKEDLIST_H
