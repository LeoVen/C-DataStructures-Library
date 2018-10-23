/**
 * @file Core.h
 *
 * @author Leonardo Vencovsky (https://github.com/LeoVen)
 *
 * @date 25/09/2018
 */

#ifndef C_DATASTRUCTURES_LIBRARY_CORE_H
#define C_DATASTRUCTURES_LIBRARY_CORE_H

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>

typedef enum Status
{
    DS_OK                        =  0, /**< Returned by a function when all operations were successful */
    DS_ERR_OUT_OF_RANGE          =  1, /**< When an invalid index or position is passed as argument */
    DS_ERR_INVALID_OPERATION     =  2, /**< When an invalid operation is made (e.g. remove element of an empty list ) */
    DS_ERR_NEGATIVE_VALUE        =  3, /**< When an invalid negative parameter is passed */
    DS_ERR_ALLOC                 =  4, /**< When a function fails to allocate memory  */
    DS_ERR_UNEXPECTED_RESULT     =  5, /**< When an unexpected result happens. Contact developers. */
    DS_ERR_ITER                  =  6, /**< When an iteration reaches an unexpected value */
    DS_ERR_NULL_POINTER          =  7, /**< When a @c NULL parameter is passed to a function */
    DS_ERR_FULL                  =  8, /**< When a structure reaches its maximum capacity */
    DS_ERR_INVALID_ARGUMENT      =  9  /**< When an argument passed is invalid for that operation */
} Status;

/// Signed value used to represent sizes and indexes. Its negative portion is
/// only used to represent errors when the function doesn't return a Status.
typedef long long int index_t;

#endif //C_DATASTRUCTURES_LIBRARY_CORE_H
