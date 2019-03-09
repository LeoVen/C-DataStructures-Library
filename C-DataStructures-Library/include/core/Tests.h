/**
 * @file Tests.h
 * 
 * @author Leonardo Vencovsky (https://github.com/LeoVen)
 *
 * @date 16/10/2018
 */

#ifndef C_DATASTRUCTURES_LIBRARY_TESTS_H
#define C_DATASTRUCTURES_LIBRARY_TESTS_H

#include "Core.h"
#include "UnitTest.h"

#ifdef __cplusplus
extern "C" {
#endif

// Includes all test functions

Status ArrayTests(void);

Status AssociativeListTests(void);

Status AVLTreeTests(void);

Status BinarySearchTreeTests(void);

Status BitArrayTests(void);

Status CircularLinkedListTests(void);

Status DequeArrayTests(void);

Status DequeListTests(void);

Status DoublyLinkedListTests(void);

Status DynamicArrayTests(void);

Status HeapTests(void);

Status PriorityListTests(void);

Status QueueArrayTests(void);

Status QueueListTests(void);

Status RedBlackTreeTests(void);

Status SinglyLinkedListTests(void);

Status SortedListTests(void);

Status StackArrayTests(void);

Status StackListTests(void);

#ifdef __cplusplus
}
#endif

#endif //C_DATASTRUCTURES_LIBRARY_TESTS_H
