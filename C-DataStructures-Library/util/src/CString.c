/**
 * @file CString.c
 * 
 * @author Leonardo Vencovsky (https://github.com/LeoVen)
 *
 * @date 03/12/2018
 */

#include "CString.h"

/// \brief A wrapper for an array of characters.
///
/// A \c String_s is an abstraction on top of an array of characters. Many high
/// level languages provide an implementation of a string and this is one
/// simple implementation because C lacks a string abstraction.
struct String_s
{
    /// \brief Character buffer.
    ///
    /// Null-terminated sequence of characters.
    char *buffer;
    
    /// \brief String current length.
    ///
    /// Current amount of characters in the buffer.
    integer_t length;

    /// \brief Total buffer capacity.
    ///
    /// When the string length reaches the buffer capacity then the buffer is
    /// reallocated.
    integer_t capacity;

    /// \brief Buffer growth rate.
    ///
    /// Buffer growth rate. The new buffer capacity is calculated as:
    ///
    /// <code> capacity *= (growth_rate / 100.0) </code>
    integer_t growth_rate;
};

///////////////////////////////////////////////////// NOT EXPOSED FUNCTIONS ///

static Status str_realloc(String string);

static Status str_accommodate(String string, integer_t size);

static bool str_buffer_full(String string);

static bool str_buffer_empty(String string);

static bool str_buffer_fits(String string, integer_t str_length);

static integer_t str_len(char *ch);

////////////////////////////////////////////// END OF NOT EXPOSED FUNCTIONS ///

/// \brief Initializes an empty string.
///
/// Initializes an empty string with initial capacity of 8 and growth rate of
/// 200, that is, twice the size after each growth.
///
/// \param string String_s to be initialized.
///
/// \return DS_ERR_ALLOC if string or buffer allocation failed.
/// \return DS_OK if all operations are successful.
Status str_init(String *string)
{
    (*string) = malloc(sizeof(String_t));

    if (!(*string))
        return DS_ERR_ALLOC;

    (*string)->buffer = malloc(sizeof(char) * 8);

    if (!((*string)->buffer))
    {
        free(*string);

        *string = NULL;

        return DS_ERR_ALLOC;
    }

    (*string)->buffer[0] = '\0';

    (*string)->capacity = 8;
    (*string)->growth_rate = 200;

    (*string)->length = 0;

    return DS_OK;
}

/// \brief Creates a String_s with custom parameters.
///
/// This function creates a string with a user-defined \c initial_capacity and
/// \c growth_rate which is the buffer's initial capacity and growth rate
/// respectively. The calculation for the new buffer size is given by:
///
/// <code> capacity *= (growth_rate / 100.0) </code>
///
/// \param[in,out] string String_s to be allocated.
/// \param[in] initial_capacity User-defined initial capacity.
/// \param[in] growth_rate User-defined growth rate.
///
/// \return DS_ERR_ALLOC if string or buffer allocation failed.
/// \return DS_ERR_INVALID_ARGUMENT if initial_capacity is less than 8 or
/// growth_rate is less than or equal to 100.
/// \return DS_OK if all operations are successful.
Status str_create(String *string, integer_t initial_capacity, integer_t growth_rate)
{
    if (initial_capacity < 8 || growth_rate <= 100)
        return DS_ERR_INVALID_ARGUMENT;

    (*string) = malloc(sizeof(String_t));

    if (!(*string))
        return DS_ERR_ALLOC;

    (*string)->buffer = malloc(sizeof(char) * initial_capacity);

    if (!((*string)->buffer))
    {
        free(*string);

        *string = NULL;

        return DS_ERR_ALLOC;
    }

    (*string)->buffer[0] = '\0';

    (*string)->capacity = initial_capacity;
    (*string)->growth_rate = growth_rate;

    (*string)->length = 0;

    return DS_OK;
}

Status str_make(String *string, char *char_array)
{
    integer_t length = str_len(char_array);

    if (length == 0)
        return DS_ERR_INVALID_ARGUMENT;

    Status st = str_create(string, (length < 8) ? 8 : length, 200);

    if (st != DS_OK)
        return st;

    (*string)->length = length;

    for (integer_t i = 0; i < length; i++)
    {
        (*string)->buffer[i] = char_array[i];
    }

    (*string)->buffer[length] = '\0';

    return DS_OK;
}

