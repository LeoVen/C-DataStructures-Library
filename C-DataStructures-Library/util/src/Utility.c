/**
 * @file Utility.c
 * 
 * @author Leonardo Vencovsky (https://github.com/LeoVen)
 *
 * @date 12/12/2018
 */

#include "Utility.h"
#include <inttypes.h>

int compare_int8_t(const void *element1, const void *element2)
{
    const int8_t *e1 = (const int8_t*)element1;
    const int8_t *e2 = (const int8_t*)element2;

    if (*e1 > *e2)
        return 1;
    else if (*e1 < *e2)
        return -1;

    return 0;
}

int compare_int16_t(const void *element1, const void *element2)
{
    const int16_t *e1 = (const int16_t*)element1;
    const int16_t *e2 = (const int16_t*)element2;

    if (*e1 > *e2)
        return 1;
    else if (*e1 < *e2)
        return -1;

    return 0;
}

int compare_int32_t(const void *element1, const void *element2)
{
    const int32_t *e1 = (const int32_t*)element1;
    const int32_t *e2 = (const int32_t*)element2;

    if (*e1 > *e2)
        return 1;
    else if (*e1 < *e2)
        return -1;

    return 0;
}

int compare_int64_t(const void *element1, const void *element2)
{
    const int64_t *e1 = (const int64_t*)element1;
    const int64_t *e2 = (const int64_t*)element2;

    if (*e1 > *e2)
        return 1;
    else if (*e1 < *e2)
        return -1;

    return 0;
}

int compare_uint8_t(const void *element1, const void *element2)
{
    const uint8_t *e1 = (const uint8_t*)element1;
    const uint8_t *e2 = (const uint8_t*)element2;

    if (*e1 > *e2)
        return 1;
    else if (*e1 < *e2)
        return -1;

    return 0;
}

int compare_uint16_t(const void *element1, const void *element2)
{
    const uint16_t *e1 = (const uint16_t*)element1;
    const uint16_t *e2 = (const uint16_t*)element2;

    if (*e1 > *e2)
        return 1;
    else if (*e1 < *e2)
        return -1;

    return 0;
}

int compare_uint32_t(const void *element1, const void *element2)
{
    const uint32_t *e1 = (const uint32_t*)element1;
    const uint32_t *e2 = (const uint32_t*)element2;

    if (*e1 > *e2)
        return 1;
    else if (*e1 < *e2)
        return -1;

    return 0;
}

int compare_uint64_t(const void *element1, const void *element2)
{
    const uint64_t *e1 = (const uint64_t*)element1;
    const uint64_t *e2 = (const uint64_t*)element2;

    if (*e1 > *e2)
        return 1;
    else if (*e1 < *e2)
        return -1;

    return 0;
}

int compare_float(const void *element1, const void *element2)
{
    const float *e1 = (const float*)element1;
    const float *e2 = (const float*)element2;

    if (*e1 > *e2)
        return 1;
    else if (*e1 < *e2)
        return -1;

    return 0;
}

int compare_double(const void *element1, const void *element2)
{
    const double *e1 = (const double*)element1;
    const double *e2 = (const double*)element2;

    if (*e1 > *e2)
        return 1;
    else if (*e1 < *e2)
        return -1;

    return 0;
}

int compare_long_double(const void *element1, const void *element2)
{
    const long double *e1 = (const long double*)element1;
    const long double *e2 = (const long double*)element2;

    if (*e1 > *e2)
        return 1;
    else if (*e1 < *e2)
        return -1;

    return 0;
}

int compare_char(const void *element1, const void *element2)
{
    const char *e1 = (const char*)element1;
    const char *e2 = (const char*)element2;

    if (*e1 > *e2)
        return 1;
    else if (*e1 < *e2)
        return -1;

    return 0;
}

int compare_string(const void *element1, const void *element2)
{
    const char *e1 = (const char*)element1;
    const char *e2 = (const char*)element2;

    return strcmp(e1, e2);
}

void *copy_int8_t(const void *element)
{
    const int8_t *e = (const int8_t*)element;

    return new_int8_t(*e);
}

