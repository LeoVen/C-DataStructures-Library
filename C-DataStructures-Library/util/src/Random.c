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

int rand_int(index_t min, index_t max)
{
    return (int)floor(rrandom() * ((double)max - (double)min + 1.0) + (double)min);
}

long long rand_long(index_t min, index_t max)
{
    return (long long)floor(rrandom() * ((double)max - (double)min + 1.0) + (double)min);
}

double rand_double(double min, double max)
{
    return rrandom() * (max - min) + min;
}

float rand_float(float min, float max)
{
    return (float)rrandom() * (max - min) + min;
}