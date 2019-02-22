/**
 * @file Heap.c
 *
 * @author Leonardo Vencovsky (https://github.com/LeoVen)
 *
 * @date 19/02/2018
 */

#include "Heap.h"

/// A Heap is a data structure that can be seen as a nearly complete binary
/// heap. Each node is represented by an element of an internal array storage.
///
/// Given an element at a position \c I :
/// - Parent      : Located at <code> (I - 1) / 2 </code>
///     - Can be simplified as <code> (I - 1) >> 1 </code>
/// - Left-Child  : Located at <code> (2 * I) + 1 </code>
///     - Can be simplified as <code> (I << 1) + 1 </code>
/// - Right-Child : Located at <code> (2 * I) + 2 </code>
///     - Can be simplified as <code> (I << 1) + 2) </code>
///
/// The advantages of heaps implemented as arrays is that there is no overhead
/// of pointers to child nodes and a couple of operations is enough to find
/// each node as shown above.
struct Heap_s
{
    /// \brief What kind of heap this is.
    ///
    ///  1 - Max-Heap
    /// -1 - Min-Heap
    enum HeapKind_e kind;

    /// \brief Data buffer.
    ///
    /// Buffer where elements are stored in.
    void **buffer;

    /// \brief Current amount of elements in the heap.
    ///
    /// Current amount of elements in the heap.
    integer_t count;

    /// \brief Buffer current size.
    ///
    /// Buffer current size. When \c count reaches \c capacity the buffer is
    /// full and must be reallocated, increasing in size according to
    /// \c growth_rate.
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

