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

/// \brief A type for a FILO singly-linked list.
///
/// A type for a <code> struct Stack_s </code> so you don't have to always
/// write the full name of it.
typedef struct Stack_s Stack_t;

/// \brief A pointer type for a FILO singly-linked list.
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

Status stk_set_limit(Stack stack, integer_t limit);

/////////////////////////////////////////////////////////////////// GETTERS ///

integer_t stk_height(Stack stack);

integer_t stk_limit(Stack stack);

////////////////////////////////////////////////////////// INPUT AND OUTPUT ///

Status stk_push(Stack stack, void *element);

Status stk_insert(Stack stack, void *element);

void *stk_pop(Stack stack);

Status stk_remove(Stack stack, void **result);

Status stk_slice(Stack stack);

/////////////////////////////////////////////////////////// STRUCTURE STATE ///

bool stk_full(Stack stack);

bool stk_empty(Stack stack);

/////////////////////////////////////////////////////////////////// UTILITY ///

void *stk_peek(Stack stack);

bool stk_contains(Stack stack, void *key);

Status stk_copy(Stack stack, Stack *result);

Status stk_stack(Stack stack1, Stack stack2);

Status stk_to_array(Stack stack,  void ***result, integer_t *length);

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

Status stk_iter_retarget(StackIterator *iter, Stack target);

Status stk_iter_free(StackIterator *iter);

///////////////////////////////////////////////////////////////// ITERATION ///

Status stk_iter_next(StackIterator iter);

Status stk_iter_to_top(StackIterator iter);

/////////////////////////////////////////////////////////// STRUCTURE STATE ///

bool stk_iter_has_next(StackIterator iter);

////////////////////////////////////////////////////////// SETTER AND GETTER ///

Status stk_iter_get(StackIterator iter, void **result);

Status stk_iter_set(StackIterator iter, void *element);

////////////////////////////////////////////////////////// INPUT AND OUTPUT ///

Status stk_iter_insert(StackIterator iter, void *element);

Status stk_iter_remove(StackIterator iter, void **result);

/////////////////////////////////////////////////////////////////// UTILITY ///

void *stk_iter_peek_next(StackIterator iter);

void *stk_iter_peek(StackIterator iter);

#ifdef __cplusplus
}
#endif

#endif //C_DATASTRUCTURES_LIBRARY_STACK_H
