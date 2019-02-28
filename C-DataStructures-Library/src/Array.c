/**
 * @file Array.c
 * 
 * @author Leonardo Vencovsky (https://github.com/LeoVen)
 *
 * @date 07/10/2018
 */

#include "Array.h"

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
///
/// \par Functions
/// Located in the file Array.c
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

    /// \brief Array's current amount of elements.
    ///
    /// Tracks the non-null elements in the array.
    integer_t count;

    /// \brief Array_s interface.
    ///
    /// An interface is like a table that has function pointers for functions
    /// that will manipulate a desired data type.
    struct Interface_s *interface;

    /// \brief A version id to keep track of modifications.
    ///
    /// This version id is used by the iterator to check if the structure was
    /// modified. The iterator can only function if its version_id is the same
    /// as the structure's version id, that is, there have been no structural
    /// modifications (except for those done by the iterator itself).
    integer_t version_id;
};

///////////////////////////////////////////////////// NOT EXPOSED FUNCTIONS ///

static void
arr_quicksort(Array_t *array, void **buffer, integer_t length,
                          compare_f comparator);

////////////////////////////////////////////// END OF NOT EXPOSED FUNCTIONS ///

/// Initializes a new Array_s with a custom interface and a defined length.
///
/// \param[in] interface An interface for the new array.
/// \param[in] length The fixed length for the array.
///
/// \return A new Array_s or NULL if allocation failed.
Array_t *
arr_new(Interface_t *interface, integer_t length)
{
    Array_t *array = malloc(sizeof(Array_t));

    if (!array)
        return NULL;

    array->buffer = malloc(sizeof(void*) * (size_t)length);

    if (!array->buffer)
    {
        free(array);

        return NULL;
    }

    for (integer_t i = 0; i < length; i++)
        array->buffer[i] = NULL;

    array->interface = interface;
    array->length = length;
    array->count = 0;
    array->version_id = 0;

    return array;
}

///
/// \param[in] array
void
arr_free(Array_t *array)
{
    for (integer_t i = 0; i < array->length; i++)
    {
        if (array->buffer[i] != NULL)
            array->interface->free(array->buffer[i]);
    }

    free(array->buffer);
    free(array);
}

///
/// \param[in] array
void
arr_free_shallow(Array_t *array)
{
    free(array->buffer);
    free(array);
}

///
/// \param[in] array
void
arr_erase(Array_t *array)
{
    for (integer_t i = 0; i < array->length; i++)
    {
        array->interface->free(array->buffer[i]);

        array->buffer[i] = NULL;
    }
}

///
/// \param[in] array
void
arr_erase_shallow(Array_t *array)
{
    for (integer_t i = 0; i < array->length; i++)
    {
        array->buffer[i] = NULL;
    }
}

///
/// \param[in] array
/// \param[in] interface
void
arr_config(Array_t *array, Interface_t *interface)
{
    array->interface = interface;
}

///
/// \param[in] array
///
/// \return
integer_t
arr_length(Array_t *array)
{
    return array->length;
}

///
/// \param[in] array
///
/// \return
integer_t
arr_count(Array_t *array)
{
    return array->count;
}

///
/// \param[in] array
/// \param[in] element
///
/// \return
integer_t
arr_set_first(Array_t *array, void *element)
{
    for (integer_t i = 0; i < array->length; i++)
    {
        if (array->buffer[i] == NULL)
        {
            array->buffer[i] = element;

            array->count += (element == NULL) ? 0 : 1;
            array->version_id++;

            return i;
        }
    }

    // Array is full
    return -1;
}

///
/// \param[in] array
/// \param[in] element
/// \param[in] index
///
/// \return
integer_t
arr_set(Array_t *array, void *element, integer_t index)
{
    if (index < 0 || index >= array->length)
        return -2;

    if (array->buffer[index] == NULL)
    {
        array->buffer[index] = element;

        array->count += (element == NULL) ? 0 : 1;
        array->version_id++;

        return 0;
    }

    // Index is already taken by another element
    return -3;
}

