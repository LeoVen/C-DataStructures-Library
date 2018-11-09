/**
 * @file Core.h
 *
 * @author Leonardo Vencovsky (https://github.com/LeoVen)
 *
 * @date 25/09/2018
 */

#ifndef C_DATASTRUCTURES_LIBRARY_CORE_H
#define C_DATASTRUCTURES_LIBRARY_CORE_H

#include <assert.h>
#include <math.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef enum Status
{
    DS_OK                        =  0, /**< Returned by a function when all operations were successful */
    DS_ERR_OUT_OF_RANGE          =  1, /**< When an invalid index or position is passed as argument */
    DS_ERR_INVALID_OPERATION     =  2, /**< When an invalid operation is made (e.g. remove element of an empty list ) */
    DS_ERR_NEGATIVE_VALUE        =  3, /**< When an invalid negative parameter is passed */
    DS_ERR_ALLOC                 =  4, /**< When a function fails to allocate memory  */
    DS_ERR_UNEXPECTED_RESULT     =  5, /**< When an unexpected result happens. Contact developers. */
    DS_ERR_ITER                  =  6, /**< When there is an iterator error */
    DS_ERR_NULL_POINTER          =  7, /**< When a @c NULL parameter is passed to a function */
    DS_ERR_FULL                  =  8, /**< When a structure reaches its maximum capacity */
    DS_ERR_INVALID_ARGUMENT      =  9, /**< When an argument passed is invalid for that operation */
    DS_ERR_NOT_FOUND             = 10, /**< When a search fails to find a value */
    DS_ERR_INCOMPLETE_TYPE       = 11  /**< When a struct was not completely initialized */
} Status;

/// Signed value used to represent sizes and indexes. Its negative portion is
/// only used to represent errors when the function doesn't return a Status.
typedef long long int index_t;

/// Prime numbers used for hashing
/// https://planetmath.org/goodhashtableprimes
static const unsigned int hash_primes[] = {
    53, 97, 193, 389, 769, 1543, 3079, 6151, 12289, 24593, 49157, 98317,
    196613, 393241, 786433, 1572869, 3145739, 6291469, 12582917, 25165843,
    50331653, 100663319, 201326611, 402653189, 805306457, 1610612741
};

/// Size of hash_primes
static const unsigned hash_primes_size = 26;

#ifdef __cplusplus
}
#endif

#endif //C_DATASTRUCTURES_LIBRARY_CORE_H
