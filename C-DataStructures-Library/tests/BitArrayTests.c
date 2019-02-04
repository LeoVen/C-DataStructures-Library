/**
 * @file BitArrayTests.c
 * 
 * @author Leonardo Vencovsky (https://github.com/LeoVen)
 *
 * @date 05/12/2018
 */

#include "BitArray.h"
#include "UnitTest.h"
#include "Utility.h"

void bit_test_resize(UnitTest ut)
{
    BitArray_t *bits = bit_new();

    if (!bits)
        goto error;

    if (!bit_resize(bits, 65536))
        goto error;

    ut_equals_unsigned_t(ut, 65536, bit_nbits(bits), __func__);
    ut_equals_unsigned_t(ut, 65536, bit_nbits_real(bits), __func__);
    ut_equals_unsigned_t(ut, 1024, bit_nwords(bits), __func__);

    if (!bit_resize(bits, 80))
        goto error;

    ut_equals_unsigned_t(ut, 80, bit_nbits(bits), __func__);
    ut_equals_unsigned_t(ut, 128, bit_nbits_real(bits), __func__);
    ut_equals_unsigned_t(ut, 2, bit_nwords(bits), __func__);

    bit_free(bits);

    return;

    error:
    printf("Error at %s\n", __func__);
    bit_free(bits);
    ut_error();
}

void bit_test_grow(UnitTest ut)
{
    BitArray_t *bits = bit_create(140);

    if (!bits)
        goto error;

    if (!bit_fill(bits))
        goto error;

    // Shouldn't toggle an increase in the array size
    if (!bit_put(bits, 191, true))
        goto error;

    unsigned_t sum = 0, nbits = bit_nbits(bits);
    for (unsigned_t i = 0; i < nbits; i++)
    {
        sum += bit_get(bits, i) ? i : 0;
    }

    // Sum from 0 to 139 + 191
    ut_equals_unsigned_t(ut, 9921, sum, __func__);
    ut_equals_unsigned_t(ut, 3, bit_nwords(bits), __func__);

    bit_free(bits);

    return;

    error:
    printf("Error at %s\n", __func__);
    bit_free(bits);
    ut_error();
}

void bit_test_clear_unused_bits(UnitTest ut)
{
    BitArray_t *bits = bit_create(140);

    if (!bits)
        goto error;

    if(!bit_fill(bits))
        goto error;

    // This should toggle bit_clear_unused_bits
    if (!bit_set(bits, 200))
        goto error;

    if (!bit_set(bits, 1000))
        goto error;

    unsigned_t sum = 0;

    unsigned_t nbits = bit_nbits(bits);

    for (unsigned_t i = 0; i < nbits; i++)
    {
        sum += bit_get(bits, i) ? i : 0;
    }

    ut_equals_unsigned_t(ut, 10930, sum, __func__);

    bit_free(bits);

    return;

    error:
    printf("Error at %s\n", __func__);
    bit_free(bits);
    ut_error();
}

void
bit_test_NOT(UnitTest ut)
{
    BitArray bits1 = bit_create(1000);

    if (!bits1)
        goto error;

    unsigned_t nbits = bit_nbits(bits1);
    // Creates a bit array of multiples of 2
    for (unsigned_t i = 0; i < nbits; i++)
    {
        if (i % 2 == 0)
        {
            if (!bit_set(bits1, i))
                goto error;
        }
    }

    if (!bit_NOT(bits1))
        goto error;

    unsigned_t sum = 0;
    // Get the sum of all numbers that are not multiples of 2
    for (unsigned_t i = 0; i < nbits; i++)
    {
        sum += bit_get(bits1, i) ? i : 0; // Sum if bit is 1
    }

    // The sum of all numbers from 0 to 1000 (not included) except for
    // multiples of 2
    // Python3: sum(
    //      [i if (i % 2 != 0) else 0 for i in range(1000)]
    // )
    ut_equals_unsigned_t(ut, 250000, sum, __func__);

    bit_free(bits1);

    return;

    error:
    printf("Error at %s\n", __func__);
    bit_free(bits1);
    ut_error();
}

