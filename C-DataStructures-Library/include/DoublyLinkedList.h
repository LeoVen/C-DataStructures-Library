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

/// A DoublyLinkedList is a linear structure like a SinglyLinkedList where each
/// node have two pointers. One to the previous node and another to the next
/// node. This allows a quicker access to elements; insertion and removal are
/// also improved being at most O(n / 2) since we can be looking for a node
/// either at the start or the end of the list.
///
/// In this implementation, the structure is composed of two pointers, one to
/// the first \c DoublyLinkedNode and another to the last one. This way
/// insertions at both ends are simplified. Also this structure holds a length
/// variable that keeps track of the structure's length, allowing for checking
/// empty lists or position parameters that are higher than the total structure
/// length.
///
/// \b Advantages over \c SinglyLinkedList
/// - Inserting, removing and accessing nodes takes half the time on average
///
/// \b Drawbacks
/// - Extra memory for yet another pointer on each element
///
/// \b Functions
///
/// Located in file DoublyLinkedList.c
struct DoublyLinkedList_s
{
    /// \brief List length.
    ///
    /// List current amount of elements linked between the \c head and \c tail
    /// pointers.
    size_t length;

    /// \brief List length limit.
    ///
    /// If it is set to 0 then the list has no limit to its length. Otherwise
    /// it won't be able to have more elements than the specified value. The
    /// list is always initialized with no restrictions to its length, that is,
    /// \c limit equals 0. The user won't be able to limit the list length if
    /// it already has more elements than the specified limit.
    size_t limit;

    /// \brief Points to the first Node on the list.
    ///
    /// Points to the first Node on the list or \c NULL if the list is empty.
    struct DoublyLinkedNode_s *head;

    /// \brief Points to the last Node on the list.
    ///
    /// Points to the first Node on the list or \c NULL if the list is empty.
    struct DoublyLinkedNode_s *tail;
};

/// Defines a type for <code> struct DoublyLinkedList_s </code>.
///
/// Every list is initialized by \c malloc with \c sizeof(DoublyLinkedList_t).
typedef struct DoublyLinkedList_s DoublyLinkedList_t;

/// Defines a type of pointer to <code> struct DoublyLinkedList_s </code>.
///
/// This typedef is used to avoid having to declare every list as a pointer
/// type since they all must be dynamically allocated.
typedef struct DoublyLinkedList_s *DoublyLinkedList;

/// Initializes a new \c DoublyLinkedList with initial length and limit as 0
/// and its pointer members pointing to \c NULL.
///
/// \param[in,out] dll The list to be initialized.
///
/// \return DS_ERR_ALLOC if list allocation failed.
/// \return DS_OK if all operations were successful.
Status dll_init(DoublyLinkedList *dll);

/// Inserts a new element at the beginning of the list. If the list is empty
/// and this is the first element being added, c\ tail will also be pointing
/// to it; in any case the \c head pointer will be pointing to this newly
/// inserted element.
///
/// \param[in] dll The list where the element is to be inserted.
/// \param[in] element The element to be inserted in the list.
///
/// \return DS_ERR_ALLOC if node allocation failed.
/// \return DS_ERR_FULL if \c limit is set (different than 0) and the list
/// length reached the specified limit.
/// \return DS_ERR_NULL_POINTER if the list reference is \c NULL.
/// \return DS_OK if all operations were successful.
Status dll_insert_head(DoublyLinkedList dll, int element);

/// Inserts a new element at the middle of the list. If the chosen position
/// equals 0 \c dll_insert_head() is called; if the position equals the list
/// length \c dll_insert_tail() is called.
///
/// \param[in] dll The list where the element is to be inserted.
/// \param[in] element The element to be inserted in the list.
/// \param[in] position Where the new element is to be inserted.
///
/// \return DS_ERR_ALLOC if node allocation failed.
/// \return DS_ERR_FULL if \c limit is set (different than 0) and the list
/// length reached the specified limit.
/// \return DS_ERR_INVALID_POSITION if \c position parameter is greater than
/// the list \c length.
/// \return DS_ERR_NULL_POINTER if the list reference is \c NULL.
/// \return DS_OK if all operations were successful.
Status dll_insert_at(DoublyLinkedList dll, int element, size_t position);

/// Inserts a new element at the end of the list. If the list is empty and this
/// is the first element being added, c\ head will also be pointing to it; in
/// in any case the \c tail pointer will be pointing to this newly inserted
/// element.
///
/// \param[in] dll The list where the element is to be inserted.
/// \param[in] element The element to be inserted in the list.
///
/// \return DS_ERR_ALLOC if node allocation failed.
/// \return DS_ERR_FULL if \c limit is set (different than 0) and the list
/// length reached the specified limit.
/// \return DS_ERR_NULL_POINTER if the list reference is \c NULL.
/// \return DS_OK if all operations were successful.
Status dll_insert_tail(DoublyLinkedList dll, int element);

/// Removes and retrieves the first element in the list located at the \c head
/// pointer.
///
/// \param[in] dll The list where the element is to be removed from.
/// \param[out] result The resulting element.
///
/// \return DS_ERR_INVALID_OPERATION if the list is empty.
/// \return DS_ERR_NULL_POINTER if the list reference is \c NULL.
/// \return DS_OK if all operations were successful.
Status dll_remove_head(DoublyLinkedList dll, int *result);

