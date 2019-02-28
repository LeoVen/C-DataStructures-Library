/**
 * @file DynamicArray.c
 *
 * @author Leonardo Vencovsky (https://github.com/LeoVen)
 *
 * @date 01/10/2018
 */

#include "DynamicArray.h"

/// A DynamicArray_s is a dynamic array that grows in size when needed. It has
/// a \c capacity that grows according to \c growth_rate. Both parameters can
/// be set by the user using the function dar_create() when creating a new
/// array. It can also be locked, disabling the buffer's growth, or unlocked,
/// enabling it back.
///
/// This data structure is also known as Vector, even though it has nothing to
/// do with a vector like in mathematics. The idea is to abstract a common
/// array so that operations like insert, remove and accesses are safe and
/// index bounds are checked.
///
/// The dynamic array can also be transformed from and to a C array, as long as
/// a copy function of your data type is provided.
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
    /// If \c locked is set to true the buffer will not grow.
    bool locked;

    /// \brief DynamicArray_s interface.
    ///
    /// An interface is like a table that has function pointers for functions
    /// that will manipulate a desired data type.
    Interface_t *interface;

    /// \brief A version id to keep track of modifications.
    ///
    /// This version id is used by the iterator to check if the structure was
    /// modified. The iterator can only function if its version_id is the same
    /// as the structure's version id, that is, there have been no structural
    /// modifications (except for those done by the iterator itself).
    integer_t version_id;
};

///////////////////////////////////////////////////// NOT EXPOSED FUNCTIONS ///

bool
dar_grow(DynamicArray_t *array, integer_t required_size);

static void
dar_quicksort(DynamicArray_t *array, void **buffer, integer_t size);

////////////////////////////////////////////// END OF NOT EXPOSED FUNCTIONS ///

/// Initializes a DynamicArray_s with an initial capacity of 32 and a growth
/// rate of 200, that is, twice the size after each growth. This function does
/// not sets any default functions. If you don't set them later you won't be
/// able to do certain operations that depend on a user-defined function.
///
/// \param[in] interface An interface defining all necessary functions for the
/// queue to operate.
///
/// \return A new DynamicArray_s or NULL if allocation failed.
DynamicArray_t *
dar_new(Interface_t *interface)
{
    DynamicArray_t *array = malloc(sizeof(DynamicArray_t));

    if (!array)
        return NULL;

    array->buffer = calloc(32, sizeof(void*));

    if (!(array->buffer))
    {
        free(array);

        return NULL;
    }

    array->capacity = 32;
    array->growth_rate = 200;
    array->size = 0;
    array->interface = interface;
    array->locked = false;
    array->version_id = 0;

    return array;
}

/// \brief Creates a DynamicArray_s.
///
/// Initializes a \c DynamicArray with a user defined \c initial_capacity and
/// \c growth_rate. This function only accepts an \c initial_capacity greater
/// than 0 and a \c growth_rate greater than 100; but keep in mind that in some
/// cases if the \c initial_capacity is too small and the \c growth_rate is too
/// close to 100 there won't be an increase in capacity and the minimum growth
/// will be triggered.
///
/// \param[in] interface An interface defining all necessary functions for the
/// dynamic array to operate.
/// \param[in] initial_capacity Buffer initial capacity.
/// \param[in] growth_rate Buffer growth rate.
///
/// \return A new DynamicArray_s or NULL if initial_capacity is less than 1, if
/// if the growth rate is less than 101 or if allocation failed.
DynamicArray_t *
dar_create(Interface_t *interface, integer_t initial_capacity,
           integer_t growth_rate)
{
    if (initial_capacity < 1 || growth_rate <= 100)
        return NULL;

    DynamicArray_t *array = malloc(sizeof(DynamicArray_t));

    if (!array)
        return NULL;

    array->buffer = malloc(sizeof(void*) * (size_t)initial_capacity);

    if (!(array->buffer))
    {
        free(array);

        return NULL;
    }

    for (integer_t i = 0; i < initial_capacity; i++)
        array->buffer[i] = NULL;

    array->capacity = initial_capacity;
    array->growth_rate = growth_rate;
    array->interface = interface;
    array->locked = false;
    array->size = 0;
    array->version_id = 0;

    return array;
}

///
/// \param[in] array
void
dar_free(DynamicArray_t *array)
{
    for (integer_t i = 0; i < array->size; i++)
        array->interface->free(array->buffer[i]);

    free(array->buffer);
    free(array);
}

