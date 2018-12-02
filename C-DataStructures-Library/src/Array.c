/**
 * @file Array.c
 * 
 * @author Leonardo Vencovsky (https://github.com/LeoVen)
 *
 * @date 07/10/2018
 */

#include "Array.h"

/// \brief A C generic array wrapper.
///
/// An Array_s is an abstraction of a C array composed of a data buffer and a
/// length variable. It is a static array, that is, it won't increase in size.
/// Higher level languages provide a quick way to get the array's length and
/// this struct abstracts exactly that.
///
/// Every position of the array is set to \c NULL. As you add elements these
/// holes will be filled up. To set a given position in the array with a new
/// element that position needs to be equal to \c NULL, that is, an empty
/// space. To remove an element from a position, that position must not be
/// \c NULL. There are functions that will remove/add an item at the first/last
/// position if available and then will provide an index to where the change
/// was done.
///
/// To initialize the array use arr_init(). This only initializes the
/// structure. If you don't set the default functions later you won't be able
/// to do certain operations. If you want to initialize it completely, use
/// instead arr_create(). Here you must pass in default functions (compare,
/// copy, display and free) according with the specifications of each type of
/// function.
///
/// To add elements to the array use arr_set(). It will only work if the index
/// represents an empty slot. You can also use arr_set_next() to place an
/// element at the next available slot or arr_set_last() to place it in the
/// last available slot. These two last functions will return an index to where
/// the element was added to.
///
/// To remove elements you can use arr_pop(). You can also use arr_pop_next()
/// to remove the next available element or arr_pop_last() to remove tha last
/// available element.
///
/// Since access to the buffer is protected you can use arr_set() and arr_get()
/// to set an index to a given value or get a value from the specified index
/// respectively. You can also use arr_get_next() to get the next available
/// element or arr_get_last() to get the last available element.
///
/// Note that arr_set() will first check if the given index is an empty slot.
/// If so it will simply populate that position. If there is an element already
/// then it will use the default free function on it and then place the new
/// element at that position.
///
/// The array maintains a version id that keeps track of structural changes
/// done to the array. This prevents any iterators from working the moment the
/// array structure is changed. It works to prevent any undefined behaviour or
/// run-time errors.
struct Array_s
{
    /// \brief Data buffer.
    ///
    /// A C array where elements are stored in.
    void **buffer;

    /// \brief Array length.
    ///
    /// Tracks the C array length.
    integer_t length;

    /// \brief Comparator function.
    ///
    /// A function that compares one element with another that returns an int
    /// with the following rules:
    ///
    /// - <code>[ > 0 ]</code> if first element is greater than the second;
    /// - <code>[ < 0 ]</code> if second element is greater than the first;
    /// - <code>[ 0 ]</code> if elements are equal.
    arr_compare_f d_compare;

    /// \brief Copy function.
    ///
    /// A function that returns an exact copy of an element.
    arr_copy_f d_copy;

    /// \brief Display function.
    ///
    /// A function that displays an element in the console. Useful for
    /// debugging.
    arr_display_f d_display;

    /// \brief Deallocator function.
    ///
    /// A function that completely frees an element from memory.
    arr_free_f d_free;

    /// \brief A version id to keep track of modifications.
    ///
    /// This version id is used by the iterator to check if the structure was
    /// modified. The iterator can only function if its version_id is the same
    /// as the structure's version id, that is, there have been no structural
    /// modifications (except for those done by the iterator itself).
    integer_t version_id;
};

///////////////////////////////////////////////////// NOT EXPOSED FUNCTIONS ///

static void arr_quicksort(Array array, void **buffer, integer_t length);

////////////////////////////////////////////// END OF NOT EXPOSED FUNCTIONS ///

Status arr_init(Array *array, integer_t length)
{
    if (length <= 0)
        return DS_ERR_INVALID_ARGUMENT;

    (*array) = malloc(sizeof(Array_t));

    if (!(*array))
        return DS_ERR_ALLOC;

    (*array)->buffer = malloc(sizeof(void*) * length);

    if (!(*array)->buffer)
    {
        free(*array);

        *array = NULL;

        return DS_ERR_ALLOC;
    }

    (*array)->d_compare = NULL;
    (*array)->d_copy = NULL;
    (*array)->d_display = NULL;
    (*array)->d_free = NULL;

    for (integer_t i = 0; i < length; i++)
        (*array)->buffer[i] = NULL;

    (*array)->length = length;

    (*array)->version_id = 0;

    return DS_OK;
}

