/**
 * @file DynamicArray.c
 * 
 * @author Leonardo Vencovsky (https://github.com/LeoVen)
 *
 * @date 01/10/2018
 */

#include "DynamicArray.h"

/// \brief A C generic array wrapper with dynamic size.
///
/// A DynamicArray_s is an Array_s that grows in size when needed. It has a
/// \c capacity that grows according to \c growth_rate. Both parameters can be
/// set by the user using the function dar_create() when creating a new array.
/// It can also be locked, disabling the buffer's growth, or unlocked, enabling
/// it back.
///
///
///
struct DynamicArray_s
{
    /// \brief Data buffer.
    ///
    /// Buffer where elements are stored in.
    void **buffer;

    /// \brief Current amount of elements in the \c DynamicArray.
    ///
    /// Current amount of elements in the \c DynamicArray.
    integer_t size;

    /// \brief Buffer maximum capacity.
    ///
    /// Buffer maximum capacity. When \c size reaches \c capacity the buffer is
    /// reallocated and increases according to \c growth_rate.
    integer_t capacity;

    /// \brief Buffer growth rate.
    ///
    /// Buffer growth rate. The new buffer capacity is calculated as:
    ///
    /// <code> capacity *= (growth_rate / 100.0) </code>
    integer_t growth_rate;

    /// \brief Flag for locked capacity.
    ///
    /// If \c locked is set to true the buffer will not grow and if any
    /// elements are inserted with a full buffer \c DS_ERR_FULL will be
    /// returned.
    bool locked;

    /// \brief Comparator function.
    ///
    /// A function that compares one element with another that returns an int
    /// with the following rules:
    ///
    /// - <code>[ > 0 ]</code> if first element is greater than the second;
    /// - <code>[ < 0 ]</code> if second element is greater than the first;
    /// - <code>[ 0 ]</code> if elements are equal.
    dar_compare_f v_compare;

    /// \brief Copy function.
    ///
    /// A function that returns an exact copy of an element.
    dar_copy_f v_copy;

    /// \brief Display function.
    ///
    /// A function that displays an element in the console. Useful for
    /// debugging.
    dar_display_f v_display;

    /// \brief Deallocator function.
    ///
    /// A function that completely frees an element from memory.
    dar_free_f v_free;

    /// \brief A version id to keep track of modifications.
    ///
    /// This version id is used by the iterator to check if the structure was
    /// modified. The iterator can only function if its version_id is the same
    /// as the structure's version id, that is, there have been no structural
    /// modifications (except for those done by the iterator itself).
    integer_t version_id;
};

///////////////////////////////////////////////////// NOT EXPOSED FUNCTIONS ///

Status dar_grow(DynamicArray d_array);

static void dar_quicksort(DynamicArray d_array, void **buffer, integer_t size);

////////////////////////////////////////////// END OF NOT EXPOSED FUNCTIONS ///

/// \brief Initializes a DynamicArray_s structure.
///
/// Initializes a DynamicArray_s with an initial capacity of 32 and a growth
/// rate of 200, that is, twice the size after each growth. This function does
/// not sets any default functions. If you don't set them later you won't be
/// able to do certain operations that depend on a user-defined function.
///
/// \param[in,out] d_array The dynamic array to be initialized.
///
/// \return DS_ERR_ALLOC if dynamic array allocation failed.
/// \return DS_OK if all operations were successful.
Status dar_init(DynamicArray *d_array)
{
    *d_array = malloc(sizeof(DynamicArray_t));

    if (!(*d_array))
        return DS_ERR_ALLOC;

    (*d_array)->buffer = calloc(32, sizeof(void*));

    if (!((*d_array)->buffer))
    {
        free(*d_array);

        *d_array = NULL;

        return DS_ERR_ALLOC;
    }

    (*d_array)->capacity = 32;
    (*d_array)->growth_rate = 200;

    (*d_array)->size = 0;
    (*d_array)->locked = false;

    (*d_array)->version_id = 0;

    (*d_array)->v_compare = NULL;
    (*d_array)->v_copy = NULL;
    (*d_array)->v_display = NULL;
    (*d_array)->v_free = NULL;

    return DS_OK;
}

