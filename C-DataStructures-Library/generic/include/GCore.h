/**
 * @file GCore.h
 * 
 * @author Leonardo Vencovsky (https://github.com/LeoVen)
 *
 * @date 05/11/2018
 */

#ifndef C_DATASTRUCTURES_LIBRARY_GENERICCORE_H
#define C_DATASTRUCTURES_LIBRARY_GENERICCORE_H

/// A type for a function that displays an element.
typedef void(*display_f)(void*);

/// A type for a function that compares two elements, returning:
/// - [ > 0] when the first element is greater than the second;
/// - [ < 0] when the first element is less than the second;
/// - 0 when both elements are equal.
typedef int(*compare_f)(void*,void*);

/// A type for a function that takes an input (first parameter) and returns a
/// copy of that element.
typedef void*(*copy_f)(void*);

/// A type for a function responsible for freeing an element from memory.
typedef void(*free_f)(void*);

#endif //C_DATASTRUCTURES_LIBRARY_GENERICCORE_H