///
/// \param[in] array
void
dar_free_shallow(DynamicArray_t *array)
{
    free(array->buffer);
    free(array);
}

///
/// \param[in] array
void
dar_erase(DynamicArray_t *array)
{
    for (integer_t i = 0; i < array->size; i++)
    {
        array->interface->free(array->buffer[i]);

        array->buffer[i] = NULL;
    }

    array->size = 0;
    array->version_id++;
}

///
/// \param[in] array
void
dar_erase_shallow(DynamicArray_t *array)
{
    for (integer_t i = 0; i < array->size; i++)
    {
        array->buffer[i] = NULL;
    }

    array->size = 0;
    array->version_id++;
}

///
/// \param[in] array
/// \param[in] new_interface
void
dar_config(DynamicArray_t *array, Interface_t *new_interface)
{
    array->interface = new_interface;
}

///
/// \param[in] array
void
dar_capacity_lock(DynamicArray_t *array)
{
    array->locked = true;
}

///
/// \param[in] array
void
dar_capacity_unlock(DynamicArray_t *array)
{
    array->locked = false;
}

///
/// \param[in] array
///
/// \return
integer_t
dar_capacity(DynamicArray_t *array)
{
    return array->capacity;
}

///
/// \param[in] array
///
/// \return
integer_t
dar_size(DynamicArray_t *array)
{
    return array->size;
}

///
/// \param[in] array
///
/// \return
integer_t
dar_growth_rate(DynamicArray_t *array)
{
    return array->growth_rate;
}

///
/// \param[in] array
///
/// \return
bool
dar_is_locked(DynamicArray_t *array)
{
    return array->locked;
}

///
/// \param[in] array
/// \param[in] index
///
/// \return
void *
dar_get(DynamicArray_t *array, integer_t index)
{
    if (dar_empty(array))
        return NULL;

    if (index >= array->size)
        return NULL;

    if (index < 0)
        return NULL;

    return array->buffer[index];
}

///
/// \param[in] array
/// \param[in] elements
/// \param[in] array_size
/// \param[in] index
///
/// \return
bool
dar_insert(DynamicArray_t *array, void **elements, integer_t array_size,
           integer_t index)
{
    if (index > array->size || index < 0 || array_size <= 0)
        return false;

    if (!dar_fits(array, array_size))
    {
        if (!dar_grow(array, array->size + array_size))
            return false;
    }

    // Shift existing elements around
    for (integer_t i = array->size; i > index; i--)
    {
        array->buffer[i + array_size - 1] = array->buffer[i - 1];
    }

    // Add new elements to the buffer
    for (integer_t i = index, j = 0; j < array_size; i++, j++)
    {
        array->buffer[i] = elements[j];
    }

    array->size += array_size;
    array->version_id++;

    return true;
}

///
/// \param[in] array
/// \param[in] element
///
/// \return
bool
dar_insert_front(DynamicArray_t *array, void *element)
{
    if (dar_full(array))
    {
        if (!dar_grow(array, array->size + 1))
            return false;
    }

    // Shift elements
    for (integer_t i = array->size; i > 0; i--)
    {
        array->buffer[i] = array->buffer[i - 1];
    }

    array->buffer[0] = element;

    array->size++;
    array->version_id++;

    return true;
}

///
/// \param[in] array
/// \param[in] element
/// \param[in] index
///
/// \return
bool
dar_insert_at(DynamicArray_t *array, void *element, integer_t index)
{
    if (index > array->size || index < 0)
        return false;

    if (index == 0)
    {
        return dar_insert_front(array, element);
    }
    else if (index == array->size)
    {
        return dar_insert_back(array, element);
    }
    else
    {
        if (dar_full(array))
        {
            if (!dar_grow(array, array->size + 1))
                return false;
        }

        // Shift elements
        for (integer_t i = array->size; i > index; i--)
        {
            array->buffer[i] = array->buffer[i - 1];
        }

        array->buffer[index] = element;

        array->size++;
        array->version_id++;
    }

    return true;
}

///
/// \param[in] array
/// \param[in] element
///
/// \return
bool
dar_insert_back(DynamicArray_t *array, void *element)
{
    if (dar_full(array))
    {
        if (!dar_grow(array, array->size + 1))
            return false;
    }

    // Simply append the new element
    array->buffer[array->size] = element;

    array->size++;
    array->version_id++;

    return true;
}

