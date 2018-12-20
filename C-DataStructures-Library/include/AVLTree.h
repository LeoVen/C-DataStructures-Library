/**
 * \file AVLTree.h
 * 
 * \author Leonardo Vencovsky (https://github.com/LeoVen)
 *
 * \date 07/11/2018
 */

#ifndef C_DATASTRUCTURES_LIBRARY_AVLTREE_H
#define C_DATASTRUCTURES_LIBRARY_AVLTREE_H

#include "Core.h"
#include "Interface.h"

#ifdef __cplusplus
extern "C" {
#endif

/// \struct AVLTree_s
/// \brief A generic, multi-purpose AVL tree.
struct AVLTree_s;

/// \ref AVLTree_t
/// \brief A type for an AVL tree.
///
/// A type for a <code> struct AVLTree_s </code> so you don't have to always
/// write the full name of it.
typedef struct AVLTree_s AVLTree_t;

/// \ref AVLTree
/// \brief A pointer type for an AVL tree.
///
/// Defines a pointer type to <code> struct AVLTree_s </code>. This typedef is
/// used to avoid having to declare every AVL tree as a pointer type since they
/// all must be dynamically allocated.
typedef struct AVLTree_s *AVLTree;

///////////////////////////////////// STRUCTURE INITIALIZATION AND DELETION ///

/// \ref avl_new
/// \brief Initializes a new AVL tree.
AVLTree_t *
avl_new(Interface_t *interface);

/// \ref avl_free
/// \brief Frees from memory an AVLTree_s and its elements.
void
avl_free(AVLTree_t *tree);

/// \ref avl_free_shallow
/// \brief Frees from memory an AVLTree_s leaving its elements intact.
void
avl_free_shallow(AVLTree_t *tree);

/// \ref avl_erase
/// \brief Frees from memory all elements of an AVLTree_s.
void
avl_erase(AVLTree_t *tree);

/// \ref avl_erase_shallow
/// \brief Frees from memory all the nodes of an AVLTree_s.
void
avl_erase_shallow(AVLTree_t *tree);

//////////////////////////////////////////////////////////// CONFIGURATIONS ///

/// \ref avl_config
/// \brief Sets a new interface for the target AVL tree.
void
avl_config(AVLTree_t *tree, Interface_t *new_interface);

/////////////////////////////////////////////////////////////////// GETTERS ///

/// \ref avl_size
/// \brief Returns the amount of elements in the AVL tree.
integer_t
avl_size(AVLTree_t *tree);

/// \ref avl_limit
/// \brief Returns the AVL tree's maximum number of elements.
integer_t
avl_limit(AVLTree_t *tree);

/////////////////////////////////////////////////////////////////// SETTERS ///

/// \ref avl_set_limit
/// \brief Sets a new limit to the AVL tree's size.
bool
avl_set_limit(AVLTree_t *tree, integer_t limit);

////////////////////////////////////////////////////////// INPUT AND OUTPUT ///

/// \ref avl_insert
/// \brief Adds a new element in the specified AVL tree.
bool
avl_insert(AVLTree_t *tree, void *element);

/// \ref avl_remove
/// \brief Removes an element from the tree that matches the given element.
bool
avl_remove(AVLTree_t *tree, void *element);

/// \ref avl_pop
/// \brief Removes the root element and frees it from memory.
bool
avl_pop(AVLTree_t *tree);

/////////////////////////////////////////////////////////// STRUCTURE STATE ///

/// \ref avl_empty
/// \brief Checks if the specified AVL tree is empty.
bool
avl_empty(AVLTree_t *tree);

/// \ref avl_full
/// \brief Checks if the specified AVL tree is full.
bool
avl_full(AVLTree_t *tree);

/////////////////////////////////////////////////////////////////// UTILITY ///

/// \ref avl_contains
/// \brief Checks if a given element is in the AVL tree.
bool
avl_contains(AVLTree_t *tree, void *element);

/// \ref avl_peek
/// \brief Returns the root element if present.
void *
avl_peek(AVLTree_t *tree);

/// \ref avl_max
/// \brief Returns the maximum element if present.
void *
avl_max(AVLTree_t *tree);

/// \ref avl_min
/// \brief Returns the minimum element if present.
void *
avl_min(AVLTree_t *tree);

/////////////////////////////////////////////////////////////////// DISPLAY ///

/// \ref avl_display
/// \brief Displays an AVL tree in the console given a display mode.
void
avl_display(AVLTree_t *tree, int display_mode);

/// \ref avl_traversal
/// \brief Displays each element in the console given a traversal order.
void
avl_traversal(AVLTree_t *tree, int traversal_mode);

///////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////// Iterator ///
///////////////////////////////////////////////////////////////////////////////

/// \todo AVLTreeIterator

///////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////// Wrapper ///
///////////////////////////////////////////////////////////////////////////////

/// \todo AVLTreeWrapper

#ifdef __cplusplus
}
#endif

#endif //C_DATASTRUCTURES_LIBRARY_AVLTREE_H