void *copy_int16_t(const void *element)
{
    const int16_t *e = (const int16_t*)element;

    return new_int16_t(*e);
}

void *copy_int32_t(const void *element)
{
    const int32_t *e = (const int32_t*)element;

    return new_int32_t(*e);
}

void *copy_int64_t(const void *element)
{
    const int64_t *e = (const int64_t*)element;

    return new_int64_t(*e);
}

void *copy_uint8_t(const void *element)
{
    const uint8_t *e = (const uint8_t*)element;

    return new_uint8_t(*e);
}

void *copy_uint16_t(const void *element)
{
    const uint16_t *e = (const uint16_t*)element;

    return new_uint16_t(*e);
}

void *copy_uint32_t(const void *element)
{
    const uint32_t *e = (const uint32_t*)element;

    return new_uint32_t(*e);
}

void *copy_uint64_t(const void *element)
{
    const uint64_t *e = (const uint64_t*)element;

    return new_uint64_t(*e);
}

void *copy_float(const void *element)
{
    const float *e = (const float*)element;

    return new_float(*e);
}

void *copy_double(const void *element)
{
    const double *e = (const double*)element;

    return new_double(*e);
}

void *copy_long_double(const void *element)
{
    const long double *e = (const long double*)element;

    return new_long_double(*e);
}

void *copy_char(const void *element)
{
    const char *e = (const char*)element;

    return new_char(*e);
}

void *copy_string(const void *element)
{
    const char *e = (const char*)element;

    return strdup(e);
}

void display_int8_t(const void *element)
{
    const int8_t *e = (const int8_t*)element;

    printf("%" PRId8 "", *e);
}

void display_int16_t(const void *element)
{
    const int16_t *e = (const int16_t*)element;

    printf("%" PRId16 "", *e);
}

void display_int32_t(const void *element)
{
    const int32_t *e = (const int32_t*)element;

    printf("%" PRId32 "", *e);
}

void display_int64_t(const void *element)
{
    const int64_t *e = (const int64_t*)element;

    printf("%" PRId64 "", *e);
}

void display_uint8_t(const void *element)
{
    const uint8_t *e = (const uint8_t*)element;

    printf("%" PRIu8 "", *e);
}

void display_uint16_t(const void *element)
{
    const uint16_t *e = (const uint16_t*)element;

    printf("%" PRIu16 "", *e);
}

void display_uint32_t(const void *element)
{
    const uint32_t *e = (const uint32_t*)element;

    printf("%" PRIu32 "", *e);
}

void display_uint64_t(const void *element)
{
    const uint64_t *e = (const uint64_t*)element;

    printf("%" PRIu64 "", *e);
}

void display_float(const void *element)
{
    const float *e = (const float*)element;

    printf("%f", *e);
}

void display_double(const void *element)
{
    const double *e = (const double*)element;

    printf("%lf", *e);
}

void display_long_double(const void *element)
{
    const long double *e = (const long double*)element;

    printf("%Lf", *e);
}

void display_char(const void *element)
{
    const char *e = (const char*)element;

    printf("%c", *e);
}

void display_string(const void *element)
{
    const char *e = (const char*)element;

    printf("%s", e);
}

unsigned_t hash_int8_t(const void *element)
{
    const int8_t *e = (const int8_t*)element;

    uint64_t x = (uint64_t)*e;

    x = (x ^ (x >> 30)) * UINT64_C(0xbf58476d1ce4e5b9);
    x = (x ^ (x >> 27)) * UINT64_C(0x94d049bb133111eb);
    x = x ^ (x >> 31);

    return x;
}

unsigned_t hash_int16_t(const void *element)
{
    const int16_t *e = (const int16_t*)element;

    uint64_t x = (uint64_t)*e;

    x = (x ^ (x >> 30)) * UINT64_C(0xbf58476d1ce4e5b9);
    x = (x ^ (x >> 27)) * UINT64_C(0x94d049bb133111eb);
    x = x ^ (x >> 31);

    return x;
}