Status str_delete(String *string)
{
    if ((*string) == NULL)
        return DS_ERR_NULL_POINTER;

    free((*string)->buffer);

    free(*string);

    *string = NULL;

    return DS_OK;
}

Status str_erase(String *string)
{
    if ((*string) == NULL)
        return DS_ERR_NULL_POINTER;

    Status st = str_delete(string);

    if (st != DS_OK)
        return st;

    st = str_init(string);

    if (st != DS_OK)
        return st;

    return DS_OK;
}

Status str_clear(String string)
{
    if (string == NULL)
        return DS_ERR_NULL_POINTER;

    string->length = 0;

    string->buffer[0] = '\0';

    return DS_OK;
}

Status str_get_string(String string, char **result)
{
    (*result) = NULL;

    if (string == NULL)
        return DS_ERR_NULL_POINTER;

    if (str_buffer_empty(string))
        return DS_ERR_INVALID_OPERATION;

    char *char_array = malloc(sizeof(char) * (string->length + 1));

    if (!char_array)
        return DS_ERR_ALLOC;

    for (integer_t i = 0; i < string->length; i++)
        char_array[i] = string->buffer[i];

    char_array[string->length] = '\0';

    *result = char_array;

    return DS_OK;
}

integer_t str_length(String string)
{
    if (string == NULL)
        return -1;

    return string->length;
}

Status str_push_char_front(String string, char ch)
{
    if (string == NULL)
        return DS_ERR_NULL_POINTER;

    Status st;

    if (str_buffer_full(string))
    {
        st = str_realloc(string);

        if (st != DS_OK)
            return st;
    }

    for (integer_t i = string->length; i > 0; i--)
    {
        string->buffer[i] = string->buffer[i - 1];
    }

    string->buffer[0] = ch;

    string->length++;

    string->buffer[string->length] = '\0';

    return DS_OK;
}

Status str_push_char_at(String string, char ch, integer_t index)
{
    if (string == NULL)
        return DS_ERR_NULL_POINTER;

    if (index < 0)
        return DS_ERR_NEGATIVE_VALUE;

    if (index > string->length)
        return DS_ERR_OUT_OF_RANGE;

    Status st;

    if (index == 0)
    {
        st = str_push_char_front(string, ch);

        if (st != DS_OK)
            return st;
    }
    else if (index == string->length)
    {
        st = str_push_char_front(string, ch);

        if (st != DS_OK)
            return st;
    }
    else
    {
        if (str_buffer_full(string))
        {
            st = str_realloc(string);

            if (st != DS_OK)
                return st;
        }

        for (integer_t i = string->length; i > index; i--)
        {
            string->buffer[i] = string->buffer[i - 1];
        }

        string->buffer[index] = ch;

        string->length++;

        string->buffer[string->length] = '\0';
    }

    return DS_OK;
}

Status str_push_char_back(String string, char ch)
{
    if (string == NULL)
        return DS_ERR_NULL_POINTER;

    Status st;

    if (str_buffer_full(string))
    {
        st = str_realloc(string);

        if (st != DS_OK)
            return st;
    }

    string->buffer[string->length] = ch;

    string->length++;

    string->buffer[string->length] = '\0';

    return DS_OK;
}

Status str_push_front(String string, char *ch)
{
    if (string == NULL || ch == NULL)
        return DS_ERR_NULL_POINTER;

    integer_t length = str_len(ch);

    if (length == 0)
        return DS_ERR_INVALID_ARGUMENT;

    Status st = str_accommodate(string, length);

    if (st != DS_OK)
        return st;

    for (integer_t i = string->length; i > 0; i--)
    {
        string->buffer[i + length - 1] = string->buffer[i - 1];
    }

    for (integer_t i = 0; i < length; i++)
    {
        string->buffer[i] = ch[i];
    }

    string->length += length;

    string->buffer[string->length] = '\0';

    return DS_OK;
}

