/**
 * @file BitArray.c
 *
 * @author Leonardo Vencovsky (https://github.com/LeoVen)
 *
 * @date 05/12/2018
 */

#include "BitArray.h"

/// A bit array (bit set, bit map, bit string or bit vector) is a compacted
/// array of bits represented by the bits in a word (in this case an unsigned_t)
/// where you can individually set or clear each bit. It is very useful at
/// implementing a Set of elements where:
/// - Union represented by the OR operator ( bit_OR() )
/// - Intersection represented by the AND operator ( bit_AND() );
/// - Symmetric difference represented by the XOR operator ( bit_XOR() ).
///
/// The bit array can be very fast at doing Set operations. In 64-bit machines
/// with 64-bit instructions it is possible to calculate 64 elements at once
/// on each cycle of a loop instead of checking a byte at a time.
///
/// The bit array is a compacted array of 1's and 0's. It is the most dense
/// storage where every bit represents an independent element in the bit array.
///
/// When storing an array of booleans, each position can only be either \c true
/// or \c false, but to represent a single boolean requires 8 bits since most
/// computers are byte-addressable. With small arrays there is no need to use
/// bit arrays but when you need to store more than 100 thousand booleans then
/// a bit array might be a good choice since the required bytes to represent
/// this array drops from 100 to 12.5 kilobytes.
///
/// \par Functions
/// Located in the file BitArray.c
struct BitArray_s
{
    /// \brief Buffer that stores the bit array's values.
    ///
    /// The array that will be used to manipulate each individual bit.
    unsigned_t *buffer;

    /// \brief Buffer size.
    ///
    /// The current size of the buffer.
    unsigned_t size;

    /// \brief Number of bits used by the user.
    ///
    /// Number of bits used by the user, which is less than or equal to the
    /// actual number of bits.
    unsigned_t used_bits;
};

// Finding how many shifts are needed for the highest integer type to be mapped
// as a buffer index. Used in bit_buffer_index().
static const unsigned_t bit_shifts =
          ((sizeof(unsigned_t) * 8) >> 6) > 0
        ? 6
        : ((sizeof(unsigned_t) * 8) >> 5) > 0
        ? 5
        : ((sizeof(unsigned_t) * 8) >> 4) > 0
        ? 4
        : 3; // One byte

static const unsigned_t bit_word_size = sizeof(unsigned_t) * 8;

///////////////////////////////////////////////////// NOT EXPOSED FUNCTIONS ///

static unsigned_t
bit_buffer_index(unsigned_t bit_index);

static bool
bit_grow(BitArray_t *bits, unsigned_t bit_size);

static bool
bit_clear_unused_bits(BitArray_t *bits);

static bool
bit_equalize(BitArray_t *bits1, BitArray_t *bits2);

static unsigned_t
bit_count(unsigned_t i);

////////////////////////////////////////////// END OF NOT EXPOSED FUNCTIONS ///

/// Creates a new bit array with a default size of 1 word, 64 bits.
///
/// \return A new bit array with a default size of 1 word.
BitArray_t *
bit_new(void)
{
    BitArray_t *bits = malloc(sizeof(BitArray_t));

    if (!bits)
        return NULL;

    bits->buffer = calloc(1, sizeof(unsigned_t));

    if (!(bits->buffer))
    {
        free(bits);
        return NULL;
    }

    bits->size = 1;
    bits->used_bits = bits->size * bit_word_size;

    return bits;
}

/// Creates a new bit array with enough space for the required amount of bits.
///
/// \param[in] required_bits Required addressable bits.
///
/// \return A new bit array with enough size to address the required amount of
/// bits.
BitArray_t *
bit_create(unsigned_t required_bits)
{
    if (required_bits == 0)
        return NULL;

    BitArray_t *bits = malloc(sizeof(BitArray_t));

    if (!bits)
        return NULL;

    unsigned_t buffer_size = bit_buffer_index(required_bits - 1) + 1;

    bits->buffer = calloc(buffer_size, sizeof(unsigned_t));

    if (!(bits->buffer))
    {
        free(bits);
        return NULL;
    }

    bits->size = buffer_size;
    bits->used_bits = required_bits;

    return bits;
}