void bit_test_AND(UnitTest ut)
{
    BitArray_t *bits1 = bit_create(1000),
               *bits2 = bit_create(1000),
               *bits3 = bit_create(1000);

    if (!bits1 || !bits2 || !bits3)
        goto error;

    unsigned_t nbits = bit_nbits(bits1);

    // Creates three sets. One of multiples of 2, one of 3's and another of 4's
    for (unsigned_t i = 0; i < nbits; i++)
    {
        if (i % 2 == 0)
        {
            if (!bit_set(bits1, i))
                goto error;
        }
        if (i % 3 == 0)
        {
            if (!bit_set(bits2, i))
                goto error;
        }
        if (i % 4 == 0)
        {
            if (!bit_set(bits3, i))
                goto error;
        }
    }

    // Intersection of multiples of 2, 3 and 4
    // Creates all the multiples of 12
    if (!bit_AND(bits1, bits2))
        goto error;
    if (!bit_AND(bits1, bits3))
        goto error;

    unsigned_t sum = 0;
    for (unsigned_t i = 0; i < nbits; i++)
    {
        sum += bit_get(bits1, i) ? i : 0; // Sum if bit is 1
    }

    // The sum of all multiples of 12 below 1000 (not included)
    // Python3: sum(
    //      [i if i % 12 == 0 else 0 for i in range(1000)]
    // )
    ut_equals_unsigned_t(ut, 41832, sum, __func__);

    bit_free(bits1);
    bit_free(bits2);
    bit_free(bits3);

    return;

    error:
    printf("Error at %s\n", __func__);
    bit_free(bits1);
    bit_free(bits2);
    bit_free(bits3);
    ut_error();
}

void bit_test_OR(UnitTest ut)
{
    BitArray_t *bits1 = bit_create(1000),
            *bits2 = bit_create(1000);

    if (!bits1 || !bits2)
        goto error;

    unsigned_t nbits = bit_nbits(bits1);

    // Creates two bit arrays where one is the complement of the other
    for (unsigned_t i = 0; i < nbits; i++)
    {
        if (i % 2 == 0)
        {
            if (!bit_set(bits1, i))
                goto error;
        }
        else
        {
            if (!bit_set(bits2, i))
                goto error;
        }
    }

    // All bits should be set to 1 because one is the complement of the other
    if (!bit_OR(bits1, bits2))
        goto error;

    unsigned_t sum = 0;
    for (unsigned_t i = 0; i < nbits; i++)
    {
        sum += bit_get(bits1, i) ? i : 0; // Sum if bit is 1
    }

    // Should equals the sum of all numbers from 0 to 1000 (not included)
    // Python3: sum(
    //      [i for i in range(1000)]
    // )

    ut_equals_unsigned_t(ut, 499500, sum, __func__);

    bit_free(bits1);
    bit_free(bits2);

    return;

    error:
    printf("Error at %s\n", __func__);
    bit_free(bits1);
    bit_free(bits2);
    ut_error();
}

