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

double rand_float(double min, double max)
{
    return floor(rrandom() * (max - min + 1.0) + min);
}