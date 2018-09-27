#include "DoublyLinkedList.h"
#include "SinglyLinkedList.h"
#include "Stack.h"
#include "Queue.h"

int main()
{
    SinglyLinkedList *sll0;

    int j;

    if (sll_init(&sll0) == DS_OK)
    {
        for (int i = 0; i < 100; i++)
            if (sll_insert_tail(sll0, i) != DS_OK)
                break;

        for (int i = 0; i < 40; i++)
        {
            if (sll_remove_head(sll0, &j) != DS_OK)
                break;
            if (sll_remove_tail(sll0, &j) != DS_OK)
                break;
        }

        if (sll_reverse(sll0) == DS_OK)
        {
            sll_display_array(sll0);

            printf("\nLength: %u\n", sll_length(sll0));
        }
    }

    SinglyLinkedList *sll1;

    if (sll_copy(sll0, &sll1) == DS_OK)
    {
        if (sll_reverse(sll0) == DS_OK)
        {
            if (sll_link(sll0, sll1) == DS_OK)
            {
                sll_display_array(sll0);
                sll_display_array(sll1);
            }
        }
    }

    sll_delete(&sll0);
    sll_delete(&sll1);

    DoublyLinkedList *dll;

    if (dll_init(&dll) == DS_OK)
    {
        for (int i = 0; i < 200; i++)
            if (dll_insert_tail(dll, i) != DS_OK)
                break;

        for (int i = 0; i < 90; i++)
        {
            if (dll_remove_head(dll, &j) != DS_OK)
                break;
            if (dll_remove_tail(dll, &j) != DS_OK)
                break;
        }

        dll_display_array(dll);

        printf("\nLength: %u\n", dll_length(dll));
    }

    dll_delete(&dll);

    Stack *stk;

    if (stk_init(&stk) == DS_OK)
    {
        for (int i = 0; i < 300; i++)
        {
            if (i % 3 == 0 || i % 5 == 0)
                stk_pop(stk, &j);
            else
                stk_push(stk, i);
        }
    }

    stk_display_array(stk);

    printf("\nHeight: %u\n", stk->height);

    stk_delete(&stk);

    Queue *que0, *que1;

    if (que_init(&que0) == DS_OK)
    {
        for (int i = 0; i < 10; i++)
            que_enqueue(que0, i);

        if (que_copy(que0, &que1) == DS_OK)
        {
            int l = que1->length;
            for (int i = 0; i < l; i++)
            {
                que_dequeue(que1, &j);

                que_enqueue(que0, j);
            }

            que_display_array(que0);
            printf("\nLength que0: %u\n", que0->length);

            que_display_array(que1);
            printf("\nLength que1: %u\n", que1->length);
        }
    }

    que_delete(&que0);

    return 0;
}