void bit_test_XOR(UnitTest ut)
{
    BitArray_t *bits1 = bit_create(1000),
               *bits2 = bit_create(1000);

    if (!bits1 || !bits2)
        goto error;

    unsigned_t nbits = bit_nbits(bits1);

    // Creates two sets. One of multiples of 2 and one of multiples of 3
    for (unsigned_t i = 0; i < nbits; i++)
    {
        if (i % 2 == 0)
        {
            if (!bit_set(bits1, i))
                goto error;
        }
        if (i % 3 == 0)
        {
            if (!bit_set(bits2, i))
                goto error;
        }
    }

    // Symmetric difference of multiples of 2 and 3
    if (!bit_XOR(bits1, bits2))
        goto error;

    unsigned_t sum = 0;
    for (unsigned_t i = 0; i < nbits; i++)
    {
        sum += bit_get(bits1, i) ? i : 0; // Sum if bit is 1
    }

    // The sum of all multiples of all the numbers below 1000 (not included)
    // that are multiples of two but not of three or multiples of three but not
    // of two equals to 250001
    // Python3: sum(
    //     [i if (
    //         ((i % 2 == 0) and (i % 3 != 0)) or
    //         ((i % 3 == 0) and (i % 2 != 0))
    //     ) else 0
    //     for i in range(1000)]
    // )
    ut_equals_unsigned_t(ut, 250001, sum, __func__);

    bit_free(bits1);
    bit_free(bits2);

    return;

    error:
    printf("Error at %s\n", __func__);
    bit_free(bits1);
    bit_free(bits2);
    ut_error();
}

void bit_test_NAND(UnitTest ut)
{
    BitArray_t *bits1 = bit_create(1000),
               *bits2 = bit_create(1000);

    if (!bits1 || !bits2)
        goto error;

    unsigned_t nbits = bit_nbits(bits1);

    // Creates two sets. One of multiples of 2 and one filled with 1 bits
    for (unsigned_t i = 0; i < nbits; i++)
    {
        if (i % 2 == 0)
        {
            if (!bit_set(bits1, i))
                goto error;
        }
    }

    // This should give all the numbers that are not multiples of 2
    if (!bit_fill(bits2))
        goto error;
    if (!bit_NAND(bits1, bits2))
        goto error;

    unsigned_t sum = 0;
    for (unsigned_t i = 0; i < nbits; i++)
    {
        sum += bit_get(bits1, i) ? i : 0; // Sum if bit is 1
    }

    // Sum of all the numbers that are not multiples of 2 up to 1000 (not
    // included)
    // Python3: sum(
    //      [i if (i % 2 != 0) else 0 for i in range(1000)]
    // )
    ut_equals_unsigned_t(ut, 250000, sum, __func__);

    bit_free(bits1);
    bit_free(bits2);

    return;

    error:
    printf("Error at %s\n", __func__);
    bit_free(bits1);
    bit_free(bits2);
    ut_error();
}

void bit_test_NOR(UnitTest ut)
{
    BitArray_t *bits1 = bit_create(1000),
               *bits2 = bit_create(1000);

    if (!bits1 || !bits2)
        goto error;

    unsigned_t nbits = bit_nbits(bits1);

    // Creates two sets. One of multiples of 2 and one filled with 0 bits
    for (unsigned_t i = 0; i < nbits; i++)
    {
        if (i % 2 == 0)
        {
            if (!bit_set(bits1, i))
                goto error;
        }
    }

    // This should give all the numbers that are not multiples of 2
    if (!bit_empty(bits2))
        goto error;
    if (!bit_NOR(bits1, bits2))
        goto error;

    unsigned_t sum = 0;
    for (unsigned_t i = 0; i < nbits; i++)
    {
        sum += bit_get(bits1, i) ? i : 0; // Sum if bit is 1
    }

    // Sum of all the numbers that are not multiples of 2 up to 1000 (not
    // included)
    // Python3: sum(
    //      [i if (i % 2 != 0) else 0 for i in range(1000)]
    // )
    ut_equals_unsigned_t(ut, 250000, sum, __func__);

    bit_free(bits1);
    bit_free(bits2);

    return;

    error:
    printf("Error at %s\n", __func__);
    bit_free(bits1);
    bit_free(bits2);
    ut_error();
}