///
/// \param[in] array
/// \param[in] element
///
/// \return
integer_t
arr_set_last(Array_t *array, void *element)
{
    for (integer_t i = array->length - 1; i >= 0; i--)
    {
        if (array->buffer[i] == NULL)
        {
            array->buffer[i] = element;

            array->count += (element == NULL) ? 0 : 1;
            array->version_id++;

            return i;
        }
    }

    // Array is full
    return -1;
}

///
/// \param[in] array
/// \param[out] result
///
/// \return
integer_t
arr_get_first(Array_t *array, void **result)
{
    *result = NULL;

    for (integer_t i = 0; i < array->length; i++)
    {
        if (array->buffer[i] != NULL)
        {
            *result = array->buffer[i];

            return i;
        }
    }

    // Array is empty
    return -1;
}

///
/// \param[in] array
/// \param[out] result
/// \param[in] index
///
/// \return
integer_t
arr_get(Array_t *array, void **result, integer_t index)
{
    if (index < 0 || index >= array->length)
        return -2;

    // This might return NULL
    *result =  array->buffer[index];

    return (*result == NULL) ? -1 : 0;
}

///
/// \param[in] array
/// \param[out] result
///
/// \return
integer_t
arr_get_last(Array_t *array, void **result)
{
    *result = NULL;

    for (integer_t i = array->length - 1; i >= 0; i--)
    {
        if (array->buffer[i] != NULL)
        {
            *result = array->buffer[i];

            return i;
        }
    }

    // Array is empty
    return -1;
}

///
/// \param[in] array
/// \param[out] result
///
/// \return
integer_t
arr_remove_first(Array_t *array, void **result)
{
    *result = NULL;

    for (integer_t i = 0; i < array->length; i++)
    {
        if (array->buffer[i] != NULL)
        {
            *result = array->buffer[i];
            array->buffer[i] = NULL;

            array->count--;
            array->version_id++;

            return i;
        }
    }

    // Array is empty
    return -1;
}

///
/// \param[in] array
/// \param[out] result
/// \param[in] index
///
/// \return
integer_t
arr_remove(Array_t *array, void **result, integer_t index)
{
    *result = NULL;

    if (index < 0 || index >= array->length)
        return -2;

    if (array->buffer[index] != NULL)
    {
        *result = array->buffer[index];
        array->buffer[index] = NULL;

        array->count--;
        array->version_id++;

        return 0;
    }

    return -1;
}

///
/// \param[in] array
/// \param[out] result
///
/// \return
integer_t
arr_remove_last(Array_t *array, void **result)
{
    *result = NULL;

    for (integer_t i = array->length - 1; i >= 0; i--)
    {
        if (array->buffer[i] != NULL)
        {
            *result = array->buffer[i];
            array->buffer[i] = NULL;

            array->count--;
            array->version_id++;

            return i;
        }
    }

    // Array is empty
    return -1;
}

///
/// \param[in] array
/// \param[in] element
///
/// \return
integer_t
arr_update_first(Array_t *array, void *element)
{
    for (integer_t i = 0; i < array->length; i++)
    {
        if (array->buffer[i] != NULL)
        {
            void *replaced = array->buffer[i];
            array->buffer[i] = element;

            array->interface->free(replaced);

            array->version_id++;

            if (element == NULL)
                array->count--;

            return i;
        }
    }

    // Array is empty
    return -1;
}

///
/// \param[in] array
/// \param[in] element
/// \param[in] index
///
/// \return
integer_t
arr_update(Array_t *array, void *element, integer_t index)
{
    if (index < 0 || index >= array->length)
        return -2;

    void *replaced = array->buffer[index];
    array->buffer[index] = element;

    if (replaced)
        array->interface->free(replaced);

    if (element == NULL)
        array->count--;

    array->version_id++;

    return 0;
}

///
/// \param[in] array
/// \param[in] element
///
/// \return
integer_t
arr_update_last(Array_t *array, void *element)
{
    for (integer_t i = array->length - 1; i >= 0; i--)
    {
        if (array->buffer[i] != NULL)
        {
            void *replaced = array->buffer[i];
            array->buffer[i] = element;

            array->interface->free(replaced);

            array->version_id++;

            if (element == NULL)
                array->count--;

            return i;
        }
    }

    // Array is empty
    return -1;
}

