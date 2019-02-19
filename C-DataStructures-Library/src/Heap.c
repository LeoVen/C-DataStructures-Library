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
/// - Parent      : Located at <code> I / 2 </code>
///     - Can be simplified as <code> I >> 1 </code>
/// - Left-Child  : Located at <code> 2 * I </code>
///     - Can be simplified as <code> I << 1 </code>
/// - Right-Child : Located at <code> 2 * I + 1 </code>
///     - Can be simplified as <code> (I << 1) + 1) </code>
///
/// The advantages of heaps implemented as arrays is that there is no overhead
/// of pointers to child nodes and a couple of operations is enough to find
/// each node as shown above.
struct Heap_s
{
    /// \brief What kind of heap this is.
    ///
    /// 1 - Max-Heap
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
hep_position_parent(integer_t position);

static integer_t
hep_position_left(integer_t position);

static integer_t
hep_position_right(integer_t position);

////////////////////////////////////////////// END OF NOT EXPOSED FUNCTIONS ///

///
/// \param[in] interface
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

bool
hep_insert(Heap_t *heap, void *element)
{
    /// \todo hep_insert
    return false;
}

void *
hep_remove(Heap_t *heap)
{
    /// \todo hep_remove
    return NULL;
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

///////////////////////////////////////////////////// NOT EXPOSED FUNCTIONS ///

static integer_t
hep_position_parent(integer_t position)
{
    return position / 2;
}

static integer_t
hep_position_left(integer_t position)
{
    return 2 * position;
}

static integer_t
hep_position_right(integer_t position)
{
    return 2 * position + 1;
}

// Increases the heap's buffer
static bool
hep_grow(Heap_t *heap)
{
    /// \todo hep_grow
    return false;
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