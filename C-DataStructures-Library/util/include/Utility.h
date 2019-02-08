/**
 * @file Utility.h
 * 
 * @author Leonardo Vencovsky (https://github.com/LeoVen)
 *
 * @date 12/12/2018
 */

#ifndef C_DATASTRUCTURES_LIBRARY_UTILITY_H
#define C_DATASTRUCTURES_LIBRARY_UTILITY_H

#include "Core.h"

int compare_int8_t(const void *element1, const void *element2);
int compare_int16_t(const void *element1, const void *element2);
int compare_int32_t(const void *element1, const void *element2);
int compare_int64_t(const void *element1, const void *element2);

int compare_uint8_t(const void *element1, const void *element2);
int compare_uint16_t(const void *element1, const void *element2);
int compare_uint32_t(const void *element1, const void *element2);
int compare_uint64_t(const void *element1, const void *element2);

int compare_float(const void *element1, const void *element2);
int compare_double(const void *element1, const void *element2);
int compare_long_double(const void *element1, const void *element2);

int compare_char(const void *element1, const void *element2);
int compare_string(const void *element1, const void *element2);

void *copy_int8_t(const void *element);
void *copy_int16_t(const void *element);
void *copy_int32_t(const void *element);
void *copy_int64_t(const void *element);

void *copy_uint8_t(const void *element);
void *copy_uint16_t(const void *element);
void *copy_uint32_t(const void *element);
void *copy_uint64_t(const void *element);

void *copy_float(const void *element);
void *copy_double(const void *element);
void *copy_long_double(const void *element);

void *copy_char(const void *element);
void *copy_string(const void *element);

void display_int8_t(const void *element);
void display_int16_t(const void *element);
void display_int32_t(const void *element);
void display_int64_t(const void *element);

void display_uint8_t(const void *element);
void display_uint16_t(const void *element);
void display_uint32_t(const void *element);
void display_uint64_t(const void *element);

void display_float(const void *element);
void display_double(const void *element);
void display_long_double(const void *element);

void display_char(const void *element);
void display_string(const void *element);

unsigned_t hash_int8_t(const void *element);
unsigned_t hash_int16_t(const void *element);
unsigned_t hash_int32_t(const void *element);
unsigned_t hash_int64_t(const void *element);

unsigned_t hash_uint8_t(const void *element);
unsigned_t hash_uint16_t(const void *element);
unsigned_t hash_uint32_t(const void *element);
unsigned_t hash_uint64_t(const void *element);

unsigned_t hash_float(const void *element);
unsigned_t hash_double(const void *element);
unsigned_t hash_long_double(const void *element);

unsigned_t hash_char(const void *element);
unsigned_t hash_string(const void *element);

void *new_int8_t(int8_t element);
void *new_int16_t(int16_t element);
void *new_int32_t(int32_t element);
void *new_int64_t(int64_t element);

void *new_uint8_t(uint8_t element);
void *new_uint16_t(uint16_t element);
void *new_uint32_t(uint32_t element);
void *new_uint64_t(uint64_t element);

void *new_float(float element);
void *new_double(double element);
void *new_long_double(long double element);

void *new_char(char element);
void *new_string(char *element);

double rrandom();

int8_t random_int8_t(int8_t min, int8_t max);
int16_t random_int16_t(int16_t min, int16_t max);
int32_t random_int32_t(int32_t min, int32_t max);
int64_t random_int64_t(int64_t min, int64_t max);

uint8_t random_uint8_t(uint8_t min, uint8_t max);
uint16_t random_uint16_t(uint16_t min, uint16_t max);
uint32_t random_uint32_t(uint32_t min, uint32_t max);
uint64_t random_uint64_t(uint64_t min, uint64_t max);

float random_float(float min, float max);
double random_double(double min, double max);
long double random_long_double(long double min, long double max);

char random_char(char min, char max);

char *random_string(uint64_t min_length, uint64_t max_length, bool alpha_only);
char random_alpha();
char random_print();

#endif //C_DATASTRUCTURES_LIBRARY_UTILITY_H
