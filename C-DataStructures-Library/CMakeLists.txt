cmake_minimum_required(VERSION 3.12)
project(C_DataStructures_Library C)

set(CMAKE_C_STANDARD 11)

set(SOURCE ./include)

include_directories(${SOURCE})

add_executable(C_DataStructures_Library main.c include/Core.h src/SinglyLinkedList.c include/SinglyLinkedList.h src/DoublyLinkedList.c include/DoublyLinkedList.h src/Stack.c include/Stack.h include/Queue.c include/Queue.h)