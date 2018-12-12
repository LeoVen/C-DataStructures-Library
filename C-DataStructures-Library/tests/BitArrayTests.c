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

Status bit_test_NOT(UnitTest ut)
{
    BitArray bits1 = NULL;

    Status st = DS_OK;

    st += bit_create(&bits1, 1024);

    if (st != DS_OK)
        goto error;

    integer_t nbits = bit_nbits(bits1);

    // Creates a bit array of multiples of 2
    for (integer_t i = 0; i < nbits; i++)
    {
        if (i % 2 == 0)
            st = bit_set(bits1, i);
    }

    st += bit_NOT(bits1);

    integer_t sum = 0;
    bool set = false;

    // Get the sum of all numbers that are not multiples of 2
    for (integer_t i = 0; i < nbits; i++)
    {
        st = bit_get(bits1, i, &set);

        if (st != DS_OK)
            goto error;

        sum += (set) ? i : 0; // Sum if bit is 1
    }

    if (st != DS_OK)
        goto error;

    // The sum of all numbers from 0 to 1024 except for multiples of 2 is 262144
    ut_equals_integer_t(ut, 262144, sum, __func__);

    bit_free(&bits1);

    return DS_OK;

    error:
    printf("Error at %s\n", __func__);
    bit_free(&bits1);
    return st;
}

Status bit_test_AND(UnitTest ut)
{
    BitArray bits1 = NULL,
             bits2 = NULL,
             bits3 = NULL;

    Status st = DS_OK;

    st += bit_create(&bits1, 1024);
    st += bit_create(&bits2, 1024);
    st += bit_create(&bits3, 1024);

    if (st != DS_OK)
        goto error;

    integer_t nbits = bit_nbits(bits1);

    // Creates three sets. One of multiples of 2, one of 3's and another of 4's
    for (integer_t i = 0; i < nbits; i++)
    {
        if (i % 2 == 0)
            st = bit_set(bits1, i);
        if (i % 3 == 0)
            st = bit_set(bits2, i);
        if (i % 4 == 0)
            st = bit_set(bits3, i);

        if (st != DS_OK)
            goto error;
    }

    integer_t sum = 0;
    bool set = false;

    // Intersection of multiples of 2, 3 and 4
    // Creates all the multiples of 12
    st += bit_AND(bits1, bits2);
    st += bit_AND(bits1, bits3);

    if (st != DS_OK)
        goto error;

    for (integer_t i = 0; i < nbits; i++)
    {
        st = bit_get(bits1, i, &set);

        if (st != DS_OK)
            goto error;

        sum += (set) ? i : 0; // Sum if bit is 1
    }

    // The sum of all multiples of 12 below 1023 (included) is 43860
    ut_equals_integer_t(ut, 43860, sum, __func__);

    bit_free(&bits1);
    bit_free(&bits2);
    bit_free(&bits3);

    return DS_OK;

    error:
    printf("Error at %s\n", __func__);
    bit_free(&bits1);
    bit_free(&bits2);
    bit_free(&bits3);
    return st;
}

Status bit_test_OR(UnitTest ut)
{
    BitArray bits1 = NULL, bits2 = NULL;

    Status st = DS_OK;

    st += bit_create(&bits1, 1024);
    st += bit_create(&bits2, 1024);

    if (st != DS_OK)
        goto error;

    integer_t nbits = bit_nbits(bits1);

    // Creates two bit arrays where one is the complement of the other
    for (integer_t i = 0; i < nbits; i++)
    {
        if (i % 2 == 0)
            st = bit_set(bits1, i);
        else
            st = bit_set(bits2, i);

        if (st != DS_OK)
            goto error;
    }

    integer_t sum = 0;
    bool set = false;

    // All bits should be set to 1 because one is the complement of the other
    st += bit_OR(bits1, bits2);

    if (st != DS_OK)
        goto error;

    for (integer_t i = 0; i < nbits; i++)
    {
        st = bit_get(bits1, i, &set);

        if (st != DS_OK)
            goto error;

        sum += (set) ? i : 0; // Sum if bit is 1
    }

    // Should equals the sum of all numbers from 0 to 1023 (included) which is
    // 523776
    ut_equals_integer_t(ut, 523776, sum, __func__);

    bit_free(&bits1);
    bit_free(&bits2);

    return DS_OK;

    error:
    printf("Error at %s\n", __func__);
    bit_free(&bits1);
    bit_free(&bits2);
    return st;
}

