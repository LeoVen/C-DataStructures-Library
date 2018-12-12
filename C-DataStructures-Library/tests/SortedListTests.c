/**
 * @file SortedListTests.c
 * 
 * @author Leonardo Vencovsky (https://github.com/LeoVen)
 *
 * @date 18/11/2018
 */
 
#include "SortedList.h"
#include "UnitTest.h"
#include "Utility.h"

// Tests insertion
Status sli_test_insertion(UnitTest ut)
{
    SortedList lists[6];

    int arr[7] = {2, 1, 0, 3, 6, 5, 4};

    Status st = DS_OK;

    st += sli_create(&(lists[0]), ASCENDING, compare_int32_t, copy_int32_t, display_int32_t, free);
    st += sli_create(&(lists[1]), ASCENDING, compare_int32_t, copy_int32_t, display_int32_t, free);
    st += sli_create(&(lists[2]), ASCENDING, compare_int32_t, copy_int32_t, display_int32_t, free);
    st += sli_create(&(lists[3]), DESCENDING, compare_int32_t, copy_int32_t, display_int32_t, free);
    st += sli_create(&(lists[4]), DESCENDING, compare_int32_t, copy_int32_t, display_int32_t, free);
    st += sli_create(&(lists[5]), DESCENDING, compare_int32_t, copy_int32_t, display_int32_t, free);

    if (st != DS_OK)
        goto error;

    for (int i = 0; i < 7; i++)
    {
        st += sli_insert(lists[0], new_int32_t(i));          // 0 -> 1 -> 2 -> 3 -> 4 -> 5 -> 6
        st += sli_insert(lists[1], new_int32_t(arr[i]));     // 0 -> 1 -> 2 -> 3 -> 4 -> 5 -> 6
        st += sli_insert(lists[2], new_int32_t(6 - i));      // 0 -> 1 -> 2 -> 3 -> 4 -> 5 -> 6
        st += sli_insert(lists[3], new_int32_t(i));          // 6 -> 5 -> 4 -> 3 -> 2 -> 1 -> 0
        st += sli_insert(lists[4], new_int32_t(arr[6 - i])); // 6 -> 5 -> 4 -> 3 -> 2 -> 1 -> 0
        st += sli_insert(lists[5], new_int32_t(6 - i));      // 6 -> 5 -> 4 -> 3 -> 2 -> 1 -> 0
    }

    if (st != DS_OK)
        goto error;

    void *R0[3] = {NULL, NULL, NULL},
            *R1[3] = {NULL, NULL, NULL},
            *R2[3] = {NULL, NULL, NULL},
            *R3[3] = {NULL, NULL, NULL},
            *R4[3] = {NULL, NULL, NULL},
            *R5[3] = {NULL, NULL, NULL};

    st += sli_get(lists[0], &(R0[0]), 0); // Front
    st += sli_get(lists[0], &(R0[1]), 3); // Middle
    st += sli_get(lists[0], &(R0[2]), 6); // End
    st += sli_get(lists[1], &(R1[0]), 0); // Front
    st += sli_get(lists[1], &(R1[1]), 3); // Middle
    st += sli_get(lists[1], &(R1[2]), 6); // End
    st += sli_get(lists[2], &(R2[0]), 0); // Front
    st += sli_get(lists[2], &(R2[1]), 3); // Middle
    st += sli_get(lists[2], &(R2[2]), 6); // End
    st += sli_get(lists[3], &(R3[0]), 0); // Front
    st += sli_get(lists[3], &(R3[1]), 3); // Middle
    st += sli_get(lists[3], &(R3[2]), 6); // End
    st += sli_get(lists[4], &(R4[0]), 0); // Front
    st += sli_get(lists[4], &(R4[1]), 3); // Middle
    st += sli_get(lists[4], &(R4[2]), 6); // End
    st += sli_get(lists[5], &(R5[0]), 0); // Front
    st += sli_get(lists[5], &(R5[1]), 3); // Middle
    st += sli_get(lists[5], &(R5[2]), 6); // End

    if (st != DS_OK)
        goto error;

    // 0 -> 1 -> 2 -> 3 -> 4 -> 5 -> 6
    ut_equals_int(ut, *(int*)R0[0], 0, __func__);
    ut_equals_int(ut, *(int*)R0[1], 3, __func__);
    ut_equals_int(ut, *(int*)R0[2], 6, __func__);
    ut_equals_int(ut, *(int*)R1[0], 0, __func__);
    ut_equals_int(ut, *(int*)R1[1], 3, __func__);
    ut_equals_int(ut, *(int*)R1[2], 6, __func__);
    ut_equals_int(ut, *(int*)R2[0], 0, __func__);
    ut_equals_int(ut, *(int*)R2[1], 3, __func__);
    ut_equals_int(ut, *(int*)R2[2], 6, __func__);
    // 6 -> 5 -> 4 -> 3 -> 2 -> 1 -> 0
    ut_equals_int(ut, *(int*)R3[0], 6, __func__);
    ut_equals_int(ut, *(int*)R3[1], 3, __func__);
    ut_equals_int(ut, *(int*)R3[2], 0, __func__);
    ut_equals_int(ut, *(int*)R4[0], 6, __func__);
    ut_equals_int(ut, *(int*)R4[1], 3, __func__);
    ut_equals_int(ut, *(int*)R4[2], 0, __func__);
    ut_equals_int(ut, *(int*)R5[0], 6, __func__);
    ut_equals_int(ut, *(int*)R5[1], 3, __func__);
    ut_equals_int(ut, *(int*)R5[2], 0, __func__);

    for (int i = 0; i < 3; i++)
    {
        free(R0[i]);free(R1[i]);free(R2[i]);free(R3[i]);free(R4[i]);free(R5[i]);
    }

    for (int i = 0; i < 6; i++)
    {
        sli_free(&(lists[i]));
    }

    return DS_OK;

    error:
    printf("Error at %s\n", __func__);
    for (int i = 0; i < 6; i++) {
        sli_free(&(lists[i]));
    }
    return st;
}