/// \brief Creates a DynamicArray_s.
///
/// Initializes a \c DynamicArray with a user defined \c initial_capacity and
/// \c growth_rate. This function only accepts an \c initial_capacity greater
/// than 4 and a \c growth_rate greater than 100; but keep in mind that in some
/// cases if the \c initial_capacity is too small and the \c growth_rate is too
/// close to 100 there won't be an increase in capacity and the minimum growth
/// will be triggered. This function makes a complete DynamicArray_s, setting
/// all necessary user-defined functions.
///
/// \param[in,out] d_array The dynamic array to be initialized.
/// \param[in] initial_capacity Buffer initial capacity.
/// \param[in] growth_rate Buffer growth rate.
/// \param[in] compare_f A function that compares two elements.
/// \param[in] copy_f A function that makes an exact copy of an element.
/// \param[in] display_f A function that displays in the console an element.
/// \param[in] free_f A function that completely frees from memory an element.
///
/// \return DS_ERR_ALLOC if dynamic array allocation failed.
/// \return DS_ERR_INVALID_ARGUMENT if initial_capacity is less than 4 or
/// growth_rate is less than or equal to 100.
/// \return DS_OK if all operations were successful.
Status dar_create(DynamicArray *d_array, integer_t initial_capacity,
        integer_t growth_rate, dar_compare_f compare_f, dar_copy_f copy_f,
        dar_display_f display_f, dar_free_f free_f)
{
    if (initial_capacity < 4 || growth_rate <= 100)
        return DS_ERR_INVALID_ARGUMENT;

    *d_array = malloc(sizeof(DynamicArray_t));

    if (!(*d_array))
        return DS_ERR_ALLOC;

    (*d_array)->buffer = malloc(sizeof(void*) * initial_capacity);

    if (!((*d_array)->buffer))
    {
        free(*d_array);

        *d_array = NULL;

        return DS_ERR_ALLOC;
    }

    for (integer_t i = 0; i < initial_capacity; i++)
        (*d_array)->buffer[i] = NULL;

    (*d_array)->capacity = initial_capacity;
    (*d_array)->growth_rate = growth_rate;
    (*d_array)->size = 0;

    (*d_array)->locked = false;

    (*d_array)->version_id = 0;

    (*d_array)->v_compare = compare_f;
    (*d_array)->v_copy = copy_f;
    (*d_array)->v_display = display_f;
    (*d_array)->v_free = free_f;

    return DS_OK;
}

Status dar_free(DynamicArray *d_array)
{
    if (*d_array == NULL)
        return DS_ERR_NULL_POINTER;

    if ((*d_array)->v_free == NULL)
        return DS_ERR_INCOMPLETE_TYPE;

    for (integer_t i = 0; i < (*d_array)->size; i++)
        (*d_array)->v_free((*d_array)->buffer[i]);

    free((*d_array)->buffer);
    free(*d_array);

    *d_array = NULL;

    return DS_OK;
}

Status dar_free_shallow(DynamicArray *d_array)
{
    if (*d_array == NULL)
        return DS_ERR_NULL_POINTER;

    free((*d_array)->buffer);
    free(*d_array);

    *d_array = NULL;

    return DS_OK;
}

Status dar_erase(DynamicArray *d_array)
{
    if (d_array == NULL)
        return DS_ERR_NULL_POINTER;

    DynamicArray new_array;

    Status st;

    st = dar_create(&new_array, (*d_array)->capacity, (*d_array)->growth_rate,
            (*d_array)->v_compare, (*d_array)->v_copy, (*d_array)->v_display,
            (*d_array)->v_free);

    if (st != DS_OK)
        return st;

    new_array->locked = (*d_array)->locked;

    st = dar_free(d_array);

    if (st != DS_OK)
        return st;

    *d_array = new_array;

    return DS_OK;
}

Status dar_set_v_compare(DynamicArray d_array, dar_compare_f function)
{
    if (d_array == NULL)
        return DS_ERR_NULL_POINTER;

    d_array->v_compare = function;

    return DS_OK;
}

Status dar_set_v_copy(DynamicArray d_array, dar_copy_f function)
{
    if (d_array == NULL)
        return DS_ERR_NULL_POINTER;

    d_array->v_copy = function;

    return DS_OK;
}

Status dar_set_v_display(DynamicArray d_array, dar_display_f function)
{
    if (d_array == NULL)
        return DS_ERR_NULL_POINTER;

    d_array->v_display = function;

    return DS_OK;
}

Status dar_set_v_free(DynamicArray d_array, dar_free_f function)
{
    if (d_array == NULL)
        return DS_ERR_NULL_POINTER;

    d_array->v_free = function;

    return DS_OK;
}

Status dar_capacity_lock(DynamicArray d_array)
{
    if (d_array == NULL)
        return DS_ERR_NULL_POINTER;

    d_array->locked = true;

    return DS_OK;
}

Status dar_capacity_unlock(DynamicArray d_array)
{
    if (d_array == NULL)
        return DS_ERR_NULL_POINTER;

    d_array->locked = false;

    return DS_OK;
}

integer_t dar_capacity(DynamicArray d_array)
{
    if (d_array == NULL)
        return -1;

    return d_array->capacity;
}

