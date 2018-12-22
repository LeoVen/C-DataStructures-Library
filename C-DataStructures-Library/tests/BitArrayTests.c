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
    ut_equals_unsigned_t(ut, 1024, bit_size(bits), __func__);

    if (!bit_resize(bits, 64))
        goto error;

    ut_equals_unsigned_t(ut, 64, bit_nbits(bits), __func__);
    ut_equals_unsigned_t(ut, 1, bit_size(bits), __func__);

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
    BitArray bits1 = bit_create(1024);

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

    // The sum of all numbers from 0 to 1024 except for multiples of 2 is 262144
    ut_equals_unsigned_t(ut, 262144, sum, __func__);

    bit_free(bits1);

    return;

    error:
    printf("Error at %s\n", __func__);
    bit_free(bits1);
    ut_error();
}

void bit_test_AND(UnitTest ut)
{
    BitArray_t *bits1 = bit_create(1024),
               *bits2 = bit_create(1024),
               *bits3 = bit_create(1024);

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

    // The sum of all multiples of 12 below 1023 (included) is 43860
    ut_equals_unsigned_t(ut, 43860, sum, __func__);

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
    BitArray_t *bits1 = bit_create(1024),
            *bits2 = bit_create(1024);

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

    // Should equals the sum of all numbers from 0 to 1023 (included) which is
    // 523776
    ut_equals_unsigned_t(ut, 523776, sum, __func__);

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
    BitArray_t *bits1 = bit_create(1024),
               *bits2 = bit_create(1024);

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

    // The sum of all multiples of all the numbers below 1023 (included) that
    // are multiples of two but not of three or multiples of three but not
    // of two equals to 262145
    // python3: sum(
    //     [i if (
    //         ((i % 2 == 0) and (i % 3 != 0)) or
    //         ((i % 3 == 0) and (i % 2 != 0))
    //     else 0
    //     for i in range(1024)]
    // )
    ut_equals_unsigned_t(ut, 262145, sum, __func__);

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
    BitArray_t *bits1 = bit_create(1024),
               *bits2 = bit_create(1024);

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

    // Sum of all the numbers that are not multiples of 2 up to 1023 (included)
    // is 262144
    ut_equals_unsigned_t(ut, 262144, sum, __func__);

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
    BitArray_t *bits1 = bit_create(1024),
               *bits2 = bit_create(1024);

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

    // Sum of all the numbers that are not multiples of 2 up to 1023 (included)
    // is 262144
    ut_equals_unsigned_t(ut, 262144, sum, __func__);

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
    BitArray_t *bits1 = bit_create(1024),
               *bits2 = bit_create(1024);

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
    // python3: sum(
    //     [0 if (
    //         ((i % 2 == 0) and (i % 3 != 0)) or
    //         ((i % 3 == 0) and (i % 2 != 0))
    //     else i
    //     for i in range(1024)]
    // )
    // Likewise:
    // python3: sum(
    //     [i if
    //         (i % 6 == 0) or
    //         ((i % 2 != 0) and (i % 3 != 0))
    //     else 0
    //     for i in range(1024)]
    // )
    if (!bit_NXOR(bits1, bits2))
        goto error;

    unsigned_t sum = 0;
    for (unsigned_t i = 0; i < nbits; i++)
    {
        sum += bit_get(bits1, i) ? i : 0; // Sum if bit is 1
    }

    // Sum totals up to 261631
    ut_equals_unsigned_t(ut, 261631, sum, __func__);

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
    BitArray_t *bits1 = bit_create(1024),
               *bits2 = bit_create(1024);

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

    // The sum of all multiples of two that are not multiples of 3 is
    ut_equals_unsigned_t(ut, 174422, sum, __func__);

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
    if (!bit_put(bits, false, 32))
        goto error;
    if (!bit_put(bits, true, 33))
        goto error;

    unsigned_t nbits = bit_nbits(bits);

    unsigned_t sum = 0;
    for (unsigned_t i = 0; i < nbits; i++)
    {
        sum += bit_get(bits, i) ? i : 0; // Sum if bit is 1
    }

    ut_equals_unsigned_t(ut, 529, sum, __func__);

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
    if (!bit_put_range(bits, true, 100, 65435))
        goto error;
    if (!bit_put_range(bits, false, 200, 65335))
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

// Runs all BitArray tests
Status BitArrayTests(void)
{
    UnitTest ut;

    Status st = ut_init(&ut);

    if (st != DS_OK)
        goto error;

    bit_test_resize(ut);
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

    ut_report(ut, "BitArray");
    ut_delete(&ut);

    return DS_OK;

    error:
    printf("Error at %s\n", __func__);
    ut_report(ut, "BitArray");
    ut_delete(&ut);
    return st;
}