///
/// \param[in] array
/// \param[in] from
/// \param[in] to
/// \param[out] result
/// \param[out] size
///
/// \return
bool
dar_remove(DynamicArray_t *array, integer_t from, integer_t to, void ***result,
           integer_t *size)
{
    if (from > to || to >= array->size || from < 0 || to < 0)
        return false;

    if (dar_empty(array))
        return false;

    // 'from' and 'to' are inclusive
    *size = to - from + 1;

    *result = malloc(sizeof(void*) * (size_t)(*size));

    if (!(*result))
        return false;

    // Passing elements to the output array
    for (integer_t i = from, j = 0; i <= to; i++, j++)
    {
        (*result)[j] = array->buffer[i];
    }

    // Shifting elements
    for (integer_t i = from, j = to + 1; j < array->size; i++, j++)
    {
        array->buffer[i] = array->buffer[j];
    }

    integer_t old_size = array->size;

    array->size -= *size;

    // Emptying spaces
    for (integer_t i = array->size; i < old_size; i++)
    {
        array->buffer[i] = NULL;
    }

    array->version_id++;

    return true;
}

///
/// \param[in] array
/// \param[out] result
///
/// \return
bool
dar_remove_front(DynamicArray_t *array, void **result)
{
    if (dar_empty(array))
        return false;

    *result = array->buffer[0];

    // Shift elements
    for (integer_t i = 0; i < array->size; i++)
    {
        array->buffer[i] = array->buffer[i + 1];
    }

    // Keep no references to removed elements in the buffer
    array->buffer[array->size - 1] = NULL;

    array->size--;
    array->version_id++;

    return true;
}

///
/// \param[in] array
/// \param[out] result
/// \param[in] index
///
/// \return
bool
dar_remove_at(DynamicArray_t *array, void **result, integer_t index)
{
    if (index >= array->size || index < 0)
        return false;

    if (dar_empty(array))
        return false;

    if (index == 0)
    {
        return dar_remove_front(array, result);
    }
    else if (index == array->size - 1)
    {
        return dar_remove_back(array, result);
    }
    else
    {
        *result = array->buffer[index];

        // Shift elements relative to index
        for (integer_t i = index; i < array->size; i++)
        {
            array->buffer[i] = array->buffer[i + 1];
        }

        // Keep no references to removed elements in the buffer
        array->buffer[array->size - 1] = NULL;

        array->size--;
        array->version_id++;
    }

    return true;
}

///
/// \param[in] array
/// \param[out] result
///
/// \return
bool
dar_remove_back(DynamicArray_t *array, void **result)
{
    if (dar_empty(array))
        return false;

    *result = array->buffer[array->size - 1];

    // Keep no references to removed elements in the buffer
    array->buffer[array->size - 1] = NULL;

    array->size--;
    array->version_id++;

    return true;
}

///
/// \param[in] array
/// \param[in] from
/// \param[in] to
///
/// \return
bool
dar_delete(DynamicArray_t *array, integer_t from, integer_t to)
{
    void **buffer;
    integer_t size;

    if (!dar_remove(array, from, to, &buffer, &size))
        return false;

    for (integer_t i = 0; i < size; i++)
    {
        array->interface->free(buffer[i]);
    }

    free(buffer);

    array->version_id++;

    return true;
}

///
/// \param[in] array1
/// \param[in] array2
///
/// \return
bool
dar_prepend(DynamicArray_t *array1, DynamicArray_t *array2)
{
    if (dar_empty(array2))
        return true;

    if (!dar_fits(array1, array2->size))
    {
        if (!dar_grow(array1, array1->size + array2->size))
            return false;
    }

    // Shift elements
    for (integer_t i = array1->size; i >= 0; i--)
    {
        array1->buffer[i + array2->size] = array1->buffer[i];
    }

    // Add new elements to the destination buffer
    for (integer_t i = 0; i < array2->size; i++)
    {
        array1->buffer[i] = array2->buffer[i];
    }

    array1->size += array2->size;

    // Empty the source buffer
    for (integer_t i = 0; i < array2->size; i++)
    {
        array2->buffer[i] = NULL;
    }

    array2->size = 0;

    array1->version_id++;
    array2->version_id++;

    return true;
}