Status arr_create(Array *array, integer_t length, arr_compare_f compare_f,
                  arr_copy_f copy_f, arr_display_f display_f, arr_free_f free_f)
{
    if (length <= 0)
        return DS_ERR_INVALID_ARGUMENT;

    (*array) = malloc(sizeof(Array_t));

    if (!(*array))
        return DS_ERR_ALLOC;

    (*array)->buffer = malloc(sizeof(void*) * length);

    if (!(*array)->buffer)
    {
        free(*array);

        *array = NULL;

        return DS_ERR_ALLOC;
    }

    (*array)->d_compare = compare_f;
    (*array)->d_copy = copy_f;
    (*array)->d_display = display_f;
    (*array)->d_free = free_f;

    for (integer_t i = 0; i < length; i++)
        (*array)->buffer[i] = NULL;

    (*array)->length = length;

    (*array)->version_id = 0;

    return DS_OK;
}

Status arr_free(Array *array)
{
    if (*array == NULL)
        return DS_ERR_NULL_POINTER;

    if ((*array)->d_free == NULL)
        return DS_ERR_INCOMPLETE_TYPE;

    for (integer_t i = 0; i < (*array)->length; i++)
    {
        (*array)->d_free((*array)->buffer[i]);
    }

    free((*array)->buffer);
    free((*array));

    (*array) = NULL;

    return DS_OK;
}

Status arr_free_shallow(Array *array)
{
    if (*array == NULL)
        return DS_ERR_NULL_POINTER;

    free((*array)->buffer);
    free((*array));

    (*array) = NULL;

    return DS_OK;
}

Status arr_erase(Array *array)
{
    if (*array == NULL)
        return DS_ERR_NULL_POINTER;

    Array new_array;

    Status st = arr_create(&new_array, (*array)->length, (*array)->d_compare,
            (*array)->d_copy, (*array)->d_display, (*array)->d_free);

    if (st != DS_OK)
        return st;

    st = arr_free(array);

    // Probably didn't set the free function...
    if (st != DS_OK)
    {
        free(new_array->buffer);
        free(new_array);

        return st;
    }

    *array = new_array;

    return DS_OK;
}

Status arr_set_func_compare(Array array, arr_compare_f function)
{
    if (array == NULL)
        return DS_ERR_NULL_POINTER;

    array->d_compare = function;

    return DS_OK;
}

Status arr_set_func_copy(Array array, arr_copy_f function)
{
    if (array == NULL)
        return DS_ERR_NULL_POINTER;

    array->d_copy = function;

    return DS_OK;
}

Status arr_set_func_display(Array array, arr_display_f function)
{
    if (array == NULL)
        return DS_ERR_NULL_POINTER;

    array->d_display = function;

    return DS_OK;
}

Status arr_set_func_free(Array array, arr_free_f function)
{
    if (array == NULL)
        return DS_ERR_NULL_POINTER;

    array->d_free = function;

    return DS_OK;
}

integer_t arr_length(Array array)
{
    if (array == NULL)
        return -1;

    return array->length;
}

integer_t arr_count(Array array)
{
    if (array == NULL)
        return -1;

    integer_t count = 0;

    for (integer_t i = 0; i < array->length; i++)
    {
        if (array->buffer[i] != NULL)
            count++;
    }

    return count;
}

integer_t arr_set_next(Array array, void *element)
{
    if (array == NULL)
        return -2;

    for (integer_t i = 0; i < array->length; i++)
    {
        if (array->buffer[i] != NULL)
        {
            array->buffer[i] = element;

            array->version_id++;

            return i;
        }
    }

    // Array is full
    return -1;
}