Status bit_test_XOR(UnitTest ut)
{
    BitArray bits1 = NULL,
            bits2 = NULL;

    Status st = DS_OK;

    st += bit_create(&bits1, 1024);
    st += bit_create(&bits2, 1024);

    if (st != DS_OK)
        goto error;

    integer_t nbits = bit_nbits(bits1);

    // Creates two sets. One of multiples of 2 and one of multiples of 3
    for (integer_t i = 0; i < nbits; i++)
    {
        if (i % 2 == 0)
            st = bit_set(bits1, i);
        if (i % 3 == 0)
            st = bit_set(bits2, i);

        if (st != DS_OK)
            goto error;
    }

    integer_t sum = 0;
    bool set = false;

    // Symmetric difference of multiples of 2 and 3
    st += bit_XOR(bits1, bits2);

    if (st != DS_OK)
        goto error;

    for (integer_t i = 0; i < nbits; i++)
    {
        st = bit_get(bits1, i, &set);

        if (st != DS_OK)
            goto error;

        sum += (set) ? i : 0; // Sum if bit is 1
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
    ut_equals_integer_t(ut, 262145, sum, __func__);

    bit_free(&bits1);
    bit_free(&bits2);

    return DS_OK;

    error:
    printf("Error at %s\n", __func__);
    bit_free(&bits1);
    bit_free(&bits2);
    return st;
}

Status bit_test_NAND(UnitTest ut)
{
    BitArray bits1 = NULL,
            bits2 = NULL;

    Status st = DS_OK;

    st += bit_create(&bits1, 1024);
    st += bit_create(&bits2, 1024);

    if (st != DS_OK)
        goto error;

    integer_t nbits = bit_nbits(bits1);

    // Creates two sets. One of multiples of 2 and one filled with 1 bits
    for (integer_t i = 0; i < nbits; i++)
    {
        if (i % 2 == 0)
            st = bit_set(bits1, i);

        if (st != DS_OK)
            goto error;
    }

    integer_t sum = 0;
    bool set = false;

    // This should give all the numbers that are not multiples of 2
    st += bit_fill(bits2);
    st += bit_NAND(bits1, bits2);

    if (st != DS_OK)
        goto error;

    for (integer_t i = 0; i < nbits; i++)
    {
        st = bit_get(bits1, i, &set);

        if (st != DS_OK)
            goto error;

        sum += (set) ? i : 0; // Sum if bit is 1
    }

    // Sum of all the numbers that are not multiples of 2 up to 1023 (included)
    // is 262144
    ut_equals_integer_t(ut, 262144, sum, __func__);

    bit_free(&bits1);
    bit_free(&bits2);

    return DS_OK;

    error:
    printf("Error at %s\n", __func__);
    bit_free(&bits1);
    bit_free(&bits2);
    return st;
}

Status bit_test_NOR(UnitTest ut)
{
    BitArray bits1 = NULL,
            bits2 = NULL;

    Status st = DS_OK;

    st += bit_create(&bits1, 1024);
    st += bit_create(&bits2, 1024);

    if (st != DS_OK)
        goto error;

    integer_t nbits = bit_nbits(bits1);

    // Creates two sets. One of multiples of 2 and one filled with 0 bits
    for (integer_t i = 0; i < nbits; i++)
    {
        if (i % 2 == 0)
            st = bit_set(bits1, i);

        if (st != DS_OK)
            goto error;
    }

    integer_t sum = 0;
    bool set = false;

    // This should give all the numbers that are not multiples of 2
    st += bit_empty(bits2);
    st += bit_NOR(bits1, bits2);

    if (st != DS_OK)
        goto error;

    for (integer_t i = 0; i < nbits; i++)
    {
        st = bit_get(bits1, i, &set);

        if (st != DS_OK)
            goto error;

        sum += (set) ? i : 0; // Sum if bit is 1
    }

    // Sum of all the numbers that are not multiples of 2 up to 1023 (included)
    // is 262144
    ut_equals_integer_t(ut, 262144, sum, __func__);

    bit_free(&bits1);
    bit_free(&bits2);

    return DS_OK;

    error:
    printf("Error at %s\n", __func__);
    bit_free(&bits1);
    bit_free(&bits2);
    return st;
}

Status bit_test_NXOR(UnitTest ut)
{
    BitArray bits1 = NULL,
            bits2 = NULL;

    Status st = DS_OK;

    st += bit_create(&bits1, 1024);
    st += bit_create(&bits2, 1024);

    if (st != DS_OK)
        goto error;

    integer_t nbits = bit_nbits(bits1);

    // Creates two sets. One of multiples of 2 and one of multiples of 3
    for (integer_t i = 0; i < nbits; i++)
    {
        if (i % 2 == 0)
            st = bit_set(bits1, i);
        if (i % 3 == 0)
            st = bit_set(bits2, i);

        if (st != DS_OK)
            goto error;
    }

    integer_t sum = 0;
    bool set = false;

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
    st += bit_NXOR(bits1, bits2);

    if (st != DS_OK)
        goto error;

    for (integer_t i = 0; i < nbits; i++)
    {
        st = bit_get(bits1, i, &set);

        if (st != DS_OK)
            goto error;

        sum += (set) ? i : 0; // Sum if bit is 1
    }

    // Sum totals up to 261631
    ut_equals_integer_t(ut, 261631, sum, __func__);

    bit_free(&bits1);
    bit_free(&bits2);

    return DS_OK;

    error:
    printf("Error at %s\n", __func__);
    bit_free(&bits1);
    bit_free(&bits2);
    return st;
}

Status bit_test_DIFF(UnitTest ut)
{
    BitArray bits1 = NULL,
            bits2 = NULL;

    Status st = DS_OK;

    st += bit_create(&bits1, 1024);
    st += bit_create(&bits2, 1024);

    if (st != DS_OK)
        goto error;

    integer_t nbits = bit_nbits(bits1);

    // Creates two sets. One of multiples of 2 and one of multiples of 3
    for (integer_t i = 0; i < nbits; i++)
    {
        if (i % 2 == 0)
            st = bit_set(bits1, i);
        if (i % 3 == 0)
            st = bit_set(bits2, i);

        if (st != DS_OK)
            goto error;
    }

    integer_t sum = 0;
    bool set = false;

    // Performs a relative complement of bits1 in bits2 (or what is only in
    // bits1 and not in bits2 sometimes written as bits1 - bits2)
    st += bit_DIFF(bits1, bits2);

    if (st != DS_OK)
        goto error;

    for (integer_t i = 0; i < nbits; i++)
    {
        st = bit_get(bits1, i, &set);

        if (st != DS_OK)
            goto error;

        sum += (set) ? i : 0; // Sum if bit is 1
    }

    // The sum of all multiples of two that are not multiples of 3 is
    ut_equals_integer_t(ut, 174422, sum, __func__);

    bit_free(&bits1);
    bit_free(&bits2);

    return DS_OK;

    error:
    printf("Error at %s\n", __func__);
    bit_free(&bits1);
    bit_free(&bits2);
    return st;
}

// Tests cardinality
Status bit_test_cardinality(UnitTest ut)
{
    BitArray bits;

    Status st = bit_init(&bits);

    if (st != DS_OK)
        return st;

    for (integer_t i = 0; i < bit_nbits(bits); i++)
    {
        if (i % 2 == 0)
        {
            st = bit_set(bits, i);

            if (st != DS_OK)
                goto error;
        }
    }

    ut_equals_integer_t(ut, 32, bit_cardinality(bits), __func__);

    bit_free(&bits);

    return DS_OK;

    error:
    printf("Error at %s\n", __func__);
    bit_free(&bits);
    return st;
}

Status bit_test_fill_empty(UnitTest ut)
{
    BitArray bits;

    Status st = bit_init(&bits);

    if (st != DS_OK)
        return st;

    // Test if calloc sets everything to 0
    ut_equals_bool(ut, true, bit_all_clear(bits), __func__);

    st = bit_fill(bits);

    if (st != DS_OK)
        goto error;

    ut_equals_bool(ut, true, bit_all_set(bits), __func__);

    st = bit_empty(bits);

    if (st != DS_OK)
        goto error;

    ut_equals_bool(ut, true, bit_all_clear(bits), __func__);

    bit_free(&bits);

    return DS_OK;

    error:
    printf("Error at %s\n", __func__);
    bit_free(&bits);
    return st;
}

// Runs all BitArray tests
Status BitArrayTests(void)
{
    UnitTest ut;

    Status st = ut_init(&ut);

    if (st != DS_OK)
        goto error;

    st += bit_test_NOT(ut);
    st += bit_test_AND(ut);
    st += bit_test_OR(ut);
    st += bit_test_XOR(ut);
    st += bit_test_NAND(ut);
    st += bit_test_NOR(ut);
    st += bit_test_NXOR(ut);
    st += bit_test_DIFF(ut);
    st += bit_test_cardinality(ut);
    st += bit_test_fill_empty(ut);

    if (st != DS_OK)
        goto error;

    ut_report(ut, "BitArray");
    ut_delete(&ut);

    return DS_OK;

    error:
    printf("Error at %s\n", __func__);
    ut_report(ut, "BitArray");
    ut_delete(&ut);
    return st;
}