integer_t dar_size(DynamicArray d_array)
{
    if (d_array == NULL)
        return -1;

    return d_array->size;
}

integer_t dar_growth_rate(DynamicArray d_array)
{
    if (d_array == NULL)
        return -1;

    return d_array->growth_rate;
}

bool dar_is_locked(DynamicArray d_array)
{
    return d_array->locked;
}

void *dar_get(DynamicArray d_array, integer_t index)
{
    if (d_array == NULL)
        return NULL;

    if (dar_empty(d_array))
        return NULL;

    if (index >= d_array->size)
        return NULL;

    if (index < 0)
        return NULL;

    if (d_array->v_copy == NULL)
        return NULL;

    return d_array->v_copy(d_array->buffer[index]);
}

Status dar_insert(DynamicArray d_array, void **array, integer_t array_size,
        integer_t index)
{
    if (d_array == NULL)
        return DS_ERR_NULL_POINTER;

    if (index > d_array->size)
        return DS_ERR_OUT_OF_RANGE;

    if (index < 0 || array_size < 0)
        return DS_ERR_NEGATIVE_VALUE;

    for (integer_t i = 0; i < array_size; i++)
    {
        if (array[i] == NULL)
            return DS_ERR_INVALID_ARGUMENT;
    }

    Status st;

    while (!dar_fits(d_array, array_size))
    {
        st = dar_grow(d_array);

        if (st != DS_OK)
            return st;
    }

    // Shift elements around
    for (integer_t i = d_array->size; i > index; i--)
    {
        d_array->buffer[i + array_size - 1] = d_array->buffer[i - 1];
    }

    // Add new elements to the buffer
    for (integer_t i = index, j = 0; j < array_size; i++, j++)
    {
        d_array->buffer[i] = array[j];
    }

    d_array->size += array_size;

    d_array->version_id++;

    return DS_OK;
}

Status dar_insert_front(DynamicArray d_array, void *element)
{
    if (d_array == NULL)
        return DS_ERR_NULL_POINTER;

    if (element == NULL)
        return DS_ERR_INVALID_ARGUMENT;

    if (dar_full(d_array))
    {
        Status st = dar_grow(d_array);

        if (st != DS_OK)
            return st;
    }

    // Shift elements
    for (integer_t i = d_array->size; i > 0; i--)
    {
        d_array->buffer[i] = d_array->buffer[i - 1];
    }

    d_array->buffer[0] = element;

    d_array->size++;

    d_array->version_id++;

    return DS_OK;
}

Status dar_insert_at(DynamicArray d_array, void *element, integer_t index)
{
    if (d_array == NULL)
        return DS_ERR_NULL_POINTER;

    if (element == NULL)
        return DS_ERR_INVALID_ARGUMENT;

    if (index > d_array->size)
        return DS_ERR_OUT_OF_RANGE;

    if (index < 0)
        return DS_ERR_NEGATIVE_VALUE;

    Status st;

    if (index == 0)
    {
        st = dar_insert_front(d_array, element);

        if (st != DS_OK)
            return st;
    }
    else if (index == d_array->size)
    {
        st = dar_insert_back(d_array, element);

        if (st != DS_OK)
            return st;
    }
    else
    {
        if (dar_full(d_array))
        {
            st = dar_grow(d_array);

            if (st != DS_OK)
                return st;
        }

        // Shift elements
        for (integer_t i = d_array->size; i > index; i--)
        {
            d_array->buffer[i] = d_array->buffer[i - 1];
        }

        d_array->buffer[index] = element;

        d_array->size++;
        d_array->version_id++;
    }

    return DS_OK;
}

Status dar_insert_back(DynamicArray d_array, void *element)
{
    if (d_array == NULL)
        return DS_ERR_NULL_POINTER;

    if (element == NULL)
        return DS_ERR_INVALID_ARGUMENT;

    if (dar_full(d_array))
    {
        Status st = dar_grow(d_array);

        if (st != DS_OK)
            return st;
    }

    // Simply append the new element
    d_array->buffer[d_array->size] = element;

    d_array->size++;

    d_array->version_id++;

    return DS_OK;
}

