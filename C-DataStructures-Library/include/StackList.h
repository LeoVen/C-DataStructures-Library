/**
 * @file StackList.h
 * 
 * @author Leonardo Vencovsky (https://github.com/LeoVen)
 *
 * @date 26/09/2018
 */

#ifndef C_DATASTRUCTURES_LIBRARY_STACKLIST_H
#define C_DATASTRUCTURES_LIBRARY_STACKLIST_H

#include "Core.h"
#include "Interface.h"

#ifdef __cplusplus
extern "C" {
#endif

/// \struct StackList_s
/// \brief A linked list implementation of a generic stack.
struct StackList_s;

/// \ref StackList_t
/// \brief A type for a singly-linked list implementation of a stack.
///
/// A type for a <code> struct StackList_s </code> so you don't have to always
/// write the full name of it.
typedef struct StackList_s StackList_t;

/// \ref StackList
/// \brief A pointer type for a singly-linked list implementation of a stack.
///
/// A pointer type to <code> struct StackList_s </code>. This typedef is used
/// to avoid having to declare every stack as a pointer type since they all
/// must be dynamically allocated.
typedef struct StackList_s *StackList;

/// \ref stl_size
/// \brief The size of a StackList_s in bytes.
extern const unsigned_t stl_size;

///////////////////////////////////// STRUCTURE INITIALIZATION AND DELETION ///

/// \ref stl_new
/// \brief Initializes a new StackList_s allocated on the heap.
StackList_t *
stl_new(Interface_t *interface);

/// \ref stl_init
/// \brief Initializes a new StackList_s allocated on the stack.
bool
stl_init(StackList_t *stack, Interface_t *interface);

/// \ref stl_free
/// \brief Frees from memory a StackList_s and all its elements.
void
stl_free(StackList_t *stack);

/// \ref stl_free_shallow
/// \brief Frees from memory a StackList_s without freeing its elements.
void
stl_free_shallow(StackList_t *stack);

/// \ref stl_erase
/// \brief Resets the StackList_s freeing all its elements.
void
stl_erase(StackList_t *stack);

/// \ref stl_erase_shallow
/// \brief Resets the StackList_s without freeing its elements.
void
stl_erase_shallow(StackList_t *stack);

//////////////////////////////////////////////////////////// CONFIGURATIONS ///

/// \ref stl_config
/// \brief Sets a new interface for a target stack.
void
stl_config(StackList_t *stack, Interface_t *new_interface);

/////////////////////////////////////////////////////////////////// GETTERS ///

/// \ref stl_count
/// \brief Returns the amount of elements in the specified stack.
integer_t
stl_count(StackList_t *stack);

/// \ref stl_limit
/// \brief Returns the current stack limit.
integer_t
stl_limit(StackList_t *stack);

/////////////////////////////////////////////////////////////////// SETTERS ///

/// \ref stl_set_limit
/// \brief Sets a limit to the amount of elements to the specified stack.
bool
stl_set_limit(StackList_t *stack, integer_t limit);

////////////////////////////////////////////////////////// INPUT AND OUTPUT ///

/// \ref stl_push
/// \brief Inserts an element to the specified StackList_s.
bool
stl_push(StackList_t *stack, void *element);

/// \ref stl_pop
/// \brief Removes and retrieves the top element in the specified stack.
bool
stl_pop(StackList_t *stack, void **result);

/// \ref stl_peek
/// \brief Returns the top element in the specified stack.
void *
stl_peek(StackList_t *stack);

/////////////////////////////////////////////////////////// STRUCTURE STATE ///

/// \ref stl_empty
/// \brief Returns true if the stack is full, false otherwise.
bool
stl_empty(StackList_t *stack);

/// \ref stl_full
/// \brief Returns true if the stack is empty, false otherwise.
bool
stl_full(StackList_t *stack);

/// \ref stl_fits
/// \brief Returns true if a given size will fit in the stack, assuming it has
/// a limit set.
bool
stl_fits(StackList_t *stack, unsigned_t size);

/////////////////////////////////////////////////////////////////// UTILITY ///

/// \ref stl_contains
/// \brief Returns true if an elements is present in the specified stack.
bool
stl_contains(StackList_t *stack, void *key);

/// \ref stl_copy
/// \brief Returns a copy of the specified stack.
StackList_t *
stl_copy(StackList_t *stack);

/// \ref stl_copy_shallow
/// \brief Creates a shallow copy of the specified stack.
StackList_t *
stl_copy_shallow(StackList_t *stack);

/// \ref stl_compare
/// \brief Compares two stacks returning an int according to \ref compare_f.
int
stl_compare(StackList_t *stack1, StackList_t *stack2);

/// \ref stl_stack
/// \brief Stack stack2 on top of stack1.
bool
stl_stack(StackList_t *stack1, StackList_t *stack2);

/// \ref stl_to_array
/// \brief Makes a copy of the stack as a C array.
void **
stl_to_array(StackList_t *stack, integer_t *length);

/////////////////////////////////////////////////////////////////// DISPLAY ///

/// \ref stl_display
/// \brief Displays a StackList_s in the console.
void
stl_display(StackList_t *stack, int display_mode);

///////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////// Iterator ///
///////////////////////////////////////////////////////////////////////////////

/// \struct StackListIterator_s.
/// \brief A StackList_s iterator.
struct StackListIterator_s;

/// \brief A type for a stack iterator.
///
/// A type for a <code> struct StackListIterator_s </code>.
typedef struct StackListIterator_s StackListIterator_t;

/// \brief A pointer type for a stack iterator.
///
/// A pointer type for a <code> struct StackListIterator_s </code>.
typedef struct StackListIterator_s *StackListIterator;

///////////////////////////////////// STRUCTURE INITIALIZATION AND DELETION ///

/// \ref stl_iter_new
/// \brief Creates a new stack iterator given a target stack.
StackListIterator_t *
stl_iter_new(StackList_t *target);

/// \ref stl_iter_retarget
/// \brief Retargets an existing iterator.
void
stl_iter_retarget(StackListIterator_t *iter, StackList_t *target);

/// \ref stl_iter_free
/// \brief Frees from memory an existing iterator.
void
stl_iter_free(StackListIterator_t *iter);

///////////////////////////////////////////////////////////////// ITERATION ///

/// \ref stl_iter_next
/// \brief Iterates to the next element if available.
bool
stl_iter_next(StackListIterator_t *iter);

/// \ref stl_iter_to_top
/// \brief Iterates to the top element in the stack.
bool
stl_iter_to_top(StackListIterator_t *iter);

/////////////////////////////////////////////////////////// STRUCTURE STATE ///

/// \ref stl_iter_has_next
/// \brief Returns true if there is another element next in the iteration.
bool
stl_iter_has_next(StackListIterator_t *iter);

////////////////////////////////////////////////////////// INPUT AND OUTPUT ///

/// \ref stl_iter_get
/// \brief Gets the element pointed by the iterator.
bool
stl_iter_get(StackListIterator_t *iter, void **result);

/// \ref stl_iter_set
/// \brief Sets the element pointed by the iterator to a new element.
bool
stl_iter_set(StackListIterator_t *iter, void *element);

/////////////////////////////////////////////////////////////////// UTILITY ///

/// \ref stl_iter_peek_next
/// \brief Returns the next element in the iteration if available.
void *
stl_iter_peek_next(StackListIterator_t *iter);

/// \ref stl_iter_peek
/// \brief Returns the current element in the iteration if available.
void *
stl_iter_peek(StackListIterator_t *iter);

#define STL_FOR_EACH(target, body)                         \
    do {                                                   \
        StackListIterator_t *iter_ = stl_iter_new(target); \
        while (stl_iter_has_next(iter_)) {                 \
            void *var = stl_iter_peek(iter_);              \
            body;                                          \
            stl_iter_next(iter_);                          \
        }                                                  \
        stl_iter_free(iter_);                              \
    } while (0);                                           \

#define STL_DECL(name)                                      \
    char name##_storage__[stl_size];                        \
    StackList_t *name = (StackList_t*)&name##_storage__[0]; \

///////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////// Wrapper ///
///////////////////////////////////////////////////////////////////////////////

/// \todo StackListWrapper

#ifdef __cplusplus
}
#endif

#endif //C_DATASTRUCTURES_LIBRARY_STACKLIST_H
