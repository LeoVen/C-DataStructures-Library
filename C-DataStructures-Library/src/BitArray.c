/**
 * @file BitArray.c
 * 
 * @author Leonardo Vencovsky (https://github.com/LeoVen)
 *
 * @date 05/12/2018
 */

#include "BitArray.h"

/// \brief An array of bits to represent 0 and 1 values.
///
/// A bit array (bit set, bit map, bit string or bit vector) is a compacted
/// array of bits represented by the bits in a word (in this case an int64_t)
/// where you can individually set or clear each bit. It is very useful at
/// implementing a Set of elements where:
/// - Union represented by the OR operator ( bit_OR() )
/// - Intersection represented by the AND operator ( bit_AND() );
/// - Symmetric difference represented by the XOR operator ( bit_XOR() ).
///
/// \todo Make the bit array grow as needed.
struct BitArray_s
{
    /// \brief Buffer of bytes.
    int64_t *buffer;

    /// \brief Buffer size.
    integer_t size;
};

///////////////////////////////////////////////////// NOT EXPOSED FUNCTIONS ///

static integer_t bit_buffer_index(integer_t bit_index);

static bool bit_receive(BitArray bits, integer_t bit_index);

static Status bit_grow(BitArray bits, integer_t words);

static int64_t bit_count(int64_t i);

////////////////////////////////////////////// END OF NOT EXPOSED FUNCTIONS ///

/// \brief Initializes a BitArray_s.
///
/// \param bits
///
/// \return
Status bit_init(BitArray *bits)
{
    *bits = malloc(sizeof(BitArray_t));

    if (!(*bits))
        return DS_ERR_ALLOC;

    (*bits)->buffer = calloc(1, sizeof(int64_t));

    if (!((*bits)->buffer))
    {
        free(*bits);

        *bits = NULL;

        return DS_ERR_ALLOC;
    }

    (*bits)->size = 1;

    return DS_OK;
}

/// \brief Initializes a BitArray_s.
///
/// \param bits
/// \param required_bits
///
/// \return
Status bit_create(BitArray *bits, integer_t required_bits)
{
    *bits = NULL;

    if (required_bits < 0)
        return DS_ERR_NEGATIVE_VALUE;

    if (required_bits == 0)
        return DS_ERR_INVALID_ARGUMENT;

    *bits = malloc(sizeof(BitArray_t));

    if (!(*bits))
        return DS_ERR_ALLOC;

    integer_t buffer_size = bit_buffer_index(required_bits - 1) + 1;

    (*bits)->buffer = calloc(buffer_size, sizeof(int64_t));

    if (!((*bits)->buffer))
    {
        free(*bits);

        *bits = NULL;

        return DS_ERR_ALLOC;
    }

    (*bits)->size = buffer_size;

    return DS_OK;
}

/// \brief Frees from memory a BitArray_s.
///
/// \param bits
///
/// \return
Status bit_free(BitArray *bits)
{
    if (*bits == NULL)
        return DS_ERR_NULL_POINTER;

    free((*bits)->buffer);
    free(*bits);

    *bits = NULL;

    return DS_OK;
}

/// \brief Returns how many words are in the bit array.
///
/// \param bits
///
/// \return
integer_t bit_size(BitArray bits)
{
    return bits->size;
}

/// \brief Returns the total amount of bits in the array.
///
/// \param bits
///
/// \return
integer_t bit_nbits(BitArray bits)
{
    return bits->size * 64;
}

/// \brief Sets to true a bit at a given bit index.
///
/// \param bits
/// \param bit_index
///
/// \return
Status bit_set(BitArray bits, integer_t bit_index)
{
    if (bits == NULL)
        return DS_ERR_NULL_POINTER;

    if (bit_index < 0)
        return DS_ERR_NEGATIVE_VALUE;

    integer_t index = bit_buffer_index(bit_index);

    bits->buffer[index] |= ((int64_t)1 << bit_index);

    return DS_OK;
}

