/**
 * @file Interface.h
 * 
 * @author Leonardo Vencovsky (https://github.com/LeoVen)
 *
 * @date 10/12/2018
 */

#ifndef C_DATASTRUCTURES_LIBRARY_INTERFACE_H
#define C_DATASTRUCTURES_LIBRARY_INTERFACE_H

#include "Core.h"

#ifdef __cplusplus
extern "C" {
#endif

/// \brief Comparator function.
///
/// A function that compares one element with another that returns an int
/// with the following rules:
///
/// - <code>[ > 0 ]</code> if first element is greater than the second;
/// - <code>[ < 0 ]</code> if second element is greater than the first;
/// - <code>[ 0 ]</code> if elements are equal.
typedef int(*compare_f)(const void *, const void *);

/// \brief Copy function.
///
/// A function that returns an exact copy of an element.
typedef void *(*copy_f)(const void *);

/// \brief Display function.
///
/// A function that displays an element in the console. Useful for
/// debugging.
typedef void(*display_f)(const void *);

/// \brief Deallocator function.
///
/// A function that completely frees an element from memory.
typedef void(*free_f)(void *);

/// \brief A function that returns a hash number for a given element.
///
/// A function that returns a hash number for a given element.
typedef unsigned_t(*hash_f)(const void *);

/// \brief A function that compares the priority of two elements.
///
/// This function is used when comparing the priority of two elements. The
/// rules are the following:
///
/// - <code>[ > 0 ]</code> if first element has a higher priority than the
/// second element;
/// - <code>[ < 0 ]</code> if second element has a higher priority than the
/// first element;
/// - <code>[ 0 ]</code> if elements have the same priority.
typedef int(*priority_f)(const void *, const void *);

/// \brief An interface used by all data structures that stores functions for a
/// user defined data type.
///
/// An interface contains functions that manipulate a user defined data type.
/// These functions are necessary for some data structures to operate or used
/// in specific functions. The functions are:
/// - compare - Compares two elements according to the specification of
/// \ref compare_f;
/// - copy - Makes a copy of an element according to the specification of
/// \ref copy_f;
/// - display - Displays an element in the console according to the
/// specification of \ref display_f;
/// - free - Frees an element from allocated memory according to the
/// specifications of \ref free_f;
/// - hash - Creates a hash number from a single element according to the
/// specification of \ref hash_f;
/// - priority - A function that compares the priority of two elements
/// according to the specification of \ref priority_f.
///
/// \par Functions
/// Located in file Interface.c
struct Interface_s
{
    compare_f compare;

    copy_f copy;

    display_f display;

    free_f free;

    hash_f hash;

    priority_f priority;
};

typedef struct Interface_s Interface_t;

typedef struct Interface_s *Interface;

/// \ref interface_new
/// \brief Creates a new interface on the heap.
Interface_t *
interface_new(compare_f compare, copy_f copy, display_f display, free_f free,
              hash_f hash, priority_f priority);

/// \ref interface_init
/// \brief Initializes a new interface on the stack.
void
interface_init(Interface_t *interface,
               compare_f compare, copy_f copy, display_f display,
               free_f free, hash_f hash, priority_f priority);

/// \ref interface_config
/// \brief Changes the configuration of an interface.
void
interface_config(Interface_t *interface,
                 compare_f compare, copy_f copy, display_f display,
                 free_f free, hash_f hash, priority_f priority);

/// \ref interface_free
/// \brief Frees from memory an Interface_s.
void
interface_free(Interface_t *interface);

#ifdef __cplusplus
}
#endif

#endif //C_DATASTRUCTURES_LIBRARY_INTERFACE_H
