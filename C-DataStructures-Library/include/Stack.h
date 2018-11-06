/**
 * @file Stack.h
 * 
 * @author Leonardo Vencovsky (https://github.com/LeoVen)
 *
 * @date 26/09/2018
 */

#ifndef C_DATASTRUCTURES_LIBRARY_STACK_H
#define C_DATASTRUCTURES_LIBRARY_STACK_H

#include "Core.h"

/// This is a linked list implementation of a \c Stack with FILO (First-in
/// Last-out) or LIFO (Last-in First-out) operations, so the first item added
/// is the last one to be removed. It is implemented as a SinglyLinkedList but
///  with restricted operations to preserve the FILO (LIFO) order of elements.
/// The function \c stk_push() is equivalent to \c sll_insert_head() and the
/// function \c stk_pop() is equivalent to \c sll_remove_head(). Removal and
/// insertions are O(1). Both push and pop functions operate on what would be
/// the \c head of a SinglyLinkedList.
///
/// \b Advantages over \c StackArray
/// - Indefinitely grows
/// - No need to reallocate buffers
///
/// \b Drawbacks
/// - No random access
/// - More memory usage as in every node there is a pointer to the next node
///
/// \b Functions
/// Located in file Stack.c
struct Stack_s
{
    /// \brief Current amount of elements in the \c Stack.
    ///
    /// Current amount of elements in the \c Stack.
    index_t height;

    /// \brief Stack height limit.
    ///
    /// If it is set to 0 or a negative value then the stack has no limit to
    /// its height. Otherwise it won't be able to have more elements than the
    /// specified value. The stack is always initialized with no restrictions
    /// to its length, that is, \c limit equals 0. The user won't be able to
    /// limit the stack height if it already has more elements than the
    /// specified limit.
    index_t limit;

    /// \brief The element at the top of the \c Stack.
    ///
    /// The element at the top of \c Stack. Push and Pop operate relative to
    /// this pointer. It points to \c NULL if the \c Stack is empty.
    struct StackNode_s *top;
};

/// Defines a type for <code> struct Stack_s </code>.
///
/// Every stack is initialized by \c malloc with \c sizeof(Stack_t).
typedef struct Stack_s Stack_t;

/// Defines a type of pointer to <code> struct Stack_s </code>.
///
/// This typedef is used to avoid having to declare every stack as a pointer
/// type since they all must be dynamically allocated.
typedef struct Stack_s *Stack;

/// Initializes a new \c Stack with initial length 0 and its pointer member
/// pointing to \c NULL.
///
/// \param[in,out] stk The stack to be initialized.
///
/// \return DS_ERR_ALLOC if stack allocation failed.
/// \return DS_OK if all operations were successful.
Status stk_init(Stack *stk);

/// Inserts an element into the specified stack. The element is added relative
/// to the \c rear pointer.
///
/// \param[in] stk The stack where the element is to be inserted.
/// \param[in] element The element to be inserted on the stack.
///
/// \return DS_ERR_ALLOC if node allocation failed.
/// \return DS_ERR_FULL if \c limit is set (more than 0) and the list height
/// reached the specified limit.
/// \return DS_ERR_NULL_POINTER if stack reference is \c NULL.
/// \return DS_OK if all operations were successful.
Status stk_push(Stack stk, int element);

/// Removes an element from the specified stack relative to the top pointer.
///
/// \param[in] stk The stack where the element is to be removed from.
/// \param[out] result The resulting element removed from the stack.
///
/// \return DS_ERR_INVALID_OPERATION if the stack is empty.
/// \return DS_ERR_NULL_POINTER if stack reference is \c NULL.
/// \return DS_OK if all operations were successful.
Status stk_pop(Stack stk, int *result);

/// Alias to stk_push().
///
/// \param[in] stk The stack where the element is to be inserted.
/// \param[in] element The element to be inserted on the stack.
///
/// \return DS_ERR_ALLOC if node allocation failed.
/// \return DS_ERR_FULL if \c limit is set (more than 0) and the list height
/// reached the specified limit.
/// \return DS_ERR_NULL_POINTER if stack reference is \c NULL.
/// \return DS_OK if all operations were successful.
///
/// \see stk_push()
Status stk_insert(Stack stk, int element);

/// Alias to stk_pop().
///
/// \param[in] stk The stack where the element is to be removed from.
/// \param[out] result The resulting element removed from the stack.
///
/// \return DS_ERR_INVALID_OPERATION if the stack is empty.
/// \return DS_ERR_NULL_POINTER if stack reference is \c NULL.
/// \return DS_OK if all operations were successful.
///
/// \see stk_pop()
Status stk_remove(Stack stk, int *result);

/// Displays a \c Stack in the console.
///
/// \param stk The stack to be displayed in the console.
///
/// \return DS_ERR_NULL_POINTER if the stack reference is \c NULL.
/// \return DS_OK if all operations were successful.
Status stk_display(Stack stk);

/// Displays a \c Stack in the console like an array with its values separated
/// by commas, delimited with brackets.
///
/// \param stk The stack to be displayed in the console.
///
/// \return DS_ERR_NULL_POINTER if the stack reference is \c NULL.
/// \return DS_OK if all operations were successful.
Status stk_display_array(Stack stk);

/// Displays a \c Stack in the console with its values separated by spaces.
///
/// \param stk The stack to be displayed in the console.
///
/// \return DS_ERR_NULL_POINTER if the stack reference is \c NULL.
/// \return DS_OK if all operations were successful.
Status stk_display_raw(Stack stk);

/// Iterates through every node of the stack and frees them from memory along
/// with its data. Then the Stack structure is deallocated and set to \c NULL.
///
/// \param stk The stack to be freed from memory.
///
/// \return DS_ERR_NULL_POINTER if the stack reference is \c NULL.
/// \return DS_OK if all operations were successful.
Status stk_delete(Stack *stk);

/// This function sets the stack to its initial state, erasing all of its data
/// and re-initializing the structure. It is equivalent to calling stk_delete()
/// and then stk_init().
///
/// \param stk The stack to be erased.
///
/// \return DS_ERR_ALLOC if stack allocation failed.
/// \return DS_ERR_NULL_POINTER if the stack reference is \c NULL.
/// \return DS_OK if all operations were successful.
Status stk_erase(Stack *stk);

int stk_peek(Stack stk);

bool stk_full(Stack stk);

bool stk_empty(Stack stk);

index_t stk_height(Stack stk);

Status stk_limit(Stack stk, index_t limit);

Status stk_copy(Stack stk, Stack *result);

#endif //C_DATASTRUCTURES_LIBRARY_STACK_H
