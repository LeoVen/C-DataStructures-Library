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
    /// \brief Node's data.
    ///
    /// A pointer to the node's data.
    void *data;

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
rbt_remove_fixup(RedBlackTree_t *tree, RedBlackTreeNode_t *X,
        RedBlackTreeNode_t *P);

static RedBlackTreeNode_t *
rbt_find(RedBlackTree_t *tree, void *element);

static RedBlackTreeNode_t *
rbt_minimum(RedBlackTreeNode_t *N);

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

////////////////////////////////////////////// END OF NOT EXPOSED FUNCTIONS ///

/// Initializes a new RedBlackTree_s with \c size, \c limit, and \c version_id
/// to 0, \c root to NULL and sets its interface.
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
/// \par Interface Requirements
/// - free
///
/// \param tree The red-black tree to be freed from memory
void
rbt_free(RedBlackTree_t *tree)
{
    rbt_free_tree(tree->root, tree->interface->free);

    free(tree);
}

/// Frees a RedBlackTree_s, freeing all of its nodes, leaving its elements
/// intact.
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

/// Recursively frees a RedBlackTree_s, freeing all of its elements using the
/// interface's free function but leaves the tree's structure and interface
/// without change.
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

/// Recursively frees a RedBlackTree_s, freeing all of its nodes, leaving its
/// elements intact. The RedBlackTree_s structure is not changed.
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
/// \par Interface Requirements
/// - None
///
/// \param tree
/// \param new_interface
void
rbt_config(RedBlackTree_t *tree, Interface_t *new_interface)
{
    tree->interface = new_interface;
}

/// Returns the amount of elements in the red-black tree.
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
/// \par Interface Requirements
/// - compare
///
/// \param tree RedBlackTree_s reference.
/// \param element The element to be added to the red-black tree.
///
/// \return True if the element was added to the tree.
/// \return False if the element is already present in the red-black tree or if
/// any allocations failed.
bool
rbt_insert(RedBlackTree_t *tree, void *element)
{
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

            if (tree->interface->compare(scan->data, element) > 0)
                scan = scan->left;
            else if (tree->interface->compare(scan->data, element) < 0)
                scan = scan->right;
            else
                return false; /* No duplicates are allowed */
        }

        RedBlackTreeNode_t *node;

        if (tree->interface->compare(parent->data, element) < 0)
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
        return false; /* Not found */

    RedBlackTreeNode_t *Y = Z;
    RedBlackTreeNode_t *X = NULL;
    RedBlackTreeNode_t *P = Z->parent;

    bool original_color = Y->color;

    if (Z->left == NULL)
    {
        X = Z->right;
        rbt_transplant(tree, Z, Z->right);
    }
    else if (Z->right == NULL)
    {
        X = Z->left;
        rbt_transplant(tree, Z, Z->left);
    }
    else
    {
        Y = rbt_minimum(Z->right);

        original_color = Y->color;

        X = Y->right;

        if (Y->parent == Z)
        {
            if (X == NULL)
                P = Y;
            else
                X->parent = Y;
        }
        else
        {
            rbt_transplant(tree, Y, Y->right);
            Y->right = Z->right;
            Y->right->parent = Y;
        }

        rbt_transplant(tree, Z, Y);
        Y->left = Z->left;
        Y->left->parent = Y;
        Y->color = Z->color;
    }
    
    if (original_color == BLACK)
        rbt_remove_fixup(tree, X, P);

    rbt_free_node(Z, tree->interface->free);
    tree->size--;
    tree->version_id++;

    return true;
}

/// Checks if the specified red-black tree is empty based on its \c size
/// counter.
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

/// Displays a RedBlackTree_s in the console. There are currently four modes:
/// - -1 Displays the tree with \c rbt_display_tree.
/// - 0 Displays the tree with \c rbt_display_simple.
/// - 1 Displays the tree with \c rbt_display_color
/// - Any other values defaults to \c rbt_display_treeview
///
/// \param tree RedBlackTree_s reference.
/// \param display_mode The way the tree is to be displayed.
void
rbt_display(RedBlackTree_t *tree, int display_mode)
{
    if (rbt_empty(tree))
        return;

    if (display_mode)
    {
        printf("\n+--------------------------------------------------+");
        printf("\n|                  Red-Black Tree                  |");
        printf("\n+--------------------------------------------------+\n");
    }

    if (display_mode == -1)
    {
        rbt_display_tree(tree->root, 1, tree->interface->display);
    }
    else if (display_mode == 0)
    {
        rbt_display_simple(tree->root, 0, tree->interface->display);
    }
    else if (display_mode == 1)
    {
        rbt_display_color(tree->root, 0, tree->interface->display);
    }
    else
    {
        char path[5000] = {};

        rbt_display_treeview(tree->root, 0, path, tree->interface->display, false);
        printf("\n");
    }
}