// Tests all the functions that shouldn't allow lists without the default
// functions set.
Status sli_test_incomplete(UnitTest ut)
{
    SortedList list, slist;

    Status st = sli_init(&list);

    if (st != DS_OK)
        return st;

    void *E = new_int32_t(1);
    void *EL[2] = {new_int32_t(1), new_int32_t(2)};
    void **RL;

    integer_t t;

    ut_equals_int(ut, sli_insert(list, E), DS_ERR_INCOMPLETE_TYPE, __func__);
    ut_equals_int(ut, sli_insert_all(list, EL, 2), DS_ERR_INCOMPLETE_TYPE, __func__);
    ut_equals_int(ut, sli_copy(list, &slist), DS_ERR_INCOMPLETE_TYPE, __func__);
    ut_equals_int(ut, sli_display(list), DS_ERR_INCOMPLETE_TYPE, __func__);
    ut_equals_int(ut, sli_display_array(list), DS_ERR_INCOMPLETE_TYPE, __func__);
    ut_equals_int(ut, sli_display_raw(list), DS_ERR_INCOMPLETE_TYPE, __func__);
    ut_equals_integer_t(ut, sli_index_first(list, E), -3, __func__);
    ut_equals_integer_t(ut, sli_index_last(list, E), -3, __func__);

    // This function (sli_to_array) require that we have at least one element
    // in the list:
    sli_set_v_compare(list, compare_int32_t);
    sli_insert(list, new_int32_t(1)); // Adding it

    // Little hack...
    sli_set_v_compare(list, NULL); // Like we have never set a comparator...

    // Testing
    ut_equals_int(ut, sli_to_array(list, &RL, &t), DS_ERR_INCOMPLETE_TYPE, __func__);

    ut_equals_int(ut, sli_order(list), DESCENDING, __func__);

    free(E);
    free(EL[0]);
    free(EL[1]);

    sli_set_v_free(list, free);
    sli_free(&list);

    return DS_OK;
}

