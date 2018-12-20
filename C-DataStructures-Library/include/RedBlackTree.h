/**
 * @file RedBlackTree.h
 * 
 * @author Leonardo Vencovsky (https://github.com/LeoVen)
 *
 * @date 13/12/2018
 */

#ifndef C_DATASTRUCTURES_LIBRARY_REDBLACKTREE_H
#define C_DATASTRUCTURES_LIBRARY_REDBLACKTREE_H

#include "Core.h"
#include "Interface.h"

#ifdef __cplusplus
extern "C" {
#endif

/// \struct RedBlackTree_s
/// \brief A generic, multi-purpose red-black tree.
struct RedBlackTree_s;

/// \ref RedBlackTree_t
/// \brief A type for a red-black tree.
///
/// A type for a <code> struct RedBlackTree_s </code> so you don't have to
/// always write the full name of it.
typedef struct RedBlackTree_s RedBlackTree_t;

/// \ref RedBlackTree
/// \brief A pointer type for a red-black tree.
///
/// Defines a pointer type to <code> struct RedBlackTree_s </code>. This
/// typedef is used to avoid having to declare every red-black tree as a
/// pointer type since they all must be dynamically allocated.
typedef struct RedBlackTree_s *RedBlackTree;

///////////////////////////////////// STRUCTURE INITIALIZATION AND DELETION ///

/// \ref rbt_new
/// \brief Initializes a new red-black tree.
RedBlackTree_t *
rbt_new(Interface_t *interface);

/// \ref rbt_free
/// \brief Frees from memory a RedBlackTree_s and its elements.
void
rbt_free(RedBlackTree_t *tree);

/// \ref rbt_free_shallow
/// \brief Frees from memory a RedBlackTree_s leaving its elements intact.
void
rbt_free_shallow(RedBlackTree_t *tree);

/// \ref rbt_erase
/// \brief Frees from memory all elements of a RedBlackTree_s.
void
rbt_erase(RedBlackTree_t *tree);

/// \ref rbt_erase_shallow
/// \brief Frees from memory all the nodes of a RedBlackTree_s.
void
rbt_erase_shallow(RedBlackTree_t *tree);

//////////////////////////////////////////////////////////// CONFIGURATIONS ///

/// \ref rbt_config
/// \brief Sets a new interface for the target red-black tree.
void
rbt_config(RedBlackTree_t *tree, Interface_t *new_interface);

/////////////////////////////////////////////////////////////////// GETTERS ///

/// \ref rbt_size
/// \brief Returns the amount of elements in the red-black tree.
integer_t
rbt_size(RedBlackTree_t *tree);

/// \ref rbt_limit
/// \brief Returns the red-black tree's maximum number of elements.
integer_t
rbt_limit(RedBlackTree_t *tree);

/////////////////////////////////////////////////////////////////// SETTERS ///

/// \ref rbt_set_limit
/// \brief Sets a new limit to the red-black tree's size.
bool
rbt_set_limit(RedBlackTree_t *tree, integer_t limit);

////////////////////////////////////////////////////////// INPUT AND OUTPUT ///

/// \ref rbt_insert
/// \brief Adds a new element in the specified red-black tree.
bool
rbt_insert(RedBlackTree_t *tree, void *element);

/// \ref rbt_remove
/// \brief Removes an element from the tree that matches the given element.
bool
rbt_remove(RedBlackTree_t *tree, void *element);

/// \ref rbt_pop
/// \brief Removes the root element and frees it from memory.
bool
rbt_pop(RedBlackTree_t *tree);

/////////////////////////////////////////////////////////// STRUCTURE STATE ///

/// \ref rbt_empty
/// \brief Checks if the specified red-black tree is empty.
bool
rbt_empty(RedBlackTree_t *tree);

/// \ref rbt_full
/// \brief Checks if the specified red-black tree is full.
bool
rbt_full(RedBlackTree_t *tree);

/////////////////////////////////////////////////////////////////// UTILITY ///

/// \ref rbt_contains
/// \brief Checks if a given element is in the red-black tree.
bool
rbt_contains(RedBlackTree_t *tree, void *element);

/// \ref rbt_peek
/// \brief Returns the root element if present.
void *
rbt_peek(RedBlackTree_t *tree);

/// \ref rbt_max
/// \brief Returns the maximum element if present.
void *
rbt_max(RedBlackTree_t *tree);

/// \ref rbt_min
/// \brief Returns the minimum element if present.
void *
rbt_min(RedBlackTree_t *tree);

/////////////////////////////////////////////////////////////////// DISPLAY ///

/// \ref rbt_display
/// \brief Displays a red-black tree in the console given a display mode.
void
rbt_display(RedBlackTree_t *tree, int display_mode);

/// \ref rbt_traversal
/// \brief Displays each element in the console given a traversal order.
void
rbt_traversal(RedBlackTree_t *tree, int traversal_mode);

///////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////// Iterator ///
///////////////////////////////////////////////////////////////////////////////

/// \todo RedBlackTreeIterator

///////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////// Wrapper ///
///////////////////////////////////////////////////////////////////////////////

/// \todo RedBlackTreeWrapper

#ifdef __cplusplus
}
#endif

#endif //C_DATASTRUCTURES_LIBRARY_REDBLACKTREE_H
