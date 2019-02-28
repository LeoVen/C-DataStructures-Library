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
#include "Interface.h"

#ifdef __cplusplus
extern "C" {
#endif

/// \struct Stack_s
/// \brief A linked list implementation of a generic stack.
struct Stack_s;

/// \ref Stack_t
/// \brief A type for a singly-linked list implementation of a stack.
///
/// A type for a <code> struct Stack_s </code> so you don't have to always
/// write the full name of it.
typedef struct Stack_s Stack_t;

/// \ref Stack
/// \brief A pointer type for a singly-linked list implementation of a stack.
///
/// A pointer type to <code> struct Stack_s </code>. This typedef is used to
/// avoid having to declare every stack as a pointer type since they all must
/// be dynamically allocated.
typedef struct Stack_s *Stack;

///////////////////////////////////// STRUCTURE INITIALIZATION AND DELETION ///

/// \ref stk_new
/// \brief Initializes a new Stack_s.
Stack_t *
stk_new(Interface_t *interface);

/// \ref stk_free
/// \brief Frees from memory a Stack_s and all its elements.
void
stk_free(Stack_t *stack);

/// \ref stk_free_shallow
/// \brief Frees from memory a Stack_s without freeing its elements.
void
stk_free_shallow(Stack_t *stack);

/// \ref stk_erase
/// \brief Resets the Stack_s freeing all its elements.
void
stk_erase(Stack_t *stack);

/// \ref stk_erase_shallow
/// \brief Resets the Stack_s without freeing its elements.
void
stk_erase_shallow(Stack_t *stack);

//////////////////////////////////////////////////////////// CONFIGURATIONS ///

/// \ref stk_config
/// \brief Sets a new interface for a target stack.
void
stk_config(Stack_t *stack, Interface_t *new_interface);

/////////////////////////////////////////////////////////////////// GETTERS ///

/// \ref stk_count
/// \brief Returns the amount of elements in the specified stack.
integer_t
stk_count(Stack_t *stack);

/// \ref stk_limit
/// \brief Returns the current stack limit.
integer_t
stk_limit(Stack_t *stack);

/////////////////////////////////////////////////////////////////// SETTERS ///

/// \ref stk_set_limit
/// \brief Sets a limit to the amount of elements to the specified stack.
bool
stk_set_limit(Stack_t *stack, integer_t limit);

////////////////////////////////////////////////////////// INPUT AND OUTPUT ///

/// \ref stk_push
/// \brief Inserts an element to the specified Stack_s.
bool
stk_push(Stack_t *stack, void *element);

/// \ref stk_pop
/// \brief Removes and retrieves the top element in the specified stack.
bool
stk_pop(Stack_t *stack, void **result);

/// \ref stk_peek
/// \brief Returns the top element in the specified stack.
void *
stk_peek(Stack_t *stack);

/////////////////////////////////////////////////////////// STRUCTURE STATE ///

/// \ref stk_empty
/// \brief Returns true if the stack is full, false otherwise.
bool
stk_empty(Stack_t *stack);

/// \ref stk_full
/// \brief Returns true if the stack is empty, false otherwise.
bool
stk_full(Stack_t *stack);

/////////////////////////////////////////////////////////////////// UTILITY ///

/// \ref stk_contains
/// \brief Returns true if an elements is present in the specified stack.
bool
stk_contains(Stack_t *stack, void *key);

/// \ref stk_copy
/// \brief Returns a copy of the specified stack.
Stack_t *
stk_copy(Stack_t *stack);

/// \ref stk_copy_shallow
/// \brief Creates a shallow copy of the specified stack.
Stack_t *
stk_copy_shallow(Stack_t *stack);

/// \ref stk_compare
/// \brief Compares two stacks returning an int according to \ref compare_f.
int
stk_compare(Stack_t *stack1, Stack_t *stack2);

/// \ref stk_stack
/// \brief Stacks one stack at the top of the other.
bool
stk_stack(Stack_t *stack1, Stack_t *stack2);

/// \ref stk_to_array
/// \brief Makes a copy of the stack as a C array.
void **
stk_to_array(Stack_t *stack, integer_t *length);

/////////////////////////////////////////////////////////////////// DISPLAY ///

/// \ref stk_display
/// \brief Displays a Stack_s in the console.
void
stk_display(Stack_t *stack, int display_mode);

///////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////// Iterator ///
///////////////////////////////////////////////////////////////////////////////

// A stack iterator. See the source file for the full documentation.
struct StackIterator_s;

/// \brief A type for a stack iterator.
///
/// A type for a <code> struct StackIterator_s </code>.
typedef struct StackIterator_s StackIterator_t;

/// \brief A pointer type for a stack iterator.
///
/// A pointer type for a <code> struct StackIterator_s </code>.
typedef struct StackIterator_s *StackIterator;

///////////////////////////////////// STRUCTURE INITIALIZATION AND DELETION ///

/// \ref stk_iter_new
/// \brief Creates a new stack iterator given a target stack.
StackIterator_t *
stk_iter_new(Stack_t *target);

/// \ref stk_iter_retarget
/// \brief Retargets an existing iterator.
void
stk_iter_retarget(StackIterator_t *iter, Stack_t *target);

/// \ref stk_iter_free
/// \brief Frees from memory an existing iterator.
void
stk_iter_free(StackIterator_t *iter);

///////////////////////////////////////////////////////////////// ITERATION ///

/// \ref stk_iter_next
/// \brief Iterates to the next element if available.
bool
stk_iter_next(StackIterator_t *iter);

/// \ref stk_iter_to_top
/// \brief Iterates to the top element in the stack.
bool
stk_iter_to_top(StackIterator_t *iter);

/////////////////////////////////////////////////////////// STRUCTURE STATE ///

/// \ref stk_iter_has_next
/// \brief Returns true if there is another element next in the iteration.
bool
stk_iter_has_next(StackIterator_t *iter);

////////////////////////////////////////////////////////// INPUT AND OUTPUT ///

/// \ref stk_iter_get
/// \brief Gets the element pointed by the iterator.
bool
stk_iter_get(StackIterator_t *iter, void **result);

/// \ref stk_iter_set
/// \brief Sets the element pointed by the iterator to a new element.
bool
stk_iter_set(StackIterator_t *iter, void *element);

/////////////////////////////////////////////////////////////////// UTILITY ///

/// \ref stk_iter_peek_next
/// \brief Returns the next element in the iteration if available.
void *
stk_iter_peek_next(StackIterator_t *iter);

/// \ref stk_iter_peek
/// \brief Returns the current element in the iteration if available.
void *
stk_iter_peek(StackIterator_t *iter);

///////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////// Wrapper ///
///////////////////////////////////////////////////////////////////////////////

/// \todo StackWrapper

#ifdef __cplusplus
}
#endif

#endif //C_DATASTRUCTURES_LIBRARY_STACK_H