Status arr_set(Array array, integer_t index, void *element)
{
    if (array == NULL)
        return DS_ERR_NULL_POINTER;

    if (index >= array->length)
        return DS_ERR_OUT_OF_RANGE;

    if (index < 0)
        return DS_ERR_NEGATIVE_VALUE;

    if (array->buffer[index] == NULL)
    {
        array->buffer[index] = element;

        array->version_id++;

        return DS_OK;
    }

    return DS_ERR_INVALID_OPERATION;
}

integer_t arr_set_last(Array array, void *element)
{
    if (array == NULL)
        return -2;

    for (integer_t i = array->length - 1; i >= 0; i--)
    {
        if (array->buffer[i] != NULL)
        {
            array->buffer[i] = element;

            array->version_id++;

            return i;
        }
    }

    // Array is full
    return -1;
}

void *arr_get_next(Array array, integer_t *index)
{
    *index = -1;

    if (array == NULL)
        return NULL;

    for (integer_t i = array->length - 1; i >= 0; i--)
    {
        if (array->buffer[i] != NULL)
        {
            *index = i;

            return array->buffer[i];
        }
    }

    // Array is empty
    return NULL;
}

void *arr_get(Array array, integer_t index)
{
    if (array == NULL)
        return NULL;

    if (index < 0 || index >= array->length)
        return NULL;

    // This might return NULL
    return array->buffer[index];
}

void *arr_get_last(Array array, integer_t *index)
{
    *index = -1;

    if (array == NULL)
        return NULL;

    for (integer_t i = array->length - 1; i >= 0; i--)
    {
        if (array->buffer[i] != NULL)
        {
            *index = i;

            return array->buffer[i];
        }
    }

    // Array is empty
    return NULL;
}

void *arr_pop_next(Array array, integer_t *index)
{
    *index = -1;

    if (array == NULL)
        return NULL;

    for (integer_t i = array->length - 1; i >= 0; i--)
    {
        if (array->buffer[i] != NULL)
        {
            void *element = array->buffer[i];

            array->buffer[i] = NULL;

            *index = i;

            array->version_id++;

            return element;
        }
    }

    return NULL;
}

void *arr_pop(Array array, integer_t index)
{
    if (array == NULL)
        return NULL;

    if (index < 0 || index >= array->length)
        return NULL;

    void *element = array->buffer[index];

    array->buffer[index] = NULL;

    array->version_id++;

    return element;
}

void *arr_pop_last(Array array, integer_t *index)
{
    *index = -1;

    if (array == NULL)
        return NULL;

    for (integer_t i = array->length - 1; i >= 0; i--)
    {
        if (array->buffer[i] != NULL)
        {
            void *element = array->buffer[i];

            array->buffer[i] = NULL;

            *index = i;

            array->version_id++;

            return element;
        }
    }

    return NULL;
}

bool arr_full(Array array)
{
    for (integer_t i = 0; i < array->length; i++)
    {
        if (array->buffer[i] == NULL)
            return false;
    }

    return true;
}

bool arr_empty(Array array)
{
    for (integer_t i = 0; i < array->length; i++)
    {
        if (array->buffer[i] != NULL)
            return false;
    }

    return true;
}

void *arr_max(Array array)
{
    if (array == NULL)
        return NULL;

    if (array->d_compare == NULL)
        return NULL;

    void *result = NULL;

    for (integer_t i = 0; i < array->length; i++)
    {
        // New element
        if (array->buffer[i] != NULL)
        {
            if (result == NULL)
                result = array->buffer[i];
            else if (array->d_compare(array->buffer[i], result) > 0)
            {
                result = array->buffer[i];
            }
        }
    }

    // Might return NULL if array is empty
    return result;
}

void *arr_min(Array array)
{
    if (array == NULL)
        return NULL;

    if (array->d_compare == NULL)
        return NULL;

    void *result = NULL;

    for (integer_t i = 0; i < array->length; i++)
    {
        // New element
        if (array->buffer[i] != NULL)
        {
            if (result == NULL)
                result = array->buffer[i];
            else if (array->d_compare(array->buffer[i], result) < 0)
            {
                result = array->buffer[i];
            }
        }
    }

    // Might return NULL if array is empty
    return result;
}

