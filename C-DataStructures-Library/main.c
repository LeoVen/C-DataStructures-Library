#include "Tests.h"
#include "SinglyLinkedList.h"
#include "DoublyLinkedList.h"
#include "Stack.h"
#include "Queue.h"
#include "Deque.h"
#include "PriorityQueue.h"
#include "DynamicArray.h"
#include "StackArray.h"
#include "QueueArray.h"
#include "Array.h"
#include "CircularBuffer.h"
#include "CircularLinkedList.h"
#include "DequeArray.h"

int main()
{
    srand(time(NULL));

    SinglyLinkedList sll0;

    int j;
    index_t l;

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

            printf("\nLength: %lld\n", sll_length(sll0));
        }
    }

    SinglyLinkedList sll1;

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

    DoublyLinkedList dll;

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

        printf("\nLength: %lld\n", dll_length(dll));
    }

    dll_delete(&dll);

    Stack stk0, stk1;

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
            printf("\nHeight stk0: %lld\n", stk0->height);

            stk_display_array(stk1);
            printf("\nHeight stk1: %lld\n", stk1->height);

        }

    }

    stk_delete(&stk0);
    stk_delete(&stk1);

    Queue que0, que1;

    if (que_init(&que0) == DS_OK)
    {
        for (int i = 0; i < 10; i++)
            que_enqueue(que0, i);

        if (que_copy(que0, &que1) == DS_OK)
        {
            l = que1->length;
            for (index_t i = 0; i < l; i++)
            {
                que_dequeue(que0, &j);

                que_enqueue(que1, j);
            }

            que_display_array(que0);
            printf("\nLength que0: %lld\n", que0->length);

            que_display_array(que1);
            printf("\nLength que1: %lld\n", que1->length);
        }
    }

    que_delete(&que0);
    que_delete(&que1);

    Deque deq0, deq1;

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

            for (index_t i = 0; i < l; i++)
            {
                deq_dequeue_front(deq0, &j);

                deq_enqueue_rear(deq1, j);
            }

            deq_display_array(deq0);
            printf("\nLength deq0: %lld\n", deq0->length);

            deq_display_array(deq1);
            printf("\nLength deq0: %lld\n", deq1->length);
        }
    }

    deq_delete(&deq0);
    deq_delete(&deq1);

    PriorityQueue prq0, prq1;

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
            printf("\nLength prq0: %lld\n", prq0->length);

            prq_display_array(prq1);
            printf("\nLength prq1: %lld\n", prq1->length);

            prq_display_priority(prq1);
        }
    }

    prq_delete(&prq0);
    prq_delete(&prq1);

    DynamicArray dar0, dar1, dar2;

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

            printf("\nArray 0 size: %lld\n", dar0->size);

            if (dar_copy(dar0, &dar1) == DS_OK)
            {
                if (dar_remove(dar1, 11, 13) == DS_OK)
                {
                    dar_display(dar1);

                    printf("\nArray 1 size: %lld\n", dar1->size);
                }
            }
        }
    }

    dar_delete(&dar0);
    dar_delete(&dar1);

    if (dar_init(&dar0) == DS_OK && dar_init(&dar1) == DS_OK && dar_init(&dar2) == DS_OK)
    {
        for (index_t i = 0, j = 9; i < j; i++, j--)
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
        printf("Size: %lld\n", dar0->size);
        dar_display_array(dar1);
        printf("Size: %lld\n", dar1->size);
        dar_display_array(dar2);
        printf("Size: %lld\n", dar2->size);

    }

    dar_delete(&dar0);
    dar_delete(&dar1);
    dar_delete(&dar2);

    StackArray sta0, sta1;

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
        printf("Height: %lld\n\n", sta0->height);

        if (sta_copy(sta0, &sta1) == DS_OK)
        {
            while (sta0->height > 0)
            {
                sta_pop(sta0, &j);

                sta_push(sta1, j);
            }

            sta_display_array(sta0);
            printf("Stack 0 Height: %lld\n\n", sta0->height);

            sta_display_array(sta1);
            printf("Stack 1 Height: %lld", sta1->height);
        }
    }

    sta_delete(&sta0);
    sta_delete(&sta1);

    QueueArray qua0, qua1;

    if (qua_init(&qua0) == DS_OK)
    {
        for (int i = 0; i < 100; i++)
        {
            if (i % 2 == 0)
                qua_enqueue(qua0, i);
            else if (i % 3 == 0)
                qua_dequeue(qua0, &j);
            else
                qua_enqueue(qua0, i);

            printf("\n");

            for (index_t k = 0; k < qua0->capacity; k++)
            {
                printf("%d ", qua0->buffer[k]);
            }

            printf("\nFront     : %lld", qua0->front);
            printf("\nRear      : %lld", qua0->rear);
            printf("\nSize      : %lld", qua0->size);
            printf("\nCapacity  : %lld", qua0->capacity);
            printf("\nFront V   : %d", qua_peek_front(qua0));
            printf("\nRear V    : %d", qua_peek_rear(qua0));
        }

        if (qua_copy(qua0, &qua1) == DS_OK)
        {
            printf("\n\n\nOriginal\n--------\n");
            qua_display(qua0);
            printf("\nSize     : %lld", qua0->size);
            printf("\nCapacity : %lld", qua0->capacity);
            printf("\nFront    : %lld", qua0->front);
            printf("\nRear     : %lld", qua0->rear);
            printf("\n\n\nCopy\n----\n");
            qua_display(qua1);
            printf("\nSize     : %lld", qua1->size);
            printf("\nCapacity : %lld", qua1->capacity);
            printf("\nFront    : %lld", qua1->front);
            printf("\nRear     : %lld\n\n", qua1->rear);
        }
    }

    qua_delete(&qua0);
    qua_delete(&qua1);

    Array array;

    if (arr_init(&array, 20) == DS_OK)
    {
        for (int i = 0; i < 20; i++)
        {
            if (arr_insert(array, (index_t)i, i) != DS_OK)
                break;
        }

        arr_display(array);
    }

    CircularBuffer cbf0, cbf1;

    if (cbf_init(&cbf0, 40) == DS_OK)
    {
        for (int i = 1; i < 80; i++)
        {
            if (i % 3 == 0)
            {
                if (cbf_remove(cbf0, &j) != DS_OK)
                    break;
            }
            else
            {
                if (cbf_insert(cbf0, i) != DS_OK)
                    break;
            }
        }

        cbf_display(cbf0);
        cbf_display_array(cbf0);
    }

    if (cbf_init(&cbf1, 10) == DS_OK)
    {
        for (int i = 0; i < 4; i++)
        {
            cbf_insert(cbf1, 1);
        }

        cbf_remove(cbf1, &j);
        cbf_remove(cbf1, &j);

        while (cbf1->end != 0)
        {
            cbf_insert(cbf1, 1);
        }

        cbf_display(cbf1);
        cbf_display_array(cbf1);

        printf("\nCapacity : %lld", cbf1->capacity);
        printf("\nSize     : %lld", cbf1->size);
        printf("\nStart    : %lld", cbf1->start);
        printf("\nEnd      : %lld\n\n", cbf1->end);

        if (cbf_erase(&cbf1) == DS_OK)
        {
            cbf_insert(cbf1, 10);

            cbf_display(cbf1);
            cbf_display_array(cbf1);

            printf("\nCapacity : %lld", cbf1->capacity);
            printf("\nSize     : %lld", cbf1->size);
            printf("\nStart    : %lld", cbf1->start);
            printf("\nEnd      : %lld\n", cbf1->end);
        }

        if (cbf_delete(&cbf1) == DS_OK)
        {
            if (cbf_copy(cbf0, &cbf1) == DS_OK)
            {
                printf("\nOriginal");
                cbf_display(cbf0);
                printf("\nCapacity : %lld", cbf0->capacity);
                printf("\nSize     : %lld", cbf0->size);
                printf("\nStart    : %lld", cbf0->start);
                printf("\nEnd      : %lld\n", cbf0->end);

                printf("\nCopy");
                cbf_display(cbf1);
                printf("\nCapacity : %lld", cbf1->capacity);
                printf("\nSize     : %lld", cbf1->size);
                printf("\nStart    : %lld", cbf1->start);
                printf("\nEnd      : %lld\n\n", cbf1->end);
            }
        }
    }

    cbf_delete(&cbf0);
    cbf_delete(&cbf1);

    CircularLinkedList cll0, cll1;

    if (cll_init(&cll0) == DS_OK)
    {
        for (int i = 0; i < 10; i++)
            cll_insert_before(cll0, i);

        cll_display(cll0);

        for (int i = 0; i < 10; i++)
        {
            cll_iter_next(cll0, 1);

            cll_display_array(cll0);
        }

        if (cll_copy(cll0, &cll1) == DS_OK)
        {
            printf("\nOriginal: ");
            cll_display(cll0);
            printf("\nCurrent data: %d", cll_peek(cll0));
            printf("\nLength: %lld\n", cll0->length);

            printf("\nCopy: ");
            cll_display(cll1);
            printf("\nCurrent data: %d", cll_peek(cll1));
            printf("\nLength: %lld\n", cll1->length);
        }

        printf("\n");

        while (cll0->length > 0)
        {
            cll_remove_before(cll0, &j);

            printf("%d ", j);
        }
    }

    cll_delete(&cll0);

    DequeArray deque;

    if (dqa_create(&deque, 16, 200) == DS_OK)
    {
        for (int i = 0; i < 20; i++)
        {
            if (i % 2 == 0)
                dqa_enqueue_front(deque, i);
            else
                dqa_enqueue_rear(deque, i);
        }

        dqa_display_array(deque);
        printf("Front element: %d\n", dqa_peek_front(deque));
        printf("Rear element: %d\n\n", dqa_peek_rear(deque));
    }

    dqa_delete(&deque);

    DequeArrayTests();
    DequeTests();
    DoublyLinkedListTests();
    DynamicArrayTests();
    CircularLinkedListTests();
    QueueArrayTests();
    QueueTests();
    SinglyLinkedListTests();
    StackArrayTests();
    StackTests();

    return 0;
}