/// Frees from memory a BitArray_s.
///
/// \param[in] bits The bit array to be freed from memory.
void
bit_free(BitArray_t *bits)
{
    free(bits->buffer);
    free(bits);
}

/// Returns how many words are in the bit array, which is the same as the
/// array's length.
///
/// \param[in] bits The target bit array.
///
/// \return The amount of words in the bit array.
unsigned_t
bit_nwords(BitArray_t *bits)
{
    return bits->size;
}

/// Returns the total amount of bits being currently used by the user.
///
/// \param[in] bits The target bit array.
///
/// \return The amount of addressable bits.
unsigned_t
bit_nbits(BitArray_t *bits)
{
    return bits->used_bits;
}

/// Returns the total amount of bits that can be addressed with the current
/// amount of words.
///
/// \param[in] bits The target bit array.
///
/// \return The amount of possible addressable bits.
unsigned_t
bit_nbits_real(BitArray_t *bits)
{
    return bits->size * sizeof(unsigned_t) * 8;
}

/// Resize the buffer to accommodate enough \c bit_size indexes. If the buffer
/// grows, all new bytes are initializes a 0; if the buffer shrinks then some
/// data is possibly lost.
///
/// \param bits
/// \param bit_size
///
/// \return
bool
bit_resize(BitArray_t *bits, unsigned_t bit_size)
{
    if (bit_size == 0)
        return false;

    // -1 because bit_size is 1-based and we need to pass a 0-based index
    unsigned_t words = bit_buffer_index(bit_size - 1);

    // words is a 0-based index, lets make it a size variable, basically
    // reverting the last step
    words++;

    unsigned_t new_size = words;

    // Grow
    if (bits->size < words)
    {
        // First clear the unused trailing bits
        bit_clear_unused_bits(bits);

        unsigned_t *new_buffer = realloc(bits->buffer,
                sizeof(unsigned_t) * new_size);

        // Reallocation failed
        if (!new_buffer)
            return false;

        bits->buffer = new_buffer;

        // Set the new words to 0
        memset(bits->buffer + bits->size, 0,
               (new_size - bits->size) * sizeof(unsigned_t));
    }
    // Shrink
    else if (bits->size > words)
    {
        // Reallocate the buffer, possibly truncating values
        unsigned_t *new_buffer = realloc(bits->buffer,
                sizeof(unsigned_t) * new_size);

        // Reallocation failed
        if (!new_buffer)
            return false;
    }
    // if bits->size == words return true

    bits->size = new_size;
    bits->used_bits = bit_size;

    return true;
}

///
/// \param bits
/// \param bit_index
///
/// \return
bool
bit_set(BitArray_t *bits, unsigned_t bit_index)
{
    if (bit_index < 0)
        return false;

    if (!bit_grow(bits, bit_index + 1))
        return false;

    unsigned_t index = bit_buffer_index(bit_index);

    bits->buffer[index] |= ((unsigned_t)1 << bit_index);

    return true;
}

///
/// \param bits
/// \param from_index
/// \param to_index
///
/// \return
bool
bit_set_range(BitArray_t *bits, unsigned_t from_index, unsigned_t to_index)
{
    if (from_index < 0 || to_index < 0)
        return false;

    if (to_index < from_index)
        return false;

    if (!bit_grow(bits, to_index + 1))
        return false;

    unsigned_t start_index = bit_buffer_index(from_index);
    unsigned_t end_index = bit_buffer_index(to_index);

    unsigned_t start_mask = ~((unsigned_t)0) << from_index;
    unsigned_t end_mask = ~((unsigned_t)0) >> -(to_index + 1);

    if (start_index == end_index)
    {
        bits->buffer[start_index] |= (start_mask & end_mask);
    }
    else
    {
        bits->buffer[start_index] |= start_mask;

        for (unsigned_t i = start_index + 1; i < end_index; i++)
        {
            bits->buffer[i] = ~((unsigned_t)0);
        }

        bits->buffer[end_index] |= end_mask;
    }

    return true;
}

///
/// \param bits
/// \param bit_index
///
/// \return
bool
bit_clear(BitArray_t *bits, unsigned_t bit_index)
{
    if (bit_index < 0)
        return false;

    if (!bit_grow(bits, bit_index + 1))
        return false;

    unsigned_t index = bit_buffer_index(bit_index);

    bits->buffer[index] &= ~((unsigned_t)1 << bit_index);

    return true;
}

