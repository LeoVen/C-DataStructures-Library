/**
 * @file Core.h
 *
 * @author Leonardo Vencovsky (https://github.com/LeoVen)
 *
 * @date 25/09/2018
 */

#ifndef C_DATASTRUCTURES_LIBRARY_CORE_H
#define C_DATASTRUCTURES_LIBRARY_CORE_H

#include <math.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#ifdef __cplusplus
extern "C" {
#endif

/// \brief Status enumerator returned by functions to report errors.
///
/// This enumerator is the core of all data structures. Since C is a low level
/// language and Exceptions are absent, this is a necessary way of reporting
/// errors to the user that can work like exceptions, but not as powerful as
/// they are.
enum Status
{
    /// Returned by a function when all operations are successful.
    DS_OK                         =  0,

    /// When an invalid index or position is passed as argument.
    DS_ERR_OUT_OF_RANGE           =  1,

    /// When an invalid operation is called.
    DS_ERR_INVALID_OPERATION      =  2,

    /// When an invalid negative parameter is passed.
    DS_ERR_NEGATIVE_VALUE         =  3,

    /// When a function fails to allocate memory.
    DS_ERR_ALLOC                  =  4,

    /// When an unexpected result happens. Contact developers.
    DS_ERR_UNEXPECTED_RESULT      =  5,

    /// When an invalid NULL parameter is passed to a function.
    DS_ERR_NULL_POINTER           =  6,

    /// When a structure reaches its maximum capacity.
    DS_ERR_FULL                   =  7,

    /// When an argument passed is invalid for that operation.
    DS_ERR_INVALID_ARGUMENT       =  8,

    /// When a search fails to find a value.
    DS_ERR_NOT_FOUND              =  9,

    /// When a structure is not completely initialized.
    DS_ERR_INCOMPLETE_TYPE        = 10,

    /// When there is an iteration error or an iterator error.
    DS_ERR_ITER                   = 11,

    /// When a structure is modified during iteration.
    DS_ERR_ITER_MODIFICATION      = 12,

    /// When the state of an iterator is invalid.
    DS_ERR_ITER_STATE             = 13,

    /// When a wrapper structure error occurs.
    DS_ERR_WRAPPER                = 14
};

/// Defines a type to an <code> enum Status </code>
typedef enum Status Status;

typedef intmax_t integer_t;

typedef uintmax_t unsigned_t;

/// Prime numbers used for hashing
/// https://planetmath.org/goodhashtableprimes
static const integer_t ds_hash_primes[] = {
    53, 97, 193, 389, 769, 1543, 3079, 6151, 12289, 24593, 49157, 98317,
    196613, 393241, 786433, 1572869, 3145739, 6291469, 12582917, 25165843,
    50331653, 100663319, 201326611, 402653189, 805306457, 1610612741
};

/// Size of ds_hash_primes
static const unsigned ds_hash_primes_size =
        sizeof(ds_hash_primes) / sizeof(ds_hash_primes[0]);

#ifdef __cplusplus
}
#endif

#endif //C_DATASTRUCTURES_LIBRARY_CORE_H