/// \brief Sets to true a given range of bits.
///
/// \param bits
/// \param from_index
/// \param to_index
///
/// \return
Status bit_set_range(BitArray bits, integer_t from_index, integer_t to_index)
{
    if (bits == NULL)
        return DS_ERR_NULL_POINTER;

    if (from_index < 0 || to_index < 0)
        return DS_ERR_NEGATIVE_VALUE;

    if (to_index < from_index)
        return DS_ERR_INVALID_ARGUMENT;

    integer_t start_index = bit_buffer_index(from_index);
    integer_t end_index = bit_buffer_index(to_index);

    // Expand to end_index
    // TODO

    return DS_OK;
}

/// \brief Sets to false a bit at a given bit index.
///
/// \param bits
/// \param bit_index
///
/// \return
Status bit_clear(BitArray bits, integer_t bit_index)
{
    if (bits == NULL)
        return DS_ERR_NULL_POINTER;

    if (bit_index < 0)
        return DS_ERR_NEGATIVE_VALUE;

    integer_t index = bit_buffer_index(bit_index);

    bits->buffer[index] &= ~((int64_t)1 << bit_index);

    return DS_OK;
}

/// \brief Sets to false a given range of bits.
///
/// \param bits
/// \param from_index
/// \param to_index
///
/// \return
Status bit_clear_range(BitArray bits, integer_t from_index, integer_t to_index)
{
    if (bits == NULL)
        return DS_ERR_NULL_POINTER;

    if (from_index < 0 || to_index < 0)
        return DS_ERR_NEGATIVE_VALUE;

    if (to_index < from_index)
        return DS_ERR_INVALID_ARGUMENT;

    // TODO

    return DS_OK;
}

/// \brief Flips the state of a bit at a given bit index.
///
/// \param bits
/// \param bit_index
///
/// \return
Status bit_flip(BitArray bits, integer_t bit_index)
{
    if (bits == NULL)
        return DS_ERR_NULL_POINTER;

    if (bit_index < 0)
        return DS_ERR_NEGATIVE_VALUE;

    integer_t index = bit_buffer_index(bit_index);

    bits->buffer[index] ^= ((int64_t)1 << bit_index);

    return DS_OK;
}

/// \brief Flips the state of a given range of bits.
///
/// \param bits
/// \param from_index
/// \param to_index
///
/// \return
Status bit_flip_range(BitArray bits, integer_t from_index, integer_t to_index)
{
    if (bits == NULL)
        return DS_ERR_NULL_POINTER;

    if (from_index < 0 || to_index < 0)
        return DS_ERR_NEGATIVE_VALUE;

    if (to_index < from_index)
        return DS_ERR_INVALID_ARGUMENT;

    // TODO

    return DS_OK;
}

/// \brief Sets the state of a bit at a given bit index.
///
/// \param bits
/// \param state
/// \param bit_index
///
/// \return
Status bit_put(BitArray bits, bool state, integer_t bit_index)
{
    if (bits == NULL)
        return DS_ERR_NULL_POINTER;

    if (bit_index < 0)
        return DS_ERR_NEGATIVE_VALUE;

    integer_t index = bit_buffer_index(bit_index);

    if (state)
    {
        bits->buffer[index] |= ((int64_t)1 << bit_index);
    }
    else
    {
        bits->buffer[index] &= ~((int64_t)1 << bit_index);
    }

    return DS_OK;
}

/// \brief Sets the state of a given range of bits.
///
/// \param bits
/// \param state
/// \param from_index
/// \param to_index
///
/// \return
Status bit_put_range(BitArray bits, bool state, integer_t from_index, integer_t to_index)
{
    if (bits == NULL)
        return DS_ERR_NULL_POINTER;

    if (from_index < 0 || to_index < 0)
        return DS_ERR_NEGATIVE_VALUE;

    if (to_index < from_index)
        return DS_ERR_INVALID_ARGUMENT;

    // TODO

    return DS_OK;
}

/// \brief Sets all bits to 1.
///
/// \param bits
///
/// \return
Status bit_fill(BitArray bits)
{
    if (bits == NULL)
        return DS_ERR_NULL_POINTER;

    for (integer_t i = 0; i < bits->size; i++)
    {
        bits->buffer[i] = ~0;
    }

    return DS_OK;
}