///
/// \param bits
/// \param from_index
/// \param to_index
///
/// \return
bool
bit_clear_range(BitArray_t *bits, unsigned_t from_index, unsigned_t to_index)
{
    if (from_index < 0 || to_index < 0)
        return false;

    if (to_index < from_index)
        return false;

    if (!bit_grow(bits, to_index + 1))
        return false;

    unsigned_t start_index = bit_buffer_index(from_index);
    unsigned_t end_index = bit_buffer_index(to_index);

    unsigned_t start_mask = ~((unsigned_t)0) << from_index;
    unsigned_t end_mask = ~((unsigned_t)0) >> -(to_index + 1);

    if (start_index == end_index)
    {
        bits->buffer[start_index] &= ~(start_mask & end_mask);
    }
    else
    {
        bits->buffer[start_index] &= ~start_mask;

        for (unsigned_t i = start_index + 1; i < end_index; i++)
        {
            bits->buffer[i] = 0;
        }

        bits->buffer[end_index] &= ~end_mask;
    }

    return true;
}

///
/// \param bits
/// \param bit_index
///
/// \return
bool
bit_flip(BitArray_t *bits, unsigned_t bit_index)
{
    if (bit_index < 0)
        return false;

    if (!bit_grow(bits, bit_index + 1))
        return false;

    unsigned_t index = bit_buffer_index(bit_index);

    bits->buffer[index] ^= ((unsigned_t)1 << bit_index);

    return true;
}

///
/// \param bits
/// \param from_index
/// \param to_index
///
/// \return
bool
bit_flip_range(BitArray_t *bits, unsigned_t from_index, unsigned_t to_index)
{
    if (from_index < 0 || to_index < 0)
        return false;

    if (to_index < from_index)
        return false;

    if (!bit_grow(bits, to_index + 1))
        return false;

    unsigned_t start_index = bit_buffer_index(from_index);
    unsigned_t end_index = bit_buffer_index(to_index);

    unsigned_t start_mask = ~((unsigned_t)0) << from_index;
    unsigned_t end_mask = ~((unsigned_t)0) >> -(to_index + 1);

    if (start_index == end_index)
    {
        bits->buffer[start_index] ^= (start_mask & end_mask);
    }
    else
    {
        bits->buffer[start_index] ^= start_mask;

        for (unsigned_t i = start_index + 1; i < end_index; i++)
        {
            bits->buffer[i] ^= ~((unsigned_t)0);
        }

        bits->buffer[end_index] ^= end_mask;
    }

    return true;
}

///
/// \param bits
/// \param bit_index
/// \param state
///
/// \return
bool
bit_put(BitArray_t *bits, unsigned_t bit_index, bool state)
{
    if (bit_index < 0)
        return false;

    if (!bit_grow(bits, bit_index + 1))
        return false;

    unsigned_t index = bit_buffer_index(bit_index);

    if (state)
    {
        bits->buffer[index] |= ((unsigned_t)1 << bit_index);
    }
    else
    {
        bits->buffer[index] &= ~((unsigned_t)1 << bit_index);
    }

    return true;
}

///
/// \param bits
/// \param from_index
/// \param to_index
/// \param state
///
/// \return
bool
bit_put_range(BitArray_t *bits, unsigned_t from_index, unsigned_t to_index, bool state)
{
    if (from_index < 0 || to_index < 0)
        return false;

    if (to_index < from_index)
        return false;

    if (!bit_grow(bits, to_index + 1))
        return false;

    unsigned_t start_index = bit_buffer_index(from_index);
    unsigned_t end_index = bit_buffer_index(to_index);

    unsigned_t start_mask = ~((unsigned_t)0) << from_index;
    unsigned_t end_mask = ~((unsigned_t)0) >> -(to_index + 1);

    if (state)
    {
        if (start_index == end_index)
        {
            bits->buffer[start_index] |= (start_mask & end_mask);
        }
        else
        {
            bits->buffer[start_index] |= start_mask;

            for (unsigned_t i = start_index + 1; i < end_index; i++)
            {
                bits->buffer[i] = ~((unsigned_t)0);
            }

            bits->buffer[end_index] |= end_mask;
        }
    }
    else
    {
        if (start_index == end_index)
        {
            bits->buffer[start_index] &= ~(start_mask & end_mask);
        }
        else
        {
            bits->buffer[start_index] &= ~start_mask;

            for (unsigned_t i = start_index + 1; i < end_index; i++)
            {
                bits->buffer[i] = 0;
            }

            bits->buffer[end_index] &= ~end_mask;
        }
    }

    return true;
}

