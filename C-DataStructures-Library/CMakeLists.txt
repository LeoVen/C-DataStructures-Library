cmake_minimum_required(VERSION 3.9.2)
project(C_DataStructures_Library)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -std=c11")
set(CMAKE_CXX_STANDARD 11)

set(INCLUDE ./include)
set(INCLUDE_CORE ./include/core)
set(INCLUDE_UNIT_TEST ./tests/UnitTest)
set(INCLUDE_UTIL ./util/include)
set(INCLUDE_INTERFACE ./interface)
include_directories(
        ${INCLUDE}
        ${INCLUDE_CORE}
        ${INCLUDE_UNIT_TEST}
        ${INCLUDE_UTIL}
        ${INCLUDE_INTERFACE}
)

set(INCLUDE_TETS_FILES
        ${INCLUDE}
        ${INCLUDE_UNIT_TEST}
        ${INCLUDE_UTIL}
        ${INCLUDE_INTERFACE}
)
file(GLOB TEST_FILES
        "./tests/*.c"
)

set(INCLUDE_BENCHMARK_FILES
        ${INCLUDE}
        ${INCLUDE_UNIT_TEST}
        ${INCLUDE_UTIL}
        ${INCLUDE_INTERFACE}
)
set(BENCHMARK_FILES
        benchmarks/AssociativeListBench.c
        benchmarks/AVLTreeBench.c
        benchmarks/HeapBench.c
        benchmarks/RedBlackTreeBench.c
)

add_executable(C_DataStructures_Library_Tests tests/main.c ${INCLUDE_TETS_FILES} ${TEST_FILES})
add_executable(C_DataStructures_Library_Benchmarks benchmarks/main.c ${INCLUDE_BENCHMARK_FILES} ${BENCHMARK_FILES})

file(GLOB ALL_SRC
        "./src/*.c"
        "./include/*.h"
        "./interface/*.c"
        "./interface/*.h"
        "./tests/UnitTest/*.h"
        "./tests/UnitTest/*.c"
        "./util/include/*.h"
        "./util/src/*.c"
)

add_library(DSLIB ${ALL_SRC})

add_dependencies(C_DataStructures_Library_Tests DSLIB)
add_dependencies(C_DataStructures_Library_Benchmarks DSLIB)

target_link_libraries(C_DataStructures_Library_Tests DSLIB m)
target_link_libraries(C_DataStructures_Library_Benchmarks DSLIB m)
