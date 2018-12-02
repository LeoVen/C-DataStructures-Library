/**
 * @file Random.c
 * 
 * @author Leonardo Vencovsky (https://github.com/LeoVen)
 *
 * @date 07/11/2018
 */

#include "Random.h"

double rrandom()
{
    return (double)rand() / (double)RAND_MAX;
}

int rand_int(int min, int max)
{
    return (int)floor(rrandom() * ((double)max - (double)min + 1.0) + (double)min);
}

long rand_long(long min, long max)
{
    return (long)floor(rrandom() * ((double)max - (double)min + 1.0) + (double)min);
}

integer_t rand_integer(integer_t min, integer_t max)
{
    return (integer_t)floor(rrandom() *((double)max - (double)min + 1.0) + (double)min);
}

double rand_double(double min, double max)
{
    return rrandom() * (max - min) + min;
}

float rand_float(float min, float max)
{
    return (float)rrandom() * (max - min) + min;
}