void bit_test_NXOR(UnitTest ut)
{
    BitArray_t *bits1 = bit_create(1000),
               *bits2 = bit_create(1000);

    if (!bits1 || !bits2)
        goto error;

    unsigned_t nbits = bit_nbits(bits1);

    // Creates two sets. One of multiples of 2 and one of multiples of 3
    for (unsigned_t i = 0; i < nbits; i++)
    {
        if (i % 2 == 0)
        {
            if (!bit_set(bits1, i))
                goto error;
        }
        if (i % 3 == 0)
        {
            if (!bit_set(bits2, i))
                goto error;
        }
    }

    // Negation of symmetric difference gives us all the numbers that are
    // multiples of 6 and all the numbers that are not multiples of 2 or 3
    // separately
    // Python3: sum(
    //     [0 if (
    //         ((i % 2 == 0) and (i % 3 != 0)) or
    //         ((i % 3 == 0) and (i % 2 != 0))
    //     else i
    //     for i in range(1000)]
    // )
    // Likewise:
    // Python3: sum(
    //     [i if
    //         (i % 6 == 0) or
    //         ((i % 2 != 0) and (i % 3 != 0))
    //     else 0
    //     for i in range(1000)]
    // )
    if (!bit_NXOR(bits1, bits2))
        goto error;

    unsigned_t sum = 0;
    for (unsigned_t i = 0; i < nbits; i++)
    {
        sum += bit_get(bits1, i) ? i : 0; // Sum if bit is 1
    }

    // Sum totals up to 249499
    ut_equals_unsigned_t(ut, 249499, sum, __func__);

    bit_free(bits1);
    bit_free(bits2);

    return;

    error:
    printf("Error at %s\n", __func__);
    bit_free(bits1);
    bit_free(bits2);
    ut_error();
}

void bit_test_DIFF(UnitTest ut)
{
    BitArray_t *bits1 = bit_create(1000),
               *bits2 = bit_create(1000);

    if (!bits1 || !bits2)
        goto error;

    unsigned_t nbits = bit_nbits(bits1);

    // Creates two sets. One of multiples of 2 and one of multiples of 3
    for (unsigned_t i = 0; i < nbits; i++)
    {
        if (i % 2 == 0)
        {
            if (!bit_set(bits1, i))
                goto error;
        }
        if (i % 3 == 0)
        {
            if (!bit_set(bits2, i))
                goto error;
        }
    }

    // Performs a relative complement of bits1 in bits2 (or what is only in
    // bits1 and not in bits2 sometimes written as bits1 - bits2)
    if (!bit_DIFF(bits1, bits2))
        goto error;

    unsigned_t sum = 0;
    for (unsigned_t i = 0; i < nbits; i++)
    {
        sum += bit_get(bits1, i) ? i : 0; // Sum if bit is 1
    }

    // Python3: sum(
    //      [i if i % 2 == 0 and not(i % 3 == 0) else 0 for i in range(1000)]
    // )
    // The sum of all multiples of two that are not multiples of 3 is
    ut_equals_unsigned_t(ut, 166334, sum, __func__);

    bit_free(bits1);
    bit_free(bits2);

    return;

    error:
    printf("Error at %s\n", __func__);
    bit_free(bits1);
    bit_free(bits2);
    ut_error();
}

// Tests cardinality
void bit_test_cardinality(UnitTest ut)
{
    BitArray_t *bits = bit_new();

    if (!bits)
        goto error;

    for (unsigned_t i = 0; i < bit_nbits(bits); i++)
    {
        if (i % 2 == 0)
        {
            if (!bit_set(bits, i))
                goto error;
        }
    }

    ut_equals_unsigned_t(ut, 32, bit_cardinality(bits), __func__);

    bit_free(bits);

    return;

    error:
    printf("Error at %s\n", __func__);
    bit_free(bits);
    ut_error();
}

void bit_test_fill_empty(UnitTest ut)
{
    BitArray_t *bits = bit_new();

    if (!bits)
        goto error;

    // Test if calloc sets everything to 0
    ut_equals_bool(ut, true, bit_all_clear(bits), __func__);

    if (!bit_fill(bits))
        goto error;

    ut_equals_bool(ut, true, bit_all_set(bits), __func__);

    if (!bit_empty(bits))
        goto error;

    ut_equals_bool(ut, true, bit_all_clear(bits), __func__);

    bit_free(bits);

    return;

    error:
    printf("Error at %s\n", __func__);
    bit_free(bits);
    ut_error();
}

