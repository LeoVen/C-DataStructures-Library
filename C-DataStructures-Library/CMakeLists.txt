cmake_minimum_required(VERSION 3.9.2)
project(C_DataStructures_Library)

set(CMAKE_STATIC_LIBRARY_PREFIX "DSLib_")
set(CMAKE_STATIC_LIBRARY_SUFFIX "_x64")

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextr -std=c11")
set(CMAKE_CXX_STANDARD 11)

set(INCLUDE ./include)
set(INCLUDE_UNIT_TEST ./tests/UnitTest)
set(INCLUDE_UTIL ./util/include)
set(INCLUDE_INTERFACE ./interface)
include_directories(${INCLUDE} ${INCLUDE_UNIT_TEST} ${INCLUDE_UTIL} ${INCLUDE_INTERFACE})

set(INCLUDE_TETS_FILES ${INCLUDE} ${INCLUDE_UNIT_TEST} ${INCLUDE_UTIL} ${INCLUDE_INTERFACE})
set(TEST_FILES
        tests/AVLTreeTests.c
        tests/BitArrayTests.c
        tests/CircularLinkedListTests.c
        tests/DequeArrayTests.c
        tests/DequeTests.c
        tests/DoublyLinkedListTests.c
        tests/DynamicArrayTests.c
        tests/QueueArrayTests.c
        tests/QueueTests.c
        tests/RedBlackTreeTests.c
        tests/SinglyLinkedListTests.c
        tests/SortedListTests.c
        tests/StackArrayTests.c
        tests/StackTests.c)
set(INCLUDE_BENCHMARK_FILES ${INCLUDE} ${INCLUDE_UNIT_TEST} ${INCLUDE_UTIL} ${INCLUDE_INTERFACE})
set(BENCHMARK_FILES
        benchmarks/AVLTreeBench.c
        benchmarks/RedBlackTreeBench.c)

add_executable(C_DataStructures_Library_Tests tests/main.c ${INCLUDE_TETS_FILES} ${TEST_FILES})
add_executable(C_DataStructures_Library_Benchmarks benchmarks/main.c ${INCLUDE_BENCHMARK_FILES} ${BENCHMARK_FILES})

add_library(Array              include/Array.h              src/Array.c)
add_library(AVLTree            include/AVLTree.h            src/AVLTree.c)
add_library(BinarySearchTree   include/BinarySearchTree.h   src/BinarySearchTree.c)
add_library(BitArray           include/BitArray.h           src/BitArray.c)
add_library(CircularLinkedList include/CircularLinkedList.h src/CircularLinkedList.c)
add_library(Deque              include/Deque.h              src/Deque.c)
add_library(DequeArray         include/DequeArray.h         src/DequeArray.c)
add_library(DoublyLinkedList   include/DoublyLinkedList.h   src/DoublyLinkedList.c)
add_library(DynamicArray       include/DynamicArray.h       src/DynamicArray.c)
add_library(PriorityQueue      include/PriorityQueue.h      src/PriorityQueue.c)
add_library(Queue              include/Queue.h              src/Queue.c)
add_library(QueueArray         include/QueueArray.h         src/QueueArray.c)
add_library(RedBlackTree       include/RedBlackTree.h       src/RedBlackTree.c)
add_library(SinglyLinkedList   include/SinglyLinkedList.h   src/SinglyLinkedList.c)
add_library(SortedList         include/SortedList.h         src/SortedList.c)
add_library(Stack              include/Stack.h              src/Stack.c)
add_library(StackArray         include/StackArray.h         src/StackArray.c)

add_library(UnitTest tests/UnitTest/UnitTest.h tests/UnitTest/UnitTest.c)

add_library(Interface interface/Interface.h interface/Interface.c)

add_library(Utility  util/include/Utility.h  util/src/Utility.c)
add_library(CString  util/include/CString.h  util/src/CString.c)
add_library(Clock    util/include/Clock.h    util/src/Clock.c)

set(LIBS Array AVLTree BinarySearchTree BitArray CircularLinkedList Deque DequeArray DoublyLinkedList DynamicArray PriorityQueue Queue QueueArray RedBlackTree SinglyLinkedList SortedList Stack StackArray)
set(UTIL Utility CString Clock)

add_dependencies(C_DataStructures_Library_Tests ${LIBS} ${UTIL} UnitTest Interface)

target_link_libraries(Utility m)
target_link_libraries(C_DataStructures_Library_Tests ${LIBS} ${UTIL} UnitTest Interface)
target_link_libraries(C_DataStructures_Library_Benchmarks ${LIBS} ${UTIL} UnitTest Interface)
