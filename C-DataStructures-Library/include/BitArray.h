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

/// \struct BitArray_s
/// \brief An array of bits to represent 0 and 1 values.
struct BitArray_s;

/// \brief A type for a bit array.
///
/// A type for a <code> struct BitArray_s </code> so you don't have to always
/// write the full name of it.
typedef struct BitArray_s BitArray_t;

/// \brief A pointer type for a bit array.
///
/// A pointer type to <code> struct BitArray_s </code>. This typedef is used to
/// avoid having to declare every bit array as a pointer type since they all
/// must be dynamically allocated.
typedef struct BitArray_s *BitArray;

///////////////////////////////////// STRUCTURE INITIALIZATION AND DELETION ///

/// ref bit_new
/// \brief Creates a new bit array.
BitArray_t *
bit_new(void);

/// \ref bit_create
/// \brief Creates a new bit array with a given required amount of bits.
BitArray_t *
bit_create(unsigned_t required_bits);

/// \ref bit_free
/// \brief Frees from memory the specified bit array.
void
bit_free(BitArray_t *bits);

/////////////////////////////////////////////////////////////////// GETTERS ///

/// \ref bit_nwords
/// \brief Returns the internal buffer's size (how many words are being used).
unsigned_t
bit_nwords(BitArray_t *bits);

/// \ref bit_nbits
/// \brief Returns the total amount of bits used by the user.
unsigned_t
bit_nbits(BitArray_t *bits);

/// \ref bit_nbits_real
/// \brief Returns the total amount of bits in the array.
unsigned_t
bit_nbits_real(BitArray_t *bits);

///////////////////////////////////////////////////////// BUFFER OPERATIONS ///

/// \ref bit_resize
/// \brief Resizes the bit array's buffer.
bool
bit_resize(BitArray_t *bits, unsigned_t bit_size);

////////////////////////////////////////////////////////// INPUT AND OUTPUT ///

/// \ref bit_set
/// \brief Sets to true a bit at a given bit index.
bool
bit_set(BitArray_t *bits, unsigned_t bit_index);

/// \ref bit_set_range
/// \brief Sets to true a given range of bits.
bool
bit_set_range(BitArray_t *bits, unsigned_t from_index, unsigned_t to_index);

/// \ref bit_clear
/// \brief Sets to false a bit at a given bit index.
bool
bit_clear(BitArray_t *bits, unsigned_t bit_index);

/// \ref bit_clear_range
/// \brief Sets to false a given range of bits.
bool
bit_clear_range(BitArray_t *bits, unsigned_t from_index, unsigned_t to_index);

/// \ref bit_flip
/// \brief Flips the state of a bit at a given bit index.
bool
bit_flip(BitArray_t *bits, unsigned_t bit_index);

/// \ref bit_flip_range
/// \brief Flips the state of a given range of bits.
bool
bit_flip_range(BitArray_t *bits, unsigned_t from_index, unsigned_t to_index);

/// \ref bit_put
/// \brief Sets the state of a bit at a given bit index.
bool
bit_put(BitArray_t *bits, unsigned_t bit_index, bool state);

/// \ref bit_put_range
/// \brief Sets the state of a given range of bits.
bool
bit_put_range(BitArray_t *bits, unsigned_t from_index, unsigned_t to_index, bool state);

/// \ref bit_fill
/// \brief Sets all bits to 1 in the specified bit array.
bool
bit_fill(BitArray_t *bits);

/// \ref bit_empty
/// \brief Sets all bits to 0 in the specified bit array.
bool
bit_empty(BitArray_t *bits);

/// \ref bit_get
/// \brief Retrieves the state of a bit at a given index.
bool
bit_get(BitArray_t *bits, unsigned_t bit_index);

/////////////////////////////////////////////////////////////////// UTILITY ///

/// \ref bit_cardinality
/// \brief Returns the amount of set bits in the bit array.
unsigned_t
bit_cardinality(BitArray_t *bits);