///
/// \param[in] array1
/// \param[in] array2
/// \param[in] index
///
/// \return
bool
dar_add(DynamicArray_t *array1, DynamicArray_t *array2, integer_t index)
{
    if (index > array1->size || index < 0)
        return false;

    if (dar_empty(array2))
        return true;

    if (index == 0)
    {
        return dar_prepend(array1, array2);
    }
    else if (index == array1->size)
    {
        return dar_append(array1, array2);
    }
    else
    {
        if (!dar_fits(array1, array2->size))
        {
            if (!dar_grow(array1, array1->size + array2->size))
                return false;
        }

        // Shift elements
        for (integer_t i = array1->size; i >= index; i--)
        {
            array1->buffer[i + array2->size] = array1->buffer[i];
        }

        // Add new elements to the destination buffer
        for (integer_t i = index, j = 0; i < index + array2->size; i++, j++)
        {
            array1->buffer[i] = array2->buffer[j];
        }

        array1->size += array2->size;

        // Empty the source buffer
        for (integer_t i = 0; i < array2->size; i++)
        {
            array2->buffer[i] = NULL;
        }

        array2->size = 0;

        array1->version_id++;
        array2->version_id++;
    }

    return true;
}

///
/// \param[in] array1
/// \param[in] array2
///
/// \return
bool
dar_append(DynamicArray_t *array1, DynamicArray_t *array2)
{
    if (dar_empty(array2))
        return true;

    if (!dar_fits(array1, array2->size))
    {
        if (!dar_grow(array1, array1->size + array2->size))
            return false;
    }

    // Add new elements to the destination buffer
    for (integer_t i = array1->size, j = 0; j < array2->size; i++, j++)
    {
        array1->buffer[i] = array2->buffer[j];
    }

    array1->size += array2->size;

    // Empty the source buffer
    for (integer_t i = 0; i < array2->size; i++)
    {
        array2->buffer[i] = NULL;
    }

    array2->size = 0;

    array1->version_id++;
    array2->version_id++;

    return true;
}

///
/// \param[in] array
/// \param[in] element
/// \param[in] index
///
/// \return
bool
dar_replace(DynamicArray_t *array, void *element, integer_t index)
{
    if (index >= array->size || index < 0)
        return false;

    if (dar_empty(array))
        return false;

    array->interface->free(array->buffer[index]);

    array->buffer[index] = element;

    array->version_id++;

    return true;
}

///
/// \param[in] array
///
/// \return
bool
dar_empty(DynamicArray_t *array)
{
    return array->size == 0;
}

///
/// \param[in] array
///
/// \return
bool
dar_full(DynamicArray_t *array)
{
    return array->size == array->capacity;
}

///
/// \param[in] array
/// \param[in] size
///
/// \return
bool
dar_fits(DynamicArray_t *array, integer_t size)
{
    return (array->size + size) <= array->capacity;
}

///
/// \param[in] array
///
/// \return
void *
dar_max(DynamicArray_t *array)
{
    if (dar_empty(array))
        return NULL;

    void *result = array->buffer[0];

    for (integer_t i = 1; i < array->size; i++)
    {
        if (array->interface->compare(array->buffer[i], result) > 0)
            result = array->buffer[i];
    }

    return result;
}

///
/// \param[in] array
///
/// \return
void *
dar_min(DynamicArray_t *array)
{
    if (dar_empty(array))
        return NULL;

    void *result = array->buffer[0];

    for (integer_t i = 1; i < array->size; i++)
    {
        if (array->interface->compare(array->buffer[i], result) < 0)
            result = array->buffer[i];
    }

    return result;
}