Status dar_remove(DynamicArray d_array, integer_t from, integer_t to,
        void ***result, integer_t *size)
{
    // In case anything goes wrong
    *result = NULL;
    *size = -1;

    if (d_array == NULL)
        return DS_ERR_NULL_POINTER;

    if (from > to)
        return DS_ERR_INVALID_ARGUMENT;

    if (to >= d_array->size)
        return DS_ERR_OUT_OF_RANGE;

    if (from < 0 || to < 0)
        return DS_ERR_NEGATIVE_VALUE;

    if (dar_empty(d_array))
        return DS_ERR_INVALID_OPERATION;

    // 'from' and 'to' are inclusive
    *size = to - from + 1;

    *result = malloc(sizeof(void*) * (*size));

    if (!(*result))
        return DS_ERR_ALLOC;

    // Passing elements to the output array
    for (integer_t i = from, j = 0; i <= to; i++, j++)
    {
        (*result)[j] = d_array->buffer[i];
    }

    // Shifting elements
    for (integer_t i = from, j = to + 1; j < d_array->size; i++, j++)
    {
        d_array->buffer[i] = d_array->buffer[j];
    }

    integer_t old_size = d_array->size;

    d_array->size -= *size;

    // Emptying spaces
    for (integer_t i = d_array->size; i < old_size; i++)
    {
        d_array->buffer[i] = NULL;
    }

    d_array->version_id++;

    return DS_OK;
}

Status dar_remove_front(DynamicArray d_array, void **result)
{
    if (d_array == NULL)
        return DS_ERR_NULL_POINTER;

    if (dar_empty(d_array))
        return DS_ERR_INVALID_OPERATION;

    *result = d_array->buffer[0];

    // Shift elements
    for (integer_t i = 0; i < d_array->size; i++)
    {
        d_array->buffer[i] = d_array->buffer[i + 1];
    }

    // Keep no references to removed elements in the buffer
    d_array->buffer[d_array->size - 1] = NULL;

    d_array->size--;

    d_array->version_id++;

    return DS_OK;
}

Status dar_remove_at(DynamicArray d_array, void **result, integer_t index)
{
    if (d_array == NULL)
        return DS_ERR_NULL_POINTER;

    if (dar_empty(d_array))
        return DS_ERR_INVALID_OPERATION;

    if (index >= d_array->size)
        return DS_ERR_OUT_OF_RANGE;

    if (index < 0)
        return DS_ERR_NEGATIVE_VALUE;

    Status st;

    if (index == 0)
    {
        st = dar_remove_front(d_array, result);

        if (st != DS_OK)
            return st;
    }
    else if (index == d_array->size - 1)
    {
        st = dar_remove_back(d_array, result);

        if (st != DS_OK)
            return st;
    }
    else
    {
        *result = d_array->buffer[index];

        // Shift elements relative to index
        for (integer_t i = index; i < d_array->size; i++)
        {
            d_array->buffer[i] = d_array->buffer[i + 1];
        }

        // Keep no references to removed elements in the buffer
        d_array->buffer[d_array->size - 1] = NULL;

        d_array->size--;

        d_array->version_id++;
    }

    return DS_OK;
}

Status dar_remove_back(DynamicArray d_array, void **result)
{
    if (d_array == NULL)
        return DS_ERR_NULL_POINTER;

    if (dar_empty(d_array))
        return DS_ERR_INVALID_OPERATION;

    *result = d_array->buffer[d_array->size - 1];

    // Keep no references to removed elements in the buffer
    d_array->buffer[d_array->size - 1] = NULL;

    d_array->size--;

    d_array->version_id++;

    return DS_OK;
}

Status dar_delete(DynamicArray d_array, integer_t from, integer_t to)
{
    if (d_array == NULL)
        return DS_ERR_NULL_POINTER;

    if (d_array->v_free == NULL)
        return DS_ERR_INCOMPLETE_TYPE;

    void **buffer;
    integer_t size;

    Status st = dar_remove(d_array, from, to, &buffer, &size);

    if (st != DS_OK)
        return st;

    for (integer_t i = 0; i < size; i++)
    {
        d_array->v_free(buffer[i]);
    }

    free(buffer);

    d_array->version_id++;

    return DS_OK;
}

Status dar_prepend(DynamicArray d_array1, DynamicArray d_array2)
{
    if (d_array1 == NULL || d_array2 == NULL)
        return DS_ERR_NULL_POINTER;

    if (dar_empty(d_array2))
        return DS_OK;

    Status st;

    while (!dar_fits(d_array1, d_array2->size))
    {
        st = dar_grow(d_array1);

        if (st != DS_OK)
            return st;
    }

    // Shift elements
    for (integer_t i = d_array1->size; i >= 0; i--)
    {
        d_array1->buffer[i + d_array2->size] = d_array1->buffer[i];
    }

    // Add new elements to the destination buffer
    for (integer_t i = 0; i < d_array2->size; i++)
    {
        d_array1->buffer[i] = d_array2->buffer[i];
    }

    d_array1->size += d_array2->size;

    // Empty the source buffer
    for (integer_t i = 0; i < d_array2->size; i++)
    {
        d_array2->buffer[i] = NULL;
    }

    d_array2->size = 0;

    d_array1->version_id++;
    d_array2->version_id++;

    return DS_OK;
}