/// \ref bit_intersects
/// \brief Returns true if any bits are intersecting.
bool
bit_intersects(BitArray_t *bits1, BitArray_t *bits2);

/// \ref bit_copy
/// \brief Creates a copy of a BitArray_s.
BitArray_t *
bit_copy(BitArray_t *bits);

/// \ref bit_to_array
/// \brief Makes a representation of a BitArray_s as an array of booleans.
bool *
bit_to_array(BitArray_t *bits, unsigned_t *size);

/// \ref bit_from_array
/// \brief Creates a new bit array from an existing boolean array
BitArray_t *
bit_from_array(bool *array, unsigned_t arr_size);

///////////////////////////////////////////////////////// SEARCH OPERATIONS ///

/// \ref bit_next_set
/// \brief Returns the index of the nearest set bit after a given index.
unsigned_t
bit_next_set(BitArray_t *bits, unsigned_t bit_index);

/// \ref bit_next_clear
/// \brief Returns the index of the nearest clear bit after a given index.
unsigned_t
bit_next_clear(BitArray_t *bits, unsigned_t bit_index);

/// \ref bit_prev_set
/// \brief Returns the index of the nearest set bit previous to an index.
unsigned_t
bit_prev_set(BitArray_t *bits, unsigned_t bit_index);

/// \ref bit_prev_clear
/// \brief Returns the index of the nearest clear bit previous to an index.
unsigned_t
bit_prev_clear(BitArray_t *bits, unsigned_t bit_index);

/// \ref bit_all_set
/// \brief Returns true if all bits are set in the bit array.
bool
bit_all_set(BitArray_t *bits);

/// \ref bit_all_clear
/// \brief Returns true if all bits are clear in the bit array.
bool
bit_all_clear(BitArray_t *bits);

///////////////////////////////////////////////////////// BINARY OPERATIONS ///

/// \ref bit_NOT
/// \brief Performs a \c NOT operation in all bits in the array.
bool
bit_NOT(BitArray_t *bits);

/// \ref bit_AND
/// \brief Performs an \c AND operation between two bit arrays.
bool
bit_AND(BitArray_t *bits1, BitArray_t *bits2);

/// \ref bit_OR
/// \brief Performs an \c OR operation between two bit arrays.
bool
bit_OR(BitArray_t *bits1, BitArray_t *bits2);

/// \ref bit_XOR
/// \brief Performs an \c XOR operation between two bit arrays.
bool
bit_XOR(BitArray_t *bits1, BitArray_t *bits2);

/// \ref bit_NAND
/// \brief Performs a \c NAND operation between two bit arrays.
bool
bit_NAND(BitArray_t *bits1, BitArray_t *bits2);

/// \ref bit_NOR
/// \brief Performs a \c NOR operation between two bit arrays.
bool
bit_NOR(BitArray_t *bits1, BitArray_t *bits2);

/// \ref bit_NXOR
/// \brief Performs a \c NXOR operation between two bit arrays.
bool
bit_NXOR(BitArray_t *bits1, BitArray_t *bits2);

//////////////////////////////////////////////////////////// SET OPERATIONS ///

/// \ref bit_DIFF
/// \brief Calculates the relative complement of bits1 in bits2.
bool
bit_DIFF(BitArray_t *bits1, BitArray_t *bits2);

/////////////////////////////////////////////////////////////////// DISPLAY ///

/// \ref bit_display
/// \brief Displays each bit individually in the console as 0's and 1's.
void
bit_display(BitArray_t *bits);

///////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////// Iterator ///
///////////////////////////////////////////////////////////////////////////////

/// \todo BitArrayIterator

///////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////// Wrapper ///
///////////////////////////////////////////////////////////////////////////////

/// \todo BitArrayWrapper

#endif //C_DATASTRUCTURES_LIBRARY_BITARRAY_H
