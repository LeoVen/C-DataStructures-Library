/**
 * @file RedBlackTree.c
 * 
 * @author Leonardo Vencovsky (https://github.com/LeoVen)
 *
 * @date 13/12/2018
 */

#include "RedBlackTree.h"

/// A red-black tree is a binary search tree where each node has a color, which
/// can be either \c RED or \c BLACK. By constraining the node colors on any
/// simple path from the root to a leaf, red-black trees ensure that no such
/// path is more than twice as long as any other, so that the tree is
/// approximately balanced.
///
/// The red-black tree needs to satisfy the following properties:
/// - Every node is either red or black;
/// - The root is black;
/// - Every NULL child is considered black;
/// - If a node is red, then both its children are black;
/// - For each node, all simple paths from the node to descendant leaves
/// contains the same number of black nodes including the NULL child.
///
/// \par Black-height bh(x)
/// The black-height of a red-black tree is the number of black nodes from, but
/// not including, the root node down to a leaf.
///
/// \par Height
/// A red-black tree with \c N internal nodes has a height of at most
/// <code> 2 * log(N + 1) </code>.
///
/// \par Functions
/// Located in the file RedBlackTree.c
struct RedBlackTree_s
{
    /// \brief Tree size.
    ///
    /// Red-black tree's current amount of elements.
    integer_t size;

    /// \brief Tree size limit.
    ///
    /// If it is set to 0 or a negative value then the tree has no limit to its
    /// size. Otherwise it won't be able to have more elements than the
    /// specified value. The tree is always initialized with no restrictions to
    /// its size, that is, \c limit equals 0. The user won't be able to limit
    /// the tree size if it already has more elements than the specified limit.
    integer_t limit;

    /// \brief The tree's root.
    ///
    /// The root element of a red-black tree.
    struct RedBlackTreeNode_s *root;

    /// \brief RedBlackTree_s interface.
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

/// \brief A RedBlackTree_s node.
///
/// Implementation detail. This node is like a binary search tree node but with
/// a color member that states if the node is either \c red or \c black.
struct RedBlackTreeNode_s
{
    /// \brief Node's key.
    ///
    /// A pointer to the node's key.
    void *key;

    /// \brief States if the node is black or not.
    ///
    /// If true, the node is black, if false, the node is red.
    bool color;

    /// \brief A pointer to its right child.
    ///
    /// A pointer to its right child where its element is greater than the
    /// current node or NULL if there is no right subtree or if this node is
    /// a leaf.
    struct RedBlackTreeNode_s *right;

    /// \brief A pointer to its left child.
    ///
    /// A pointer to its left child where its element is smaller than the
    /// current node or NULL if there is no left subtree or if this node is
    /// a leaf.
    struct RedBlackTreeNode_s *left;