Status str_push_at(String string, char *ch, integer_t index)
{
    if (string == NULL || ch == NULL)
        return DS_ERR_NULL_POINTER;

    if (index < 0)
        return DS_ERR_NEGATIVE_VALUE;

    if (index > string->length)
        return DS_ERR_OUT_OF_RANGE;

    Status st;

    if (index == 0)
    {
        st = str_push_front(string, ch);

        if (st != DS_OK)
            return st;
    }
    else if (index == string->length)
    {
        st = str_push_back(string, ch);

        if (st != DS_OK)
            return st;
    }
    else
    {
        integer_t length = str_len(ch);

        if (length == 0)
            return DS_ERR_INVALID_ARGUMENT;

        st = str_accommodate(string, length);

        if (st != DS_OK)
            return st;

        for (integer_t i = string->length; i >= index; i--)
        {
            string->buffer[i + length] = string->buffer[i];
        }

        for (integer_t i = index, j = 0; i < index + length; i++, j++)
        {
            string->buffer[i] = ch[j];
        }

        string->length += length;

        string->buffer[string->length] = '\0';
    }

    return DS_OK;
}

Status str_push_back(String string, char *ch)
{
    if (string == NULL || ch == NULL)
        return DS_ERR_NULL_POINTER;

    integer_t length = str_len(ch);

    if (length == 0)
        return DS_ERR_INVALID_ARGUMENT;

    Status st = str_accommodate(string, length);

    if (st != DS_OK)
        return st;

    for (integer_t i = string->length, j = 0; i < length + string->length; i++, j++)
    {
        string->buffer[i] = ch[j];
    }

    string->length += length;

    string->buffer[string->length] = '\0';

    return DS_OK;
}

Status str_prepend(String string1, String string2)
{
    if (string1 == NULL || string2 == NULL)
        return DS_ERR_NULL_POINTER;

    if (str_buffer_empty(string2))
        return DS_OK;

    Status st = str_accommodate(string1, string2->length);

    if (st != DS_OK)
        return st;

    for (integer_t i = string1->length; i > 0; i--)
    {
        string1->buffer[i + string2->length - 1] = string1->buffer[i - 1];
    }

    for (integer_t i = 0; i < string2->length; i++)
    {
        string1->buffer[i] = string2->buffer[i];
    }

    string1->length += string2->length;

    string1->buffer[string1->length] = '\0';

    st = str_clear(string2);

    if (st != DS_OK)
        return st;

    return DS_OK;
}

Status str_add(String string1, String string2, integer_t index)
{
    if (string1 == NULL || string2 == NULL)
        return DS_ERR_NULL_POINTER;

    Status st;

    if (index == 0)
    {
        st = str_prepend(string1, string2);

        if (st != DS_OK)
            return st;
    }
    else if (index == string1->length)
    {
        st = str_append(string1, string2);

        if (st != DS_OK)
            return st;
    }
    else
    {
        st = str_accommodate(string1, string2->length);

        if (st != DS_OK)
            return st;

        for (integer_t i = string1->length; i >= index; i--)
        {
            string1->buffer[i + string2->length] = string1->buffer[i];
        }

        for (integer_t i = index, j = 0; i < index + string2->length; i++, j++)
        {
            string1->buffer[i] = string2->buffer[j];
        }

        string1->length += string2->length;

        string1->buffer[string1->length] = '\0';
    }

    return DS_OK;
}

Status str_append(String string1, String string2)
{
    if (string1 == NULL || string2 == NULL)
        return DS_ERR_NULL_POINTER;

    if (str_buffer_empty(string2))
        return DS_OK;

    Status st = str_accommodate(string1, string2->length);

    if (st != DS_OK)
        return st;

    for (integer_t i = string1->length, j = 0;
                   i < string1->length + string2->length; i++, j++)
    {
        string1->buffer[i] = string2->buffer[j];
    }

    string1->length += string2->length;

    string1->buffer[string1->length] = '\0';

    return DS_OK;
}

Status str_pop_char_front(String string)
{
    if (string == NULL)
        return DS_ERR_NULL_POINTER;

    if (str_buffer_empty(string))
        return DS_ERR_INVALID_OPERATION;

    for (integer_t i = 0; i < string->length; i++)
    {
        string->buffer[i] = string->buffer[i + 1];
    }

    string->length--;

    string->buffer[string->length] = '\0';

    return DS_OK;
}

