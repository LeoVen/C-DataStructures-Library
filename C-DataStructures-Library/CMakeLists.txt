cmake_minimum_required(VERSION 3.12)
project(C_DataStructures_Library)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextr -std=c11")
set(CMAKE_CXX_STANDARD 11)

set(INCLUDE ./include)
set(UNIT_TEST ./tests/UnitTest)
set(UTIL ./util/include)
include_directories(${INCLUDE} ${UNIT_TEST} ${UTIL})

add_executable(C_DataStructures_Library main.c include/Core.h src/SinglyLinkedList.c include/SinglyLinkedList.h src/DoublyLinkedList.c include/DoublyLinkedList.h src/Stack.c include/Stack.h src/Queue.c include/Queue.h src/Deque.c include/Deque.h src/PriorityQueue.c include/PriorityQueue.h src/DynamicArray.c include/DynamicArray.h src/StackArray.c include/StackArray.h src/QueueArray.c include/QueueArray.h src/Array.c include/Array.h src/CircularLinkedList.c include/CircularLinkedList.h tests/QueueArrayTests.c tests/UnitTest/UnitTest.c tests/UnitTest/UnitTest.h include/Tests.h tests/SinglyLinkedListTests.c tests/DoublyLinkedListTests.c tests/StackArrayTests.c tests/DequeTests.c tests/QueueTests.c tests/CircularLinkedListTests.c tests/DynamicArrayTests.c tests/StackTests.c src/DequeArray.c include/DequeArray.h tests/DequeArrayTests.c src/BinarySearchTree.c include/BinarySearchTree.h util/src/Integers.c util/include/Integers.h util/include/Util.h util/src/Floats.c util/include/Floats.h util/src/Chars.c util/include/Chars.h src/AVLTree.c include/AVLTree.h util/src/Random.c util/include/Random.h src/SortedList.c include/SortedList.h include/CoreSort.h include/CoreTree.h util/src/Status.c util/include/Status.h tests/SortedListTests.c)

target_link_libraries(C_DataStructures_Library m)