Status dar_add(DynamicArray d_array1, DynamicArray d_array2, integer_t index)
{
    if (d_array1 == NULL || d_array2 == NULL)
        return DS_ERR_NULL_POINTER;

    if (index > d_array1->size)
        return DS_ERR_OUT_OF_RANGE;

    if (index < 0)
        return DS_ERR_NEGATIVE_VALUE;

    if (dar_empty(d_array2))
        return DS_OK;

    Status st;

    if (index == d_array1->size)
    {
        st = dar_append(d_array1, d_array2);

        if (st != DS_OK)
            return st;
    }
    else if (index == 0)
    {
        st = dar_prepend(d_array1, d_array2);

        if (st != DS_OK)
            return st;
    }
    else
    {
        while (!dar_fits(d_array1, d_array2->size))
        {
            st = dar_grow(d_array1);

            if (st != DS_OK)
                return st;
        }

        for (integer_t i = d_array1->size; i >= index; i--)
        {
            d_array1->buffer[i + d_array2->size] = d_array1->buffer[i];
        }

        for (integer_t i = index, j = 0; i < index + d_array2->size; i++, j++)
        {
            d_array1->buffer[i] = d_array2->buffer[j];
        }

        d_array1->size += d_array2->size;

        // Empty the source buffer
        for (integer_t i = 0; i < d_array2->size; i++)
        {
            d_array2->buffer[i] = NULL;
        }

        d_array2->size = 0;

        d_array1->version_id++;
        d_array2->version_id++;

    }

    return DS_OK;
}

Status dar_append(DynamicArray d_array1, DynamicArray d_array2)
{
    if (d_array1 == NULL || d_array2 == NULL)
        return DS_ERR_NULL_POINTER;

    if (dar_empty(d_array2))
        return DS_OK;

    Status st;

    while (!dar_fits(d_array1, d_array2->size))
    {
        st = dar_grow(d_array1);

        if (st != DS_OK)
            return st;
    }

    // Add new elements to the destination buffer
    for (integer_t i = d_array1->size, j = 0; j < d_array2->size; i++, j++)
    {
        d_array1->buffer[i] = d_array2->buffer[j];
    }

    d_array1->size += d_array2->size;

    // Empty the source buffer
    for (integer_t i = 0; i < d_array2->size; i++)
    {
        d_array2->buffer[i] = NULL;
    }

    d_array2->size = 0;

    d_array1->version_id++;
    d_array2->version_id++;

    return DS_OK;
}

Status dar_replace(DynamicArray d_array, void *element, integer_t index)
{
    if (d_array == NULL)
        return DS_ERR_NULL_POINTER;

    if (dar_empty(d_array))
        return DS_ERR_INVALID_OPERATION;

    if (index >= d_array->size)
        return DS_ERR_OUT_OF_RANGE;

    if (index < 0)
        return DS_ERR_NEGATIVE_VALUE;

    if (d_array->v_free == NULL)
        return DS_ERR_INCOMPLETE_TYPE;

    d_array->v_free(d_array->buffer[index]);

    d_array->buffer[index] = element;

    d_array->version_id++;

    return DS_OK;
}

bool dar_empty(DynamicArray d_array)
{
    return d_array->size == 0;
}

bool dar_full(DynamicArray d_array)
{
    return d_array->size == d_array->capacity;
}

bool dar_fits(DynamicArray d_array, integer_t size)
{
    return (d_array->size + size) <= d_array->capacity;
}

void *dar_max(DynamicArray d_array)
{
    if (d_array == NULL)
        return NULL;

    if (dar_empty(d_array))
        return NULL;

    void *result = d_array->buffer[0];

    for (integer_t i = 0; i < d_array->size; i++)
    {
        if (d_array->v_compare(d_array->buffer[i], result) > 0)
            result = d_array->buffer[i];
    }

    return result;
}

void *dar_min(DynamicArray d_array)
{
    if (d_array == NULL)
        return NULL;

    if (dar_empty(d_array))
        return NULL;

    void *result = d_array->buffer[0];

    for (integer_t i = 0; i < d_array->size; i++)
    {
        if (d_array->v_compare(d_array->buffer[i], result) < 0)
            result = d_array->buffer[i];
    }

    return result;
}

integer_t dar_index_first(DynamicArray d_array, void *key)
{
    if (d_array == NULL)
        return -3;

    if (d_array->v_compare == NULL)
        return -2;

    for (integer_t index = 0; index < d_array->size; index++)
    {
        if (d_array->v_compare(d_array->buffer[index], key) == 0)
            return index;
    }

    // Not found
    return -1;
}

