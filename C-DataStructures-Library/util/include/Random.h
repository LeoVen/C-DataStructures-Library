/**
 * @file Random.h
 * 
 * @author Leonardo Vencovsky (https://github.com/LeoVen)
 *
 * @date 07/11/2018
 */

#ifndef C_DATASTRUCTURES_LIBRARY_RANDOM_H
#define C_DATASTRUCTURES_LIBRARY_RANDOM_H

#include "Core.h"

/// Generates a random number between 0 and 1.
double random();

/// Generates an integer between min and max with both included.
int rand_int(index_t min, index_t max);

/// Generates a long integer between min and max with both included.
long long rand_long(index_t min, index_t max);

/// Generates a double between min and max with both included.
double rand_float(double min, double max);

#endif //C_DATASTRUCTURES_LIBRARY_RANDOM_H
