/**
 * @file CString.h
 * 
 * @author Leonardo Vencovsky (https://github.com/LeoVen)
 *
 * @date 03/12/2018
 */

#ifndef C_DATASTRUCTURES_LIBRARY_STRING_H
#define C_DATASTRUCTURES_LIBRARY_STRING_H

#include "Core.h"

// An array of characters. See the source file for the full documentation.
struct String_s;

/// \brief A type for a string implementation.
///
/// A type for a <code> struct String_s </code> so you don't have to always
/// write the full name of it.
typedef struct String_s String_t;

/// \brief A pointer type to a string implementation.
///
/// Useful for not having to declare every variable as pointer type. This
/// typedef does that for you.
typedef struct String_s *String;

///////////////////////////////////// STRUCTURE INITIALIZATION AND DELETION ///

Status str_init(String *string);

Status str_create(String *string, integer_t initial_capacity, integer_t growth_rate);

Status str_make(String *string, char *content);

Status str_delete(String *string);

Status str_erase(String *string);

Status str_clear(String string);

/////////////////////////////////////////////////////////////////// GETTERS ///

Status str_get_string(String string, char **result);

integer_t str_length(String string);

////////////////////////////////////////////////////////// INPUT AND OUTPUT ///

Status str_push_char_front(String string, char ch);

Status str_push_char_at(String string, char ch, integer_t index);

Status str_push_char_back(String string, char ch);

Status str_push_front(String string, char *ch);

Status str_push_at(String string, char *ch, integer_t index);

Status str_push_back(String string, char *ch);

Status str_prepend(String string1, String string2);

Status str_add(String string1, String string2, integer_t index);

Status str_append(String string1, String string2);

Status str_pop_char_front(String string);

Status str_pop_char_at(String string, integer_t index);

Status str_pop_char_back(String string);

Status str_remove(String string, integer_t from, integer_t to);

Status str_slice(String string, String *result, integer_t from, integer_t to);

/////////////////////////////////////////////////////////// STRUCTURE STATE ///

bool str_emtpy(String string);

/////////////////////////////////////////////////////////////////// UTILITY ///

Status str_front(String string, char *result);

Status str_back(String string, char *result);

int str_compare(String string1, String str2);

bool str_greater(String string1, String str2);

bool str_equals(String string1, String str2);

bool str_lesser(String string1, String str2);

bool str_equals_str(String string1, char *string);

bool str_substring(String string1, String string2);

bool str_substr(String string, char *ch);

Status str_find_substring(String string, String key, integer_t *pos);

Status str_find_substr(String string, char *key, integer_t *pos);

Status str_reverse(String string);

Status str_copy(String string, String *result);

Status str_swap(String *string1, String *string2);

Status str_case_upper(String string);

Status str_case_lower(String string);

Status str_capitalize(String string);

Status str_title(String string);

/////////////////////////////////////////////////////////////////// DISPLAY ///

Status str_display(String string);

Status str_display_raw(String string);

#endif //C_DATASTRUCTURES_LIBRARY_STRING_H