/// \brief Sets all bits to 0.
///
/// \param bits
///
/// \return
Status bit_empty(BitArray bits)
{
    if (bits == NULL)
        return DS_ERR_NULL_POINTER;

    for (integer_t i = 0; i < bits->size; i++)
    {
        bits->buffer[i] = 0;
    }

    return DS_OK;
}

/// \brief Retrieves the state of a bit at a given index.
///
/// \param bits
/// \param bit_index
/// \param result
///
/// \return
Status bit_get(BitArray bits, integer_t bit_index, bool *result)
{
    *result = false;

    if (bits == NULL)
        return DS_ERR_NULL_POINTER;

    if (bit_index < 0)
        return DS_ERR_NEGATIVE_VALUE;

    integer_t index = bit_buffer_index(bit_index);

    int64_t value = bits->buffer[index] & ((int64_t)1 << bit_index);

    *result = (value) ? true : false;

    return DS_OK;
}

/// \brief Returns the amount of set bits in the bit array.
///
/// \param bits
///
/// \return
integer_t bit_cardinality(BitArray bits)
{
    if (bits == NULL)
        return -1;

    integer_t sum = 0;

    for (integer_t i = 0; i < bits->size; i++)
        sum += bit_count(bits->buffer[i]);

    return sum;
}

/// \brief Returns true if any bits are intersecting.
///
/// Returns true if the specified BitArray 1 has any bits set to true that are
/// also set to true in the BitArray 2.
///
/// \param bits1
/// \param bits2
///
/// \return
bool bit_intersects(BitArray bits1, BitArray bits2)
{
    integer_t size = bits1->size < bits2->size ? bits1->size : bits2->size;

    for (integer_t i = 0; i < size; i++)
    {
        if (bits1->buffer[i] & bits2->buffer[i] != 0)
            return true;
    }

    return false;
}

/// \brief Creates a copy of a BitArray_s.
///
/// \param bits
/// \param result
///
/// \return
Status bit_copy(BitArray bits, BitArray *result)
{
    *result = NULL;

    if (bits == NULL)
        return DS_ERR_NULL_POINTER;

    Status st = bit_create(result, bits->size * 64);

    if (st != DS_OK)
        return st;

    for (integer_t i = 0; i < bits->size; i++)
    {
        (*result)->buffer[i] = bits->buffer[i];
    }

    return DS_OK;
}

/// \brief Makes a representation of a BitArray_s as an array of booleans.
///
/// \param bits
/// \param result
///
/// \return
Status bit_to_array(BitArray bits, bool **result)
{
    if (bits == NULL)
        return DS_ERR_NULL_POINTER;

    integer_t nbits = bit_nbits(bits);

    *result = malloc(sizeof(bool) * nbits);

    if (!(*result))
        return DS_ERR_ALLOC;

    for (integer_t i = 0; i < nbits; i++)
    {
        (*result)[i] = bit_receive(bits, i);
    }

    return DS_OK;
}

/// \brief Returns the index of the nearest set bit after a given index.
///
/// Returns the index of the nearest bit that is set to true that occurs on or
/// after the specified starting index. If no such bit exists, if the bit array
/// reference is \c NULL, or if a negative index is given as the starting
/// index, then -1 is returned.
///
/// \param bits
/// \param bit_index
///
/// \return
integer_t bit_next_set(BitArray bits, integer_t bit_index)
{
    // TODO
    return -1;
}

/// \brief Returns the index of the nearest clear bit after a given index.
///
/// Returns the index of the nearest bit that is set to false that occurs on or
/// after the specified starting index. If no such bit exists, if the bit array
/// reference is \c NULL, or if a negative index is given as the starting
/// index, then -1 is returned.
///
/// \param bits
/// \param bit_index
///
/// \return
integer_t bit_next_clear(BitArray bits, integer_t bit_index)
{
    // TODO
    return -1;
}

/// \brief Returns the index of the nearest set bit previous to an index.
///
/// Returns the index of the nearest bit that is set to true that occurs on or
/// before the specified starting index. If no such bit exists, if the bit
/// array reference is \c NULL, or if a negative index is given as the starting
/// index, then -1 is returned.
///
/// \param bits
/// \param bit_index
///
/// \return
integer_t bit_prev_set(BitArray bits, integer_t bit_index)
{
    // TODO
    return -1;
}