///
/// \param[in] array
///
/// \return
bool
arr_full(Array_t *array)
{
    return array->count >= array->length;
}

///
/// \param[in] array
///
/// \return
bool
arr_empty(Array_t *array)
{
    return array->count <= 0;
}

///
/// \param[in] array
///
/// \return
void *
arr_max(Array_t *array)
{
    void *result = NULL;

    for (integer_t i = 0; i < array->length; i++)
    {
        if (array->buffer[i] != NULL)
        {
            if (result == NULL)
                result = array->buffer[i];
            else if (array->interface->compare(array->buffer[i], result) > 0)
            {
                result = array->buffer[i];
            }
        }
    }

    // Might return NULL if array is empty
    return result;
}

///
/// \param[in] array
///
/// \return
void *
arr_min(Array_t *array)
{
    void *result = NULL;

    for (integer_t i = 0; i < array->length; i++)
    {
        if (array->buffer[i] != NULL)
        {
            if (result == NULL)
                result = array->buffer[i];
            else if (array->interface->compare(array->buffer[i], result) < 0)
            {
                result = array->buffer[i];
            }
        }
    }

    // Might return NULL if array is empty
    return result;
}

///
/// \param[in] array
/// \param[in] key
///
/// \return
integer_t
arr_index_first(Array_t *array, void *key)
{
    for (integer_t index = 0; index < array->length; index++)
    {
        if (array->buffer[index] != NULL)
            if (array->interface->compare(array->buffer[index], key) == 0)
                return index;
    }

    // Not found
    return -1;
}

///
/// \param[in] array
/// \param[in] key
///
/// \return
integer_t
arr_index_last(Array_t *array, void *key)
{
    for (integer_t index = array->length - 1; index >= 0; index--)
    {
        if (array->buffer[index] != NULL)
            if (array->interface->compare(array->buffer[index], key) == 0)
                return index;
    }

    // Not found
    return -1;
}

///
/// \param[in] array
/// \param[in] key
///
/// \return
bool
arr_contains(Array_t *array, void *key)
{
    for (integer_t i = 0; i < array->length; i++)
    {
        if (array->buffer[i] != NULL)
            if (array->interface->compare(array->buffer[i], key) == 0)
                return true;
    }

    return false;
}

///
/// \param[in] array
/// \param[in] index1
/// \param[in] index2
///
/// \return
integer_t
arr_switch(Array_t *array, integer_t index1, integer_t index2)
{
    if (index1 < 0 || index1 >= array->length ||
        index2 < 0 || index2 >= array->length)
        return -2;

    void *temp = array->buffer[index1];
    array->buffer[index1] = array->buffer[index2];
    array->buffer[index2] = temp;

    array->version_id++;

    return 0;
}

///
/// \param[in] array
///
/// \return
integer_t
arr_reverse(Array_t *array)
{
    for (integer_t i = 0; i < array->length / 2; i++)
    {
        if (arr_switch(array, i, array->length - i - 1) < 0)
            return -1;
    }

    return 0;
}

///
/// \param[in] array
///
/// \return
Array_t *
arr_copy(Array_t *array)
{
    Array_t *result = arr_new(array->interface, array->length);

    if (!result)
        return NULL;

    for (integer_t i = 0; i < array->length; i++)
    {
        result->buffer[i] = array->buffer[i] == NULL
                            ? NULL
                            : array->interface->copy(array->buffer[i]);
    }

    return result;
}

///
/// \param[in] array
///
/// \return
Array_t *
arr_copy_shallow(Array_t *array)
{
    Array_t *result = arr_new(array->interface, array->length);

    if (!result)
        return NULL;

    for (integer_t i = 0; i < array->length; i++)
    {
        result->buffer[i] = array->buffer[i];
    }

    return result;
}

///
/// \param[in] array
void
arr_sort(Array_t *array)
{
    arr_quicksort(array, array->buffer, array->length,
            array->interface->compare);

    array->version_id++;
}