///
/// \param bits
///
/// \return
bool
bit_fill(BitArray_t *bits)
{
    for (unsigned_t i = 0; i < bits->size; i++)
    {
        bits->buffer[i] = ~((unsigned_t)0);
    }

    return true;
}

///
/// \param bits
///
/// \return
bool
bit_empty(BitArray_t *bits)
{
    for (unsigned_t i = 0; i < bits->size; i++)
    {
        bits->buffer[i] = (unsigned_t)0;
    }

    return true;
}

///
/// \param bits
/// \param bit_index
///
/// \return
bool
bit_get(BitArray_t *bits, unsigned_t bit_index)
{
    unsigned_t index = bit_buffer_index(bit_index);

    unsigned_t value = bits->buffer[index] & ((unsigned_t)1 << bit_index);

    return (value) ? true : false;
}

///
/// \param bits
///
/// \return
unsigned_t
bit_cardinality(BitArray_t *bits)
{
    unsigned_t sum = 0;

    for (unsigned_t i = 0; i < bits->size; i++)
        sum += bit_count(bits->buffer[i]);

    return sum;
}

/// Returns true if the specified bit array 1 has any bits set to true that are
/// also set to true in the bit array 2.
///
/// \param bits1
/// \param bits2
///
/// \return
bool
bit_intersects(BitArray_t *bits1, BitArray_t *bits2)
{
    unsigned_t size = bits1->size < bits2->size ? bits1->size : bits2->size;

    for (unsigned_t i = 0; i < size; i++)
    {
        if ((bits1->buffer[i] & bits2->buffer[i]) != 0)
            return true;
    }

    return false;
}

///
/// \param bits
///
/// \return
BitArray_t *
bit_copy(BitArray_t *bits)
{
    BitArray_t *result = bit_create(bit_nbits(bits));

    if (!result)
        return NULL;

    for (unsigned_t i = 0; i < bits->size; i++)
    {
        result->buffer[i] = bits->buffer[i];
    }

    return result;
}

///
/// \param bits
/// \param size
///
/// \return
bool *
bit_to_array(BitArray_t *bits, unsigned_t *size)
{
    unsigned_t nbits = bit_nbits(bits);

    bool *result = malloc(sizeof(bool) * nbits);

    if (!result)
        return NULL;

    for (unsigned_t i = 0; i < nbits; i++)
    {
        result[i] = bit_get(bits, i);
    }

    return result;
}

/// Creates a new bit array from an existing boolean array.
///
/// \param array
/// \param arr_size
///
/// \return
BitArray_t *
bit_from_array(bool *array, unsigned_t arr_size)
{
   BitArray_t *bits = bit_create(arr_size);

   if (!bits)
       return NULL;

   for (unsigned_t i = 0; i < arr_size; i++)
   {
       if (!bit_put(bits, i, array[i]))
       {
           bit_free(bits);

           return NULL;
       }
   }

   return bits;
}

/// Returns the index of the nearest bit that is set to true that occurs on or
/// after the specified starting index. If no such bit exists, if the bit array
/// reference is \c NULL, or if a negative index is given as the starting
/// index, then -1 is returned.
///
/// \param bits
/// \param bit_index
///
/// \return
unsigned_t
bit_next_set(BitArray_t *bits, unsigned_t bit_index)
{
    /// \todo bit_next_set
    return -1;
}

/// Returns the index of the nearest bit that is set to false that occurs on or
/// after the specified starting index. If no such bit exists, if the bit array
/// reference is \c NULL, or if a negative index is given as the starting
/// index, then -1 is returned.
///
/// \param bits
/// \param bit_index
///
/// \return
unsigned_t
bit_next_clear(BitArray_t *bits, unsigned_t bit_index)
{
    /// \todo bit_next_clear
    return -1;
}