/// \brief Returns the index of the nearest clear bit previous to an index.
///
/// Returns the index of the nearest bit that is set to false that occurs on or
/// before the specified starting index. If no such bit exists, if the bit
/// array reference is \c NULL, or if a negative index is given as the starting
/// index, then -1 is returned.
///
/// \param bits
/// \param bit_index
///
/// \return
integer_t bit_prev_clear(BitArray bits, integer_t bit_index)
{
    // TODO
    return -1;
}

/// \brief Returns true if all bits are set in the bit array.
///
/// \param bits
///
/// \return
bool bit_all_set(BitArray bits)
{
    for (integer_t i = 0; i < bits->size; i++)
    {
        if (bits->buffer[i] != (~0))
            return false;
    }

    return true;
}

/// \brief Returns true if all bits are clear in the bit array.
///
/// \param bits
///
/// \return
bool bit_all_clear(BitArray bits)
{
    for (integer_t i = 0; i < bits->size; i++)
    {
        if (bits->buffer[i] != 0)
            return false;
    }

    return true;
}

/// \brief Performs a \c NOT operation in all bits in the array.
///
/// \param bits
///
/// \return
Status bit_NOT(BitArray bits)
{
    if (bits == NULL)
        return DS_ERR_NULL_POINTER;

    for (integer_t i = 0; i < bits->size; i++)
    {
        bits->buffer[i] = ~bits->buffer[i];
    }

    return DS_OK;
}

/// \brief Performs an \c AND operation between two bit arrays.
///
/// \param bits1
/// \param bits2
///
/// \return
Status bit_AND(BitArray bits1, BitArray bits2)
{
    if (bits1 == NULL || bits2 == NULL)
        return DS_ERR_NULL_POINTER;

    // Get the shortest buffer size
    integer_t size = (bits1->size > bits2->size) ? bits2->size : bits1->size;

    for (integer_t i = 0; i < size; i++)
    {
        bits1->buffer[i] &= bits2->buffer[i];
    }

    return DS_OK;
}

/// \brief Performs an \c OR operation between two bit arrays.
///
/// \param bits1
/// \param bits2
///
/// \return
Status bit_OR(BitArray bits1, BitArray bits2)
{
    if (bits1 == NULL || bits2 == NULL)
        return DS_ERR_NULL_POINTER;

    // Get the shortest buffer size
    integer_t size = (bits1->size > bits2->size) ? bits2->size : bits1->size;

    for (integer_t i = 0; i < size; i++)
    {
        bits1->buffer[i] |= bits2->buffer[i];
    }

    return DS_OK;
}

/// \brief Performs an \c XOR operation between two bit arrays.
///
/// \param bits1
/// \param bits2
///
/// \return
Status bit_XOR(BitArray bits1, BitArray bits2)
{
    if (bits1 == NULL || bits2 == NULL)
        return DS_ERR_NULL_POINTER;

    // Get the shortest buffer size
    integer_t size = (bits1->size > bits2->size) ? bits2->size : bits1->size;

    for (integer_t i = 0; i < size; i++)
    {
        bits1->buffer[i] ^= bits2->buffer[i];
    }

    return DS_OK;
}

/// \brief Performs a \c NAND operation between two bit arrays.
///
/// \param bits1
/// \param bits2
///
/// \return
Status bit_NAND(BitArray bits1, BitArray bits2)
{
    if (bits1 == NULL || bits2 == NULL)
        return DS_ERR_NULL_POINTER;

    // Get the shortest buffer size
    integer_t size = (bits1->size > bits2->size) ? bits2->size : bits1->size;

    for (integer_t i = 0; i < size; i++)
    {
        bits1->buffer[i] = ~(bits1->buffer[i] & bits2->buffer[i]);
    }

    return DS_OK;
}

/// \brief Performs a \c NOR operation between two bit arrays.
///
/// \param bits1
/// \param bits2
///
/// \return
Status bit_NOR(BitArray bits1, BitArray bits2)
{
    if (bits1 == NULL || bits2 == NULL)
        return DS_ERR_NULL_POINTER;

    // Get the shortest buffer size
    integer_t size = (bits1->size > bits2->size) ? bits2->size : bits1->size;

    for (integer_t i = 0; i < size; i++)
    {
        bits1->buffer[i] = ~(bits1->buffer[i] | bits2->buffer[i]);
    }

    return DS_OK;
}