unsigned_t hash_int32_t(const void *element)
{
    const int32_t *e = (const int32_t*)element;

    uint64_t x = (uint64_t)*e;

    x = (x ^ (x >> 30)) * UINT64_C(0xbf58476d1ce4e5b9);
    x = (x ^ (x >> 27)) * UINT64_C(0x94d049bb133111eb);
    x = x ^ (x >> 31);

    return x;
}

unsigned_t hash_int64_t(const void *element)
{
    const int64_t *e = (const int64_t*)element;

    uint64_t x = (uint64_t)*e;

    x = (x ^ (x >> 30)) * UINT64_C(0xbf58476d1ce4e5b9);
    x = (x ^ (x >> 27)) * UINT64_C(0x94d049bb133111eb);
    x = x ^ (x >> 31);

    return x;
}

unsigned_t hash_uint8_t(const void *element)
{
    const uint8_t *e = (const uint8_t*)element;

    uint64_t x = (uint64_t)*e;

    x = (x ^ (x >> 30)) * UINT64_C(0xbf58476d1ce4e5b9);
    x = (x ^ (x >> 27)) * UINT64_C(0x94d049bb133111eb);
    x = x ^ (x >> 31);

    return x;
}

unsigned_t hash_uint16_t(const void *element)
{
    const uint16_t *e = (const uint16_t*)element;

    uint64_t x = (uint64_t)*e;

    x = (x ^ (x >> 30)) * UINT64_C(0xbf58476d1ce4e5b9);
    x = (x ^ (x >> 27)) * UINT64_C(0x94d049bb133111eb);
    x = x ^ (x >> 31);

    return x;
}

unsigned_t hash_uint32_t(const void *element)
{
    const uint32_t *e = (const uint32_t*)element;

    uint64_t x = (uint64_t)*e;

    x = (x ^ (x >> 30)) * UINT64_C(0xbf58476d1ce4e5b9);
    x = (x ^ (x >> 27)) * UINT64_C(0x94d049bb133111eb);
    x = x ^ (x >> 31);

    return x;
}

unsigned_t hash_uint64_t(const void *element)
{
    const uint64_t *e = (const uint64_t*)element;

    uint64_t x = (uint64_t)*e;

    x = (x ^ (x >> 30)) * UINT64_C(0xbf58476d1ce4e5b9);
    x = (x ^ (x >> 27)) * UINT64_C(0x94d049bb133111eb);
    x = x ^ (x >> 31);

    return x;
}

unsigned_t hash_float(const void *element)
{
    const float *e = (const float*)element;

    uint64_t x;

    memcpy(&x, e, sizeof(float));

    return x & 0xFFFFFFF8;
}

unsigned_t hash_double(const void *element)
{
    const double *e = (const double*)element;

    uint64_t x;

    memcpy(&x, e, sizeof(double));

    return x & 0xFFFFFFFFFFFFFFF0;
}

unsigned_t hash_long_double(const void *element)
{
    /// \todo hash_long_dougle
    return 0;
}

unsigned_t hash_char(const void *element)
{
    /// \todo hash_char
    return 0;
}

unsigned_t hash_string(const void *element)
{
    const char *str = (const char*)element;

    uint64_t x = 5381;
    int c;

    while ((c = *str++))
        x = ((x << 5) + x) + c;

    return x;
}

void *new_int8_t(int8_t element)
{
    int8_t *e = malloc(sizeof(int8_t));

    *e = element;

    return e;
}

void *new_int16_t(int16_t element)
{
    int16_t *e = malloc(sizeof(int16_t));

    *e = element;

    return e;
}

void *new_int32_t(int32_t element)
{
    int32_t *e = malloc(sizeof(int32_t));

    *e = element;

    return e;
}

void *new_int64_t(int64_t element)
{
    int64_t *e = malloc(sizeof(int64_t));

    *e = element;

    return e;
}

void *new_uint8_t(uint8_t element)
{
    uint8_t *e = malloc(sizeof(uint8_t));

    *e = element;

    return e;
}

void *new_uint16_t(uint16_t element)
{
    uint16_t *e = malloc(sizeof(uint16_t));

    *e = element;

    return e;
}

void *new_uint32_t(uint32_t element)
{
    uint32_t *e = malloc(sizeof(uint32_t));

    *e = element;

    return e;
}