integer_t arr_index_first(Array array, void *key)
{
    if (array == NULL)
        return -3;

    if (array->d_compare == NULL)
        return -2;

    for (integer_t index = 0; index < array->length; index++)
    {
        if (array->buffer[index] != NULL)
            if (array->d_compare(array->buffer[index], key) == 0)
                return index;
    }

    // Not found
    return -1;
}

integer_t arr_index_last(Array array, void *key)
{
    if (array == NULL)
        return -3;

    if (array->d_compare == NULL)
        return -2;

    for (integer_t index = array->length - 1; index >= 0; index--)
    {
        if (array->buffer[index] != NULL)
            if (array->d_compare(array->buffer[index], key) == 0)
                return index;
    }

    // Not found
    return -1;
}

bool arr_contains(Array array, void *key)
{
    for (integer_t i = 0; i < array->length; i++)
    {
        if (array->buffer[i] != NULL)
            if (array->d_compare(array->buffer[i], key) == 0)
                return true;
    }

    return false;
}

Status arr_switch(Array array, integer_t pos1, integer_t pos2)
{
    if (array == NULL)
        return DS_ERR_NULL_POINTER;

    if (pos1 >= array->length || pos2 >= array->length)
        return DS_ERR_OUT_OF_RANGE;

    if (pos1 < 0 || pos2 < 0)
        return DS_ERR_NEGATIVE_VALUE;

    void *temp = array->buffer[pos1];
    array->buffer[pos1] = array->buffer[pos2];
    array->buffer[pos2] = temp;

    array->version_id++;

    return DS_OK;
}

Status arr_reverse(Array array)
{
    if (array == NULL)
        return DS_ERR_NULL_POINTER;

    Status st;

    for (integer_t i = 0; i < array->length / 2; i++)
    {
        st = arr_switch(array, i, array->length - i - 1);

        if (st != DS_OK)
            return st;
    }

    return DS_OK;
}

Status arr_copy(Array array, Array *result)
{
    if (array == NULL)
        return DS_ERR_NULL_POINTER;

    if (array->d_copy == NULL)
        return DS_ERR_INCOMPLETE_TYPE;

    Status st = arr_create(result, array->length, array->d_compare,
                           array->d_copy, array->d_display, array->d_free);

    if (st != DS_OK)
        return st;

    for (integer_t i = 0; i < array->length; i++)
        (*result)->buffer[i] = array->buffer[i] == NULL ? NULL :
                               array->d_copy(array->buffer[i]);

    return DS_OK;
}

Status arr_sort(Array array)
{
    if (array == NULL)
        return DS_ERR_NULL_POINTER;

    if (array->d_compare == NULL)
        return DS_ERR_INCOMPLETE_TYPE;

    arr_quicksort(array, array->buffer, array->length);

    array->version_id++;

    return DS_OK;
}

Status arr_to_array(Array array, void ***result, integer_t *length)
{
    // If anything goes wrong...
    *result = NULL;
    *length = -1;

    if (array == NULL)
        return DS_ERR_NULL_POINTER;

    if (array->d_copy == NULL)
        return DS_ERR_INCOMPLETE_TYPE;

    *length = array->length;

    *result = malloc(sizeof(void*) * (*length));

    if (!(*result))
        return DS_ERR_NULL_POINTER;

    for (integer_t i = 0; i < *length; i++)
    {
        (*result)[i] = (array->buffer[i] == NULL) ?
                NULL :
                array->d_copy(array->buffer[i]);
    }

    return DS_OK;
}

Status arr_display(Array array)
{
    if (array == NULL)
        return DS_ERR_NULL_POINTER;

    if (array->d_display == NULL)
        return DS_ERR_INCOMPLETE_TYPE;

    if (arr_empty(array))
    {
        printf("\nArray\n[ empty ]\n");

        return DS_OK;
    }

    printf("\nArray\n[ ");

    for (integer_t i = 0; i < array->length - 1; i++)
    {
        array->d_display(array->buffer[i]);

        printf(", ");
    }

    array->d_display(array->buffer[array->length - 1]);

    printf(" ]\n");

    return DS_OK;
}

