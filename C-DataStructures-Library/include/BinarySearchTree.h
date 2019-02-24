/**
 * @file BinarySearchTree.h
 * 
 * @author Leonardo Vencovsky (https://github.com/LeoVen)
 *
 * @date 31/10/2018
 */

#ifndef C_DATASTRUCTURES_LIBRARY_BINARYSEARCHTREE_H
#define C_DATASTRUCTURES_LIBRARY_BINARYSEARCHTREE_H

#include "Core.h"
#include "Interface.h"

#ifdef __cplusplus
extern "C" {
#endif

/// \struct BinarySearchTree_s
/// \brief A generic, multi-purpose binary search tree.
struct BinarySearchTree_s;

/// \ref BinarySearchTree_t
/// \brief A type for a binary search tree.
///
/// A type for a <code> struct BinarySearchTree_s </code> so you don't have to
/// always write the full name of it.
typedef struct BinarySearchTree_s BinarySearchTree_t;

/// \ref BinarySearchTree
/// \brief A pointer type for a binary search tree.
///
/// Defines a pointer type to <code> struct BinarySearchTree_s </code>. This
/// typedef is used to avoid having to declare every binary search tree as a
/// pointer type since they all must be dynamically allocated.
typedef struct BinarySearchTree_s *BinarySearchTree;

///////////////////////////////////// STRUCTURE INITIALIZATION AND DELETION ///

/// \ref bst_new
/// \brief Initializes a new binary search tree.
BinarySearchTree_t *
bst_new(Interface_t *interface);

/// \ref bst_free
/// \brief Frees from memory a BinarySearchTree_s and its elements.
void
bst_free(BinarySearchTree_t *tree);

/// \ref bst_free_shallow
/// \brief Frees from memory a BinarySearchTree_s leaving its elements intact.
void
bst_free_shallow(BinarySearchTree_t *tree);

/// \ref bst_erase
/// \brief Frees from memory all elements of a BinarySearchTree_s.
void
bst_erase(BinarySearchTree_t *tree);

/// \ref bst_erase_shallow
/// \brief Frees from memory all the nodes of a BinarySearchTree_s.
void
bst_erase_shallow(BinarySearchTree_t *tree);

//////////////////////////////////////////////////////////// CONFIGURATIONS ///

/// \ref bst_config
/// \brief Sets a new interface for the target binary search tree.
void
bst_config(BinarySearchTree_t *tree, Interface_t *new_interface);

/////////////////////////////////////////////////////////////////// GETTERS ///

/// \ref bst_count
/// \brief Returns the amount of elements in the binary search tree.
integer_t
bst_count(BinarySearchTree_t *tree);

/// \ref bst_limit
/// \brief Returns the binary search tree's maximum number of elements.
integer_t
bst_limit(BinarySearchTree_t *tree);

/////////////////////////////////////////////////////////////////// SETTERS ///

/// \ref bst_set_limit
/// \brief Sets a new limit to the binary search tree's size.
bool
bst_set_limit(BinarySearchTree_t *tree, integer_t limit);

////////////////////////////////////////////////////////// INPUT AND OUTPUT ///

/// \ref bst_insert
/// \brief Adds a new element in the specified binary search tree.
bool
bst_insert(BinarySearchTree_t *tree, void *element);

/// \ref bst_remove
/// \brief Removes an element from the tree that matches the given element.
bool
bst_remove(BinarySearchTree_t *tree, void *element);

/// \ref bst_pop
/// \brief Removes the root element and frees it from memory.
bool
bst_pop(BinarySearchTree_t *tree);

/////////////////////////////////////////////////////////// STRUCTURE STATE ///

/// \ref bst_empty
/// \brief Checks if the specified binary search tree is empty.
bool
bst_empty(BinarySearchTree_t *tree);

/// \ref bst_full
/// \brief Checks if the specified binary search tree is full.
bool
bst_full(BinarySearchTree_t *tree);

/////////////////////////////////////////////////////////////////// UTILITY ///

/// \ref bst_contains
/// \brief Checks if a given element is in the binary search tree.
bool
bst_contains(BinarySearchTree_t *tree, void *element);

/// \ref bst_peek
/// \brief Returns the root element if present.
void *
bst_peek(BinarySearchTree_t *tree);

/// \ref bst_max
/// \brief Returns the maximum element if present.
void *
bst_max(BinarySearchTree_t *tree);

/// \ref bst_min
/// \brief Returns the minimum element if present.
void *
bst_min(BinarySearchTree_t *tree);

/////////////////////////////////////////////////////////////////// DISPLAY ///

/// \ref bst_display
/// \brief Displays a binary search tree in the console given a display mode.
void
bst_display(BinarySearchTree_t *tree, int display_mode);

/// \ref bst_traversal
/// \brief Displays each element in the console given a traversal order.
void
bst_traversal(BinarySearchTree_t *tree, int traversal_mode);

///////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////// Iterator ///
///////////////////////////////////////////////////////////////////////////////

/// \todo BinarySearchTreeIterator

///////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////// Wrapper ///
///////////////////////////////////////////////////////////////////////////////

/// \todo BinarySearchTreeWrapper

#ifdef __cplusplus
}
#endif

#endif //C_DATASTRUCTURES_LIBRARY_BINARYSEARCHTREE_H
