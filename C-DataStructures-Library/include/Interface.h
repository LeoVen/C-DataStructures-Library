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
///
/// This function is also used when comparing the priority of two elements. The
/// rules are the following:
///
/// - <code>[ > 0 ]</code> if first element has a higher priority than the
/// second element;
/// - <code>[ < 0 ]</code> if second element has a higher priority than the
/// first element;
/// - <code>[ 0 ]</code> if elements have the same priority.
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

/// \brief An interface used by all data structures that stores functions for
/// comparing, copying, displaying and freeing keys or values.
///
/// \todo Description.
///
/// \c Functions
/// Located in file Interface.c
struct Interface_s
{
    compare_f compare;

    copy_f copy;

    display_f display;

    free_f free;
};

typedef struct Interface_s Interface_t;

typedef struct Interface_s *Interface;

/// \ref interface_new
/// \brief Creates a new interface.
Interface_t *
interface_new(compare_f compare, copy_f copy, display_f display, free_f free);

/// \ref interface_config
/// \brief Changes the configuration of an interface.
void
interface_config(Interface_t *interface,
        compare_f compare, copy_f copy, display_f display, free_f free);

/// \ref interface_free
/// \brief Frees from memory an Interface_s.
void
interface_free(Interface_t *interface);

#ifdef __cplusplus
}
#endif

#endif //C_DATASTRUCTURES_LIBRARY_INTERFACE_H