///
/// \param[in] array
/// \param[in] key
///
/// \return
integer_t
dar_index_first(DynamicArray_t *array, void *key)
{
    for (integer_t index = 0; index < array->size; index++)
    {
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
dar_index_last(DynamicArray_t *array, void *key)
{
    for (integer_t index = array->size - 1; index >= 0; index--)
    {
        if (array->interface->compare(array->buffer[index], key) == 0)
            return index;
    }

    // Not found
    return -1;
}

///
/// \param[in] array
/// \param[in] element
///
/// \return
bool
dar_contains(DynamicArray_t *array, void *element)
{
    for (integer_t i = 0; i < array->size; i++)
    {
        if (array->interface->compare(array->buffer[i], element) == 0)
            return true;
    }

    return false;
}

///
/// \param[in] array
/// \param[in] pos1
/// \param[in] pos2
///
/// \return
bool
dar_switch(DynamicArray_t *array, integer_t pos1, integer_t pos2)
{
    if (pos1 >= array->size || pos2 >= array->size || pos1 < 0 || pos2 < 0)
        return false;

    void *temp = array->buffer[pos1];
    array->buffer[pos1] = array->buffer[pos2];
    array->buffer[pos2] = temp;

    array->version_id++;

    return true;
}

///
/// \param[in] array
///
/// \return
bool
dar_reverse(DynamicArray_t *array)
{
    for (integer_t i = 0; i < array->size / 2; i++)
    {
        if (!dar_switch(array, i, array->size - i - 1))
            return false;
    }

    return true;
}

///
/// \param[in] array
///
/// \return
DynamicArray_t *
dar_copy(DynamicArray_t *array)
{
    DynamicArray_t *result = dar_create(array->interface, array->capacity,
            array->growth_rate);

    if (!result)
        return NULL;

    for (integer_t i = 0; i < array->size; i++)
    {
        result->buffer[i] = array->interface->copy(array->buffer[i]);
    }

    result->size = array->size;
    result->locked = array->locked;

    return result;
}

///
/// \param[in] array
///
/// \return
DynamicArray_t *
dar_copy_shallow(DynamicArray_t *array)
{
    DynamicArray_t *result = dar_create(array->interface, array->capacity,
                                        array->growth_rate);

    if (!result)
        return NULL;

    for (integer_t i = 0; i < array->size; i++)
    {
        result->buffer[i] = array->buffer[i];
    }

    result->size = array->size;
    result->locked = array->locked;

    return result;
}

///
/// \param[in] array
/// \param[in] length
///
/// \return
void **
dar_to_array(DynamicArray_t *array, integer_t *length)
{
    *length = -1;

    void **result = malloc(sizeof(void*) * (size_t)(array->size));

    if (!(*result))
        return NULL;

    for (integer_t i = 0; i < *length; i++)
    {
        result[i] = array->interface->copy(array->buffer[i]);
    }

    *length = array->size;

    return result;
}

///
/// \param[in] interface
/// \param[in] buffer
/// \param[in] length
/// \param[in] growth_rate
///
/// \return
DynamicArray_t *
dar_from_array(Interface_t *interface, void **buffer, integer_t length,
               integer_t growth_rate)
{
    DynamicArray_t *result = dar_create(interface, length, growth_rate);

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
void
dar_sort(DynamicArray_t *array)
{
    dar_quicksort(array, array->buffer, array->size);

    array->version_id++;
}

///
/// \param[in] array
/// \param[in] display_mode
void
dar_display(DynamicArray_t *array, int display_mode)
{
    if (dar_empty(array))
    {
        printf("\nDynamicArray\n[ Empty ] \n");
        return;
    }

    switch (display_mode)
    {
        case 0:
            printf("\n");
            for (integer_t i = 0; i < array->size; i++)
            {
                array->interface->display(array->buffer[i]);
                printf(" ");
            }
            printf("\n");
            break;
        case 1:
            printf("\nDynamicArray\n");
            for (integer_t i = 0; i < array->size; i++)
            {
                array->interface->display(array->buffer[i]);
                printf("\n");
            }
            break;
        default:
            printf("\nDynamicArray\n");
            printf("\n[ ");
            for (integer_t i = 0; i < array->size - 1; i++)
            {
                array->interface->display(array->buffer[i]);

                printf(", ");
            }
            array->interface->display(array->buffer[array->size - 1]);
            printf(" ]\n");
            break;
    }
}

///////////////////////////////////////////////////// NOT EXPOSED FUNCTIONS ///

bool
dar_grow(DynamicArray_t *array, integer_t required_capacity)
{
    if (array->locked)
        return false;

    integer_t old_capacity = array->capacity;

    // Either grows or get the required capacity
    integer_t new_capacity = (integer_t) ((double) (array->capacity) *
                               ((double) (array->growth_rate) / 100.0));

    // 4 is the minimum growth
    if (array->capacity - old_capacity < 4)
        array->capacity = old_capacity + 4;

    // Not enough...
    if (new_capacity < required_capacity)
        new_capacity = required_capacity;

    array->capacity = new_capacity;

    void **new_buffer = realloc(array->buffer,
            sizeof(void*) * (size_t)array->capacity);

    if (!new_buffer)
    {
        array->capacity = old_capacity;

        return false;
    }

    array->buffer = new_buffer;
    array->version_id++;

    return true;
}

static void
dar_quicksort(DynamicArray_t *array, void **buffer, integer_t size)
{
    if (size < 2)
        return;

    void *pivot = buffer[size / 2];

    integer_t i, j;
    for (i = 0, j = size - 1; ; i++, j--)
    {
        while (array->interface->compare(buffer[i], pivot) < 0)
            i++;

        while (array->interface->compare(buffer[j], pivot) > 0)
            j--;

        if (i >= j)
            break;

        void *temp = buffer[i];
        buffer[i] = buffer[j];
        buffer[j] = temp;
    }

    dar_quicksort(array, buffer, i);
    dar_quicksort(array, buffer + i, size - i);
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

static bool dar_iter_target_modified(DynamicArrayIterator_t *iter);

////////////////////////////////////////////// END OF NOT EXPOSED FUNCTIONS ///

///
/// \param[in] target
///
/// \return
DynamicArrayIterator_t *
dar_iter_new(DynamicArray_t *target)
{
    DynamicArrayIterator_t *iter = malloc(sizeof(DynamicArrayIterator_t));

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
void
dar_iter_retarget(DynamicArrayIterator_t *iter, DynamicArray_t *target)
{
    iter->target = target;
    iter->target_id = target->version_id;
}

///
/// \param[in] iter
void
dar_iter_free(DynamicArrayIterator_t *iter)
{
    free(iter);
}

///
/// \param[in] iter
///
/// \return
bool
dar_iter_next(DynamicArrayIterator_t *iter)
{
    if (dar_iter_target_modified(iter))
        return false;

    if (!dar_iter_has_next(iter))
        return false;

    iter->cursor++;

    return true;
}

///
/// \param[in] iter
///
/// \return
bool
dar_iter_prev(DynamicArrayIterator_t *iter)
{
    if (dar_iter_target_modified(iter))
        return false;

    if (!dar_iter_has_next(iter))
        return false;

    iter->cursor--;

    return true;
}

///
/// \param[in] iter
///
/// \return
bool
dar_iter_to_start(DynamicArrayIterator_t *iter)
{
    if (dar_iter_target_modified(iter))
        return false;

    iter->cursor = 0;

    return true;
}

///
/// \param[in] iter
///
/// \return
bool
dar_iter_to_end(DynamicArrayIterator_t *iter)
{
    if (dar_iter_target_modified(iter))
        return false;

    iter->cursor = iter->target->size - 1;

    return true;
}

///
/// \param[in] iter
///
/// \return
bool
dar_iter_has_next(DynamicArrayIterator_t *iter)
{
    return iter->cursor < iter->target->size - 1;
}

///
/// \param[in] iter
///
/// \return
bool
dar_iter_has_prev(DynamicArrayIterator_t *iter)
{
    return iter->cursor > 0;
}

///
/// \param[in] iter
/// \param[in] element
///
/// \return
bool
dar_iter_set(DynamicArrayIterator_t *iter, void *element)
{
    if (dar_iter_target_modified(iter))
        return false;

    // The user is responsible for freeing the element
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
dar_iter_get(DynamicArrayIterator_t *iter, void **result)
{
    if (dar_iter_target_modified(iter))
        return false;

    *result = iter->target->buffer[iter->cursor];

    return true;
}

/////////////////////////////////////////////////////////////////// UTILITY ///

///
/// \param[in] iter
///
/// \return
void *
dar_iter_peek_next(DynamicArrayIterator_t *iter)
{
    if (dar_iter_target_modified(iter))
        return NULL;

    if (!dar_iter_has_next(iter))
        return NULL;

    return iter->target->buffer[iter->cursor + 1];
}

///
/// \param[in] iter
///
/// \return
void *
dar_iter_peek(DynamicArrayIterator_t *iter)
{
    if (dar_iter_target_modified(iter))
        return NULL;

    return iter->target->buffer[iter->cursor];
}

///
/// \param[in] iter
///
/// \return
void *
dar_iter_peek_prev(DynamicArrayIterator_t *iter)
{
    if (dar_iter_target_modified(iter))
        return NULL;

    if (!dar_iter_has_prev(iter))
        return NULL;

    return iter->target->buffer[iter->cursor - 1];
}

///////////////////////////////////////////////////// NOT EXPOSED FUNCTIONS ///

static bool dar_iter_target_modified(DynamicArrayIterator_t *iter)
{
    return iter->target_id != iter->target->version_id;
}

////////////////////////////////////////////// END OF NOT EXPOSED FUNCTIONS ///