void bit_test_set(UnitTest ut)
{
    BitArray_t *bits = bit_new();

    if (!bits)
        goto error;

    if (!bit_set(bits, 32))
        goto error;

    unsigned_t nbits = bit_nbits(bits);

    unsigned_t sum = 0;
    for (unsigned_t i = 0; i < nbits; i++)
    {
        sum += bit_get(bits, i) ? i : 0; // Sum if bit is 1
    }

    ut_equals_unsigned_t(ut, 32, sum, __func__);

    if (!bit_empty(bits))
        goto error;

    // BitArray has the default size. Provoke an increase in size
    if (!bit_set(bits, 1))
        goto error;
    if (!bit_set(bits, 128))
        goto error;
    if (!bit_set(bits, 1000))
        goto error;
    if (!bit_set(bits, 20000))
        goto error;

    nbits = bit_nbits(bits);
    sum = 0;
    for (unsigned_t i = 0; i < nbits; i++)
    {
        sum += bit_get(bits, i) ? i : 0; // Sum if bit is 1
    }

    // 1 + 128 + 1000 + 20000 == 21129
    ut_equals_unsigned_t(ut, 21129, sum, __func__);

    bit_free(bits);

    return;

    error:
    printf("Error at %s\n", __func__);
    bit_free(bits);
    ut_error();
}

void bit_test_clear(UnitTest ut)
{
    BitArray_t *bits = bit_new();

    if (!bits)
        goto error;

    if (!bit_fill(bits))
        goto error;
    if (!bit_clear(bits, 32))
        goto error;

    unsigned_t nbits = bit_nbits(bits);

    unsigned_t sum = 0;
    for (unsigned_t i = 0; i < nbits; i++)
    {
        sum += bit_get(bits, i) ? i : 0; // Sum if bit is 1
    }

    ut_equals_unsigned_t(ut, 1984, sum, __func__);

    if (!bit_empty(bits))
        goto error;

    if (!bit_clear(bits, 20000))
        goto error;

    nbits = bit_nbits_real(bits);

    ut_equals_unsigned_t(ut, 20032, nbits, __func__);

    bit_free(bits);

    return;

    error:
    printf("Error at %s\n", __func__);
    bit_free(bits);
    ut_error();
}

void bit_test_flip(UnitTest ut)
{
    BitArray_t *bits = bit_new();

    if (!bits)
        goto error;

    if (!bit_set_range(bits, 0, 32))
        goto error;
    if (!bit_flip(bits, 32))
        goto error;
    if (!bit_flip(bits, 33))
        goto error;

    unsigned_t nbits = bit_nbits(bits);

    unsigned_t sum = 0;
    for (unsigned_t i = 0; i < nbits; i++)
    {
        sum += bit_get(bits, i) ? i : 0; // Sum if bit is 1
    }

    ut_equals_unsigned_t(ut, 529, sum, __func__);

    if (!bit_empty(bits))
        goto error;

    if (!bit_flip(bits, 1))
        goto error;
    if (!bit_flip(bits, 128))
        goto error;
    if (!bit_flip(bits, 1000))
        goto error;
    if (!bit_flip(bits, 20000))
        goto error;

    nbits = bit_nbits(bits);
    sum = 0;
    for (unsigned_t i = 0; i < nbits; i++)
    {
        sum += bit_get(bits, i) ? i : 0; // Sum if bit is 1
    }

    // 1 + 128 + 1000 + 20000 == 21129
    ut_equals_unsigned_t(ut, 21129, sum, __func__);

    bit_free(bits);

    return;

    error:
    printf("Error at %s\n", __func__);
    bit_free(bits);
    ut_error();
}