// Tests limit functionality
Status sli_test_limit(UnitTest ut)
{
    SortedList list;

    Status st = sli_create(&list, ASCENDING, compare_int32_t, copy_int32_t,
            display_int32_t, free);

    if (st != DS_OK)
        return st;

    st = sli_set_limit(list, 10);

    if (st != DS_OK)
        goto error;

    void *elem;
    for (int i = 0; i < 20; i++)
    {
        elem = new_int32_t(i);

        st = sli_insert(list, elem);

        if (st == DS_ERR_FULL)
        {
            free(elem);
        }
    }

    ut_equals_int(ut, st, DS_ERR_FULL, __func__);
    ut_equals_integer_t(ut, sli_length(list), sli_limit(list), __func__);
    ut_equals_int(ut, sli_set_limit(list, 9), DS_ERR_INVALID_OPERATION, __func__);

    int *t = new_int32_t(-1);

    ut_equals_int(ut, sli_insert(list, t), DS_ERR_FULL, __func__);

    // Removes the limit
    ut_equals_int(ut, sli_set_limit(list, 0), DS_OK, __func__);
    ut_equals_integer_t(ut, sli_limit(list), 0, __func__);
    ut_equals_int(ut, sli_insert(list, t), DS_OK, __func__);

    sli_free(&list);

    return DS_OK;

    error:
    printf("Error at %s\n", __func__);
    sli_free(&list);
    return st;
}

// Tests indexof functions
Status sli_test_indexof(UnitTest ut)
{
    SortedList list;

    Status st = sli_create(&list, ASCENDING, compare_int32_t, copy_int32_t, display_int32_t, free);

    if (st != DS_OK)
        return st;

    void *elem;

    // 0, 0, 0, 1, 1, 1, 2, 2, 2
    for (int i = 0; i < 9; i++)
    {
        elem = new_int32_t(i % 3);

        st = sli_insert(list, elem);

        if (st != DS_OK)
        {
            free(elem);
            goto error;
        }
    }

    void *r0, *r1, *r2;
    void *d0, *d1, *d2;
    integer_t f1, f2;

    void *n0 = new_int32_t(0), *n1 = new_int32_t(1), *n2 = new_int32_t(2);

    st += sli_get(list, &r0, 0);
    st += sli_get(list, &d0, sli_index_first(list, n0));
    st += sli_get(list, &r1, 3);
    st += sli_get(list, &d1, sli_index_first(list, n1));
    st += sli_get(list, &r2, 6);
    st += sli_get(list, &d2, sli_index_first(list, n2));

    if (st != DS_OK)
        goto free_all;

    ut_equals_int(ut, *(int*)r0, *(int*)d0, __func__);
    ut_equals_int(ut, *(int*)r1, *(int*)d1, __func__);
    ut_equals_int(ut, *(int*)r2, *(int*)d2, __func__);

    free(r0);free(r1);free(r2);
    free(d0);free(d1);free(d2);

    st += sli_get(list, &r0, 2);
    st += sli_get(list, &d0, sli_index_last(list, n0));
    st += sli_get(list, &r1, 5);
    st += sli_get(list, &d1, sli_index_last(list, n1));
    st += sli_get(list, &r2, 8);
    st += sli_get(list, &d2, sli_index_last(list, n2));

    if (st != DS_OK)
        goto free_all;

    ut_equals_int(ut, *(int*)r0, *(int*)d0, __func__);
    ut_equals_int(ut, *(int*)r1, *(int*)d1, __func__);
    ut_equals_int(ut, *(int*)r2, *(int*)d2, __func__);

    void *n3 = new_int32_t(3);

    f1 = sli_index_first(list, n3);
    f2 = sli_index_last(list, n3);

    ut_equals_integer_t(ut, f1, -1, __func__);
    ut_equals_integer_t(ut, f2, -1, __func__);

    free(r0);free(r1);free(r2);
    free(d0);free(d1);free(d2);
    free(n0);free(n1);free(n2);free(n3);

    sli_free(&list);

    return DS_OK;

    free_all:
    free(r0);free(r1);free(r2);
    free(d0);free(d1);free(d2);
    free(n0);free(n1);free(n2);
    goto error;

    error:
    printf("Error at %s\n", __func__);
    sli_free(&list);
    return st;
}

// Runs all SortedList tests
Status SortedListTests(void)
{
    UnitTest ut;

    Status st = ut_init(&ut);

    if (st != DS_OK)
        goto error;

    st += sli_test_insertion(ut);
    st += sli_test_incomplete(ut);
    st += sli_test_limit(ut);
    st += sli_test_indexof(ut);

    if (st != DS_OK)
        goto error;

    ut_report(ut, "SortedList");

    ut_delete(&ut);

    return DS_OK;

    error:
    printf("Error at %s\n", __func__);
    ut_report(ut, "SortedList");
    ut_delete(&ut);
    return st;
}