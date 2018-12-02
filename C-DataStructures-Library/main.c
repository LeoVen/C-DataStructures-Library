#include "Util.h"
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
#include "CircularLinkedList.h"
#include "DequeArray.h"
#include "BinarySearchTree.h"
#include "AVLTree.h"
#include "SortedList.h"

int main()
{
    srand(time(NULL));

    Status st;

    SinglyLinkedList sll0;

    int j;
    integer_t l;
    void *result, *element;

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

    Queue que0, que1;

    if (que_create(&que0, compare_int, copy_int, display_int, free) == DS_OK)
    {
        for (int i = 0; i < 10; i++)
            que_enqueue(que0, new_int(i));

        printf("\nOriginal Queue:");
        que_display_array(que0);

        if (que_copy(que0, &que1) == DS_OK)
        {
            printf("\nCopied Queue:");
            que_display_array(que1);

            l = que_length(que1);
            for (integer_t i = 0; i < l; i++)
            {
                que_dequeue(que0, &result);

                que_enqueue(que1, result);
            }

            que_display_array(que0);
            printf("\nLength que0: %lld\n", que_length(que0));

            que_display_array(que1);
            printf("\nLength que1: %lld\n", que_length(que1));
        }
    }

    que_free(&que0);
    que_free(&que1);

    Deque deq0, deq1;

    if (deq_create(&deq0, compare_int, copy_int, display_int, free) == DS_OK)
    {
        for (int i = 0; i < 100; i++)
        {
            if (i % 4 == 0 && !deq_empty(deq0))
            {
                deq_dequeue_front(deq0, &result);

                free(result);
            }
            else if (i % 5 == 0 && !deq_empty(deq0))
            {
                deq_dequeue_rear(deq0, &result);

                free(result);
            }
            else if (i % 3 == 0)
            {
                element = new_int(i);

                st = deq_enqueue_front(deq0, element);

                if (st != DS_OK)
                    free(element);
            }
            else
            {
                element = new_int(i);

                st = deq_enqueue_rear(deq0, element);

                if (st != DS_OK)
                    free(element);
            }
        }

        if (deq_copy(deq0, &deq1) == DS_OK)
        {
            l = deq_length(deq0);

            for (integer_t i = 0; i < l; i++)
            {
                deq_dequeue_front(deq0, &result);

                deq_enqueue_rear(deq1, result);
            }

            deq_display_array(deq0);
            printf("Length deq0: %lld\n", deq_length(deq0));

            deq_display_array(deq1);
            printf("Length deq1: %lld\n", deq_length(deq1));

            deq_free(&deq1);
        }

        deq_free(&deq0);
    }

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

    void *arr[4] = {new_int(5), new_int(5), new_int(5), new_int(5)};

    if(dar_create(&dar0,32,200,compare_int,copy_int,display_int,free) == DS_OK)
    {
        for (int i = 0; i < 15; i++)
        {
            element = new_int(i);

            st = dar_insert_back(dar0, element);

            if (st != DS_OK)
            {
                free(element);
                status_print(st);
            }
        }

        dar_display(dar0);

        if (dar_insert(dar0, arr, 4, dar_size(dar0)) == DS_OK)
        {
            dar_display(dar0);

            printf("\nArray 0 size: %lld\n", dar_size(dar0));

            if (dar_copy(dar0, &dar1) == DS_OK)
            {
                if (dar_delete(dar1, 8, 13) == DS_OK)
                {
                    dar_display(dar1);

                    printf("\nArray 1 size: %lld\n", dar_size(dar1));
                }

                dar_free(&dar1);
            }
        }
        dar_free(&dar0);
    }

    if(dar_create(&dar0,32,200,compare_int,copy_int,display_int,free) == DS_OK
    && dar_create(&dar1,32,200,compare_int,copy_int,display_int,free) == DS_OK
    && dar_create(&dar2,32,200,compare_int,copy_int,display_int,free) == DS_OK)
    {
        for (int i = 0, k = 9; i < k; i++, k--)
        {
            element = new_int(i);

            st = dar_insert_back(dar0, element);

            if (st != DS_OK)
            {
                status_print(st);
                free(element);
            }

            element = new_int(k);

            st = dar_insert_front(dar1, element);

            if (st != DS_OK)
            {
                status_print(st);
                free(element);
            }

            element = new_int(9);

            st = dar_insert_back(dar2, element);

            if (st != DS_OK)
            {
                status_print(st);
                free(element);
            }
        }

        printf("\nOriginal\n");
        dar_display_array(dar0);
        dar_display_array(dar1);
        dar_display_array(dar2);

        if (dar_append(dar0, dar1) == DS_OK)
        {
            printf("\nAppend dar1 to dar0\ndar0: ");
            dar_display_array(dar0);

            //dar_delete(dar0, 5, 9);
            printf("SIZE 0 : %lld\n", dar_size(dar0));
            printf("SIZE 1 : %lld\n", dar_size(dar1));
        }

        if (dar_prepend(dar1, dar0) == DS_OK)
        {
            printf("\nAppend dar0 to dar1\ndar1: ");
            dar_display_array(dar1);

            //dar_delete(dar1, 5, 9);
            printf("SIZE 0 : %lld\n", dar_size(dar0));
            printf("SIZE 1 : %lld\n", dar_size(dar1));
        }

        if (dar_add(dar1, dar2, 2) == DS_OK)
        {
            printf("\nAdd dar2 to dar1 at index 2\ndar0: ");
            dar_display_array(dar1);

            //dar_delete(dar0, 2, dar_size(dar2) + 1);
            printf("SIZE 0 : %lld\n", dar_size(dar0));
            printf("SIZE 2 : %lld\n", dar_size(dar2));
        }

        printf("\nEnd: ");

        dar_display_array(dar0);
        printf("Size: %lld\n", dar_size(dar0));
        dar_display_array(dar1);
        printf("Size: %lld\n", dar_size(dar1));
        dar_display_array(dar2);
        printf("Size: %lld\n", dar_size(dar2));

        dar_free(&dar0);
        dar_free(&dar1);
        dar_free(&dar2);
    }

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

            for (integer_t k = 0; k < qua0->capacity; k++)
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

    CircularLinkedList cll0, cll1;

    if (cll_create(&cll0, compare_int, copy_int, display_int, free) == DS_OK)
    {
        for (int i = 0; i < 10; i++)
        {
            element =  new_int(i);

            st = cll_insert_before(cll0, element);

            if (st != DS_OK)
                free(element);
        }

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
            printf("\nCurrent data: %d", *(int*)cll_peek(cll0));
            printf("\nLength: %lld\n", cll_length(cll0));

            printf("\nCopy: ");
            cll_display(cll1);
            printf("\nCurrent data: %d", *(int*)cll_peek(cll1));
            printf("\nLength: %lld\n", cll_length(cll1));
        }

        printf("\n");

        while (cll_length(cll0) > 0)
        {
            cll_remove_before(cll0, &result);

            printf("%d ", *(int*)result);

            free(result);
        }
    }

    cll_free(&cll0);
    cll_free(&cll1);

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

    BinarySearchTree tree;

    int max1 = 0, max2 = 0, temp;

    if (bst_init(&tree) == DS_OK)
    {
        for (int i = 0; i < 100; i++)
        {
            temp = rand() % 193;

            max1 += temp;

            if (bst_insert(tree, temp) != DS_OK)
                break;
        }

        bst_display(tree, 3);

        while (!bst_empty(tree))
        {
            if (bst_pop(tree, &j) != DS_OK)
                break;

            max2 += j;
        }

        printf("\nTotal inserted values : %d", max1);
        printf("\nTotal removed values  : %d", max2);
    }

    bst_delete(&tree);

    if (bst_init(&tree) == DS_OK)
    {
        int total_in = 0, total_out = 0;
        unsigned count;

        for (int i = 0; i <= 10; i++)
        {
            for (int k = 0; k <= 10; k++)
            {
                if (bst_insert(tree, k) != DS_OK)
                    break;

                total_in += k;
            }
        }

        for (int i = 0; i <= 10; i++)
        {
            bst_remove_all(tree, i, &count);

            total_out += i * count;
        }

        printf("\nTotal In  : %d", total_in);
        printf("\nTotal Out : %d", total_out);
    }

    bst_delete(&tree);

    AVLTree avl;

    max1 = 0, max2 = 0;

    if (avl_init(&avl) == DS_OK)
    {
        for (int i = 0; i < 100; i++)
        {
            temp = rand() % 193;

            max1 += temp;

            if (avl_insert(avl, temp) != DS_OK)
            {
                printf("\nSOMETHING WENT WRONG");

                break;
            }
        }

        avl_display(avl, 3);

        while (!avl_empty(avl))
        {
            if (avl_pop(avl, &j) != DS_OK)
            {
                printf("\nSOMETHING WENT WRONG");

                break;
            }

            max2 += j;
        }

        printf("\nTotal inserted values : %d", max1);
        printf("\nTotal removed values  : %d", max2);
    }

    avl_delete(&avl);

    long long total = 0, rands = 100000;
    for (int i = 0; i < rands; i++)
        total += rand_long(-20, 20);

    printf("\nTotal: %lld", total);

    SortedList slist;

    int *content, *max, *min;

    if (sli_create(&slist, ASCENDING, compare_int, copy_int, display_int, free) == DS_OK)
    {
        sli_set_limit(slist, 40);

        for (int i = 60; i > 0; i--)
        {
            content = new_int(rand());

            st = sli_insert(slist, content);

            if (st == DS_ERR_FULL)
                free(content); // Could not add it to the list

            if (st != DS_ERR_FULL && st != DS_OK)
                printf("ERROR");
        }

        sli_display(slist);

        // READONLY!!
        // Don't free this element. It is still in the list
        max = sli_max(slist);
        min = sli_min(slist);

        if (max && min)
        {
            printf("\nMAX: %d", *max);
            printf("\nMIN: %d", *min);
            printf("\nLIST LENGTH : %lld", sli_length(slist));
            printf("\nLIST LIMIT  : %lld", sli_limit(slist));
            printf("\n\n");
        }
    }

    sli_erase(&slist);

    for (int i = 0; i < 20; i++)
    {
        sli_insert(slist, new_int(rand() % 10));
    }

    sli_display_array(slist);

    int *search = new_int(1);

    printf("\nFIRST INDEX OF %d : %lld", *search, sli_index_first(slist, search));
    printf("\nLAST  INDEX OF %d : %lld", *search, sli_index_last(slist, search));

    free(search);

    sli_erase(&slist);

    sli_set_order(slist, DESCENDING);

    sli_insert(slist, new_int(10));
    sli_insert(slist, new_int(8));
    sli_insert(slist, new_int(11));
    sli_insert(slist, new_int(9));

    sli_display_raw(slist);

    sli_free(&slist);

    int *elem;
    if (sli_create(&slist, ASCENDING, compare_int, copy_int, display_int, free) == DS_OK)
    {
        for (int i = 0; i < 10000; i++)
        {
            elem = new_int(rand());

            st = sli_insert(slist, elem);

            if (st != DS_OK)
            {
                free(elem);

                printf("ERROR\n");
            }
        }

        sli_display_array(slist);

        SortedListIterator sli_iter;
        bool removed = false;

        if (sli_iter_init(&sli_iter, slist) == DS_OK)
        {
            // Using iterator to remove all even numbers (except for the last number)
            while (sli_iter_has_next(sli_iter))
            {
                sli_iter_next(sli_iter);

                elem = sli_iter_peek_prev(sli_iter);

                if (elem != NULL)
                {
                    if ((*elem) % 2 == 0)
                    {
                        st = sli_iter_remove_prev(sli_iter, &result);

                        if (st != DS_OK)
                            printf("ERROR\n");

                        free(result);
                    }
                }
            }

            sli_display_array(slist);

            // Rewind
            sli_iter_to_head(sli_iter);

            // Remove all numbers divisible by 3
            while (sli_iter_has_next(sli_iter))
            {
                elem = sli_iter_peek(sli_iter);

                if (elem != NULL)
                {
                    if ((*elem) % 3 == 0)
                    {
                        st = sli_iter_remove_curr(sli_iter, &result);

                        if (st != DS_OK)
                            printf("ERROR\n");

                        free(result);

                        removed = true;
                    }
                    else
                        removed = false;
                }

                if (!removed)
                    sli_iter_next(sli_iter);
            }

            sli_display_array(slist);

            // Rewind
            sli_iter_to_head(sli_iter);

            // Remove all numbers divisible by 5 (except for the first number)
            while (sli_iter_has_next(sli_iter))
            {
                elem = sli_iter_peek_next(sli_iter);

                if (elem != NULL)
                {
                    if ((*elem) % 5 == 0)
                    {
                        st = sli_iter_remove_next(sli_iter, &result);

                        if (st != DS_OK)
                            status_print(st);

                        free(result);

                        removed = true;
                    }
                    else
                        removed = false;
                }

                if (!removed)
                    sli_iter_next(sli_iter);
            }

            sli_display_array(slist);

            while (!sli_empty(slist))
            {
                st = sli_remove(slist, &result, rand() % sli_length(slist));

                if (st != DS_OK)
                    status_print(st);

                free(result);
            }

            // Should be empty
            sli_display_array(slist);
        }

        sli_iter_free(&sli_iter);
    }

    sli_free(&slist);

    if (sli_create(&slist, ASCENDING, compare_int, copy_int, display_int, free) == DS_OK)
    {
        for (int i = 0; i < 20; i++)
        {
            sli_insert(slist, new_int(i));
        }

        SortedList new_slist;
        if (sli_unlink(slist, &new_slist, sli_length(slist) / 2) == DS_OK)
        {
            sli_display_array(slist);
            printf("Length: %lld\n", sli_length(slist));
            max = sli_max(slist);
            min = sli_min(slist);
            if (max && min)
            {
                printf("MAX: %d\n", *max);
                printf("MIN: %d\n", *min);
            }

            sli_reverse(new_slist);

            sli_display_array(new_slist);
            printf("Length: %lld\n", sli_length(new_slist));
            max = sli_max(new_slist);
            min = sli_min(new_slist);
            if (max && min)
            {
                printf("MAX: %d\n", *max);
                printf("MIN: %d\n", *min);
            }

            sli_free(&new_slist);
        }
    }

    void **result_array;

    printf("\n\nsli_to_array:\n");
    if (sli_to_array(slist, &result_array, &l) == DS_OK)
    {
        for (integer_t i = 0; i < l; i++)
        {
            display_int(result_array[i]);

            printf(" ");
        }

        for (integer_t i = 0; i < l; i++)
            free(result_array[i]);

        free(result_array);

        printf("\nOriginal List:");
        sli_display_array(slist);
    }

    if (sli_erase(&slist) == DS_OK)
    {
        printf("\n\nSublist\n\n");

        for (int i = 0; i < 20; i++)
            sli_insert(slist, new_int(i));

        printf("\nOriginal List:");
        sli_display_array(slist);

        SortedList sli_sub;

        if (sli_sublist(slist, &sli_sub, 5, 14) == DS_OK)
        {
            printf("\nNew original list:");
            sli_display_array(slist);
            printf("Length: %lld\n", sli_length(slist));
            max = sli_max(slist);
            min = sli_min(slist);
            if (max && min)
            {
                printf("MAX: %d\n", *max);
                printf("MIN: %d\n", *min);
            }

            printf("\nCreated sublist:");
            sli_display_array(sli_sub);
            printf("Length: %lld\n", sli_length(sli_sub));
            max = sli_max(sli_sub);
            min = sli_min(sli_sub);
            if (max && min)
            {
                printf("MAX: %d\n", *max);
                printf("MIN: %d\n", *min);
            }

            sli_free(&sli_sub);
        }
        else
            printf("\nERROR");
    }

    sli_free(&slist);

    Array array;

    if (arr_create(&array, 20, compare_int, copy_int, display_int, free) == DS_OK)
    {
        for (int i = 0; i < arr_length(array); i++)
        {
            if (arr_set(array, (integer_t)i, new_int(i)) != DS_OK)
                break;
        }

        arr_display(array);
    }

    arr_free(&array);

    if (arr_create(&array, 1000, compare_int, copy_int, display_int, free) == DS_OK)
    {
        for (int i = 0; i < arr_length(array); i++)
        {
            if (arr_set(array, (integer_t)i, new_int(rand())) != DS_OK)
                break;
        }

        printf("\nOriginal Array");
        arr_display(array);

        if (arr_sort(array) == DS_OK)
        {
            printf("\nSorted Array");
            arr_display(array);
        }
    }

    arr_free(&array);


    Stack stk0, stk1;

    if (stk_create(&stk0, compare_int, copy_int, display_int, free) == DS_OK)
    {
        for (int i = 0; i < 300; i++)
        {
            if (i % 3 == 0 || i % 5 == 0)
            {
                stk_slice(stk0);
            }
            else
                stk_push(stk0, new_int(i));
        }

        printf("\nOriginal Stack");
        stk_display_array(stk0);

        if (stk_copy(stk0, &stk1) == DS_OK)
        {
            printf("\nCopied Stack");
            stk_display_array(stk1);

            while (stk_height(stk0) > 0)
            {
                stk_remove(stk0, &result);

                stk_push(stk1, result);
            }

            stk_display_array(stk0);
            printf("\nHeight stk0: %lld\n", stk_height(stk0));

            stk_display_array(stk1);
            printf("\nHeight stk1: %lld\n", stk_height(stk1));

        }

    }

    stk_free(&stk0);
    stk_free(&stk1);

    // Wrapper showcase
    SortedListWrapper listw;

    if (sli_create(&slist, ASCENDING, compare_int, copy_int, display_int, free) == DS_OK)
    {
        if (sli_wrap_init(&listw, slist) == DS_OK)
        {
            for (int i = 0; i < 100; i++)
            {
                listw->insert(new_int(rand_int(-100, 100)));
            }

            listw->display_array();

            printf("\n");
            while (!listw->empty())
            {
                if (listw->remove_max(&result) == DS_OK)
                {
                    display_int(result);
                    printf(" ");
                    free(result);
                }
            }

            sli_wrap_free(&listw);
        }

        sli_free(&slist);
    }

    printf("\n\n\n");

    DequeArrayTests();
    DequeTests();
    DoublyLinkedListTests();
    DynamicArrayTests();
    CircularLinkedListTests();
    QueueArrayTests();
    QueueTests();
    SinglyLinkedListTests();
    SortedListTests();
    StackArrayTests();
    StackTests();

    return 0;
}