integer_t dar_index_last(DynamicArray d_array, void *key)
{
    if (d_array == NULL)
        return -3;

    if (d_array->v_compare == NULL)
        return -2;

    for (integer_t index = d_array->size - 1; index >= 0; index--)
    {
        if (d_array->v_compare(d_array->buffer[index], key) == 0)
            return index;
    }

    // Not found
    return -1;
}

bool dar_contains(DynamicArray d_array, void *element)
{
    for (integer_t i = 0; i < d_array->size; i++)
    {
        if (d_array->v_compare(d_array->buffer[i], element) == 0)
            return true;
    }

    return false;
}

Status dar_switch(DynamicArray d_array, integer_t pos1, integer_t pos2)
{
    if (d_array == NULL)
        return DS_ERR_NULL_POINTER;

    if (pos1 >= d_array->size || pos2 >= d_array->size)
        return DS_ERR_OUT_OF_RANGE;

    if (pos1 < 0 || pos2 < 0)
        return DS_ERR_NEGATIVE_VALUE;

    void *temp = d_array->buffer[pos1];
    d_array->buffer[pos1] = d_array->buffer[pos2];
    d_array->buffer[pos2] = temp;

    d_array->version_id++;

    return DS_OK;
}

Status dar_reverse(DynamicArray d_array)
{
    if (d_array == NULL)
        return DS_ERR_NULL_POINTER;

    Status st;

    for (integer_t i = 0; i < d_array->size / 2; i++)
    {
        st = dar_switch(d_array, i, d_array->size - i - 1);

        if (st != DS_OK)
            return st;
    }

    return DS_OK;
}

Status dar_copy(DynamicArray d_array, DynamicArray *result)
{
    if (d_array == NULL)
        return DS_ERR_NULL_POINTER;

    if (d_array->v_copy == NULL)
        return DS_ERR_INCOMPLETE_TYPE;

    Status st = dar_create(result, d_array->capacity, d_array->growth_rate,
                           d_array->v_compare, d_array->v_copy,
                           d_array->v_display, d_array->v_free);

    if (st != DS_OK)
        return st;

    if (dar_empty(d_array))
        return DS_OK;

    for (integer_t i = 0; i < d_array->size; i++)
    {
        (*result)->buffer[i] = d_array->v_copy(d_array->buffer[i]);
    }

    (*result)->size = d_array->size;
    (*result)->locked = d_array->locked;

    return DS_OK;
}

Status dar_to_array(DynamicArray d_array, void ***result, integer_t *length)
{
    // If anything goes wrong...
    *result = NULL;
    *length = -1;

    if (d_array == NULL)
        return DS_ERR_NULL_POINTER;

    if (d_array->v_copy == NULL)
        return DS_ERR_INCOMPLETE_TYPE;

    *length = d_array->size;

    *result = malloc(sizeof(void*) * (*length));

    if (!(*result))
        return DS_ERR_NULL_POINTER;

    for (integer_t i = 0; i < *length; i++)
    {
        (*result)[i] = d_array->v_copy(d_array->buffer[i]);
    }

    return DS_OK;
}

Status dar_sort(DynamicArray d_array)
{
    if (d_array == NULL)
        return DS_ERR_NULL_POINTER;

    if (d_array->v_compare == NULL)
        return DS_ERR_INCOMPLETE_TYPE;

    dar_quicksort(d_array, d_array->buffer, d_array->size);

    d_array->version_id++;

    return DS_OK;
}

Status dar_display(DynamicArray d_array)
{
    if (d_array == NULL)
        return DS_ERR_NULL_POINTER;

    if (d_array->v_display == NULL)
        return DS_ERR_INCOMPLETE_TYPE;

    if (dar_empty(d_array))
    {
        printf("\n[ Empty ] \n");

        return DS_OK;
    }

    printf("\nDynamicArray\n[ ");

    for (integer_t i = 0; i < d_array->size - 1; i++)
    {
        d_array->v_display(d_array->buffer[i]);

        printf(", ");
    }

    d_array->v_display(d_array->buffer[d_array->size - 1]);

    printf(" ]\n");

    return DS_OK;
}

Status dar_display_array(DynamicArray d_array)
{
    if (d_array == NULL)
        return DS_ERR_NULL_POINTER;

    if (d_array->v_display == NULL)
        return DS_ERR_INCOMPLETE_TYPE;

    if (dar_empty(d_array))
    {
        printf("\n[ empty ] \n");

        return DS_OK;
    }

    printf("\n[ ");

    for (integer_t i = 0; i < d_array->size - 1; i++)
    {
        d_array->v_display(d_array->buffer[i]);

        printf(", ");
    }

    d_array->v_display(d_array->buffer[d_array->size - 1]);

    printf(" ]\n");

    return DS_OK;
}