///
/// \param[in] array
/// \param[in] comparator
void
arr_sortby(Array_t *array, compare_f comparator)
{
    arr_quicksort(array, array->buffer, array->length, comparator);

    array->version_id++;
}

///
/// \param[in] array
/// \param[out] length
/// \param[in] shallow
///
/// \return
void **
arr_to_array(Array_t *array, integer_t *length, bool shallow)
{
    void **result = malloc(sizeof(void*) * (size_t)(*length));

    if (!(*result))
        return false;

    *length = array->length;

    if (shallow)
    {
        for (integer_t i = 0; i < *length; i++)
        {
            result[i] = array->buffer[i];
        }
    }
    else
    {
        for (integer_t i = 0; i < *length; i++)
        {
            result[i] = array->buffer[i] == NULL
                        ? NULL
                        : array->interface->copy(array->buffer[i]);
        }
    }

    return result;
}

///
/// \param[in] interface
/// \param[in] buffer
/// \param[in] length
///
/// \return
Array_t *
arr_from_array(Interface_t *interface, void **buffer, integer_t length)
{
    if (length <= 0)
        return NULL;

    Array_t *result = arr_new(interface, length);

    if (!result)
        return NULL;

    for (integer_t i = 0; i < length; i++)
    {
        result->buffer[i] = buffer[i];
    }

    result->version_id++;

    return result;
}

///
/// \param[in] array
/// \param[in] display_mode
void
arr_display(Array_t *array, int display_mode)
{
    if (arr_empty(array) && display_mode)
    {
        printf("\nArray\n[ empty ]\n");

        return;
    }

    switch(display_mode)
    {
        case 0:
            printf("\n");
            for (integer_t i = 0; i < array->length - 1; i++)
            {
                if (array->buffer[i])
                {
                    array->interface->display(array->buffer[i]);
                    printf(" ");
                }
            }
            array->interface->display(array->buffer[array->length - 1]);
            printf("\n");
            break;
        default:
            printf("\nArray\n[ ");
            for (integer_t i = 0; i < array->length - 1; i++)
            {
                if (array->buffer[i])
                    array->interface->display(array->buffer[i]);
                else
                    printf("NULL");
                printf(", ");
            }
            array->interface->display(array->buffer[array->length - 1]);
            printf(" ]\n");
            break;
    }
}

///////////////////////////////////////////////////// NOT EXPOSED FUNCTIONS ///

static void
arr_quicksort(Array_t *array, void **buffer, integer_t length,
                          compare_f comparator)
{
    if (length < 2)
        return;

    void *pivot = buffer[length / 2];

    integer_t i, j;
    for (i = 0, j = length - 1; ; i++, j--)
    {
        while (comparator(buffer[i], pivot) < 0)
            i++;

        while (comparator(buffer[j], pivot) > 0)
            j--;

        if (i >= j)
            break;

        void *temp = buffer[i];
        buffer[i] = buffer[j];
        buffer[j] = temp;
    }

    arr_quicksort(array, buffer, i, comparator);
    arr_quicksort(array, buffer + i, length - i, comparator);
}

////////////////////////////////////////////// END OF NOT EXPOSED FUNCTIONS ///

///////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////// Iterator ///
///////////////////////////////////////////////////////////////////////////////

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

static bool
arr_iter_target_modified(ArrayIterator_t *iter);

////////////////////////////////////////////// END OF NOT EXPOSED FUNCTIONS ///

///
/// \param[in] target
///
/// \return
ArrayIterator_t *
arr_iter_new(Array_t *target)
{
    ArrayIterator_t *iter = malloc(sizeof(ArrayIterator_t));

    if (!iter)
        return NULL;

    iter->target = target;
    iter->target_id = target->version_id;
    iter->cursor = 0;

    return iter;
}

///
/// \param[in] iter
/// \param[in] target
///
/// \return
bool
arr_iter_retarget(ArrayIterator_t *iter, Array_t *target)
{
    iter->target = target;
    iter->target_id = target->version_id;
    iter->cursor = 0;

    return true;
}

///
/// \param[in] iter
void
arr_iter_free(ArrayIterator_t *iter)
{
    free(iter);
}

