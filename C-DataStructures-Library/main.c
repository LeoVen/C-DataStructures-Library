#include "SinglyLinkedList.h"
#include "DoublyLinkedList.h"
#include "Stack.h"
#include "Queue.h"
#include "Deque.h"
#include "PriorityQueue.h"
#include "DynamicArray.h"
#include "StackArray.h"
#include "QueueArray.h"

int main()
{
    SinglyLinkedList *sll0;

    int j;

    size_t l;

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

    Stack *stk0, *stk1;

    if (stk_init(&stk0) == DS_OK)
    {
        for (int i = 0; i < 300; i++)
        {
            if (i % 3 == 0 || i % 5 == 0)
                stk_pop(stk0, &j);
            else
                stk_push(stk0, i);
        }

        if (stk_copy(stk0, &stk1) == DS_OK)
        {
            while (stk0->height > 0)
            {
                stk_pop(stk0, &j);

                stk_push(stk1, j);
            }

            stk_display_array(stk0);
            printf("\nHeight stk0: %u\n", stk0->height);

            stk_display_array(stk1);
            printf("\nHeight stk1: %u\n", stk1->height);

        }

    }

    stk_delete(&stk0);
    stk_delete(&stk1);

    Queue *que0, *que1;

    if (que_init(&que0) == DS_OK)
    {
        for (int i = 0; i < 10; i++)
            que_enqueue(que0, i);

        if (que_copy(que0, &que1) == DS_OK)
        {
            l = que1->length;
            for (size_t i = 0; i < l; i++)
            {
                que_dequeue(que0, &j);

                que_enqueue(que1, j);
            }

            que_display_array(que0);
            printf("\nLength que0: %u\n", que0->length);

            que_display_array(que1);
            printf("\nLength que1: %u\n", que1->length);
        }
    }

    que_delete(&que0);
    que_delete(&que1);

    Deque *deq0, *deq1;

    if (deq_init(&deq0) == DS_OK)
    {
        for (int i = 0; i < 100; i++)
        {
            if (i % 4 == 0)
                deq_dequeue_front(deq0, &j);
            else if (i % 5 == 0)
                deq_dequeue_rear(deq0, &j);
            else if (i % 3 == 0)
                deq_enqueue_front(deq0, i);
            else
                deq_enqueue_rear(deq0, i);
        }

        if (deq_copy(deq0, &deq1) == DS_OK)
        {
            l = deq0->length;

            for (size_t i = 0; i < l; i++)
            {
                deq_dequeue_front(deq0, &j);

                deq_enqueue_rear(deq1, j);
            }

            deq_display_array(deq0);
            printf("\nLength deq0: %u\n", deq0->length);

            deq_display_array(deq1);
            printf("\nLength deq0: %u\n", deq1->length);
        }
    }

    deq_delete(&deq0);
    deq_delete(&deq1);

    PriorityQueue *prq0, *prq1;

    if (prq_init(&prq0) == DS_OK)
    {
        for (int i = 0; i < 50; i++)
        {
            if (prq_enqueue(prq0, i, i % 6) != DS_OK)
                break;
        }

        if (prq_copy(prq0, &prq1) == DS_OK)
        {
            prq_display_array(prq0);
            printf("\nLength prq0: %u\n", prq0->length);

            prq_display_array(prq1);
            printf("\nLength prq1: %u\n", prq1->length);

            prq_display_priority(prq1);
        }
    }

    prq_delete(&prq0);
    prq_delete(&prq1);

    DynamicArray *dar0, *dar1, *dar2;

    int arr[4] = {5, 5, 5, 5};

    if (dar_init(&dar0) == DS_OK)
    {
        for (int i = 0; i < 15; i++)
        {
            if (dar_insert_back(dar0, i) != DS_OK)
                break;
        }

        dar_display(dar0);

        if (dar_insert(dar0, arr, 4, dar0->size) == DS_OK)
        {
            dar_display(dar0);

            printf("\nArray 0 size: %u\n", dar0->size);

            if (dar_copy(dar0, &dar1) == DS_OK)
            {
                if (dar_remove(dar1, 11, 13) == DS_OK)
                {
                    dar_display(dar1);

                    printf("\nArray 1 size: %u\n", dar1->size);
                }
            }
        }
    }

    dar_delete(&dar0);
    dar_delete(&dar1);

    if (dar_init(&dar0) == DS_OK && dar_init(&dar1) == DS_OK && dar_init(&dar2) == DS_OK)
    {
        for (size_t i = 0, j = 9; i < j; i++, j--)
        {
            if (dar_insert_back(dar0, i) != DS_OK || dar_insert_front(dar1, j) != DS_OK || dar_insert_back(dar2, 9))
                break;
        }

        printf("\nOriginal\n");
        dar_display_array(dar0);
        dar_display_array(dar1);
        dar_display_array(dar2);

        if (dar_append(dar0, dar1) == DS_OK)
        {
            printf("\nAppend dar1 to dar0\ndar0: ");
            dar_display_array(dar0);

            dar_remove(dar0, 5, 9);
        }

        if (dar_prepend(dar1, dar0) == DS_OK)
        {
            printf("\nAppend dar0 to dar1\ndar1: ");
            dar_display_array(dar1);

            dar_remove(dar1, 5, 9);
        }

        if (dar_add(dar0, dar2, 2) == DS_OK)
        {
            printf("\nAdd dar2 to dar0 at index 2\ndar0: ");
            dar_display_array(dar0);

            dar_remove(dar0, 2, dar2->size + 1);
        }

        printf("\nEnd: ");

        dar_display_array(dar0);
        printf("Size: %u\n", dar0->size);
        dar_display_array(dar1);
        printf("Size: %u\n", dar1->size);
        dar_display_array(dar2);
        printf("Size: %u\n", dar2->size);

    }

    dar_delete(&dar0);
    dar_delete(&dar1);
    dar_delete(&dar2);

    StackArray *sta0, *sta1;

    if (sta_init(&sta0) == DS_OK)
    {
        for (int i = 0; i < 100; i++)
        {
            if (i % 3 == 0 || i % 5 == 0)
                sta_pop(sta0, &j);
            else
                sta_push(sta0, i);
        }

        sta_display_array(sta0);
        printf("Height: %u\n\n", sta0->height);

        if (sta_copy(sta0, &sta1) == DS_OK)
        {
            while (sta0->height > 0)
            {
                sta_pop(sta0, &j);

                sta_push(sta1, j);
            }

            sta_display_array(sta0);
            printf("Stack 0 Height: %u\n\n", sta0->height);

            sta_display_array(sta1);
            printf("Stack 1 Height: %u", sta1->height);
        }
    }

    sta_delete(&sta0);
    sta_delete(&sta1);

    QueueArray *qua0, *qua1;

    if (qua_init(&qua0) == DS_OK)
    {
        for (int i = 0; i < 100; i++)
        {
            if (i % 2 == 0)
                qua_enqueue(qua0, i);
            else if (i % 3 == 0)
                qua_dequeue(qua0, &j);

            if (i % 15 == 0)
            {
                printf("\n");
                qua_display_array(qua0);

                printf("Length   : %u", qua0->length);
                printf("\nCapacity : %u", qua0->capacity);
                printf("\nFront    : %u", qua0->front);
                printf("\nRear     : %u", qua0->rear);

            }
        }

        if (qua_copy(qua0, &qua1) == DS_OK)
        {
            printf("\n\n\nOriginal\n--------");
            qua_display(qua0);
            printf("\nLength   : %u", qua0->length);
            printf("\nCapacity : %u", qua0->capacity);
            printf("\nFront    : %u", qua0->front);
            printf("\nRear     : %u", qua0->rear);
            printf("\n\n\nCopy\n----");
            qua_display(qua1);
            printf("\nLength   : %u", qua1->length);
            printf("\nCapacity : %u", qua1->capacity);
            printf("\nFront    : %u", qua1->front);
            printf("\nRear     : %u", qua1->rear);
        }
    }

    qua_delete(&qua0);
    qua_delete(&qua1);

    return 0;
}