/// Returns the index of the nearest bit that is set to true that occurs on or
/// before the specified starting index. If no such bit exists, if the bit
/// array reference is \c NULL, or if a negative index is given as the starting
/// index, then -1 is returned.
///
/// \param bits
/// \param bit_index
///
/// \return
unsigned_t
bit_prev_set(BitArray_t *bits, unsigned_t bit_index)
{
    /// \todo bit_prev_set
    return -1;
}

/// Returns the index of the nearest bit that is set to false that occurs on or
/// before the specified starting index. If no such bit exists, if the bit
/// array reference is \c NULL, or if a negative index is given as the starting
/// index, then -1 is returned.
///
/// \param bits
/// \param bit_index
///
/// \return
unsigned_t
bit_prev_clear(BitArray_t *bits, unsigned_t bit_index)
{
    /// \todo bit_prev_clear
    return -1;
}

///
/// \param bits
///
/// \return
bool
bit_all_set(BitArray_t *bits)
{
    for (unsigned_t i = 0; i < bits->size; i++)
    {
        if (bits->buffer[i] != (~(unsigned_t)0))
            return false;
    }

    return true;
}

///
/// \param bits
///
/// \return
bool
bit_all_clear(BitArray_t *bits)
{
    for (unsigned_t i = 0; i < bits->size; i++)
    {
        if (bits->buffer[i] != (unsigned_t)0)
            return false;
    }

    return true;
}

///
/// \param bits
///
/// \return
bool
bit_NOT(BitArray_t *bits)
{
    for (unsigned_t i = 0; i < bits->size; i++)
    {
        bits->buffer[i] = ~bits->buffer[i];
    }

    return true;
}

///
/// \param bits1
/// \param bits2
///
/// \return
bool
bit_AND(BitArray_t *bits1, BitArray_t *bits2)
{
    if (!bit_equalize(bits1, bits2))
        return false;

    for (unsigned_t i = 0; i < bits1->size; i++)
    {
        bits1->buffer[i] &= bits2->buffer[i];
    }

    return true;
}

///
/// \param bits1
/// \param bits2
///
/// \return
bool
bit_OR(BitArray_t *bits1, BitArray_t *bits2)
{
    if (!bit_equalize(bits1, bits2))
        return false;

    for (unsigned_t i = 0; i < bits1->size; i++)
    {
        bits1->buffer[i] |= bits2->buffer[i];
    }

    return true;
}

///
/// \param bits1
/// \param bits2
///
/// \return
bool
bit_XOR(BitArray_t *bits1, BitArray_t *bits2)
{
    if (!bit_equalize(bits1, bits2))
        return false;

    for (unsigned_t i = 0; i < bits1->size; i++)
    {
        bits1->buffer[i] ^= bits2->buffer[i];
    }

    return true;
}

///
/// \param bits1
/// \param bits2
///
/// \return
bool
bit_NAND(BitArray_t *bits1, BitArray_t *bits2)
{
    if (!bit_equalize(bits1, bits2))
        return false;

    for (unsigned_t i = 0; i < bits1->size; i++)
    {
        bits1->buffer[i] = ~(bits1->buffer[i] & bits2->buffer[i]);
    }

    return true;
}

///
/// \param bits1
/// \param bits2
///
/// \return
bool
bit_NOR(BitArray_t *bits1, BitArray_t *bits2)
{
    if (!bit_equalize(bits1, bits2))
        return false;

    for (unsigned_t i = 0; i < bits1->size; i++)
    {
        bits1->buffer[i] = ~(bits1->buffer[i] | bits2->buffer[i]);
    }

    return true;
}

///
/// \param bits1
/// \param bits2
///
/// \return
bool
bit_NXOR(BitArray_t *bits1, BitArray_t *bits2)
{
    if (!bit_equalize(bits1, bits2))
        return false;

    for (unsigned_t i = 0; i < bits1->size; i++)
    {
        bits1->buffer[i] = ~(bits1->buffer[i] ^ bits2->buffer[i]);
    }

    return true;
}

