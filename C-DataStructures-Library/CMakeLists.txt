cmake_minimum_required(VERSION 3.9.2)
project(C_DataStructures_Library)

set(CMAKE_STATIC_LIBRARY_PREFIX "DSLib_")
set(CMAKE_STATIC_LIBRARY_SUFFIX "_x64")

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextr -std=c11")
set(CMAKE_CXX_STANDARD 11)

set(INCLUDE ./include)
set(UNIT_TEST ./tests/UnitTest)
set(UTIL ./util/include)
include_directories(${INCLUDE} ${UNIT_TEST} ${UTIL})

set(INCLUDE_FILES include/Core.h include/Tests.h include/CoreSort.h include/CoreTree.h)
set(TEST_FILES
        tests/BitArrayTests.c
        tests/CircularLinkedListTests.c
        tests/DequeArrayTests.c
        tests/DequeTests.c
        tests/DoublyLinkedListTests.c
        tests/DynamicArrayTests.c
        tests/QueueArrayTests.c
        tests/QueueTests.c
        tests/SinglyLinkedListTests.c
        tests/SortedListTests.c
        tests/StackArrayTests.c
        tests/StackTests.c)

add_executable(C_DataStructures_Library tests/main.c ${INCLUDE_FILES} ${TEST_FILES})

add_library(Array include/Array.h src/Array.c)
add_library(AVLTree include/AVLTree.h src/AVLTree.c)
add_library(BinarySearchTree include/BinarySearchTree.h src/BinarySearchTree.c)
add_library(BitArray include/BitArray.h src/BitArray.c)
add_library(CircularLinkedList include/CircularLinkedList.h src/CircularLinkedList.c)
add_library(Deque include/Deque.h src/Deque.c)
add_library(DequeArray include/DequeArray.h src/DequeArray.c)
add_library(DoublyLinkedList include/DoublyLinkedList.h src/DoublyLinkedList.c)
add_library(DynamicArray include/DynamicArray.h src/DynamicArray.c)
add_library(PriorityQueue include/PriorityQueue.h src/PriorityQueue.c)
add_library(Queue include/Queue.h src/Queue.c)
add_library(QueueArray include/QueueArray.h src/QueueArray.c)
add_library(SinglyLinkedList include/SinglyLinkedList.h src/SinglyLinkedList.c)
add_library(SortedList include/SortedList.h src/SortedList.c)
add_library(Stack include/Stack.h src/Stack.c)
add_library(StackArray include/StackArray.h src/StackArray.c)

add_library(UnitTest tests/UnitTest/UnitTest.h tests/UnitTest/UnitTest.c)
add_library(CString util/include/CString.h util/src/CString.c)
add_library(Chars util/include/Chars.h util/src/Chars.c)
add_library(Floats util/include/Floats.h util/src/Floats.c)
add_library(Integers util/include/Integers.h util/src/Integers.c)
add_library(Random util/include/Random.h util/src/Random.c)
add_library(Status util/include/Status.h util/src/Status.c)

set(LIBS Array AVLTree BinarySearchTree BitArray CircularLinkedList Deque DequeArray DoublyLinkedList DynamicArray PriorityQueue Queue QueueArray SinglyLinkedList SortedList Stack StackArray)

set(UTIL Chars Floats Integers Random Status)

add_dependencies(C_DataStructures_Library ${LIBS} UnitTest ${UTIL} CString)

target_link_libraries(C_DataStructures_Library m ${LIBS} UnitTest ${UTIL} CString)