///
/// \param[in] iter
///
/// \return
bool
arr_iter_next(ArrayIterator_t *iter)
{
    if (arr_iter_target_modified(iter))
        return false;

    if (!arr_iter_has_next(iter))
        return false;

    iter->cursor++;

    return true;
}

///
/// \param[in] iter
///
/// \return
bool
arr_iter_prev(ArrayIterator_t *iter)
{
    if (arr_iter_target_modified(iter))
        return false;

    if (!arr_iter_has_prev(iter))
        return false;

    iter->cursor--;

    return false;
}

///
/// \param[in] iter
///
/// \return
bool
arr_iter_to_start(ArrayIterator_t *iter)
{
    if (arr_iter_target_modified(iter))
        return false;

    iter->cursor = 0;

    return true;
}

///
/// \param[in] iter
///
/// \return
bool
arr_iter_to_end(ArrayIterator_t *iter)
{
    if (arr_iter_target_modified(iter))
        return false;

    iter->cursor = iter->target->length - 1;

    return true;
}

///
/// \param[in] iter
///
/// \return
bool
arr_iter_has_next(ArrayIterator_t *iter)
{
    return iter->cursor < iter->target->length - 1;
}

///
/// \param[in] iter
///
/// \return
bool
arr_iter_has_prev(ArrayIterator_t *iter)
{
    return iter->cursor > 0;
}

bool
arr_iter_test(ArrayIterator_t *iter)
{
    return iter->target->buffer[iter->cursor] != NULL;
}

///
/// \param[in] iter
/// \param[in] element
///
/// \return
bool
arr_iter_set(ArrayIterator_t *iter, void *element)
{
    if (arr_iter_target_modified(iter))
        return false;

    if (iter->target->buffer[iter->cursor] != NULL)
        iter->target->interface->free(iter->target->buffer[iter->cursor]);

    iter->target->buffer[iter->cursor] = element;

    iter->target_id++;
    iter->target->version_id++;

    return true;
}

///
/// \param[in] iter
/// \param[in] result
///
/// \return
bool
arr_iter_get(ArrayIterator_t *iter, void **result)
{
    if (arr_iter_target_modified(iter))
        return false;

    if (iter->target->buffer[iter->cursor] != NULL)
        *result = iter->target->interface->copy(iter->target->buffer[iter->cursor]);
    else
        *result = NULL;

    return true;
}

///
/// \param[in] iter
/// \param[in] result
///
/// \return
bool
arr_iter_pop(ArrayIterator_t *iter, void **result)
{
    if (arr_iter_target_modified(iter))
        return false;

    *result = iter->target->buffer[iter->cursor];

    iter->target->buffer[iter->cursor] = NULL;

    iter->target_id++;
    iter->target->version_id++;

    return true;
}

///
/// \param[in] iter
///
/// \return
void *
arr_iter_peek_next(ArrayIterator_t *iter)
{
    if (arr_iter_target_modified(iter))
        return NULL;

    if (!arr_iter_has_next(iter))
        return NULL;

    return iter->target->buffer[iter->cursor + 1];
}

///
/// \param[in] iter
///
/// \return
void *
arr_iter_peek(ArrayIterator_t *iter)
{
    if (arr_iter_target_modified(iter))
        return NULL;

    return iter->target->buffer[iter->cursor];
}

///
/// \param[in] iter
///
/// \return
void *
arr_iter_peek_prev(ArrayIterator_t *iter)
{
    if (arr_iter_target_modified(iter))
        return NULL;

    if (!arr_iter_has_prev(iter))
        return NULL;

    return iter->target->buffer[iter->cursor - 1];
}

///////////////////////////////////////////////////// NOT EXPOSED FUNCTIONS ///

static bool
arr_iter_target_modified(ArrayIterator_t *iter)
{
    return iter->target_id != iter->target->version_id;
}

////////////////////////////////////////////// END OF NOT EXPOSED FUNCTIONS ///

///////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////// Wrapper ///
///////////////////////////////////////////////////////////////////////////////

/// \todo ArrayWrapper