/// Removes an element at the middle of the list. If the chosen position equals
/// 0 \c dll_remove_head() is called; if the position equals the list length -
/// 1 \c dll_remove_tail() is called.
///
/// \param[in] dll The list where the element is to be removed from.
/// \param[out] result The resulting element.
/// \param[in] position Where the element is to be removed from.
///
/// \return DS_ERR_INVALID_OPERATION if the list is empty.
/// \return DS_ERR_INVALID_POSITION if \c position parameter is greater or
/// equal to the list \c length.
/// \return DS_ERR_NULL_POINTER if the list reference is \c NULL.
/// \return DS_OK if all operations were successful.
Status dll_remove_at(DoublyLinkedList dll, int *result, size_t position);

/// Removes and retrieves the last element in the list located at the \c tail
/// pointer.
///
/// \param[in] dll The list where the element is to be removed from.
/// \param[out] result The resulting element.
///
/// \return DS_ERR_INVALID_OPERATION if the list is empty.
/// \return DS_ERR_NULL_POINTER if the list reference is \c NULL.
/// \return DS_OK if all operations were successful.
Status dll_remove_tail(DoublyLinkedList dll, int *result);

/// Updates an element at a given position. This function is 0 based, that is,
/// the element 0 is the first element.
///
/// \param[in] dll The list where the element is to be update.
/// \param[in] element The new element value.
/// \param[in] position The position of the element to be updated.
///
/// \return DS_ERR_INVALID_OPERATION if the list is empty.
/// \return DS_ERR_INVALID_POSITION if \c position parameter is greater or
/// equal to the list \c length.
/// \return DS_ERR_NULL_POINTER if the list reference is \c NULL.
/// \return DS_OK if all operations were successful.
Status dll_update(DoublyLinkedList dll, int element, size_t position);

/// Retrieves an element at a given position without removing it. This function
/// simulates an index access like in arrays. If the position parameter is
/// greater than half the size of the list, the iteration will start at the end
/// of the list. This is important because it allows at maximum O(n / 2) time
/// for accessing, inserting and removing elements at the middle of the list.
///
/// \param[in] dll The list to retrieve the element from.
/// \param[out] result The resulting element.
/// \param[in] position The position of the element to be retrieved.
///
/// \return DS_ERR_INVALID_OPERATION if the list is empty.
/// \return DS_ERR_INVALID_POSITION if \c position parameter is greater or
/// equal to the list \c length.
/// \return DS_ERR_NULL_POINTER if the list reference is \c NULL.
/// \return DS_OK if all operations were successful.
Status dll_get(DoublyLinkedList dll, int *result, size_t position);

/// Displays a \c DoublyLinkedList in the console.
///
/// \param dll The list to be displayed in the console.
///
/// \return DS_ERR_NULL_POINTER if the list reference is \c NULL.
/// \return DS_OK if all operations were successful.
Status dll_display(DoublyLinkedList dll);

/// Displays a \c DoublyLinkedList in the console like an array with its
/// elements separated by commas, delimited with brackets.
///
/// \param dll The list to be displayed in the console.
///
/// \return DS_ERR_NULL_POINTER if the list reference is \c NULL.
/// \return DS_OK if all operations were successful.
Status dll_display_array(DoublyLinkedList dll);

/// Displays a \c DoublyLinkedList in the console with its elements separated
/// by spaces.
///
/// \param dll The list to be displayed in the console.
///
/// \return DS_ERR_NULL_POINTER if the list reference is \c NULL.
/// \return DS_OK if all operations were successful.
Status dll_display_raw(DoublyLinkedList dll);

/// Iterates through every node of the list and frees them from memory along
/// with its data. Then the DoublyLinkedList structure is deallocated and set
/// to \c NULL.
///
/// \param dll The list to be freed from memory.
///
/// \return DS_ERR_NULL_POINTER if the list reference is \c NULL.
/// \return DS_OK if all operations were successful.
Status dll_delete(DoublyLinkedList *dll);

/// This function sets the list to its initial state, erasing all of its data
/// and re-initializing the structure. It is equivalent to calling dll_delete()
/// and then dll_init().
///
/// \param dll The list to be erased.
///
/// \return DS_ERR_ALLOC if list allocation failed.
/// \return DS_ERR_NULL_POINTER if the list reference is \c NULL.
/// \return DS_OK if all operations were successful.
Status dll_erase(DoublyLinkedList *dll);

bool dll_contains(DoublyLinkedList dll, int key);

bool dll_empty(DoublyLinkedList dll);

size_t dll_length(DoublyLinkedList dll);

Status dll_limit(DoublyLinkedList dll, size_t limit);

//Status dll_link(DoublyLinkedList dll1, DoublyLinkedList dll2);

//Status dll_link_at(DoublyLinkedList dll1, DoublyLinkedList dll2, size_t position);

//Status dll_unlink(DoublyLinkedList dll, DoublyLinkedList result, size_t position);

//Status dll_unlink_at(DoublyLinkedList dll, DoublyLinkedList result, size_t position1, size_t position2);

Status dll_copy(DoublyLinkedList dll, DoublyLinkedList *result);

Status dll_reverse(DoublyLinkedList dll);


#endif //C_DATASTRUCTURES_LIBRARY_DOUBLYLINKEDLIST_H