Status arr_display_raw(Array array)
{
    if (array == NULL)
        return DS_ERR_NULL_POINTER;

    if (array->d_display == NULL)
        return DS_ERR_INCOMPLETE_TYPE;

    printf("\n");

    for (integer_t i = 0; i < array->length - 1; i++)
    {
        array->d_display(array->buffer[i]);

        printf(" ");
    }

    printf("\n");

    return DS_OK;
}

///////////////////////////////////////////////////// NOT EXPOSED FUNCTIONS ///

static void arr_quicksort(Array array, void **buffer, integer_t length)
{
    if (length < 2)
        return;

    void *pivot = buffer[length / 2];

    integer_t i, j;
    for (i = 0, j = length - 1; ; i++, j--)
    {
        while (array->d_compare(buffer[i], pivot) < 0)
            i++;

        while (array->d_compare(buffer[j], pivot) > 0)
            j--;

        if (i >= j)
            break;

        void *temp = buffer[i];
        buffer[i] = buffer[j];
        buffer[j] = temp;
    }

    arr_quicksort(array, buffer, i);
    arr_quicksort(array, buffer + i, length - i);
}

////////////////////////////////////////////// END OF NOT EXPOSED FUNCTIONS ///

///////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////// Iterator ///
///////////////////////////////////////////////////////////////////////////////

/// \brief An iterator for an Array_s.
///
/// A simple iterator that can traverse backwards and forwards through the
/// elements of the array.
struct ArrayIterator_s
{
    /// \brief Target Array_s.
    ///
    /// Target Array_s. The iterator might need to use some information
    /// provided by the array or change some of its data members.
    struct Array_s *target;

    /// \brief Current element.
    ///
    /// Index of the current element pointed by the cursor;
    integer_t cursor;

    /// \brief Target version ID.
    ///
    /// When the iterator is initialized it stores the version_id of the target
    /// structure.
    integer_t target_id;
};

///////////////////////////////////////////////////// NOT EXPOSED FUNCTIONS ///

static bool arr_iter_target_modified(ArrayIterator iter);

static bool arr_iter_invalid_state(ArrayIterator iter);

////////////////////////////////////////////// END OF NOT EXPOSED FUNCTIONS ///

Status arr_iter_init(ArrayIterator *iter, Array target)
{
    *iter = malloc(sizeof(ArrayIterator_t));

    if (!(*iter))
        return DS_ERR_ALLOC;

    (*iter)->target = target;
    (*iter)->target_id = target->version_id;
    (*iter)->cursor = 0;

    return DS_OK;
}

Status arr_iter_retarget(ArrayIterator *iter, Array target)
{
    Status st = arr_iter_free(iter);

    if (st != DS_OK)
        return st;

    st = arr_iter_init(iter, target);

    if (st != DS_OK)
        return st;

    return DS_OK;
}

Status arr_iter_free(ArrayIterator *iter)
{
    if (*iter == NULL)
        return DS_ERR_NULL_POINTER;

    free(*iter);

    *iter = NULL;

    return DS_OK;
}

Status arr_iter_next(ArrayIterator iter)
{
    if (iter == NULL)
        return DS_ERR_NULL_POINTER;

    if (arr_iter_invalid_state(iter))
        return DS_ERR_ITER_STATE;

    if (arr_iter_target_modified(iter))
        return DS_ERR_ITER_MODIFICATION;

    if (!arr_iter_has_next(iter))
        return DS_ERR_ITER;

    iter->cursor++;

    return DS_OK;
}

Status arr_iter_prev(ArrayIterator iter)
{
    if (iter == NULL)
        return DS_ERR_NULL_POINTER;

    if (arr_iter_invalid_state(iter))
        return DS_ERR_ITER_STATE;

    if (arr_iter_target_modified(iter))
        return DS_ERR_ITER_MODIFICATION;

    if (!arr_iter_has_next(iter))
        return DS_ERR_ITER;

    iter->cursor--;

    return DS_OK;
}

Status arr_iter_to_start(ArrayIterator iter)
{
    if (iter == NULL)
        return DS_ERR_NULL_POINTER;

    if (arr_iter_invalid_state(iter))
        return DS_ERR_ITER_STATE;

    if (arr_iter_target_modified(iter))
        return DS_ERR_ITER_MODIFICATION;

    iter->cursor = 0;

    return DS_OK;
}