void bit_test_put(UnitTest ut)
{
    BitArray_t *bits = bit_new();

    if (!bits)
        goto error;

    if (!bit_set_range(bits, 0, 32))
        goto error;
    if (!bit_put(bits, 32, false))
        goto error;
    if (!bit_put(bits, 33, true))
        goto error;

    unsigned_t nbits = bit_nbits(bits);

    unsigned_t sum = 0;
    for (unsigned_t i = 0; i < nbits; i++)
    {
        sum += bit_get(bits, i) ? i : 0; // Sum if bit is 1
    }

    ut_equals_unsigned_t(ut, 529, sum, __func__);

    if (!bit_empty(bits))
        goto error;

    if (!bit_put(bits, 1, true))
        goto error;
    if (!bit_put(bits, 128, true))
        goto error;
    if (!bit_put(bits, 1000, true))
        goto error;
    if (!bit_put(bits, 20000, true))
        goto error;

    nbits = bit_nbits(bits);
    sum = 0;
    for (unsigned_t i = 0; i < nbits; i++)
    {
        sum += bit_get(bits, i) ? i : 0; // Sum if bit is 1
    }

    // 1 + 128 + 1000 + 20000 == 21129
    ut_equals_unsigned_t(ut, 21129, sum, __func__);

    bit_free(bits);

    return;

    error:
    printf("Error at %s\n", __func__);
    bit_free(bits);
    ut_error();
}

void bit_test_set_range(UnitTest ut)
{
    BitArray_t *bits = bit_new();

    if (!bits)
        goto error;

    if (!bit_set_range(bits, 50, 63))
        goto error;
    if (!bit_set_range(bits, 100, 201))
        goto error;
    if (!bit_set_range(bits, 1231, 1232))
        goto error;
    if (!bit_set_range(bits, 7310, 9877))
        goto error;
    if (!bit_set_range(bits, 32767, 65535))
        goto error;

    unsigned_t nbits = bit_nbits(bits);

    unsigned_t sum = 0;
    for (unsigned_t i = 0; i < nbits; i++)
    {
        sum += bit_get(bits, i) ? i : 0; // Sum if bit is 1
    }

    ut_equals_unsigned_t(ut, 1632715832, sum, __func__);

    bit_free(bits);

    return;

    error:
    printf("Error at %s\n", __func__);
    bit_free(bits);
    ut_error();
}

void bit_test_clear_range(UnitTest ut)
{
    BitArray_t *bits = bit_new();

    if (!bits)
        goto error;

    if (!bit_resize(bits, 65536))
        goto error;
    if (!bit_fill(bits))
        goto error;
    if (!bit_clear_range(bits, 50, 63))
        goto error;
    if (!bit_clear_range(bits, 100, 201))
        goto error;
    if (!bit_clear_range(bits, 1231, 1232))
        goto error;
    if (!bit_clear_range(bits, 7310, 9877))
        goto error;
    if (!bit_clear_range(bits, 32767, 65535))
        goto error;

    unsigned_t nbits = bit_nbits(bits);

    unsigned_t sum = 0;
    for (unsigned_t i = 0; i < nbits; i++)
    {
        sum += bit_get(bits, i) ? i : 0; // Sum if bit is 1
    }

    ut_equals_unsigned_t(ut, 514735048, sum, __func__);

    bit_free(bits);

    return;

    error:
    printf("Error at %s\n", __func__);
    bit_free(bits);
    ut_error();
}

