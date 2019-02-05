/**
 * @file AssociativeListTests.c
 * 
 * @author Leonardo Vencovsky (https://github.com/LeoVen)
 *
 * @date 28/12/2018
 */

#include "AssociativeList.h"
#include "UnitTest.h"
#include "Utility.h"

void ali_test_IO(UnitTest ut)
{
    Interface_t *string_interface = interface_new(compare_string, copy_string,
            display_string, free, hash_string, NULL);
    Interface_t *double_interface = interface_new(compare_double, copy_double,
            display_double, free, hash_double, NULL);

    AssociativeList_t *list= ali_new(string_interface, double_interface, false);

    if (!list || !string_interface || !double_interface)
        goto error;

    if (!ali_insert(list, new_string("Apple"), new_double(0.49)))
        goto error;
    if (!ali_insert(list, new_string("Grape Juice"), new_double(1.29)))
        goto error;
    if (!ali_insert(list, new_string("Maple Syrup"), new_double(2.99)))
        goto error;
    if (!ali_insert(list, new_string("Soybeans"), new_double(0.99)))
        goto error;

    double *result[2] = {ali_get(list, "Apple"), ali_get(list, "Maple Syrup")};

    ut_equals_double(ut, 0.49, *result[0], __func__);
    ut_equals_double(ut, 2.99, *result[1], __func__);

    char *str_k = new_string("Grape Juice");
    double *dbl_v = new_double(1.99);

    ut_equals_bool(ut, false, ali_insert(list, str_k, dbl_v), __func__);

    void *R = NULL;

    if (!ali_remove(list, "Apple", &R))
        goto error;
    free(R);
    if (!ali_remove(list, "Soybeans", &R))
        goto error;
    free(R);
    if (!ali_pop(list, "Grape Juice"))
        goto error;
    if (!ali_pop(list, "Maple Syrup"))
        goto error;

    ut_equals_integer_t(ut, 0, ali_length(list), __func__);

    if (!ali_insert(list, str_k, dbl_v))
        goto error;

    ut_equals_integer_t(ut, 1, ali_length(list), __func__);

    ali_free(list);
    interface_free(string_interface);
    interface_free(double_interface);

    return;

    error:
    printf("Error at %s\n", __func__);
    ali_free(list);
    interface_free(string_interface);
    interface_free(double_interface);
    ut_error();
}

// Runs all AssociativeList tests
Status AssociativeListTests(void)
{
    UnitTest ut;

    Status st = ut_init(&ut);

    if (st != DS_OK)
        goto error;

    ali_test_IO(ut);

    ut_report(ut, "AssociativeList");

    ut_delete(&ut);

    return DS_OK;

    error:
    printf("Error at %s\n", __func__);
    ut_report(ut, "AssociativeList");
    ut_delete(&ut);
    return st;
}