    /// \brief Pointer to parent node.
    ///
    /// Pointer to parent node or NULL if this is the root node.
    struct RedBlackTreeNode_s *parent;
};

/// \brief A type for a red-black tree node.
///
/// Defines a type to a <code> struct RedBlackTreeNode_s </code>.
typedef struct RedBlackTreeNode_s RedBlackTreeNode_t;

/// \brief A pointer type for a red-black tree node.
///
/// Defines a pointer type to a <code> struct RedBlackTreeNode_s </code>.
typedef struct RedBlackTreeNode_s *RedBlackTreeNode;

static const bool BLACK = true;
static const bool RED = false;

///////////////////////////////////////////////////// NOT EXPOSED FUNCTIONS ///

static RedBlackTreeNode_t *
rbt_new_node(void *element);

static void
rbt_free_node(RedBlackTreeNode_t *node, free_f function);

static void
rbt_free_node_shallow(RedBlackTreeNode_t *node);

static void
rbt_free_tree(RedBlackTreeNode_t *root, free_f function);

static void
rbt_free_tree_shallow(RedBlackTreeNode_t *root);

// Rotations, re-balancing and other things to maintain the red-black tree's
// properties
static void
rbt_rotate_left(RedBlackTree_t *tree, RedBlackTreeNode_t *X);

static void
rbt_rotate_right(RedBlackTree_t *tree, RedBlackTreeNode_t *X);

static void
rbt_insert_fixup(RedBlackTree_t *tree, RedBlackTreeNode_t *Z);

static void
rbt_remove_fixup(RedBlackTree_t *tree, RedBlackTreeNode_t *X);

static RedBlackTreeNode_t *
rbt_find(RedBlackTree_t *tree, void *element);

static RedBlackTreeNode_t *
rbt_successor(RedBlackTreeNode_t *N);

static RedBlackTreeNode_t *
rbt_minimum(RedBlackTreeNode_t *N);

static RedBlackTreeNode_t *
rbt_maximum(RedBlackTreeNode_t *N);

static void
rbt_transplant(RedBlackTree_t *tree, RedBlackTreeNode_t *U,
               RedBlackTreeNode_t *V);

static bool
rbt_color(RedBlackTreeNode_t *node);

// Displaying modes
static void
rbt_display_tree(RedBlackTreeNode_t *root, integer_t height,
                 display_f function);

static void
rbt_display_color(RedBlackTreeNode_t *root, integer_t height,
        display_f function);

static void
rbt_display_simple(RedBlackTreeNode_t *root, integer_t height,
                   display_f function);

static void
rbt_display_treeview(RedBlackTreeNode_t *root, integer_t depth, char *path,
                     display_f function, bool direction);

// Traversal
static void
rbt_traversal_preorder(RedBlackTreeNode_t *root, display_f function);

static void
rbt_traversal_inorder(RedBlackTreeNode_t *root, display_f function);

static void
rbt_traversal_postorder(RedBlackTreeNode_t *root, display_f function);

static void
rbt_traversal_leaves(RedBlackTreeNode_t *root, display_f function);

////////////////////////////////////////////// END OF NOT EXPOSED FUNCTIONS ///

/// Initializes a new RedBlackTree_s with \c size, \c limit, and \c version_id
/// to 0, \c root to NULL and sets its interface.
///
/// \par Interface Requirements
/// - None
///
/// \param interface An interface defining all necessary functions for the
/// red-black tree to operate.
///
/// \return A new RedBlackTree_s or NULL if allocation failed.
RedBlackTree_t *
rbt_new(Interface_t *interface)
{
    RedBlackTree_t *tree = malloc(sizeof(RedBlackTree_t));

    if (!tree)
        return NULL;

    tree->size = 0;
    tree->limit = 0;
    tree->version_id = 0;
    tree->root = NULL;

    tree->interface = interface;

    return tree;
}

/// Frees a RedBlackTree_s, freeing all of its elements using the interface's
/// free function.
///
/// \par Interface Requirements
/// - free
///
/// \param tree The red-black tree to be freed from memory.
void
rbt_free(RedBlackTree_t *tree)
{
    rbt_free_tree(tree->root, tree->interface->free);

    free(tree);
}

/// Frees a RedBlackTree_s, freeing all of its nodes, leaving its elements
/// intact.
///
/// \par Interface Requirements
/// - None
///
/// \param tree The red-black tree to be freed from memory.
void
rbt_free_shallow(RedBlackTree_t *tree)
{
    rbt_free_tree_shallow(tree->root);

    free(tree);
}

/// Frees a RedBlackTree_s, freeing all of its elements using the interface's
/// free function but leaves the tree's structure without a change.
///
/// \par Interface Requirements
/// - free
///
/// \param tree The red-black tree to have all of its elements freed from
/// memory.
void
rbt_erase(RedBlackTree_t *tree)
{
    rbt_free_tree(tree->root, tree->interface->free);

    tree->root = NULL;
    tree->size = 0;
}

/// Frees a RedBlackTree_s, freeing all of its nodes, leaving its elements
/// intact. The RedBlackTree_s structure is not changed.
///
/// \par Interface Requirements
/// - None
///
/// \param tree The red-black tree to have all of its nodes freed from memory.
void
rbt_erase_shallow(RedBlackTree_t *tree)
{
    rbt_free_tree_shallow(tree->root);

    tree->root = NULL;
    tree->size = 0;
}

/// Changes the red-black tree's interface.
///
/// \par Interface Requirements
/// - None
///
/// \param tree RedBlackTree_s reference.
/// \param new_interface A new interface for the specified red-black tree.
void
rbt_config(RedBlackTree_t *tree, Interface_t *new_interface)
{
    tree->interface = new_interface;
}

/// Returns the amount of elements in the red-black tree.
///
/// \par Interface Requirements
/// - None
///
/// \param tree RedBlackTree_s reference.
///
/// \return The amount of elements in the red-black tree.
integer_t
rbt_size(RedBlackTree_t *tree)
{
    return tree->size;
}

/// Returns the red-black tree's maximum number of elements defined by the
/// user.
///
/// \par Interface Requirements
/// - None
///
/// \param tree RedBlackTree_s reference.
///
/// \return The red-black tree's maximum number of elements.
integer_t
rbt_limit(RedBlackTree_t *tree)
{
    return tree->limit;
}

/// Sets a limit to the amount of elements in the red-black tree.
///
/// \par Interface Requirements
/// - None
///
/// \param tree RedBlackTree_s reference.
/// \param limit The specified limit
///
/// \return false if the limit is less than the tree's current size and greater
/// than 0. Returns true if the limit was successfully set.
bool
rbt_set_limit(RedBlackTree_t *tree, integer_t limit)
{
    if (tree->size > limit && limit > 0)
        return false;

    tree->limit = limit;

    return true;
}

/// Adds a new element in the specified red-black tree. The tree does not
/// accepts duplicate values.
///
/// \par Interface Requirements
/// - compare
///
/// \param tree RedBlackTree_s reference.
/// \param element The element to be added to the red-black tree.
///
/// \return True if the element was added to the tree.
/// \return False if the element is already present in the red-black tree, if
/// the tree has a limited size or if any allocations failed.
bool
rbt_insert(RedBlackTree_t *tree, void *element)
{
    if (rbt_full(tree))
        return false;

    if (rbt_empty(tree))
    {
        tree->root = rbt_new_node(element);

        if (!tree->root)
            return false;

        tree->root->color = BLACK;
    }
    else
    {
        RedBlackTreeNode_t *scan = tree->root;
        RedBlackTreeNode_t *parent = scan;

        while (scan != NULL)
        {
            parent = scan;

            if (tree->interface->compare(scan->key, element) > 0)
                scan = scan->left;
            else if (tree->interface->compare(scan->key, element) < 0)
                scan = scan->right;
            else
                return false; /* No duplicates are allowed */
        }

        RedBlackTreeNode_t *node;

        if (tree->interface->compare(parent->key, element) < 0)
        {
            parent->right = rbt_new_node(element);

            if (!parent->right)
                return false;

            parent->right->parent = parent;
            node = parent->right;
        }
        else
        {
            parent->left = rbt_new_node(element);

            if (!parent->left)
                return false;

            parent->left->parent = parent;
            node = parent->left;
        }

        rbt_insert_fixup(tree, node);
    }

    tree->size++;
    tree->version_id++;

    return true;
}

/// Removes an element, if present, that matches a given element from the
/// specified red-black tree.
///
/// \par Interface Requirements
/// - compare
/// - free
///
/// \param tree RedBlackTree_s reference.
/// \param element The element to be removed has to match this element.
///
/// \return True if the element was removed.
/// \return False if the element was not found.
bool
rbt_remove(RedBlackTree_t *tree, void *element)
{
    RedBlackTreeNode_t *Z = rbt_find(tree, element);

    if (Z == NULL)
        return false;

    if (rbt_size(tree) == 1)
    {
        // Remove the last node
        rbt_free_node(tree->root, tree->interface->free);

        tree->root = NULL;
    }
    else
    {
        RedBlackTreeNode_t *Y = NULL;
        RedBlackTreeNode_t *X = NULL;

        if (Z->left == NULL || Z->right == NULL)
            Y = Z;
        else
            Y = rbt_successor(Z);

        if (Y->left != NULL)
            X = Y->left;
        else
            X = Y->right;

        if (X)
            X->parent = Y->parent;

        // Disassociate Y from the tree
        if (Y->parent == NULL)
            tree->root = X;
        else if (Y == Y->parent->left)
            Y->parent->left = X;
        else
            Y->parent->right = X;

        // Move Y's element to Z's and Z's to Y's because Y is to be deleted
        // instead of Z
        if (Y != Z)
        {
            void *temp = Y->key;
            Y->key = Z->key;
            Z->key = temp;
        }

        if (rbt_color(Y) == BLACK)
            rbt_remove_fixup(tree, X);

        rbt_free_node(Y, tree->interface->free);
    }

    tree->size--;
    tree->version_id++;

    return true;
}

/// Removes the root element and frees it from memory.
///
/// \par Interface Requirements
/// - None
///
/// \param tree RedBlackTree_s reference.
///
/// \return True if the element was removed.
/// \return False if the element was not found.
bool
rbt_pop(RedBlackTree_t *tree)
{
    void *element = rbt_peek(tree);

    if (!element)
        return false;

    return rbt_remove(tree, element);
}

/// Checks if the specified red-black tree is empty based on its \c size
/// counter.
///
/// \par Interface Requirements
/// - None
///
/// \param tree RedBlackTree_s reference.
///
/// \return True if \c size equals 0, otherwise false.
bool
rbt_empty(RedBlackTree_t *tree)
{
    return tree->size == 0;
}

/// Checks if the specified red-black tree is full. The tree can only be full
/// if the limit is greater than 0 and the tree's size equals the limit.
///
/// \par Interface Requirements
/// - None
///
/// \param tree RedBlackTree_s reference.
///
/// \return True if \c limit is greater than 0 and the tree's size equals the
/// tree's limit, otherwise false.
bool
rbt_full(RedBlackTree_t *tree)
{
    return tree->limit > 0 && tree->size >= tree->limit;
}

/// Checks if a given element is in the red-black tree.
///
/// \par Interface Requirements
/// - None
///
/// \param tree RedBlackTree_s reference.
/// \param element The element to be searched in the tree.
///
/// \return True if the the red-black tree contains the element, otherwise
/// false.
bool
rbt_contains(RedBlackTree_t *tree, void *element)
{
    RedBlackTreeNode_t *node = rbt_find(tree, element);

    return node != NULL;
}

/// Returns the root element.
///
/// \par Interface Requirements
/// - None
///
/// \param tree RedBlackTree_s reference.
///
/// \return Returns the root element or NULL if the tree is empty.
void *
rbt_peek(RedBlackTree_t *tree)
{
    if (rbt_empty(tree))
        return NULL;

    return tree->root->key;
}

/// Returns the maximum element in the tree or NULL if it is empty.
///
/// \par Interface Requirements
/// - None
///
/// \param tree RedBlackTree_s reference.
///
/// \return The maximum element or NULL if the tree is empty.
void *
rbt_max(RedBlackTree_t *tree)
{
    if (rbt_empty(tree))
        return NULL;

    RedBlackTreeNode_t *scan = tree->root;

    while (scan->right != NULL)
        scan = scan->right;

    return scan->key;
}

/// Returns the minimum element in the tree or NULL if it is empty.
///
/// \par Interface Requirements
/// - None
///
/// \param tree RedBlackTree_s reference.
///
/// \return The minimum element or NULL if the tree is empty.
void *
rbt_min(RedBlackTree_t *tree)
{
    if (rbt_empty(tree))
        return NULL;

    RedBlackTreeNode_t *scan = tree->root;

    while (scan->left != NULL)
        scan = scan->left;

    return scan->key;
}

/// Displays a RedBlackTree_s in the console. There are currently four modes:
/// - -1 Displays the tree with \c rbt_display_tree.
/// - 0 Displays the tree with \c rbt_display_simple.
/// - 1 Displays the tree with \c rbt_display_color
/// - Any other values defaults to \c rbt_display_treeview
///
/// \par Interface Requirements
/// - display
///
/// \param tree RedBlackTree_s reference.
/// \param display_mode The way the tree is to be displayed.
void
rbt_display(RedBlackTree_t *tree, int display_mode)
{
    if (display_mode)
    {
        printf("\n+--------------------------------------------------+");
        printf("\n|                  Red-Black Tree                  |");
        printf("\n+--------------------------------------------------+\n");
    }

    if (rbt_empty(tree) && display_mode)
    {
        printf(" EMPTY\n");
    }

    if (display_mode == -1)
    {
        rbt_display_tree(tree->root, 0, tree->interface->display);
    }
    else if (display_mode == 0)
    {
        rbt_display_simple(tree->root, 0, tree->interface->display);
    }
    else if (display_mode == 1)
    {
        rbt_display_color(tree->root, 1, tree->interface->display);
    }
    else
    {
        char path[5000] = {};

        rbt_display_treeview(tree->root, 0, path, tree->interface->display, false);
        printf("\n");
    }
}

/// Displays an RedBlackTree_s in the console. There are currently four modes:
/// - -1 Pre-order traversal.
/// - 0 In-order traversal.
/// - 1 Post-order traversal.
/// - Any other values defaults to leaves traversal
///
/// \par Interface Requirements
/// - display
///
///
/// \param tree RedBlackTree_s reference.
/// \param traversal_mode The way the tree is to be traversed.
void
rbt_traversal(RedBlackTree_t *tree, int traversal_mode)
{
    switch (traversal_mode)
    {
        case -1:
            printf("Pre-order Traversal\n");
            rbt_traversal_preorder(tree->root, tree->interface->display);
            break;
        case 0:
            printf("In-order Traversal\n");
            rbt_traversal_inorder(tree->root, tree->interface->display);
            break;
        case 1:
            printf("Post-order Traversal\n");
            rbt_traversal_postorder(tree->root, tree->interface->display);
            break;
        default:
            printf("Leaves Traversal\n");
            rbt_traversal_leaves(tree->root, tree->interface->display);
            break;
    }

    printf("\n");
}

///////////////////////////////////////////////////// NOT EXPOSED FUNCTIONS ///

static RedBlackTreeNode_t *
rbt_new_node(void *element)
{
    RedBlackTreeNode_t *node = malloc(sizeof(RedBlackTreeNode_t));

    if (!node)
        return NULL;

    // All new nodes are red
    node->color = RED;
    node->key = element;

    node->parent = NULL;
    node->left = NULL;
    node->right = NULL;

    return node;
}

static void
rbt_free_node(RedBlackTreeNode_t *node, free_f function)
{
    function(node->key);

    free(node);
}

static void
rbt_free_node_shallow(RedBlackTreeNode_t *node)
{
    free(node);
}

static void
rbt_free_tree(RedBlackTreeNode_t *root, free_f function)
{
    RedBlackTreeNode_t *scan = root;
    RedBlackTreeNode_t *up = NULL;

    while (scan != NULL)
    {
        if (scan->left != NULL)
        {
            RedBlackTreeNode_t *left = scan->left;

            scan->left = up;
            up = scan;
            scan = left;
        }
        else if (scan->right != NULL)
        {
            RedBlackTreeNode_t *right = scan->right;

            scan->left = up;
            scan->right = NULL;
            up = scan;
            scan = right;
        }
        else
        {
            if (up == NULL)
            {
                rbt_free_node(scan, function);
                scan = NULL;
            }

            while (up != NULL)
            {
                rbt_free_node(scan, function);

                if (up->right != NULL)
                {
                    scan = up->right;
                    up->right = NULL;
                    break;
                }
                else
                {
                    scan = up;
                    up = up->left;
                }
            }
        }
    }
}

static void
rbt_free_tree_shallow(RedBlackTreeNode_t *root)
{
    RedBlackTreeNode_t *scan = root;
    RedBlackTreeNode_t *up = NULL;

    while (scan != NULL)
    {
        if (scan->left != NULL)
        {
            RedBlackTreeNode_t *left = scan->left;

            scan->left = up;
            up = scan;
            scan = left;
        }
        else if (scan->right != NULL)
        {
            RedBlackTreeNode_t *right = scan->right;

            scan->left = up;
            scan->right = NULL;
            up = scan;
            scan = right;
        }
        else
        {
            if (up == NULL)
            {
                rbt_free_node_shallow(scan);
                scan = NULL;
            }

            while (up != NULL)
            {
                rbt_free_node_shallow(scan);

                if (up->right != NULL)
                {
                    scan = up->right;
                    up->right = NULL;
                    break;
                }
                else
                {
                    scan = up;
                    up = up->left;
                }
            }
        }
    }
}

static void
rbt_rotate_left(RedBlackTree_t *tree, RedBlackTreeNode_t *X)
{
    RedBlackTreeNode_t *Y = X->right;

    X->right = Y->left;

    if (Y->left != NULL)
        Y->left->parent = X;

    Y->parent = X->parent;

    if (X->parent == NULL)
        tree->root = Y;
    else if (X == X->parent->left)
        X->parent->left = Y;
    else
        X->parent->right = Y;

    Y->left = X;
    X->parent = Y;
}

static void
rbt_rotate_right(RedBlackTree_t *tree, RedBlackTreeNode_t *X)
{
    RedBlackTreeNode_t *Y = X->left;

    X->left = Y->right;

    if (Y->right != NULL)
        Y->right->parent = X;

    Y->parent = X->parent;

    if (X->parent == NULL)
        tree->root = Y;
    else if (X == X->parent->left)
        X->parent->left = Y;
    else
        X->parent->right = Y;

    Y->right = X;
    X->parent = Y;
}

static void
rbt_insert_fixup(RedBlackTree_t *tree, RedBlackTreeNode_t *Z)
{
    RedBlackTreeNode_t *Y;

    // Cases 1, 2, 3 are symmetric to 4, 5, 6
    while (rbt_color(Z->parent) == RED)
    {
        if (Z->parent == Z->parent->parent->left)
        {
            // Uncle
            Y = Z->parent->parent->right;

            // Uncle is red, recolor parent and grandparent
            // CASE 1 (symmetrical to 4)
            if (rbt_color(Y) == RED)
            {
                Z->parent->color = BLACK;
                Y->color = BLACK;
                Z->parent->parent->color = RED;
                Z = Z->parent->parent;
            }
            else
            {
                // CASE 2 (symmetrical to 5)
                // Left-right skewed
                if (Z == Z->parent->right)
                {
                    Z = Z->parent;
                    rbt_rotate_left(tree, Z);
                }
                // Left-Left skewed
                // CASE 3 (symmetrical to 6)
                Z->parent->color = BLACK;
                Z->parent->parent->color = RED;
                rbt_rotate_right(tree, Z->parent->parent);
            }
        }
        else /* if Z->parent == Z->parent->parent->right */
        {
            // Uncle
            Y = Z->parent->parent->left;

            // Uncle is red, recolor parent and grandparent
            // CASE 4 (symmetrical to 1)
            if (rbt_color(Y) == RED)
            {
                Z->parent->color = BLACK;
                Y->color = BLACK;
                Z->parent->parent->color = RED;
                Z = Z->parent->parent;
            }
            else
            {
                // CASE 5 (symmetrical to 2)
                // Right-left skewed
                if (Z == Z->parent->left)
                {
                    Z = Z->parent;
                    rbt_rotate_right(tree, Z);
                }
                // CASE 6 (symmetrical to 3)
                // Right-Right skewed
                Z->parent->color = BLACK;
                Z->parent->parent->color = RED;
                rbt_rotate_left(tree, Z->parent->parent);
            }
        }
    }

    //keep root always black
    tree->root->color = BLACK;
}

static void
rbt_remove_fixup(RedBlackTree_t *tree, RedBlackTreeNode_t *X)
{
    RedBlackTreeNode_t *W = NULL;

    while (X && X != tree->root && rbt_color(X) == BLACK)
    {
        if (X == X->parent->left)
        {
            W = X->parent->right;

            // CASE 1
            if (rbt_color(W) == RED)
            {
                W->color = BLACK;
                X->parent->color = RED;
                rbt_rotate_left(tree, X->parent);
                W = X->parent->right;
            }

            // CASE 2
            if (rbt_color(W->left) == BLACK && rbt_color(W->right) == BLACK)
            {
                W->color = RED;
                X = X->parent;
            }
            else
            {
                // CASE 3
                if (rbt_color(W->right) == BLACK)
                {
                    if (W->left != NULL)
                        W->left->color = BLACK;

                    W->color = RED;
                    rbt_rotate_right(tree, W);
                    W = X->parent->right;
                }

                // CASE 4
                W->color = X->parent->color;
                X->parent->color = BLACK;

                if (W->right != NULL)
                    W->right->color = BLACK;

                rbt_rotate_left(tree, X->parent);
                X = tree->root;
            }
        }
        else /* if X == X->parent->right */
        {
            W = X->parent->left;

            // CASE 1
            if (rbt_color(W) == RED)
            {
                W->color = BLACK;
                X->parent->color = RED;
                rbt_rotate_right(tree, X->parent);
                W = X->parent->left;
            }

            // CASE 2
            if (rbt_color(W->left) == BLACK && rbt_color(W->right) == BLACK)
            {
                W->color = RED;
                X = X->parent;
            }
            else
            {
                // CASE 3
                if (rbt_color(W->left) == BLACK)
                {
                    if (W->right != NULL)
                        W->right->color = BLACK;

                    W->color = RED;
                    rbt_rotate_left(tree, W);
                    W = X->parent->left;
                }

                // CASE 4
                W->color = X->parent->color;
                X->parent->color = BLACK;

                if (W->left != NULL)
                    W->left->color = BLACK;

                rbt_rotate_right(tree, X->parent);
                X = tree->root;
            }
        }
    } /* End While */

    if (X != NULL)
        X->color = BLACK;
}

static RedBlackTreeNode_t *
rbt_find(RedBlackTree_t *tree, void *element)
{
    RedBlackTreeNode_t *scan = tree->root;

    while (scan != NULL)
    {
        if (tree->interface->compare(scan->key, element) > 0)
            scan = scan->left;
        else if (tree->interface->compare(scan->key, element) < 0)
            scan = scan->right;
        else
            return scan;
    }

    return NULL;
}

static RedBlackTreeNode_t *
rbt_successor(RedBlackTreeNode_t *N)
{
    if (N->right != NULL)
        return rbt_minimum(N->right);

    RedBlackTreeNode_t *Y = N->parent;

    while (Y != NULL && N == Y->right)
    {
        N = Y;
        Y = Y->parent;
    }

    return Y;
}

static RedBlackTreeNode_t *
rbt_minimum(RedBlackTreeNode_t *N)
{
    // Finds the minimum node of subtree N
    while (N->left != NULL)
        N = N->left;

    return N;
}

static RedBlackTreeNode_t *
rbt_maximum(RedBlackTreeNode_t *N)
{
    // Finds the maximum node of subtree N
    while (N->right != NULL)
        N = N->right;

    return N;
}

static void
rbt_transplant(RedBlackTree_t *tree, RedBlackTreeNode_t *U,
        RedBlackTreeNode_t *V)
{
    if (U->parent == NULL)
        tree->root = V;
    else if (U == U->parent->left)
        U->parent->left = V;
    else
        U->parent->right = V;

    if (V == NULL)
        return;

    V->parent = U->parent;
}

static bool
rbt_color(RedBlackTreeNode_t *node)
{
    if (node == NULL)
        return BLACK;

    return node->color;
}

static void
rbt_display_tree(RedBlackTreeNode_t *root, integer_t height,
                 display_f function)
{
    if (root == NULL)
        return;

    rbt_display_tree(root->right, height + 1, function);

    for (integer_t i = 0; i < height; i++)
        printf("|------- ");

    function(root->key);

    printf("\n");

    rbt_display_tree(root->left, height + 1, function);
}

static void
rbt_display_color(RedBlackTreeNode_t *root, integer_t height,
        display_f function)
{
    if (root == NULL)
        return;

    rbt_display_color(root->right, height + 1, function);

    for (integer_t i = 0; i < height - 1; i++)
        printf("        ");

    printf("|---%c---< ", root->color ? 'B' : 'R');

    function(root->key);
    printf("\n");

    rbt_display_color(root->left, height + 1, function);
}

static void
rbt_display_simple(RedBlackTreeNode_t *root, integer_t height,
        display_f function)
{
    if (root == NULL)
        return;

    rbt_display_simple(root->right, height + 1, function);

    for (integer_t i = 0; i < height; i++)
        printf("        ");

    function(root->key);
    printf("\n");

    rbt_display_simple(root->left, height + 1, function);
}

static void
rbt_display_treeview(RedBlackTreeNode_t *root, integer_t depth, char *path,
        display_f function, bool direction)
{
    const integer_t spaces = 8;

    if (root == NULL)
        return;

    depth++;

    rbt_display_treeview(root->right, depth, path, function, true);

    path[depth - 2] = 0;

    if(direction)
        path[depth - 2] = 1;

    if(root->left)
        path[depth - 1] = 1;

    printf("\n");

    for (integer_t i = 0; i < depth - 1; i++)
    {
        if (i == depth - 2)
            printf("%c", direction ? 218 : 192);
        else if (path[i])
            printf("%c", 179);
        else
            printf(" ");

        for (integer_t j = 1; j < spaces; j++)
            if (i < depth - 2)
                printf(" ");
            else
                printf("%c", 196);
    }

    printf(" ");
    function(root->key);
    printf("\n");

    for (int i = 0; i < depth; i++)
    {
        if (path[i] && (root->left || i != depth -1))
            printf("%c", 179);
        else
            printf(" ");

        for (integer_t j = 1; j < spaces; j++)
            printf(" ");
    }

    rbt_display_treeview(root->left, depth, path, function, false);
}

static void
rbt_traversal_preorder(RedBlackTreeNode_t *root, display_f function)
{
    if (root == NULL)
        return;

    function(root->key);
    printf(" ");

    rbt_traversal_preorder(root->left, function);

    rbt_traversal_preorder(root->right, function);
}

static void
rbt_traversal_inorder(RedBlackTreeNode_t *root, display_f function)
{
    if (root == NULL)
        return;

    rbt_traversal_inorder(root->left, function);

    function(root->key);
    printf(" ");

    rbt_traversal_inorder(root->right, function);
}

static void
rbt_traversal_postorder(RedBlackTreeNode_t *root, display_f function)
{
    if (root == NULL)
        return;

    rbt_traversal_postorder(root->left, function);

    rbt_traversal_postorder(root->right, function);

    function(root->key);
    printf(" ");
}

static void
rbt_traversal_leaves(RedBlackTreeNode_t *root, display_f function)
{
    if (root == NULL)
        return;

    rbt_traversal_leaves(root->left, function);

    rbt_traversal_leaves(root->right, function);

    if (root->right == NULL && root->left == NULL)
    {
        function(root->key);
        printf(" ");
    }
}

////////////////////////////////////////////// END OF NOT EXPOSED FUNCTIONS ///

///////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////// Iterator ///
///////////////////////////////////////////////////////////////////////////////

/// \todo RedBlackTreeIterator

///////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////// Wrapper ///
///////////////////////////////////////////////////////////////////////////////

/// \todo RedBlackTreeWrapper
