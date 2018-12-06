/**
 * @file BitArray.h
 * 
 * @author Leonardo Vencovsky (https://github.com/LeoVen)
 *
 * @date 05/12/2018
 */

#ifndef C_DATASTRUCTURES_LIBRARY_BITARRAY_H
#define C_DATASTRUCTURES_LIBRARY_BITARRAY_H

#include "Core.h"

// An array of bits. See the source file for the full documentation.
struct BitArray_s;

/// \brief A type for a bit array.
///
/// A type for a <code> struct BitArray_s </code> so you don't have to always
/// write the full name of it.
typedef struct BitArray_s BitArray_t;

/// \brief A pointer type for a bit array.
///
/// Useful for not having to declare every variable as pointer type. This
/// typedef does that for you.
typedef struct BitArray_s *BitArray;

///////////////////////////////////// STRUCTURE INITIALIZATION AND DELETION ///

Status bit_init(BitArray *bits);

Status bit_create(BitArray *bits, integer_t required_bits);

Status bit_free(BitArray *bits);

/////////////////////////////////////////////////////////////////// GETTERS ///

integer_t bit_size(BitArray bits);

integer_t bit_nbits(BitArray bits);

////////////////////////////////////////////////////////// INPUT AND OUTPUT ///

Status bit_set(BitArray bits, integer_t bit_index);

Status bit_set_range(BitArray bits, integer_t from_index, integer_t to_index);

Status bit_clear(BitArray bits, integer_t bit_index);

Status bit_clear_range(BitArray bits, integer_t from_index, integer_t to_index);

Status bit_flip(BitArray bits, integer_t bit_index);

Status bit_flip_range(BitArray bits, integer_t from_index, integer_t to_index);

Status bit_put(BitArray bits, bool state, integer_t bit_index);

Status bit_put_range(BitArray bits, bool state, integer_t from_index, integer_t to_index);

Status bit_fill(BitArray bits);

Status bit_empty(BitArray bits);

Status bit_get(BitArray bits, integer_t bit_index, bool *result);

/////////////////////////////////////////////////////////////////// UTILITY ///

integer_t bit_cardinality(BitArray bits);

bool bit_intersects(BitArray bits1, BitArray bits2);

Status bit_copy(BitArray bits, BitArray *result);

Status bit_to_array(BitArray bits, bool **result);

///////////////////////////////////////////////////////// SEARCH OPERATIONS ///

integer_t bit_next_set(BitArray bits, integer_t bit_index);

integer_t bit_next_clear(BitArray bits, integer_t bit_index);

integer_t bit_prev_set(BitArray bits, integer_t bit_index);

integer_t bit_prev_clear(BitArray bits, integer_t bit_index);

bool bit_all_set(BitArray bits);

bool bit_all_clear(BitArray bits);

///////////////////////////////////////////////////////// BINARY OPERATIONS ///

Status bit_NOT(BitArray bits);

Status bit_AND(BitArray bits1, BitArray bits2);

Status bit_OR(BitArray bits1, BitArray bits2);

Status bit_XOR(BitArray bits1, BitArray bits2);

Status bit_NAND(BitArray bits1, BitArray bits2);

Status bit_NOR(BitArray bits1, BitArray bits2);

Status bit_NXOR(BitArray bits1, BitArray bits2);

//////////////////////////////////////////////////////////// SET OPERATIONS ///

Status bit_DIFF(BitArray bits1, BitArray bits2);

/////////////////////////////////////////////////////////////////// DISPLAY ///

Status bit_display(BitArray bits);

Status bit_display_array(BitArray bits);

#endif //C_DATASTRUCTURES_LIBRARY_BITARRAY_H
