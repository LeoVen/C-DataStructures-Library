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

#ifdef __cplusplus
extern "C" {
#endif

/// A SinglyLinkedList is a linear structure where its elements are not stored
/// in contiguous memory allowing constant insertion and removal at both ends
/// of the list. Insertion and removal at the middle of the list are at most
/// O(n - 1). There are no buffer reallocation nor shifting of elements since
/// the data is 'linked' by nodes and at any time these links can be removed;
/// this makes it trivial to remove elements at the middle without having to
/// shift elements.
///
/// In this implementation, the structure is composed of two pointers, one to
/// the first \c SinglyLinkedNode and another to the last one. This way
/// insertions at both ends are simplified. Also this structure holds a length
/// variable that keeps track of the structure's length, allowing for checking
/// empty lists or position parameters that are higher than the total structure
/// length.
///
/// \b Advantages over \c Array
/// - Dynamic size
/// - Easy insertion/removal
///
/// \b Drawbacks
/// - No random access
/// - Extra memory for a pointer on each element
///
/// \b Functions
///
/// Located in file SinglyLinkedList.c
///
/// \b TODO
/// - Add Link and Unlink functions
struct SinglyLinkedList_s
{
    /// \brief List length.
    ///
    /// List current amount of elements linked between the \c head and \c tail
    /// pointers.
    index_t length;

    /// \brief List length limit.
    ///
    /// If it is set to 0 then the list has no limit to its length. Otherwise
    /// it won't be able to have more elements than the specified value. The
    /// list is always initialized with no restrictions to its length, that is,
    /// \c limit equals 0. The user won't be able to limit the list length if
    /// it already has more elements than the specified limit.
    index_t limit;

    /// \brief Points to the first Node on the list.
    ///
    /// Points to the first Node on the list or \c NULL if the list is empty.
    struct SinglyLinkedNode_s *head;

    /// \brief Points to the last Node on the list.
    ///
    /// Points to the first Node on the list or \c NULL if the list is empty.
    struct SinglyLinkedNode_s *tail;
};

/// Defines a type for <code> struct SinglyLinkedList_s </code>.
///
/// Every list is initialized by \c malloc with \c sizeof(SinglyLinkedList_t).
typedef struct SinglyLinkedList_s SinglyLinkedList_t;

/// Defines a type of pointer to <code> struct SinglyLinkedList_s </code>.
///
/// This typedef is used to avoid having to declare every list as a pointer
/// type since they all must be dynamically allocated.
typedef struct SinglyLinkedList_s *SinglyLinkedList;

/// Initializes a new \c SinglyLinkedList with initial length and limit as 0
/// and its pointer members pointing to \c NULL.
///
/// \param[in,out] sll The list to be initialized.
///
/// \return DS_ERR_ALLOC if list allocation failed.
/// \return DS_OK if all operations were successful.
Status sll_init(SinglyLinkedList *sll);

/// Inserts a new element at the beginning of the list. If the list is empty
/// and this is the first element being added, c\ tail will also be pointing
/// to it; in any case the \c head pointer will be pointing to this newly
/// inserted element.
///
/// \param[in] sll The list where the element is to be inserted.
/// \param[in] element The element to be inserted in the list.
///
/// \return DS_ERR_ALLOC if node allocation failed.
/// \return DS_ERR_FULL if \c limit is set (different than 0) and the list
/// length reached the specified limit.
/// \return DS_ERR_NULL_POINTER if the list reference is \c NULL.
/// \return DS_OK if all operations were successful.
Status sll_insert_head(SinglyLinkedList sll, int element);

/// Inserts a new element at the middle of the list. If the chosen position
/// equals 0 \c sll_insert_head() is called; if the position equals the list
/// length \c sll_insert_tail() is called.
///
/// \param[in] sll The list where the element is to be inserted.
/// \param[in] element The element to be inserted in the list.
/// \param[in] position Where the new element is to be inserted.
///
/// \return DS_ERR_ALLOC if node allocation failed.
/// \return DS_ERR_NEGATIVE_VALUE if \c position parameter is a negative value.
/// \return DS_ERR_FULL if \c limit is set (different than 0) and the list
/// length reached the specified limit.
/// \return DS_ERR_OUT_OF_RANGE if \c position parameter is greater than the
/// list \c length.
/// \return DS_ERR_NULL_POINTER if the list reference is \c NULL.
/// \return DS_OK if all operations were successful.
Status sll_insert_at(SinglyLinkedList sll, int element, index_t position);

/// Inserts a new element at the end of the list. If the list is empty and this
/// is the first element being added, c\ head will also be pointing to it; in
/// in any case the \c tail pointer will be pointing to this newly inserted
/// element.
///
/// \param[in] sll The list where the element is to be inserted.
/// \param[in] element The element to be inserted in the list.
///
/// \return DS_ERR_ALLOC if node allocation failed.
/// \return DS_ERR_FULL if \c limit is set (different than 0) and the list
/// length reached the specified limit.
/// \return DS_ERR_NULL_POINTER if the list reference is \c NULL.
/// \return DS_OK if all operations were successful.
Status sll_insert_tail(SinglyLinkedList sll, int element);

/// Removes and retrieves the first element in the list located at the \c head
/// pointer.
///
/// \param[in] sll The list where the element is to be removed from.
/// \param[out] result The resulting element.
///
/// \return DS_ERR_INVALID_OPERATION if the list is empty.
/// \return DS_ERR_NULL_POINTER if the list reference is \c NULL.
/// \return DS_OK if all operations were successful.
Status sll_remove_head(SinglyLinkedList sll, int *result);

/// Removes an element at the middle of the list. If the chosen position equals
/// 0 \c sll_remove_head() is called; if the position equals the list length
/// minus 1 \c sll_remove_tail() is called.
///
/// \param[in] sll The list where the element is to be removed from.
/// \param[out] result The resulting element.
/// \param[in] position Where the element is to be removed from.
///
/// \return DS_ERR_INVALID_OPERATION if the list is empty.
/// \return DS_ERR_NEGATIVE_VALUE if \c position parameter is a negative value.
/// \return DS_ERR_OUT_OF_RANGE if \c position parameter is greater than or
/// equal to the list \c length.
/// \return DS_ERR_NULL_POINTER if the list reference is \c NULL.
/// \return DS_OK if all operations were successful.
Status sll_remove_at(SinglyLinkedList sll, int *result, index_t position);

/// Removes and retrieves the last element in the list located at the \c tail
/// pointer.
///
/// \param[in] sll The list where the element is to be removed from.
/// \param[out] result The resulting element.
///
/// \return DS_ERR_INVALID_OPERATION if the list is empty.
/// \return DS_ERR_NULL_POINTER if the list reference is \c NULL.
/// \return DS_OK if all operations were successful.
Status sll_remove_tail(SinglyLinkedList sll, int *result);

/// Updates an element at a given position. This function is 0 based, that is,
/// the element 0 is the first element.
///
/// \param[in] sll The list where the element is to be update.
/// \param[in] element The new element value.
/// \param[in] position The position of the element to be updated.
///
/// \return DS_ERR_INVALID_OPERATION if the list is empty.
/// \return DS_ERR_NEGATIVE_VALUE if \c position parameter is a negative value.
/// \return DS_ERR_OUT_OF_RANGE if \c position parameter is greater than or
/// equal to the list \c length.
/// \return DS_ERR_NULL_POINTER if the list reference is \c NULL.
/// \return DS_OK if all operations were successful.
Status sll_update(SinglyLinkedList sll, int element, index_t position);

/// Retrieves an element at a given position without removing it. This function
/// simulates an index access like in arrays.
///
/// \param[in] sll The list to retrieve the element from.
/// \param[out] result The resulting element.
/// \param[in] position The position of the element to be retrieved.
///
/// \return DS_ERR_INVALID_OPERATION if the list is empty.
/// \return DS_ERR_NEGATIVE_VALUE if \c position parameter is a negative value.
/// \return DS_ERR_OUT_OF_RANGE if \c position parameter is greater than or
/// equal to the list \c length.
/// \return DS_ERR_NULL_POINTER if the list reference is \c NULL.
/// \return DS_OK if all operations were successful.
Status sll_get(SinglyLinkedList sll, int *result, index_t position);

/// Displays a \c SinglyLinkedList in the console.
///
/// \param sll The list to be displayed in the console.
///
/// \return DS_ERR_NULL_POINTER if the list reference is \c NULL.
/// \return DS_OK if all operations were successful.
Status sll_display(SinglyLinkedList sll);

/// Displays a \c SinglyLinkedList in the console like an array with its
/// elements separated by commas, delimited with brackets.
///
/// \param sll The list to be displayed in the console.
///
/// \return DS_ERR_NULL_POINTER if the list reference is \c NULL.
/// \return DS_OK if all operations were successful.
Status sll_display_array(SinglyLinkedList sll);

/// Displays a \c SinglyLinkedList in the console with its values separated by
/// spaces.
///
/// \param sll The list to be displayed in the console.
///
/// \return DS_ERR_NULL_POINTER if the list reference is \c NULL.
/// \return DS_OK if all operations were successful.
Status sll_display_raw(SinglyLinkedList sll);

/// Iterates through every node of the list and frees them from memory along
/// with its data. Then the SinglyLinkedList structure is deallocated and set
/// to \c NULL.
///
/// \param sll The list to be freed from memory.
///
/// \return DS_ERR_NULL_POINTER if the list reference is \c NULL.
/// \return DS_OK if all operations were successful.
Status sll_delete(SinglyLinkedList *sll);

/// This function sets the list to its initial state, erasing all of its data
/// and re-initializing the structure. It is equivalent to calling sll_delete()
/// and then sll_init().
///
/// \param sll The list to be erased.
///
/// \return DS_ERR_ALLOC if list allocation failed.
/// \return DS_ERR_NULL_POINTER if the list reference is \c NULL.
/// \return DS_OK if all operations were successful.
Status sll_erase(SinglyLinkedList *sll);

bool sll_contains(SinglyLinkedList sll, int key);

bool sll_full(SinglyLinkedList sll);

bool sll_empty(SinglyLinkedList sll);

index_t sll_length(SinglyLinkedList sll);

Status sll_limit(SinglyLinkedList sll, index_t limit);

int sll_max(SinglyLinkedList sll);

int sll_min(SinglyLinkedList sll);

index_t sll_index_first(SinglyLinkedList sll, int key);

index_t sll_index_last(SinglyLinkedList sll, int key);

Status sll_link(SinglyLinkedList sll1, SinglyLinkedList sll2);

Status sll_link_at(SinglyLinkedList sll1, SinglyLinkedList sll2, index_t position);

Status sll_unlink(SinglyLinkedList sll, SinglyLinkedList result, index_t position);

//Status sll_unlink_at(SinglyLinkedList sll, SinglyLinkedList result, index_t position1, index_t position2);

Status sll_copy(SinglyLinkedList sll, SinglyLinkedList *result);

Status sll_reverse(SinglyLinkedList sll);

#ifdef __cplusplus
}
#endif

#endif //C_DATASTRUCTURES_LIBRARY_SINGLYLINKEDLIST_H