void bit_test_flip_range(UnitTest ut)
{
    BitArray_t *bits = bit_new();

    if (!bits)
        goto error;

    // 100 - 199 && 65336 - 65435
    if (!bit_resize(bits, 65536))
        goto error;
    if (!bit_flip_range(bits, 100, 65435))
        goto error;
    if (!bit_flip_range(bits, 200, 65335))
        goto error;

    unsigned_t nbits = bit_nbits(bits);

    unsigned_t sum = 0;
    for (unsigned_t i = 0; i < nbits; i++)
    {
        sum += bit_get(bits, i) ? i : 0; // Sum if bit is 1
    }

    ut_equals_unsigned_t(ut, 6553500, sum, __func__);

    bit_free(bits);

    return;

    error:
    printf("Error at %s\n", __func__);
    bit_free(bits);
    ut_error();
}

void bit_test_put_range(UnitTest ut)
{
    BitArray_t *bits = bit_new();

    if (!bits)
        goto error;

    if (!bit_resize(bits, 65536))
        goto error;
    if (!bit_put_range(bits, 100, 65435, true))
        goto error;
    if (!bit_put_range(bits, 200, 65335, false))
        goto error;

    unsigned_t nbits = bit_nbits(bits);

    unsigned_t sum = 0;
    for (unsigned_t i = 0; i < nbits; i++)
    {
        sum += bit_get(bits, i) ? i : 0; // Sum if bit is 1
    }

    ut_equals_unsigned_t(ut, 6553500, sum, __func__);

    bit_free(bits);

    return;

    error:
    printf("Error at %s\n", __func__);
    bit_free(bits);
    ut_error();
}

void bit_test_intersects(UnitTest ut)
{
    BitArray_t *bits1 = bit_new(),
               *bits2 = bit_new();

    if (!bits1 || !bits2)
        goto error;

    if (!bit_set(bits1, 0) || !bit_set(bits1, 62) || !bit_set(bits1, 30))
        goto error;
    if (!bit_set(bits2, 1) || !bit_set(bits2, 63) || !bit_set(bits2, 31))
        goto error;

    // Case 0: no intersection
    ut_equals_bool(ut, false, bit_intersects(bits1, bits2), __func__);

    if (!bit_put(bits2, 0, true))
        goto error;

    // Case: first bit intersects
    ut_equals_bool(ut, true, bit_intersects(bits1, bits2), __func__);

    if (!bit_flip(bits2, 0))
        goto error;
    if (!bit_flip(bits2, 30))
        goto error;

    // Case2: middle bit intersects
    ut_equals_bool(ut, true, bit_intersects(bits1, bits2), __func__);

    if (!bit_clear(bits2, 30))
        goto error;
    if (!bit_put(bits1, 63, true))
        goto error;

    // Case3: last bit intersects
    ut_equals_bool(ut, true, bit_intersects(bits1, bits2), __func__);

    bit_free(bits1);
    bit_free(bits2);

    return;

    error:
    printf("Error at %s\n", __func__);
    bit_free(bits1);
    bit_free(bits2);
    ut_error();
}

// Runs all BitArray tests
Status BitArrayTests(void)
{
    UnitTest ut;

    Status st = ut_init(&ut);

    if (st != DS_OK)
        goto error;

    bit_test_resize(ut);
    bit_test_grow(ut);
    bit_test_clear_unused_bits(ut);
    bit_test_NOT(ut);
    bit_test_AND(ut);
    bit_test_OR(ut);
    bit_test_XOR(ut);
    bit_test_NAND(ut);
    bit_test_NOR(ut);
    bit_test_NXOR(ut);
    bit_test_DIFF(ut);
    bit_test_cardinality(ut);
    bit_test_fill_empty(ut);
    bit_test_set(ut);
    bit_test_clear(ut);
    bit_test_flip(ut);
    bit_test_put(ut);
    bit_test_set_range(ut);
    bit_test_clear_range(ut);
    bit_test_flip_range(ut);
    bit_test_put_range(ut);
    bit_test_intersects(ut);

    ut_report(ut, "BitArray");
    ut_delete(&ut);

    return DS_OK;

    error:
    printf("Error at %s\n", __func__);
    ut_report(ut, "BitArray");
    ut_delete(&ut);
    return st;
}