Status dar_display_raw(DynamicArray d_array)
{
    if (d_array == NULL)
        return DS_ERR_NULL_POINTER;

    if (d_array->v_display == NULL)
        return DS_ERR_INCOMPLETE_TYPE;

    printf("\n");

    if (dar_empty(d_array))
        return DS_OK;

    for (integer_t i = 0; i < d_array->size; i++)
    {
        d_array->v_display(d_array->buffer[i]);

        printf(" ");
    }

    printf("\n");

    return DS_OK;
}

///////////////////////////////////////////////////// NOT EXPOSED FUNCTIONS ///

Status dar_grow(DynamicArray d_array)
{
    if (d_array == NULL)
        return DS_ERR_NULL_POINTER;

    if (d_array->locked)
        return DS_ERR_FULL;

    integer_t old_capacity = d_array->capacity;

    d_array->capacity = (integer_t) ((double) (d_array->capacity) *
            ((double) (d_array->growth_rate) / 100.0));

    // 4 is the minimum growth
    if (d_array->capacity - old_capacity < 4)
        d_array->capacity = old_capacity + 4;

    void **new_buffer = realloc(d_array->buffer, sizeof(void*) * d_array->capacity);

    if (!new_buffer)
    {
        d_array->capacity = old_capacity;

        return DS_ERR_ALLOC;
    }

    d_array->buffer = new_buffer;

    d_array->version_id++;

    return DS_OK;
}

static void dar_quicksort(DynamicArray d_array, void **buffer, integer_t size)
{
    if (size < 2)
        return;

    void *pivot = buffer[size / 2];

    integer_t i, j;
    for (i = 0, j = size - 1; ; i++, j--)
    {
        while (d_array->v_compare(buffer[i], pivot) < 0)
            i++;

        while (d_array->v_compare(buffer[j], pivot) > 0)
            j--;

        if (i >= j)
            break;

        void *temp = buffer[i];
        buffer[i] = buffer[j];
        buffer[j] = temp;
    }

    dar_quicksort(d_array, buffer, i);
    dar_quicksort(d_array, buffer + i, size - i);
}

////////////////////////////////////////////// END OF NOT EXPOSED FUNCTIONS ///

///////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////// Iterator ///
///////////////////////////////////////////////////////////////////////////////

