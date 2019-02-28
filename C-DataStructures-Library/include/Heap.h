/**
 * @file Heap.h
 *
 * @author Leonardo Vencovsky (https://github.com/LeoVen)
 *
 * @date 19/02/2018
 */

#ifndef C_DATASTRUCTURES_LIBRARY_HEAP_H
#define C_DATASTRUCTURES_LIBRARY_HEAP_H

#include "Core.h"
#include "Interface.h"

#ifdef __cplusplus
extern "C" {
#endif

/// \struct Heap_s
/// \brief A generic, array-based heap.
struct Heap_s;

/// \ref Heap_t
/// \brief A type for a heap.
///
/// A type for a <code> struct Heap_s </code> so you don't have to always write
/// the full name of it.
typedef struct Heap_s Heap_t;

/// \ref Heap
/// \brief A pointer type for a heap.
///
/// Defines a pointer type to <code> struct Heap_s </code>. This typedef is
/// used to avoid having to declare every heap as a pointer type since they all
/// must be dynamically allocated.
typedef struct Heap_s *Heap;

/// \enum HeapKind_e
/// \brief Defines an enum for the two kinds of a heap.
enum HeapKind_e
{
    /// A max-heap. The parent element is greater than its children and root is
    /// the highest element.
    MaxHeap = 1,

    /// A min-heap. The parent element is lesser than its children and root is
    /// the lowest element.
    MinHeap = -1
};

/// \ref HeapKind
/// \brief A type for <code> enum HeapKind_e </code>.
typedef enum HeapKind_e HeapKind;

///////////////////////////////////// STRUCTURE INITIALIZATION AND DELETION ///

/// \ref hep_new
/// \brief Initializes a new heap with default parameters.
Heap_t *
hep_new(Interface_t *interface, HeapKind kind);

/// \ref hep_create
/// \brief Initializes a new heap with custom parameters.
Heap_t *
hep_create(Interface_t *interface, integer_t size, integer_t growth_rate,
           HeapKind kind);

/// \ref hep_free
/// \brief Frees from memory a Heap_s and its elements.
void
hep_free(Heap_t *heap);

/// \ref hep_free_shallow
/// \brief Frees from memory a Heap_s leaving its elements intact.
void
hep_free_shallow(Heap_t *heap);

/// \ref hep_erase
/// \brief Frees from memory all elements of a Heap_s.
void
hep_erase(Heap_t *heap);

/// \ref hep_erase_shallow
/// \brief Removes all references to the elements inserted into the Heap_s.
void
hep_erase_shallow(Heap_t *heap);

//////////////////////////////////////////////////////////// CONFIGURATIONS ///

/// \ref hep_config
/// \brief Sets a new interface for the target heap.
void
hep_config(Heap_t *heap, Interface_t *new_interface);

/////////////////////////////////////////////////////////////////// GETTERS ///

/// \ref hep_count
/// \brief Returns the amount of elements in the specified heap.
integer_t
hep_count(Heap_t *heap);

/// \ref hep_capacity
/// \brief Returns the total buffer capacity of the specified heap.
integer_t
hep_capacity(Heap_t *heap);

/// \ref hep_growth
/// \brief Returns the growth rate of the specified heap.
integer_t
hep_growth(Heap_t *heap);

/// \ref hep_locked
/// \brief Returns true if the heap's buffer is locked, false otherwise.
bool
hep_locked(Heap_t *heap);

/// \ref hep_kind
/// \brief Returns if the heap is a MaxHeap or a MinHeap.
HeapKind
hep_kind(Heap_t *heap);

/////////////////////////////////////////////////////////////////// SETTERS ///

/// \ref hep_set_growth
/// \brief Sets a new growth rate for the heap's buffer.
bool
hep_set_growth(Heap_t *heap, integer_t growth_rate);

/// \ref hep_capacity_lock
/// \brief Locks the buffer's growth for the specified heap.
void
hep_capacity_lock(Heap_t *heap);

/// \ref hep_capacity_unlock
/// \brief Unlocks the buffer's growth for the specified heap.
void
hep_capacity_unlock(Heap_t *heap);

////////////////////////////////////////////////////////// INPUT AND OUTPUT ///

/// \ref hep_insert
/// \brief Inserts an element in the heap.
bool
hep_insert(Heap_t *heap, void *element);

/// \ref hep_remove
/// \brief Removes the top element from the heap.
bool
hep_remove(Heap_t *heap, void **result);

/// \ref hep_peek
/// \brief Return the root element of the heap.
void *
hep_peek(Heap_t *heap);

/////////////////////////////////////////////////////////// STRUCTURE STATE ///

/// \ref hep_empty
/// \brief Returns true if the heap is empty, otherwise false.
bool
hep_empty(Heap_t *heap);

/// \ref hep_full
/// \brief Returns true if the heap is full, otherwise false.
bool
hep_full(Heap_t *heap);

/////////////////////////////////////////////////////////////////// UTILITY ///

/// \ref hep_heapify
/// \brief Fixes the tree if the root element is changed.
bool
hep_heapify(Heap_t *heap);

/// \ref hep_copy
/// \brief Makes a copy of an existing heap.
Heap_t *
hep_copy(Heap_t *heap);

/// \ref hep_copy_shallow
/// \brief Makes a shallow copy of an existing heap.
Heap_t *
hep_copy_shallow(Heap_t *heap);

/////////////////////////////////////////////////////////////////// DISPLAY ///

/// \ref hep_display
/// \brief Displays a Heap_s in the console.
void
hep_display(Heap_t *heap, int display_mode);

///////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////// Iterator ///
///////////////////////////////////////////////////////////////////////////////

/// \todo HeapIterator

///////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////// Wrapper ///
///////////////////////////////////////////////////////////////////////////////

/// \todo HeapWrapper

#ifdef __cplusplus
}
#endif

#endif //C_DATASTRUCTURES_LIBRARY_HEAP_H