    /// \brief Heap_s interface.
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

static integer_t
hep_p(integer_t position);

static integer_t
hep_l(integer_t position);

static integer_t
hep_r(integer_t position);

static bool
hep_grow(Heap_t *heap);

bool
hep_float_up(Heap_t *heap, integer_t index);

bool
hep_float_down(Heap_t *heap, integer_t index);

void
hep_display_tree(Heap_t *heap, integer_t index, integer_t height);

////////////////////////////////////////////// END OF NOT EXPOSED FUNCTIONS ///

///
/// \param[in] interface
/// \param[in] kind
///
/// \return
Heap_t *
hep_new(Interface_t *interface, HeapKind kind)
{
    if (!(kind == MaxHeap || kind == MinHeap))
        return NULL;

    Heap_t *heap = malloc(sizeof(Heap_t));

    if (!heap)
        return NULL;

    heap->buffer = malloc(sizeof(void *) * 32);

    if (!heap->buffer)
    {
        free(heap);
        return NULL;
    }

    for (integer_t i = 0; i < 32; i++)
        heap->buffer[i] = NULL;

    heap->capacity = 32;
    heap->growth_rate = 200;
    heap->count = 0;
    heap->version_id = 0;

    heap->locked = false;

    heap->interface = interface;
    heap->kind = kind;

    return heap;
}

///
/// \param[in] interface
/// \param[in] size
/// \param[in] growth_rate
/// \param[in] kind
///
/// \return
Heap_t *
hep_create(Interface_t *interface, integer_t size, integer_t growth_rate,
           HeapKind kind)
{
    if (size < 1 || growth_rate < 101)
        return NULL;

    if (!(kind == MaxHeap || kind == MinHeap))
        return NULL;

    Heap_t *heap = malloc(sizeof(Heap_t));

    if (!heap)
        return NULL;

    heap->buffer = malloc(sizeof(void *) * (size_t) size);

    if (!heap->buffer)
    {
        free(heap);
        return NULL;
    }

    for (integer_t i = 0; i < size; i++)
        heap->buffer[i] = NULL;

    heap->capacity = size;
    heap->growth_rate = growth_rate;
    heap->count = 0;
    heap->version_id = 0;

    heap->interface = interface;
    heap->kind = kind;

    return heap;
}

///
/// \param[in] heap
void
hep_free(Heap_t *heap)
{
    for (integer_t i = 0; i < heap->count; i++)
    {
        heap->interface->free(heap->buffer[i]);
    }

    free(heap->buffer);
    free(heap);
}

///
/// \param[in] heap
void
hep_free_shallow(Heap_t *heap)
{
    free(heap->buffer);
    free(heap);
}

///
/// \param[in] heap
void
hep_erase(Heap_t *heap)
{
    for (integer_t i = 0; i < heap->count; i++)
    {
        heap->interface->free(heap->buffer[i]);
        heap->buffer[i] = NULL;
    }

    heap->count = 0;
    heap->version_id++;
}

///
/// \param[in] heap
void
hep_erase_shallow(Heap_t *heap)
{
    for (integer_t i = 0; i < heap->count; i++)
    {
        heap->buffer[i] = NULL;
    }

    heap->count = 0;
    heap->version_id++;
}

///
/// \param[in] heap
/// \param[in] new_interface
void
hep_config(Heap_t *heap, Interface_t *new_interface)
{
    heap->interface = new_interface;
}

///
/// \param[in] heap
///
/// \return
integer_t
hep_count(Heap_t *heap)
{
    return heap->count;
}

///
/// \param[in] heap
///
/// \return
integer_t
hep_capacity(Heap_t *heap)
{
    return heap->capacity;
}

///
/// \param[in] heap
///
/// \return
integer_t
hep_growth(Heap_t *heap)
{
    return heap->growth_rate;
}

///
/// \param[in] heap
///
/// \return
bool
hep_locked(Heap_t *heap)
{
    return heap->locked;
}

///
/// \param[in] heap
///
/// \return
HeapKind
hep_kind(Heap_t *heap)
{
    return heap->kind;
}

///
/// \param[in] heap
/// \param[in] growth_rate
///
/// \return
bool
hep_set_growth(Heap_t *heap, integer_t growth_rate)
{
    if (growth_rate < 101)
        return false;

    heap->growth_rate = growth_rate;

    return true;
}

///
/// \param[in] heap
void
hep_capacity_lock(Heap_t *heap)
{
    heap->locked = true;
}

///
/// \param[in] heap
void
hep_capacity_unlock(Heap_t *heap)
{
    heap->locked = false;
}

///
/// \param[in] heap
/// \param[in] element
///
/// \return
bool
hep_insert(Heap_t *heap, void *element)
{
    integer_t C = heap->count;

    if (hep_full(heap))
    {
        if (!hep_grow(heap))
            return false;
    }

    if (C == 0)
    {
        heap->buffer[heap->count] = element;
        heap->count++;

        return true;
    }

    heap->buffer[C] = element;
    heap->count++;

    if (!hep_float_up(heap, C))
        return false;

    return true;
}

///
/// \param[in] heap
/// \param[out] result
///
/// \return
bool
hep_remove(Heap_t *heap, void **result)
{
    if (hep_empty(heap))
        return false;

    *result = heap->buffer[0];

    // Swap bottom element with root
    heap->buffer[0] = heap->buffer[heap->count - 1];
    heap->buffer[heap->count - 1] = NULL;

    heap->count--;

    if (!hep_float_down(heap, 0))
        return false;

    return true;
}

///
/// \param[in] heap
///
/// \return
void *
hep_peek(Heap_t *heap)
{
    if (hep_empty(heap))
        return NULL;

    return heap->buffer[0];
}

///
/// \param[in] heap
///
/// \return
bool
hep_empty(Heap_t *heap)
{
    return heap->count == 0;
}

///
/// \param[in] heap
///
/// \return
bool
hep_full(Heap_t *heap)
{
    return heap->count == heap->capacity;
}

///
/// \param[in] heap
///
/// \return
bool
hep_heapify(Heap_t *heap)
{
    return hep_float_down(heap, 0);
}

///
/// \param[in] heap
///
/// \return
Heap_t *
hep_copy(Heap_t *heap)
{
    Heap_t *copy = hep_create(heap->interface, heap->count, heap->growth_rate,
                              heap->kind);

    if (!copy)
        return NULL;

    copy->locked = copy->locked;

    for (integer_t i = 0; i < heap->count; i++)
    {
        copy->buffer[i] = heap->interface->copy(heap->buffer[i]);
    }

    copy->count = heap->count;
    copy->version_id++;

    return copy;
}

///
/// \param[in] heap
///
/// \return
Heap_t *
hep_copy_shallow(Heap_t *heap)
{
    Heap_t *copy = hep_create(heap->interface, heap->count, heap->growth_rate,
                              heap->kind);

    if (!copy)
        return NULL;

    copy->locked = copy->locked;

    for (integer_t i = 0; i < heap->count; i++)
    {
        copy->buffer[i] = heap->buffer[i];
    }

    copy->count = heap->count;
    copy->version_id++;

    return copy;
}

///
/// \param[in] heap
/// \param[in] display_mode
void
hep_display(Heap_t *heap, int display_mode)
{
    if (hep_empty(heap))
    {
        printf("\nHeap\n[ empty ]\n");
        return;
    }

    switch (display_mode)
    {
        case -1:
            printf("\nHeap\n");
            for (integer_t i = 0; i < heap->count; i++)
            {
                heap->interface->display(heap->buffer[i]);
                printf("\n");
            }
            break;
        case 0:
            printf("\nHeap\n");
            for (integer_t i = 0; i < heap->count; i++)
            {
                heap->interface->display(heap->buffer[i]);
                printf(" ");
            }
            printf("\n");
            break;
        case 1:
            printf("\nHeap\n");
            hep_display_tree(heap, 0, 0);
            printf("\n");
            break;
        default:
            printf("\nHeap\n[ ");
            for (integer_t i = 0; i < heap->count - 1; i++)
            {
                heap->interface->display(heap->buffer[i]);
                printf(", ");
            }
            heap->interface->display(heap->buffer[heap->count - 1]);
            printf(" ]\n");
            break;
    }
}

///////////////////////////////////////////////////// NOT EXPOSED FUNCTIONS ///

// Parent
static integer_t
hep_p(integer_t position)
{
    return (position - 1) / 2;
}

// Left child
static integer_t
hep_l(integer_t position)
{
    return (2 * position) + 1;
}

// Right child
static integer_t
hep_r(integer_t position)
{
    return (2 * position) + 2;
}

// Increases the heap's buffer
static bool
hep_grow(Heap_t *heap)
{
    if (heap->locked)
        return false;

    integer_t old_capacity = heap->capacity;

    // capacity = capacity * (growth_rate / 100)
    heap->capacity = (integer_t) ((double) (heap->capacity) *
                                   ((double) (heap->growth_rate) / 100.0));

    // 4 is the minimum growth
    if (heap->capacity - old_capacity < 4)
        heap->capacity = old_capacity + 4;

    void **new_buffer = realloc(heap->buffer,
                                sizeof(void*) * (size_t)heap->capacity);

    // Reallocation failed
    if (!new_buffer)
    {
        heap->capacity = old_capacity;
        return false;
    }

    heap->buffer = new_buffer;

    return true;
}

/// Floats up the newly added element, maintaining the heap properties
bool
hep_float_up(Heap_t *heap, integer_t index)
{
    if (index < 0)
        return false;

    integer_t C = index;

    // Maintaining the heap property
    void *child = heap->buffer[C];
    void *parent = heap->buffer[hep_p(C)];

    // This modifier changes the compare function's result.
    // If the heap is a MinHeap and the comparison returns -1, it means that
    // the first element is less than the second, but lower elements have
    // higher priority in this case (-1 * -1 == 1), so it must be brought up.
    // It is very important that the HeapKind enum follows the pattern for
    // MaxHeap = 1 and MinHeap = -1
    integer_t mod = heap->kind;

    // Float up
    while (C > 0 && heap->interface->compare(child, parent) * mod > 0)
    {
        // Swap child with parent
        void *tmp = heap->buffer[C];
        heap->buffer[C] = heap->buffer[hep_p(C)];
        heap->buffer[hep_p(C)] = tmp;

        C = hep_p(C);

        child = heap->buffer[C];
        parent = heap->buffer[hep_p(C)];
    }

    return true;
}

// Floats down the placeholder when the root was removed, maintaining the heap
// property
bool
hep_float_down(Heap_t *heap, integer_t index)
{
    if (index < 0)
        return false;

    // Modifier here is very important because it defines if this heap is a
    // min-heap or a max-heap and it inverts the compare function output when
    // working with a min-heap.
    integer_t mod = heap->kind;

    // Float down
    while (index < heap->count)
    {
        integer_t L = hep_l(index); // Left
        integer_t R = hep_r(index); // Right
        integer_t C = index;        // Current (largest)

        // Check if both child nodes
        if (L < heap->count &&
            heap->interface->compare(heap->buffer[L], heap->buffer[C]) * mod > 0)
        {
            C = L;
        }

        if (R < heap->count &&
            heap->interface->compare(heap->buffer[R], heap->buffer[C]) * mod > 0)
        {
            C = R;
        }

        if (C != index)
        {
            // Swap index with C
            void *tmp = heap->buffer[index];
            heap->buffer[index] = heap->buffer[C];
            heap->buffer[C] = tmp;

            index = C;
        }
        else
            break;
    }

    return true;
}

void
hep_display_tree(Heap_t *heap, integer_t index, integer_t height)
{
    if (index >= heap->count || index < 0)
        return;

    hep_display_tree(heap, hep_r(index), height + 1);

    for (integer_t i = 0; i < height; i++)
        printf("|------- ");

    heap->interface->display(heap->buffer[index]);
    printf("\n");

    hep_display_tree(heap, hep_l(index), height + 1);
}

////////////////////////////////////////////// END OF NOT EXPOSED FUNCTIONS ///

///////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////// Iterator ///
///////////////////////////////////////////////////////////////////////////////

/// \todo HeapIterator

///////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////// Wrapper ///
///////////////////////////////////////////////////////////////////////////////

/// \todo HeapWrapper