Status str_pop_char_at(String string, integer_t index)
{
    if (string == NULL)
        return DS_ERR_NULL_POINTER;

    if (str_buffer_empty(string))
        return DS_ERR_INVALID_OPERATION;

    if (index >= string->length)
        return DS_ERR_OUT_OF_RANGE;

    Status st;

    if (index == 0)
    {
        st = str_pop_char_front(string);

        if (st != DS_OK)
            return st;
    }
    else if (index == string->length - 1)
    {
        st = str_pop_char_back(string);

        if (st != DS_OK)
            return st;
    }
    else
    {
        for (integer_t i = index; i < string->length; i++)
        {
            string->buffer[i] = string->buffer[i + 1];
        }

        string->length--;

        string->buffer[string->length] = '\0';
    }

    return DS_OK;
}

Status str_pop_char_back(String string)
{
    if (string == NULL)
        return DS_ERR_NULL_POINTER;

    if (str_buffer_empty(string))
        return DS_ERR_INVALID_OPERATION;

    string->length--;

    string->buffer[string->length] = '\0';

    return DS_OK;
}

Status str_remove(String string, integer_t from, integer_t to)
{
    // TODO
    return DS_ERR_INVALID_OPERATION;
}

Status str_slice(String string, String *result, integer_t from, integer_t to)
{
    // TODO
    return DS_ERR_INVALID_OPERATION;
}

bool str_emtpy(String string)
{
    return string->length == 0;
}

Status str_front(String string, char *result)
{
    if (string == NULL)
        return DS_ERR_NULL_POINTER;

    if (str_buffer_empty(string))
        return DS_ERR_INVALID_OPERATION;

    *result = string->buffer[0];

    return DS_OK;
}

Status str_back(String string, char *result)
{
    if (string == NULL)
        return DS_ERR_NULL_POINTER;

    if (str_buffer_empty(string))
        return DS_ERR_INVALID_OPERATION;

    *result = string->buffer[string->length - 1];

    return DS_OK;
}

int str_compare(String string1, String string2)
{
    if (str_greater(string1, string2))
        return 1;
    else if (str_lesser(string1, string2))
        return -1;

    return 0;
}

bool str_greater(String string1, String string2)
{
    integer_t length = (string1->length > string2->length) ?
            string2->length : string1->length;

    for (integer_t i = 0; i < length; i++)
    {
        if (string1->buffer[i] - string2->buffer[i] < 0)
            return true;
        else if (string1->buffer[i] - string2->buffer[i] > 0)
            return false;
    }

    if (string1->length > string2->length)
        return true;

    return false;
}

bool str_equals(String string1, String string2)
{
    if (string1->length != string2->length)
        return false;

    for (integer_t i = 0; i < string1->length; i++)
        if (string1->buffer[i] != string2->buffer[i])
            return false;

    return true;
}

bool str_lesser(String string1, String string2)
{
    integer_t length = (string1->length > string2->length) ?
            string2->length : string1->length;

    for (integer_t i = 0; i < length; i++)
    {
        if (string1->buffer[i] - string2->buffer[i] > 0)
            return true;
        else if (string1->buffer[i] - string2->buffer[i] < 0)
            return false;
    }

    if (string1->length < string2->length)
        return false;

    return true;
}

bool str_equals_str(String string, char *char_array)
{
    if (string == NULL || char_array == NULL)
        return false;

    integer_t length = str_len(char_array);

    if (string->length != length)
        return false;

    for (integer_t i = 0; i < length; i++)
        if (string->buffer[i] != char_array[i])
            return false;

    return true;
}

// Returns true if string1 has substring string2
bool str_substring(String string1, String string2)
{
    // TODO
    return false;
}

// Returns true if string has substring ch
bool str_substr(String string, char *ch)
{
    // TODO
    return false;
}

// Finds the position of a string inside a string
Status str_find_substring(String string, String key, integer_t *pos)
{
    // TODO
    return DS_ERR_INVALID_OPERATION;
}

// Finds the position of an array of char inside a string
Status str_find_substr(String string, char *key, integer_t *pos)
{
    // TODO
    return DS_ERR_INVALID_OPERATION;
}

