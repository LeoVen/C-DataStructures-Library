cmake_minimum_required(VERSION 3.12)
project(C_DataStructures_Library C)

set (CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -Wextra")

set(CMAKE_C_STANDARD 11)

set(SOURCE ./include)

include_directories(${SOURCE})

add_executable(C_DataStructures_Library main.c include/Core.h src/SinglyLinkedList.c include/SinglyLinkedList.h src/DoublyLinkedList.c include/DoublyLinkedList.h src/Stack.c include/Stack.h src/Queue.c include/Queue.h src/Deque.c include/Deque.h src/PriorityQueue.c include/PriorityQueue.h src/DynamicArray.c include/DynamicArray.h src/StackArray.c include/StackArray.h src/QueueArray.c include/QueueArray.h src/Array.c include/Array.h src/CircularBuffer.c include/CircularBuffer.h src/CircularLinkedList.c include/CircularLinkedList.h)