Status arr_iter_to_end(ArrayIterator iter)
{
    if (iter == NULL)
        return DS_ERR_NULL_POINTER;

    if (arr_iter_invalid_state(iter))
        return DS_ERR_ITER_STATE;

    if (arr_iter_target_modified(iter))
        return DS_ERR_ITER_MODIFICATION;

    iter->cursor = iter->target->length - 1;

    return DS_OK;
}

bool arr_iter_has_next(ArrayIterator iter)
{
    return iter->cursor < iter->target->length - 1;
}

bool arr_iter_has_prev(ArrayIterator iter)
{
    return iter->cursor > 0;
}

Status arr_iter_set(ArrayIterator iter, void *element)
{
    if (iter == NULL)
        return DS_ERR_NULL_POINTER;

    if (arr_iter_invalid_state(iter))
        return DS_ERR_ITER_STATE;

    if (arr_iter_target_modified(iter))
        return DS_ERR_ITER_MODIFICATION;

    if (iter->target->d_free == NULL)
        return DS_ERR_INCOMPLETE_TYPE;

    if (iter->target->buffer[iter->cursor] != NULL)
        iter->target->d_free(iter->target->buffer[iter->cursor]);

    iter->target->buffer[iter->cursor] = element;

    iter->target_id++;
    iter->target->version_id++;

    return DS_OK;
}

Status arr_iter_get(ArrayIterator iter, void **result)
{
    if (iter == NULL)
        return DS_ERR_NULL_POINTER;

    if (arr_iter_invalid_state(iter))
        return DS_ERR_ITER_STATE;

    if (arr_iter_target_modified(iter))
        return DS_ERR_ITER_MODIFICATION;

    if (iter->target->d_copy == NULL)
        return DS_ERR_INCOMPLETE_TYPE;

    if (iter->target->buffer[iter->cursor] != NULL)
        *result = iter->target->d_copy(iter->target->buffer[iter->cursor]);
    else
        *result = NULL;

    return DS_OK;
}

Status arr_iter_pop(ArrayIterator iter, void **result)
{
    if (iter == NULL)
        return DS_ERR_NULL_POINTER;

    if (arr_iter_invalid_state(iter))
        return DS_ERR_ITER_STATE;

    if (arr_iter_target_modified(iter))
        return DS_ERR_ITER_MODIFICATION;

    *result = iter->target->buffer[iter->cursor];

    iter->target->buffer[iter->cursor] = NULL;

    iter->target_id++;
    iter->target->version_id++;

    return DS_OK;
}

void *arr_iter_peek_next(ArrayIterator iter)
{
    if (iter == NULL)
        return NULL;

    if (arr_iter_invalid_state(iter))
        return NULL;

    if (arr_iter_target_modified(iter))
        return NULL;

    if (!arr_iter_has_next(iter))
        return NULL;

    return iter->target->buffer[iter->cursor + 1];
}

void *arr_iter_peek(ArrayIterator iter)
{
    if (iter == NULL)
        return NULL;

    if (arr_iter_invalid_state(iter))
        return NULL;

    if (arr_iter_target_modified(iter))
        return NULL;

    return iter->target->buffer[iter->cursor];
}

void *arr_iter_peek_prev(ArrayIterator iter)
{
    if (iter == NULL)
        return NULL;

    if (arr_iter_invalid_state(iter))
        return NULL;

    if (arr_iter_target_modified(iter))
        return NULL;

    if (!arr_iter_has_prev(iter))
        return NULL;

    return iter->target->buffer[iter->cursor - 1];
}

///////////////////////////////////////////////////// NOT EXPOSED FUNCTIONS ///

static bool arr_iter_target_modified(ArrayIterator iter)
{
    return iter->target_id != iter->target->version_id;
}

static bool arr_iter_invalid_state(ArrayIterator iter)
{
    if (iter->target == NULL)
        return true;

    return iter->cursor < 0 || iter->cursor >= iter->target->length;
}

////////////////////////////////////////////// END OF NOT EXPOSED FUNCTIONS ///