/// \brief A DynamicArray_s iterator.
///
/// A simple iterator that can traverse backwards and forwards through the
/// elements of the dynamic array.
struct DynamicArrayIterator_s
{
    /// \brief Target DynamicArray_s.
    ///
    /// Target DynamicArray_s. The iterator might need to use some information
    /// provided by the array or change some of its data members.
    struct DynamicArray_s *target;

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

static bool dar_iter_target_modified(DynamicArrayIterator iter);

static bool dar_iter_invalid_state(DynamicArrayIterator iter);

////////////////////////////////////////////// END OF NOT EXPOSED FUNCTIONS ///

Status dar_iter_init(DynamicArrayIterator *iter, DynamicArray target)
{
    *iter = malloc(sizeof(DynamicArrayIterator_t));

    if (!(*iter))
        return DS_ERR_ALLOC;

    (*iter)->target = target;
    (*iter)->target_id = target->version_id;
    (*iter)->cursor = 0;

    return DS_OK;
}

Status dar_iter_retarget(DynamicArrayIterator *iter, DynamicArray target)
{
    Status st = dar_iter_free(iter);

    if (st != DS_OK)
        return st;

    st = dar_iter_init(iter, target);

    if (st != DS_OK)
        return st;

    return DS_OK;
}

Status dar_iter_free(DynamicArrayIterator *iter)
{
    if (*iter == NULL)
        return DS_ERR_NULL_POINTER;

    free(*iter);

    *iter = NULL;

    return DS_OK;
}

Status dar_iter_next(DynamicArrayIterator iter)
{
    if (iter == NULL)
        return DS_ERR_NULL_POINTER;

    if (dar_iter_invalid_state(iter))
        return DS_ERR_ITER_STATE;

    if (dar_iter_target_modified(iter))
        return DS_ERR_ITER_MODIFICATION;

    if (!dar_iter_has_next(iter))
        return DS_ERR_ITER;

    iter->cursor++;

    return DS_OK;
}

Status dar_iter_prev(DynamicArrayIterator iter)
{
    if (iter == NULL)
        return DS_ERR_NULL_POINTER;

    if (dar_iter_invalid_state(iter))
        return DS_ERR_ITER_STATE;

    if (dar_iter_target_modified(iter))
        return DS_ERR_ITER_MODIFICATION;

    if (!dar_iter_has_next(iter))
        return DS_ERR_ITER;

    iter->cursor--;

    return DS_OK;
}

Status dar_iter_to_start(DynamicArrayIterator iter)
{
    if (iter == NULL)
        return DS_ERR_NULL_POINTER;

    if (dar_iter_invalid_state(iter))
        return DS_ERR_ITER_STATE;

    if (dar_iter_target_modified(iter))
        return DS_ERR_ITER_MODIFICATION;

    iter->cursor = 0;

    return DS_OK;
}

Status dar_iter_to_end(DynamicArrayIterator iter)
{
    if (iter == NULL)
        return DS_ERR_NULL_POINTER;

    if (dar_iter_invalid_state(iter))
        return DS_ERR_ITER_STATE;

    if (dar_iter_target_modified(iter))
        return DS_ERR_ITER_MODIFICATION;

    iter->cursor = iter->target->size - 1;

    return DS_OK;
}

bool dar_iter_has_next(DynamicArrayIterator iter)
{
    return iter->cursor < iter->target->size - 1;
}

bool dar_iter_has_prev(DynamicArrayIterator iter)
{
    return iter->cursor > 0;
}

Status dar_iter_set(DynamicArrayIterator iter, void *element)
{
    if (iter == NULL)
        return DS_ERR_NULL_POINTER;

    if (dar_iter_invalid_state(iter))
        return DS_ERR_ITER_STATE;

    if (dar_iter_target_modified(iter))
        return DS_ERR_ITER_MODIFICATION;

    if (iter->target->v_free == NULL)
        return DS_ERR_INCOMPLETE_TYPE;

    if (iter->target->buffer[iter->cursor] != NULL)
        iter->target->v_free(iter->target->buffer[iter->cursor]);

    iter->target->buffer[iter->cursor] = element;

    iter->target_id++;
    iter->target->version_id++;

    return DS_OK;
}

Status dar_iter_get(DynamicArrayIterator iter, void **result)
{
    if (iter == NULL)
        return DS_ERR_NULL_POINTER;

    if (dar_iter_invalid_state(iter))
        return DS_ERR_ITER_STATE;

    if (dar_iter_target_modified(iter))
        return DS_ERR_ITER_MODIFICATION;

    if (iter->target->v_copy == NULL)
        return DS_ERR_INCOMPLETE_TYPE;

    if (iter->target->buffer[iter->cursor] != NULL)
        *result = iter->target->v_copy(iter->target->buffer[iter->cursor]);
    else
        *result = NULL;

    return DS_OK;
}

Status dar_iter_pop(DynamicArrayIterator iter, void **result)
{
    if (iter == NULL)
        return DS_ERR_NULL_POINTER;

    if (dar_iter_invalid_state(iter))
        return DS_ERR_ITER_STATE;

    if (dar_iter_target_modified(iter))
        return DS_ERR_ITER_MODIFICATION;

    Status st = dar_remove_at(iter->target, result, iter->cursor);

    if (st != DS_OK)
        return st;

    iter->target_id = iter->target->version_id;

    return DS_OK;
}

/////////////////////////////////////////////////////////////////// UTILITY ///

void *dar_iter_peek_next(DynamicArrayIterator iter)
{
    if (iter == NULL)
        return NULL;

    if (dar_iter_invalid_state(iter))
        return NULL;

    if (dar_iter_target_modified(iter))
        return NULL;

    if (!dar_iter_has_next(iter))
        return NULL;

    return iter->target->buffer[iter->cursor + 1];
}

void *dar_iter_peek(DynamicArrayIterator iter)
{
    if (iter == NULL)
        return NULL;

    if (dar_iter_invalid_state(iter))
        return NULL;

    if (dar_iter_target_modified(iter))
        return NULL;

    return iter->target->buffer[iter->cursor];
}

void *dar_iter_peek_prev(DynamicArrayIterator iter)
{
    if (iter == NULL)
        return NULL;

    if (dar_iter_invalid_state(iter))
        return NULL;

    if (dar_iter_target_modified(iter))
        return NULL;

    if (!dar_iter_has_prev(iter))
        return NULL;

    return iter->target->buffer[iter->cursor - 1];
}

///////////////////////////////////////////////////// NOT EXPOSED FUNCTIONS ///

static bool dar_iter_target_modified(DynamicArrayIterator iter)
{
    return iter->target_id != iter->target->version_id;
}

static bool dar_iter_invalid_state(DynamicArrayIterator iter)
{
    if (iter->target == NULL)
        return true;

    return iter->cursor < 0 || iter->cursor >= iter->target->size;
}

////////////////////////////////////////////// END OF NOT EXPOSED FUNCTIONS ///
