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

/// A BinarySearchTree is a node-based binary tree with the following
/// properties:
/// - The left subtree of a node contains only nodes with keys lesser than the
/// node’s key;
/// - The right subtree of a node contains only nodes with keys greater than
/// the node’s key;
/// - The left and right subtree each must also be a binary search tree.
///
/// This Binary Search Tree implementation allows duplicate values. If a
/// duplicate value is inserted, the \c count located at the node is increased.
///
/// To initialize a BinarySearchTree use bst_init(). After that you can insert
/// elements using bst_insert(). To remove elements you can use bst_remove() or
/// if you wish to remove the \c root element use bst_pop(). You can also
/// remove all elements using bst_remove_all().
///
/// \b Functions
///
/// Located in file BinarySearchTree.c
struct BinarySearchTree_s
{
    /// \brief Total elements int the tree.
    ///
    /// Total elements in the tree. This is not the total amount of nodes in
    /// the tree as repetitive values are stores in the same node with a \c
    /// count variable.
    index_t count;

    /// \brief Root node.
    ///
    /// The first node in the tree.
    struct BinarySearchTreeNode_s *root;
};

/// Defines a type for <code> struct BinarySearchTree_s </code>.
///
/// Every list is initialized by \c malloc with \c sizeof(BinarySearchTree_t).
typedef struct BinarySearchTree_s BinarySearchTree_t;

/// Defines a type of pointer to <code> struct BinarySearchTree_s </code>.
///
/// This typedef is used to avoid having to declare every BST as a pointer
/// type since they all must be dynamically allocated.
typedef struct BinarySearchTree_s *BinarySearchTree;

/// Initializes a BinarySearchTree.
///
/// \param bst
///
/// \return
Status bst_init(BinarySearchTree *bst);

/// Inserts the specified element into the tree.
///
/// \param bst
/// \param element
///
/// \return
Status bst_insert(BinarySearchTree bst, int element);

/// Removes the specified element from the tree.
///
/// \param bst
/// \param element
///
/// \return
Status bst_remove(BinarySearchTree bst, int element);

/// Removes all instances of the specified element from the tree.
///
/// \param bst
/// \param element
/// \param total
///
/// \return
Status bst_remove_all(BinarySearchTree bst, int element, unsigned *total);

/// Removes and retrieves root element.
///
/// \param bst
/// \param result
///
/// \return
Status bst_pop(BinarySearchTree bst, int *result);

Status bst_display(BinarySearchTree bst, int display);

Status bst_delete(BinarySearchTree *bst);

Status bst_erase(BinarySearchTree *bst);

bool bst_empty(BinarySearchTree bst);

bool bst_contains(BinarySearchTree bst, int value);

int bst_max(BinarySearchTree bst);

int bst_min(BinarySearchTree bst);

int bst_peek(BinarySearchTree bst);

index_t bst_height(BinarySearchTree bst);

Status bst_traversal(BinarySearchTree bst, int traversal);

#endif //C_DATASTRUCTURES_LIBRARY_BINARYSEARCHTREE_H