void *new_uint64_t(uint64_t element)
{
    uint64_t *e = malloc(sizeof(uint64_t));

    *e = element;

    return e;
}

void *new_float(float element)
{
    float *e = malloc(sizeof(float));

    *e = element;

    return e;
}

void *new_double(double element)
{
    double *e = malloc(sizeof(double));

    *e = element;

    return e;
}

void *new_long_double(long double element)
{
    long double *e = malloc(sizeof(long double));

    *e = element;

    return e;
}

void *new_char(char element)
{
    char *e = malloc(sizeof(char));

    *e = element;

    return e;
}

void *new_string(char *element)
{
    return strdup(element);
}

double rrandom()
{
    return (double)rand() / ((double)RAND_MAX + 1.0);
}

int8_t random_int8_t(int8_t min, int8_t max)
{
    return (int8_t)floor(rrandom() *
            ((double)max - (double)min + 1.0) + (double)min);
}

int16_t random_int16_t(int16_t min, int16_t max)
{
    return (int16_t)floor(rrandom() *
            ((double)max - (double)min + 1.0) + (double)min);
}

int32_t random_int32_t(int32_t min, int32_t max)
{
    return (int32_t)floor(rrandom() *
            ((double)max - (double)min + 1.0) + (double)min);
}

int64_t random_int64_t(int64_t min, int64_t max)
{
    return (int64_t)floor(rrandom() *
            ((double)max - (double)min + 1.0) + (double)min);
}

uint8_t random_uint8_t(uint8_t min, uint8_t max)
{
    return (uint8_t)floor(rrandom() *
            ((double)max - (double)min + 1.0) + (double)min);
}

uint16_t random_uint16_t(uint16_t min, uint16_t max)
{
    return (uint16_t)floor(rrandom() *
            ((double)max - (double)min + 1.0) + (double)min);
}

uint32_t random_uint32_t(uint32_t min, uint32_t max)
{
    return (uint32_t)floor(rrandom() *
            ((double)max - (double)min + 1.0) + (double)min);
}

uint64_t random_uint64_t(uint64_t min, uint64_t max)
{
    return (uint64_t)floor(rrandom() *
            ((double)max - (double)min + 1.0) + (double)min);
}

float random_float(float min, float max)
{
    return ((float)rand()/(float)((float)RAND_MAX + 1.0)) *
            ((max - min + 1.0f) + min);
}

double random_double(double min, double max)
{
    return rrandom() * (max - min + 1.0) + min;
}

long double random_long_double(long double min, long double max)
{
    return ((long double)rand() / ((long double)RAND_MAX + 1.0)) *
            ((max - min + 1.0f) + min);
}

char random_char(char min, char max)
{
    return (char)floor(rrandom() *
            ((double)max - (double)min + 1.0) + (double)min);
}

char *random_string(uint64_t min_length, uint64_t max_length, bool alpha_only)
{
    if (max_length < min_length)
        return NULL;

    unsigned_t length = random_uint64_t(min_length, max_length);

    char *string = malloc(sizeof(char) * (length + 1));

    if (!string)
        return NULL;

    if (alpha_only)
    {
        const char *alpha =
              "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789";
        const size_t alpha_size = 62;

        for (size_t i = 0; i < length; i++)
        {
            string[i] = alpha[random_uint64_t(0, alpha_size - 1)];
        }
    }
    else
    {
        for (size_t i = 0; i < length; i++)
        {
            string[i] = random_print();
        }
    }

    string[length] = '\0';

    return string;
}

char random_alpha()
{
    const char alpha[] = {'0', '1', '2', '3', '4', '5', '6', '7', '8', '9',
                          'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j',
                          'k', 'l', 'm', 'n', 'o', 'p', 'q', 'r', 's', 't',
                          'u', 'v', 'w', 'x', 'y', 'z', 'A', 'B', 'C', 'D',
                          'E', 'F', 'G', 'H', 'I', 'J', 'K', 'L', 'M', 'N',
                          'O', 'P', 'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X',
                          'Y', 'Z'};
    return alpha[random_int8_t(0, 61)];
}

char random_print()
{
    return random_int8_t(32, 126);
}