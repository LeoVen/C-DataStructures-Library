/**
 * \file AVLTree.h
 * 
 * \author Leonardo Vencovsky (https://github.com/LeoVen)
 *
 * \date 07/11/2018
 */

#ifndef C_DATASTRUCTURES_LIBRARY_AVLTREE_H
#define C_DATASTRUCTURES_LIBRARY_AVLTREE_H

#ifdef __cplusplus
extern "C"
{
#endif

#include "Core.h"

/// A \c AVLTree is much like a \c BinarySearchTree where the difference
/// between heights of left and right subtrees cannot be more than one for
/// all nodes. To maintain this property left and right rotations are used.
///
/// \b Advantages over \c BinarySearchTree
/// - The height of an \c AVLTree is always <code>log n</code> so all
/// operations like search, insert and remove are running on
/// <code>O(log n)</code>
///
/// \b Drawbacks
/// - Insertion is slower because to maintain the AVL property many
/// rotations are needed
///
/// \b Functions
///
/// Located in file AVLTree.c
struct AVLTree_s
{
    size_t size;			  /*!< Total \c AVLTree elements */
    struct AVLTreeNode *root; /*!< Pointer to the very first node in the \c AVLTree  */
};

/// Defines a type for <code> struct AVLTree_s </code>.
///
/// Every AVL tree is initialized by \c malloc with \c sizeof(AVLTree_t).
typedef struct AVLTree_s AVLTree_t;

/// Defines a type of pointer to <code> struct AVLTree_s </code>.
///
/// This typedef is used to avoid having to declare every AVL tree as a pointer
/// type since they all must be dynamically allocated.
typedef struct AVLTree_s *AVLTree;

Status avl_init(AVLTree *avl);

Status avl_insert(AVLTree avl, int value);

Status avl_remove(AVLTree avl, int value);

Status avl_pop(AVLTree avl, int *result);

Status avl_display(AVLTree avl, int display);

Status avl_delete(AVLTree *avl);

Status avl_erase(AVLTree *avl);

int avl_max(AVLTree avl);

int avl_min(AVLTree avl);

int avl_peek(AVLTree avl);

bool avl_contains(AVLTree avl, int value);

bool avl_empty(AVLTree avl);

Status avl_traversal(AVLTree avl, int traversal);

#ifdef __cplusplus
}
#endif

#endif //C_DATASTRUCTURES_LIBRARY_AVLTREE_H
