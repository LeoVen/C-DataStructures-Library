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

#ifdef __cplusplus
extern "C" {
#endif

// A singly-linked list implementation of a stack. See the source file for the
// full documentation.
struct Stack_s;

/// \brief A type for a FIFO singly-linked list.
///
/// A type for a <code> struct Stack_s </code> so you don't have to always
/// write the full name of it.
typedef struct Stack_s Stack_t;

/// \brief A pointer type for a FIFO singly-linked list.
///
/// Useful for not having to declare every variable as pointer type. This
/// typedef does that for you.
typedef struct Stack_s *Stack;

/// \brief Comparator function type.
///
/// A type for a function that compares two elements, returning:
/// - [ > 0] when the first element is greater than the second;
/// - [ < 0] when the first element is less than the second;
/// - 0 when both elements are equal.
typedef int(*stk_compare_f)(void *, void *);

/// \brief A Copy function type.
///
/// A type for a function that takes an input (first parameter) and returns an
/// exact copy of that element.
typedef void *(*stk_copy_f)(void *);

/// \brief Display function type.
///
/// A type for a function that displays an element in the console. Please do
/// not print a newline character.
typedef void(*stk_display_f)(void *);

/// \brief A Free function type.
///
/// A type for a function responsible for completely freeing an element from
/// memory.
typedef void(*stk_free_f)(void *);

///////////////////////////////////// STRUCTURE INITIALIZATION AND DELETION ///

Status stk_init(Stack *stack);

Status stk_create(Stack *stack, stk_compare_f compare_f, stk_copy_f copy_f,
                  stk_display_f display_f, stk_free_f free_f);

Status stk_free(Stack *stack);

Status stk_free_shallow(Stack *stack);

Status stk_erase(Stack *stack);

/////////////////////////////////////////////////////////////////// SETTERS ///

Status stk_set_func_compare(Stack stack, stk_compare_f function);

Status stk_set_func_copy(Stack stack, stk_copy_f function);

Status stk_set_func_display(Stack stack, stk_display_f function);

Status stk_set_func_free(Stack stack, stk_free_f function);

Status stk_set_limit(Stack stack, index_t limit);

/////////////////////////////////////////////////////////////////// GETTERS ///

index_t stk_height(Stack stack);

index_t stk_limit(Stack stk);

////////////////////////////////////////////////////////// INPUT AND OUTPUT ///

Status stk_push(Stack stack, void *element);

void *stk_pop(Stack stack);

Status stk_insert(Stack stack, void *element);

Status stk_remove(Stack stack, void **result);

Status stk_slice(Stack stack);

/////////////////////////////////////////////////////////// STRUCTURE STATE ///

bool stk_full(Stack stk);

bool stk_empty(Stack stk);

/////////////////////////////////////////////////////////////////// UTILITY ///

void *stk_peek(Stack stk);

Status stk_copy(Stack stack, Stack *result);

Status stk_stack(Stack stack1, Stack stack2);

/////////////////////////////////////////////////////////////////// DISPLAY ///

Status stk_display(Stack stack);

Status stk_display_array(Stack stack);

Status stk_display_raw(Stack stack);

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

Status stk_iter_init(StackIterator *iter, Stack target);

// TODO
///////////////////////////////////////////////////////////////// ITERATION ///

/////////////////////////////////////////////////////////// STRUCTURE STATE ///

////////////////////////////////////////////////////////// SETTER AND GETTER ///

////////////////////////////////////////////////////////// INPUT AND OUTPUT ///

/////////////////////////////////////////////////////////////////// UTILITY ///

#ifdef __cplusplus
}
#endif

#endif //C_DATASTRUCTURES_LIBRARY_STACK_H
