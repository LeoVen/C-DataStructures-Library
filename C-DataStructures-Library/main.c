#include "DoublyLinkedList.h"
#include "SinglyLinkedList.h"

int main()
{
    SinglyLinkedList *sll;

    int j;

    if (sll_init(&sll) == DS_OK)
    {
        for (int i = 0; i < 100; i++)
            if (sll_insert_tail(sll, i) != DS_OK)
                break;

        for (int i = 0; i < 40; i++)
        {
            if (sll_remove_head(sll, &j) != DS_OK)
                break;
            if (sll_remove_tail(sll, &j) != DS_OK)
                break;
        }

        if (sll_reverse(sll) == DS_OK)
        {
            sll_display_array(sll);

            printf("\nLength: %u\n", sll_length(sll));
        }
    }

    sll_delete(&sll);

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

    return 0;
}