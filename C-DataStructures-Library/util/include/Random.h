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

#ifdef __cplusplus
extern "C" {
#endif

/// Generates a random number between 0 and 1.
double rrandom();

/// Generates an integer between min and max with both included.
int rand_int(int min, int max);

/// Generates a long integer between min and max with both included.
long rand_long(long min, long max);

/// Generates a defined integer between min and max with both included.
integer_t rand_integer(integer_t min, integer_t max);

/// Generates a float between min and max.
float rand_float(float min, float max);

/// Generates a double between min and max.
double rand_double(double min, double max);

/// Generates a char between min and max.
char rand_char(char min, char max);

/// Generates a random alphanumeric char.
char rand_alpha();

#ifdef __cplusplus
}
#endif

#endif //C_DATASTRUCTURES_LIBRARY_RANDOM_H