Status str_reverse(String string)
{
    // TODO
    return DS_ERR_INVALID_OPERATION;
}

Status str_copy(String string, String *result)
{
    *result = NULL;

    if (string == NULL)
        return DS_ERR_NULL_POINTER;

    Status st = str_create(result, string->capacity, string->growth_rate);

    if (st != DS_OK)
        return st;

    char *s1 = string->buffer, *s2 = (*result)->buffer;

    while ((*s2++ = *s1++));

    (*result)->length = string->length;

    return DS_OK;
}

Status str_swap(String *string1, String *string2)
{
    String temp = (*string1);

    (*string1) = (*string2);
    (*string2) = temp;

    return DS_OK;
}

Status str_case_upper(String string)
{
    if (string == NULL)
        return DS_ERR_NULL_POINTER;

    if (str_buffer_empty(string))
        return DS_ERR_INVALID_OPERATION;

    char ch;

    for (integer_t i = 0; i < string->length; i++)
    {
        ch = string->buffer[i];

        if (ch >= 'a' && ch <= 'z')
            string->buffer[i] -= 32;
    }

    return DS_OK;
}

Status str_case_lower(String string)
{
    if (string == NULL)
        return DS_ERR_NULL_POINTER;

    if (str_buffer_empty(string))
        return DS_ERR_INVALID_OPERATION;

    char ch;

    for (integer_t i = 0; i < string->length; i++)
    {
        ch = string->buffer[i];

        if (ch >= 'A' && ch <= 'Z')
            string->buffer[i] += 32;
    }

    return DS_OK;
}

// First letter is transformed to upper case
Status str_capitalize(String string)
{
    // TODO
    return DS_ERR_INVALID_OPERATION;
}

// Every letter after a space is transformed to upper case
Status str_title(String string)
{
    // TODO
    return DS_ERR_INVALID_OPERATION;
}

Status str_display(String string)
{
    if (string == NULL)
        return DS_ERR_NULL_POINTER;

    if (str_buffer_empty(string))
    {
        printf("\nString\n[ empty ]\n");
        return DS_OK;
    }

    printf("\nString\n%s\n", string->buffer);

    return DS_OK;
}

Status str_display_raw(String string)
{
    if (string == NULL)
        return DS_ERR_NULL_POINTER;

    printf("\n");

    if (str_buffer_empty(string))
        return DS_OK;

    printf("%s", string->buffer);

    return DS_OK;
}

///////////////////////////////////////////////////// NOT EXPOSED FUNCTIONS ///

static Status str_realloc(String string)
{
    if (string == NULL)
        return DS_ERR_NULL_POINTER;

    integer_t old_capacity = string->capacity;

    // capacity = capacity * (growth_rate / 100)
    string->capacity = (integer_t) ((double) (string->capacity) *
            ((double) (string->growth_rate) / 100.0));

    char *new_buffer = realloc(string->buffer,
            sizeof(char) * string->capacity);

    if (!new_buffer)
    {
        string->capacity = old_capacity;

        return DS_ERR_ALLOC;
    }

    string->buffer = new_buffer;

    string->buffer[string->length] = '\0';

    return DS_OK;
}

static Status str_accommodate(String string, integer_t size)
{
    if (string == NULL)
        return DS_ERR_NULL_POINTER;

    // There is enough capacity already (+1 for '\0')
    if (string->capacity > string->length + size + 1)
        return DS_OK;

    char *new_buffer = realloc(string->buffer,
            sizeof(char) * (string->capacity + size));

    if (!new_buffer)
        return DS_ERR_ALLOC;

    string->capacity += size;

    string->buffer = new_buffer;

    string->buffer[string->length] = '\0';

    return DS_OK;
}

static bool str_buffer_full(String string)
{
    // +1 So we are able to write '\0' at the end
    return (string->length + 1) >= string->capacity;
}

static bool str_buffer_empty(String string)
{
    return string->length == 0;
}

static bool str_buffer_fits(String string, integer_t str_length)
{
    return (string->length + str_length) < string->capacity;
}

static integer_t str_len(char *ch)
{
    char *h = ch;

    while (*ch)
        ++ch;

    return ch - h;
}

////////////////////////////////////////////// END OF NOT EXPOSED FUNCTIONS ///