/// \brief Performs a \c NXOR operation between two bit arrays.
///
/// \param bits1
/// \param bits2
///
/// \return
Status bit_NXOR(BitArray bits1, BitArray bits2)
{
    if (bits1 == NULL || bits2 == NULL)
        return DS_ERR_NULL_POINTER;

    // Get the shortest buffer size
    integer_t size = (bits1->size > bits2->size) ? bits2->size : bits1->size;

    for (integer_t i = 0; i < size; i++)
    {
        bits1->buffer[i] = ~(bits1->buffer[i] ^ bits2->buffer[i]);
    }

    return DS_OK;
}

/// \brief Calculates the relative complement of bits1 in bits2.
///
/// \param bits1
/// \param bits2
///
/// \return
Status bit_DIFF(BitArray bits1, BitArray bits2)
{
    if (bits1 == NULL || bits2 == NULL)
        return DS_ERR_NULL_POINTER;

    // Get the shortest buffer size
    integer_t size = (bits1->size > bits2->size) ? bits2->size : bits1->size;

    for (integer_t i = 0; i < size; i++)
    {
        bits1->buffer[i] &= ~(bits2->buffer[i]);
    }

    return DS_OK;
}

/// \brief Displays each bit individually in the console.
///
/// \param bits
///
/// \return
Status bit_display(BitArray bits)
{
    if (bits == NULL)
        return DS_ERR_NULL_POINTER;

    printf("\nBitArray\n[ ");

    integer_t nbits = bit_nbits(bits);

    for (integer_t i = 0; i < nbits - 1; i++)
    {
        printf("%d, ", bit_receive(bits, i) ? 1 : 0);
    }

    printf("%d ]\n", bit_receive(bits, nbits - 1) ? 1 : 0);

    return DS_OK;
}

/// \brief Displays each bit individually in the console like an array.
///
/// \param bits
///
/// \return
Status bit_display_array(BitArray bits)
{
    if (bits == NULL)
        return DS_ERR_NULL_POINTER;

    printf("\n[ ");

    integer_t nbits = bit_nbits(bits);

    for (integer_t i = 0; i < nbits - 1; i++)
    {
        printf("%d, ", bit_receive(bits, i) ? 1 : 0);
    }

    printf("%d ]\n", bit_receive(bits, nbits - 1) ? 1 : 0);

    return DS_OK;
}

///////////////////////////////////////////////////// NOT EXPOSED FUNCTIONS ///

static integer_t bit_buffer_index(integer_t bit_index)
{
    return bit_index >> 6;
}

static bool bit_receive(BitArray bits, integer_t bit_index)
{
    integer_t index = bit_buffer_index(bit_index);

    int64_t value = bits->buffer[index] & ((int64_t)1 << bit_index);

    return (value) ? true : false;
}

static Status bit_grow(BitArray bits, integer_t words)
{
    if (bits->size >= words)
        return DS_OK;

    // Either double in size or allocate to fit enough words
    integer_t new_size = (words > bits->size * 2) ? words : bits->size * 2;

    int64_t *new_buffer = realloc(bits->buffer, sizeof(int64_t) * new_size);

    // Reallocation failed
    if (!new_buffer)
        return DS_ERR_ALLOC;

    // This needs to be checked
    memset(bits->buffer + bits->size - 1, 0, new_size - bits->size - 1);

    // TODO

    return DS_OK;
}

// Counts the number of set bits in an int64_t using the "bit population count"
// as it is called
static int64_t bit_count(int64_t i)
{
    i = i - ((i >> 1) & 0x5555555555555555L);
    i = (i & 0x3333333333333333L) + ((i >> 2) & 0x3333333333333333L);
    i = (i + (i >> 4)) & 0x0f0f0f0f0f0f0f0fL;
    i = i + (i >> 8);
    i = i + (i >> 16);
    i = i + (i >> 32);
    return (int)i & 0x7f;
}

////////////////////////////////////////////// END OF NOT EXPOSED FUNCTIONS ///