///////////////////////////////////////////////////// NOT EXPOSED FUNCTIONS ///

static RedBlackTreeNode_t *
rbt_new_node(void *element)
{
    RedBlackTreeNode node = malloc(sizeof(RedBlackTreeNode_t));

    if (!node)
        return NULL;

    // All new nodes are red
    node->color = RED;
    node->data = element;

    node->parent = NULL;
    node->left = NULL;
    node->right = NULL;

    return node;
}

static void
rbt_free_node(RedBlackTreeNode_t *node, free_f function)
{
    function(node->data);

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

            if (rbt_color(Y) == RED)
            {
                // CASE 1
                Z->parent->color = BLACK;
                Y->color = BLACK;
                Z->parent->parent->color = RED;
                Z = Z->parent->parent;
            }
            else
            {
                if (Z == Z->parent->right)
                {
                    // CASE 2
                    Z = Z->parent;
                    rbt_rotate_left(tree, Z);
                }
                // CASE 3
                Z->parent->color = BLACK;
                Z->parent->parent->color = RED;
                rbt_rotate_right(tree, Z->parent->parent);
            }
        }
        else /* if Z->parent == Z->parent->parent->right */
        {
            // Uncle
            Y = Z->parent->parent->left;

            if (rbt_color(Y) == RED)
            {
                // CASE 4
                Z->parent->color = BLACK;
                Y->color = BLACK;
                Z->parent->parent->color = RED;
                Z = Z->parent->parent;
            }
            else
            {
                if (Z == Z->parent->left)
                {
                    // CASE 5
                    Z = Z->parent;
                    rbt_rotate_right(tree, Z);
                }
                // CASE 6
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
rbt_remove_fixup(RedBlackTree_t *tree, RedBlackTreeNode_t *X,
                 RedBlackTreeNode_t *P)
{
    RedBlackTreeNode_t *W = NULL;

    while (X != tree->root && rbt_color(X) == BLACK)
    {
        if (X != NULL)
            P = X->parent;

        if (X == P->left)
        {
            W = P->right;

            if (rbt_color(W) == RED)
            {
                W->color = BLACK;
                P->color = RED;
                rbt_rotate_left(tree, P);
                W = P->right;
            }

            if (rbt_color(W->left) == BLACK && rbt_color(W->right) == BLACK)
            {
                W->color = RED;
                X = P;
            }
            else
            {
                if (rbt_color(W->right) == BLACK)
                {
                    if (W->left != NULL)
                        W->left->color = BLACK;

                    W->color = RED;
                    rbt_rotate_right(tree, W);
                    W = P->right;
                }

                W->color = P->color;
                P->color = BLACK;

                if (W->right != NULL)
                    W->right->color = BLACK;

                rbt_rotate_left(tree, P);
                X = tree->root;
            }
        }
        else /* if X == P->right */
        {
            W = P->left;

            if (rbt_color(W) == RED)
            {
                W->color = BLACK;
                P->color = RED;
                rbt_rotate_right(tree, P);
                W = P->left;
            }

            if (rbt_color(W->left) == BLACK && rbt_color(W->right) == BLACK)
            {
                W->color = RED;
                X = P;
            }
            else
            {
                if (rbt_color(W->left) == BLACK)
                {
                    if (W->right != NULL)
                        W->right->color = BLACK;

                    W->color = RED;
                    rbt_rotate_left(tree, W);
                    W = P->left;
                }

                W->color = P->color;
                P->color = BLACK;

                if (W->left != NULL)
                    W->left->color = BLACK;

                rbt_rotate_right(tree, P);
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
        if (tree->interface->compare(scan->data, element) > 0)
            scan = scan->left;
        else if (tree->interface->compare(scan->data, element) < 0)
            scan = scan->right;
        else
            return scan;
    }

    return NULL;
}

static RedBlackTreeNode_t *
rbt_minimum(RedBlackTreeNode_t *N)
{
    // Finds the minimum node of subtree N
    while (N->left != NULL)
        N = N->left;

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

    for (integer_t i = 0; i < height - 1; i++)
        printf("        ");

    printf("|---%c---< ", root->color ? 'B' : 'R');

    function(root->data);
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

    for (integer_t i = 0; i < height; i++)
        printf("|-------");

    printf("%c ", root->color ? 'B' : 'R');
    function(root->data);

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

    function(root->data);
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
    function(root->data);
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

////////////////////////////////////////////// END OF NOT EXPOSED FUNCTIONS ///

///////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////// Iterator ///
///////////////////////////////////////////////////////////////////////////////

/// \todo RedBlackTreeIterator

///////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////// Wrapper ///
///////////////////////////////////////////////////////////////////////////////

/// \todo RedBlackTreeWrapper
