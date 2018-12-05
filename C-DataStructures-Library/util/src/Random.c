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
    return (double)rand() / (double)(RAND_MAX + 1);
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

float rand_float(float min, float max)
{
    return (float)rrandom() * (max - min) + min;
}
double rand_double(double min, double max)
{
    return rrandom() * (max - min) + min;
}

char rand_char(char min, char max)
{
    return (char)floor(rrandom() * ((double)max - (double)min + 1.0) + (double)min);
}

char rand_alpha()
{
    const char alpha[] = {'0', '1', '2', '3', '4', '5', '6', '7', '8', '9',
                          'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j',
                          'k', 'l', 'm', 'n', 'o', 'p', 'q', 'r', 's', 't',
                          'u', 'v', 'w', 'x', 'y', 'z', 'A', 'B', 'C', 'D',
                          'E', 'F', 'G', 'H', 'I', 'J', 'K', 'L', 'M', 'N',
                          'O', 'P', 'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X',
                          'Y', 'Z'};
    return alpha[rand_int(0, 61)];
}