///
/// \param bits1
/// \param bits2
///
/// \return
bool
bit_DIFF(BitArray_t *bits1, BitArray_t *bits2)
{
    if (!bit_equalize(bits1, bits2))
        return false;

    for (unsigned_t i = 0; i < bits1->size; i++)
    {
        bits1->buffer[i] &= ~(bits2->buffer[i]);
    }

    return true;
}

/// Displays each bit individually in the console as 0's and 1's like an array
/// where each number is separated by commas and the set is delimited by
/// brackets.
///
/// \param bits The bit array to be displayed in the console.
void
bit_display(BitArray_t *bits)
{
    printf("\nBitArray\n[ ");

    unsigned_t nbits = bit_nbits(bits);

    for (unsigned_t i = 0; i < nbits - 1; i++)
    {
        printf("%d, ", bit_get(bits, i) ? 1 : 0);
    }

    printf("%d ]\n", bit_get(bits, nbits - 1) ? 1 : 0);
}

///////////////////////////////////////////////////// NOT EXPOSED FUNCTIONS ///

// Translates a bit index to a word index
static unsigned_t
bit_buffer_index(unsigned_t bit_index)
{
    return bit_index >> bit_shifts;
}

// Increases in size the bit array if needed.
static bool
bit_grow(BitArray_t *bits, unsigned_t bit_size)
{
    if (bit_size == 0)
        return false;

    if (bits->used_bits >= bit_size)
        return true;

    // Clears unused bits because they might be a valid bit index so its
    // default value must be 0
    if (!bit_clear_unused_bits(bits))
        return false;

    // There is still enough space with the current array
    if (bit_size <= bits->size * bit_word_size)
    {
        bits->used_bits = bit_size;

        return true;
    }

    // Either double in size or allocate to fit enough words
    unsigned_t new_bit_size = (bit_size > bits->used_bits * 2)
            ? bit_size : bits->used_bits * 2;

    unsigned_t new_size = bit_buffer_index(new_bit_size - 1) + 1;

    unsigned_t *new_buffer = realloc(bits->buffer,
            sizeof(unsigned_t) * new_size);

    // Reallocation failed
    if (!new_buffer)
        return false;

    bits->buffer = new_buffer;

    // Set the new words to 0
    memset(bits->buffer + bits->size, 0,
            (new_size - bits->size) * sizeof(unsigned_t));

    bits->size = new_size;
    bits->used_bits = new_bit_size;

    return true;
}

// Clears the trailing unused bits.
static bool
bit_clear_unused_bits(BitArray_t *bits)
{
    // Getting mask for the last word
    unsigned_t diff = (bits->size * bit_word_size) - bits->used_bits;

    unsigned_t mask;

    if (diff > 0)
    {
        mask = (unsigned_t)1 << (63 - diff);
        mask |= (mask - 1);
    }
    else
    {
        mask = ~(unsigned_t)0;
    }

    // Erase it
    bits->buffer[bits->size - 1] &= mask;

    return true;
}

// Makes sure that both bit arrays have the same number of used bits
static bool
bit_equalize(BitArray_t *bits1, BitArray_t *bits2)
{
    // Get the highest used bits from both bit arrays
    unsigned_t bit_size = (bits1->used_bits > bits2->used_bits)
                          ? bits1->used_bits
                          : bits2->used_bits;

    // Make sure both are the same size
    if (!bit_grow(bits1, bit_size) || !bit_grow(bits2, bit_size))
        return false;

    return true;
}

// Counts the number of set bits in an unsigned_t using the
// "bit population count"
static unsigned_t
bit_count(unsigned_t i)
{
    i = i - ((i >> 1) & 0x5555555555555555L);
    i = (i & 0x3333333333333333L) + ((i >> 2) & 0x3333333333333333L);
    i = (i + (i >> 4)) & 0x0f0f0f0f0f0f0f0fL;
    i = i + (i >> 8);
    i = i + (i >> 16);
    i = i + (i >> 32);
    return (unsigned_t)i & 0x7f;
}

////////////////////////////////////////////// END OF NOT EXPOSED FUNCTIONS ///

///////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////// Iterator ///
///////////////////////////////////////////////////////////////////////////////

/// \todo BitArrayIterator

///////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////// Wrapper ///
///////////////////////////////////////////////////////////////////////////////

/// \todo BitArrayWrapper
