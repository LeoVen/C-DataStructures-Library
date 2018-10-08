/**
 * @file DoublyLinkedList.h
 *
 * @author Leonardo Vencovsky (https://github.com/LeoVen)
 *
 * @date 25/09/2018
 */

#ifndef C_DATASTRUCTURES_LIBRARY_DOUBLYLINKEDLIST_H
#define C_DATASTRUCTURES_LIBRARY_DOUBLYLINKEDLIST_H

#include "Core.h"

typedef struct DoublyLinkedList
{
    size_t length;                 /*!< List length */
    struct DoublyLinkedNode_s *head; /*!< Pointer to the first Node on the list */
    struct DoublyLinkedNode_s *tail; /*!< Pointer to the last Node on the list */
} DoublyLinkedList_t, *DoublyLinkedList;

Status dll_init(DoublyLinkedList *dll);

Status dll_insert_head(DoublyLinkedList dll, int value);

Status dll_insert_at(DoublyLinkedList dll, int value, size_t position);

Status dll_insert_tail(DoublyLinkedList dll, int value);

Status dll_remove_head(DoublyLinkedList dll, int *result);

Status dll_remove_at(DoublyLinkedList dll, int *result, size_t position);

Status dll_remove_tail(DoublyLinkedList dll, int *result);

Status dll_update(DoublyLinkedList dll, int value, size_t position);

Status dll_get(DoublyLinkedList dll, int *result, size_t position);

Status dll_display(DoublyLinkedList dll);

Status dll_display_array(DoublyLinkedList dll);

Status dll_display_raw(DoublyLinkedList dll);

Status dll_delete(DoublyLinkedList *dll);

Status dll_erase(DoublyLinkedList *dll);

bool dll_contains(DoublyLinkedList dll, int key);

size_t dll_length(DoublyLinkedList dll);

bool dll_empty(DoublyLinkedList dll);

//Status dll_link(DoublyLinkedList dll1, DoublyLinkedList dll2);

//Status dll_link_at(DoublyLinkedList dll1, DoublyLinkedList dll2, size_t position);

//Status dll_unlink(DoublyLinkedList dll, DoublyLinkedList result, size_t position);

//Status dll_unlink_at(DoublyLinkedList dll, DoublyLinkedList result, size_t position1, size_t position2);

Status dll_copy(DoublyLinkedList dll, DoublyLinkedList *result);

Status dll_reverse(DoublyLinkedList dll);


#endif //C_DATASTRUCTURES_LIBRARY_DOUBLYLINKEDLIST_H
