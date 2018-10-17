/**
 * @file SinglyLinkedListTests.c
 * 
 * @author Leonardo Vencovsky (https://github.com/LeoVen)
 *
 * @date 17/10/2018
 */

#include "SinglyLinkedList.h"
#include "UnitTest.h"

// Tests insertions and removals at the middle of the list
Status sll_test_middle(UnitTest ut)
{
    SinglyLinkedList list;

    Status st = sll_init(&list);

    if (st != DS_OK)
        return st;

    for (int i = 0; i < 10; i++)
    {
        st = sll_insert_tail(list, i);

        if (st != DS_OK)
            goto error;
    }

    int j;
    st += sll_insert_at(list, 99, 5);
    st += sll_remove_at(list, &j, 5);

    if (st != DS_OK)
        goto error;

    ut_equals_int(ut, j, 99, "sll_test_middle");

    sll_delete(&list);

    return DS_OK;

    error:
    sll_delete(&list);
    return st;
}

// Runs all SinglyLinkedList tests
Status SinglyLinkedListTests(void)
{
    UnitTest ut;

    Status st = ut_init(&ut);

    if (st != DS_OK)
        goto error;

    st = sll_test_middle(ut);

    if (st != DS_OK)
        goto error;

    ut_report(ut, "SinglyLinkedList");

    ut_delete(&ut);

    return DS_OK;

    error:
    ut_report(ut, "SinglyLinkedList");
    ut_delete(&ut);
    return st;
}