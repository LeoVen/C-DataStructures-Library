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

Status AssociativeListTests(void);

Status AVLTreeTests(void);

Status BitArrayTests(void);

Status DequeArrayTests(void);

Status DequeTests(void);

Status DoublyLinkedListTests(void);

Status DynamicArrayTests(void);

Status CircularLinkedListTests(void);

Status QueueArrayTests(void);

Status QueueTests(void);

Status RedBlackTreeTests(void);

Status SinglyLinkedListTests(void);

Status SortedListTests(void);

Status StackArrayTests(void);

Status StackTests(void);

#ifdef __cplusplus
}
#endif

#endif //C_DATASTRUCTURES_LIBRARY_TESTS_H
