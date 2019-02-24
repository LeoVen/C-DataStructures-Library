/**
 * @file BinarySearchTree.c
 * 
 * @author Leonardo Vencovsky (https://github.com/LeoVen)
 *
 * @date 31/10/2018
 */

#include "BinarySearchTree.h"

/// A BinarySearchTree_s is a node-based binary tree with the following
/// properties:
/// - The left subtree of a node contains only nodes with keys lesser than the
/// node’s key;
/// - The right subtree of a node contains only nodes with keys greater than
/// the node’s key;
/// - The left and right subtree each must also be a binary search tree.
///
/// This Binary Search Tree does not allow duplicate values.
///
/// To crate a new BinarySearchTree_s use bst_new(). After that you can insert
/// elements using bst_insert(). To remove elements you can use bst_remove() or
/// if you wish to remove the \c root element use bst_pop().
///
/// \par Functions
/// Located in the file BinarySearchTree.c
struct BinarySearchTree_s
{
    /// \brief Total elements int the tree.
    ///
    /// Total elements in the tree linked together by the root node.
    integer_t count;

    /// \brief Tree size limit.
    ///
    /// If it is set to 0 or a negative value then the tree has no limit to its
    /// size. Otherwise it won't be able to have more elements than the
    /// specified value. The tree is always initialized with no restrictions to
    /// its size, that is, \c limit equals 0. The user won't be able to limit
    /// the tree size if it already has more elements than the specified limit.
    integer_t limit;

    /// \brief The tree's root node.
    ///
    /// The root element of a binary search tree.
    struct BinarySearchTreeNode_s *root;

    /// \brief BinarySearchTree_s interface.
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

/// \brief A BinarySearchTree_s node.
///
/// Implementation detail. This node is a binary search tree node with a
/// <code> void * </code> key pointer that represents the element of that node
/// and three pointers: one to its right child, an element that is greater than
/// the current element; one to its left child, an element that is lesser than
/// the current element; and one to its parent, an element that was inserted
/// before this one. They all can be NULL. When parent is NULL then this node
/// is the root node; when right or left are NULL, it means that there are no
/// sub-trees following that branch.
struct BinarySearchTreeNode_s
{
    /// \brief Node's data.
    ///
    /// A pointer to the node's data.
    void *key;

    /// \brief A pointer to its right child.
    ///
    /// A pointer to its right child where its element is greater than the
    /// current node or NULL if there is no right subtree or if this node is
    /// a leaf.
    struct BinarySearchTreeNode_s *right;

    /// \brief A pointer to its left child.
    ///
    /// A pointer to its left child where its element is smaller than the
    /// current node or NULL if there is no left subtree or if this node is
    /// a leaf.
    struct BinarySearchTreeNode_s *left;

    /// \brief Pointer to parent node.
    ///
    /// Pointer to parent node or NULL if this is the root node.
    struct BinarySearchTreeNode_s *parent;
};

/// \brief A type for a binary search tree node.
///
/// Defines a type to a <code> struct BinarySearchTreeNode_s </code>.
typedef struct BinarySearchTreeNode_s BinarySearchTreeNode_t;

/// \brief A pointer type for a binary search tree node.
///
/// Defines a pointer type to a <code> struct BinarySearchTreeNode_s </code>.
typedef struct BinarySearchTreeNode_s *BinarySearchTreeNode;

///////////////////////////////////////////////////// NOT EXPOSED FUNCTIONS ///

static BinarySearchTreeNode_t *
bst_new_node(void *element);

static void
bst_free_node(BinarySearchTreeNode_t *node, free_f function);

static void
bst_free_node_shallow(BinarySearchTreeNode_t *node);

static void
bst_free_tree(BinarySearchTreeNode_t *root, free_f function);

static void
bst_free_tree_shallow(BinarySearchTreeNode_t *root);

BinarySearchTreeNode_t *
bst_node_find(BinarySearchTree_t *tree, void *element);

int
bst_node_height(BinarySearchTreeNode_t *root);

// Displaying modes
static void
bst_display_tree(BinarySearchTreeNode_t *root, integer_t height,
                 display_f function);

static void
bst_display_height(BinarySearchTreeNode_t *root, integer_t height,
                   display_f function);

static void
bst_display_simple(BinarySearchTreeNode_t *root, integer_t height,
                   display_f function);

static void
bst_display_treeview(BinarySearchTreeNode_t *root, integer_t depth, char *path,
                     display_f function, bool direction);

// Traversal
static void
bst_traversal_preorder(BinarySearchTreeNode_t *root, display_f function);

static void
bst_traversal_inorder(BinarySearchTreeNode_t *root, display_f function);

static void
bst_traversal_postorder(BinarySearchTreeNode_t *root, display_f function);

static void
bst_traversal_leaves(BinarySearchTreeNode_t *root, display_f function);


////////////////////////////////////////////// END OF NOT EXPOSED FUNCTIONS ///

///
/// \param[in] interface
///
/// \return
BinarySearchTree_t *
bst_new(Interface_t *interface)
{
    BinarySearchTree_t *tree = malloc(sizeof(BinarySearchTree_t));

    if (!tree)
        return NULL;

    tree->count = 0;
    tree->limit = 0;
    tree->version_id = 0;
    tree->root = NULL;

    tree->interface = interface;

    return tree;
}

///
/// \param[in] tree
void
bst_free(BinarySearchTree_t *tree)
{
    bst_free_tree(tree->root, tree->interface->free);

    free(tree);
}

///
/// \param[in] tree
void
bst_free_shallow(BinarySearchTree_t *tree)
{
    bst_free_tree_shallow(tree->root);

    free(tree);
}

///
/// \param[in] tree
void
bst_erase(BinarySearchTree_t *tree)
{
    bst_free_tree(tree->root, tree->interface->free);

    tree->root = NULL;
    tree->count = 0;
}

///
/// \param[in] tree
void
bst_erase_shallow(BinarySearchTree_t *tree)
{
    bst_free_tree_shallow(tree->root);

    tree->root = NULL;
    tree->count = 0;
}

///
/// \param[in] tree
/// \param[in] new_interface
void
bst_config(BinarySearchTree_t *tree, Interface_t *new_interface)
{
    tree->interface = new_interface;
}

///
/// \param[in] tree
///
/// \return
integer_t
bst_count(BinarySearchTree_t *tree)
{
    return tree->count;
}

///
/// \param[in] tree
///
/// \return
integer_t
bst_limit(BinarySearchTree_t *tree)
{
    return tree->limit;
}

///
/// \param[in] tree
/// \param[in] limit
///
/// \return
bool
bst_set_limit(BinarySearchTree_t *tree, integer_t limit)
{
    if (tree->count > limit && limit > 0)
        return false;

    tree->limit = limit;

    return true;
}

/// Inserts the specified element into the tree.
///
/// \param[in] tree
/// \param[in] element
///
/// \return
bool
bst_insert(BinarySearchTree_t *tree, void *element)
{
    if (bst_full(tree))
        return false;

    if (bst_empty(tree))
    {
        tree->root = bst_new_node(element);

        if (!tree->root)
            return false;
    }
    else
    {
        BinarySearchTreeNode_t *scan = tree->root;
        BinarySearchTreeNode_t *parent = scan;

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

        if (tree->interface->compare(parent->key, element) < 0)
        {
            parent->right = bst_new_node(element);

            if (!parent->right)
                return false;

            parent->right->parent = parent;
        }
        else
        {
            parent->left = bst_new_node(element);

            if (!parent->left)
                return false;

            parent->left->parent = parent;
        }
    }

    tree->count++;
    tree->version_id++;

    return true;
}

/// Removes the specified element from the tree.
///
/// \param[in] tree
/// \param[in] element
///
/// \return
bool
bst_remove(BinarySearchTree_t *tree, void *element)
{
    BinarySearchTreeNode_t *temp = NULL, *node = bst_node_find(tree, element);

    if (node == NULL)
        return false;

    bool is_root = node->parent == NULL;

    // Deleting a leaf. No need to update parent pointers.
    if (node->left == NULL && node->right == NULL)
    {
        // Deleting last element
        if (is_root)
            tree->root = NULL;
        else
        {
            // This is the right leaf of a node
            if (node->parent->right == node)
                node->parent->right = NULL;
            // This is the left leaf of a node
            else
                node->parent->left = NULL;
        }

        bst_free_node(node, tree->interface->free);
    }
    // Only right subtree. Need to update right subtree parent pointer.
    else if (node->left == NULL)
    {
        // Short-circuit the right subtree
        if (is_root)
        {
            tree->root = node->right;

            tree->root->parent = NULL;
        }
        else
        {
            // Linking the subtree parent pointer
            node->right->parent = node->parent;

            // This is the right child of a node
            if (node->parent->right == node)
                node->parent->right = node->right;
            // This is the left child of a node
            else
                node->parent->left = node->right;
        }

        bst_free_node(node, tree->interface->free);
    }
    // Only left subtree. Need to update left subtree parent pointer.
    else if (node->right == NULL)
    {
        // Short-circuit the left subtree
        if (is_root)
        {
            tree->root = node->left;

            tree->root->parent = NULL;
        }
        else
        {
            // Linking the subtree parent pointer
            node->left->parent = node->parent;

            // This is the right child of a node
            if (node->parent->right == node)
                node->parent->right = node->left;
            // This is the left child of a node
            else
                node->parent->left = node->left;
        }

        bst_free_node(node, tree->interface->free);
    }
    // Node has left and right subtrees
    else
    {
        // Replace current value with successor's (temp) and then delete it.
        // Note that we don't care about is_root since we are only replacing
        // the node's contents.
        temp = node->right;

        // Finding successor node (temp)
        while (temp->left != NULL)
            temp = temp->left;

        // Storing key in a temporary value
        void *temp_key = temp->key;

        // Deleting temp
        // This node can not be an inner node so there are only three
        // options. Its a leaf, or it has either left or right subtrees
        // but not both.
        if (temp->left == NULL && temp->right == NULL)
        {
            // Can't be root

            // This is the right leaf of a node
            if (temp->parent->right == temp)
                temp->parent->right = NULL;
            // This is the left leaf of a node
            else
                temp->parent->left = NULL;
        }
        // Only right subtree. Need to update right subtree parent pointer.
        else if (temp->left == NULL)
        {
            // Can't be root

            // Linking the subtree parent pointer
            temp->right->parent = temp->parent;

            // This is the right child of a node
            if (temp->parent->right == temp)
                temp->parent->right = temp->right;
            // This is the left child of a node
            else
                temp->parent->left = temp->right;
        }
        // Only left subtree. Need to update left subtree parent pointer.
        else if (temp->right == NULL)
        {
            // Can't be root

            // Linking the subtree parent pointer
            temp->left->parent = temp->parent;

            // This is the right child of a node
            if (temp->parent->right == temp)
                temp->parent->right = temp->left;
            // This is the left child of a node
            else
                temp->parent->left = temp->left;
        }

        // Delete temp node
        bst_free_node_shallow(temp);
        tree->interface->free(node->key);


        // Finally switching values
        node->key = temp_key;
    }

    tree->count--;
    tree->version_id++;

    return true;
}

/// Removes and frees root element.
///
/// \param[in] tree
///
/// \return
bool
bst_pop(BinarySearchTree_t *tree)
{
    void *element = bst_peek(tree);

    if (!element)
        return false;

    return bst_remove(tree, element);
}

///
/// \param[in] tree
///
/// \return
bool
bst_empty(BinarySearchTree_t *tree)
{
    return tree->count == 0;
}

///
/// \param[in] tree
///
/// \return
bool
bst_full(BinarySearchTree_t *tree)
{
    return tree->limit > 0 && tree->count >= tree->limit;
}

///
/// \param[in] tree
/// \param[in] element
///
/// \return
bool
bst_contains(BinarySearchTree_t *tree, void *element)
{
    BinarySearchTreeNode_t *node = bst_node_find(tree, element);

    return node == NULL ? false : true;
}

///
/// \param[in] tree
///
/// \return
void *
bst_peek(BinarySearchTree_t *tree)
{
    if (bst_empty(tree))
        return NULL;

    return tree->root->key;
}

///
/// \param[in] tree
///
/// \return
void *
bst_max(BinarySearchTree_t *tree)
{
    if (bst_empty(tree))
        return NULL;

    BinarySearchTreeNode_t *scan = tree->root;

    while (scan->right != NULL)
        scan = scan->right;

    return scan->key;
}

///
/// \param[in] tree
///
/// \return
void *
bst_min(BinarySearchTree_t *tree)
{
    if (bst_empty(tree))
        return NULL;

    BinarySearchTreeNode_t *scan = tree->root;

    while (scan->left != NULL)
        scan = scan->left;

    return scan->key;
}

///
/// \param[in] tree
/// \param[in] display_mode
void
bst_display(BinarySearchTree_t *tree, int display_mode)
{
    if (display_mode)
    {
        printf("\n+--------------------------------------------------+");
        printf("\n|                Binary Search Tree                |");
        printf("\n+--------------------------------------------------+\n");
    }

    if (bst_empty(tree) && display_mode)
    {
        printf(" EMPTY\n");
    }

    if (display_mode == -1)
    {
        bst_display_tree(tree->root, 0, tree->interface->display);
    }
    else if (display_mode == 0)
    {
        bst_display_simple(tree->root, 0, tree->interface->display);
    }
    else if (display_mode == 1)
    {
        bst_display_height(tree->root, 1, tree->interface->display);
    }
    else
    {
        char path[5000] = {};

        bst_display_treeview(tree->root, 0, path, tree->interface->display, false);
        printf("\n");
    }
}

///
/// \param[in] tree
/// \param[in] traversal_mode
void
bst_traversal(BinarySearchTree_t *tree, int traversal_mode)
{
    switch (traversal_mode)
    {
        case -1:
            printf("Pre-order Traversal\n");
            bst_traversal_preorder(tree->root, tree->interface->display);
            break;
        case 0:
            printf("In-order Traversal\n");
            bst_traversal_inorder(tree->root, tree->interface->display);
            break;
        case 1:
            printf("Post-order Traversal\n");
            bst_traversal_postorder(tree->root, tree->interface->display);
            break;
        default:
            printf("Leaves Traversal\n");
            bst_traversal_leaves(tree->root, tree->interface->display);
            break;
    }

    printf("\n");
}

///////////////////////////////////////////////////// NOT EXPOSED FUNCTIONS ///

static BinarySearchTreeNode_t *
bst_new_node(void *element)
{
    BinarySearchTreeNode_t * node = malloc(sizeof(BinarySearchTreeNode_t));

    if (!node)
        return NULL;

    node->key = element;

    node->parent = NULL;
    node->left = NULL;
    node->right = NULL;

    return node;
}

static void
bst_free_node(BinarySearchTreeNode_t *node, free_f function)
{
    function(node->key);

    free(node);
}

static void
bst_free_node_shallow(BinarySearchTreeNode_t *node)
{
    free(node);
}

static void
bst_free_tree(BinarySearchTreeNode_t *root, free_f function)
{
    BinarySearchTreeNode_t *scan = root;
    BinarySearchTreeNode_t *up = NULL;

    while (scan != NULL)
    {
        if (scan->left != NULL)
        {
            BinarySearchTreeNode_t *left = scan->left;

            scan->left = up;
            up = scan;
            scan = left;
        }
        else if (scan->right != NULL)
        {
            BinarySearchTreeNode_t *right = scan->right;

            scan->left = up;
            scan->right = NULL;
            up = scan;
            scan = right;
        }
        else
        {
            if (up == NULL)
            {
                bst_free_node(scan, function);
                scan = NULL;
            }

            while (up != NULL)
            {
                bst_free_node(scan, function);

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
bst_free_tree_shallow(BinarySearchTreeNode_t *root)
{
    BinarySearchTreeNode_t *scan = root;
    BinarySearchTreeNode_t *up = NULL;

    while (scan != NULL)
    {
        if (scan->left != NULL)
        {
            BinarySearchTreeNode_t *left = scan->left;

            scan->left = up;
            up = scan;
            scan = left;
        }
        else if (scan->right != NULL)
        {
            BinarySearchTreeNode_t *right = scan->right;

            scan->left = up;
            scan->right = NULL;
            up = scan;
            scan = right;
        }
        else
        {
            if (up == NULL)
            {
                bst_free_node_shallow(scan);
                scan = NULL;
            }

            while (up != NULL)
            {
                bst_free_node_shallow(scan);

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

BinarySearchTreeNode_t *
bst_node_find(BinarySearchTree_t *tree, void *element)
{
    BinarySearchTreeNode_t *scan = tree->root;

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

int
bst_node_height(BinarySearchTreeNode_t *root)
{
    if (root == NULL)
        return 0;

    int r_height = bst_node_height(root->right);

    int l_height = bst_node_height(root->left);

    return (l_height > r_height) ? l_height + 1 : r_height + 1;
}

static void
bst_display_tree(BinarySearchTreeNode_t *root, integer_t height,
                 display_f function)
{
    if (root == NULL)
        return;

    bst_display_tree(root->right, height + 1, function);

    for (integer_t i = 0; i < height; i++)
        printf("|------- ");

    function(root->key);

    printf("\n");

    bst_display_tree(root->left, height + 1, function);
}

static void
bst_display_height(BinarySearchTreeNode_t *root, integer_t height,
                   display_f function)
{
    if (root == NULL)
        return;

    bst_display_height(root->right, height + 1, function);

    for (integer_t i = 0; i < height - 1; i++)
        printf("        ");

    printf("|--%d---< ", bst_node_height(root));

    function(root->key);
    printf("\n");

    bst_display_height(root->left, height + 1, function);
}

static void
bst_display_simple(BinarySearchTreeNode_t *root, integer_t height,
                   display_f function)
{
    if (root == NULL)
        return;

    bst_display_simple(root->right, height + 1, function);

    for (integer_t i = 0; i < height; i++)
        printf("        ");

    function(root->key);
    printf("\n");

    bst_display_simple(root->left, height + 1, function);
}

static void
bst_display_treeview(BinarySearchTreeNode_t *root, integer_t depth, char *path,
                     display_f function, bool direction)
{
    const integer_t spaces = 8;

    if (root == NULL)
        return;

    depth++;

    bst_display_treeview(root->right, depth, path, function, true);

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

    bst_display_treeview(root->left, depth, path, function, false);
}

static void
bst_traversal_preorder(BinarySearchTreeNode_t *root, display_f function)
{
    if (root == NULL)
        return;

    function(root->key);
    printf(" ");

    bst_traversal_preorder(root->left, function);

    bst_traversal_preorder(root->right, function);
}

static void
bst_traversal_inorder(BinarySearchTreeNode_t *root, display_f function)
{
    if (root == NULL)
        return;

    bst_traversal_inorder(root->left, function);

    function(root->key);
    printf(" ");

    bst_traversal_inorder(root->right, function);
}

static void
bst_traversal_postorder(BinarySearchTreeNode_t *root, display_f function)
{
    if (root == NULL)
        return;

    bst_traversal_postorder(root->left, function);

    bst_traversal_postorder(root->right, function);

    function(root->key);
    printf(" ");
}

static void
bst_traversal_leaves(BinarySearchTreeNode_t *root, display_f function)
{
    if (root == NULL)
        return;

    bst_traversal_leaves(root->left, function);

    bst_traversal_leaves(root->right, function);

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

/// \todo BinarySearchTreeIterator

///////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////// Wrapper ///
///////////////////////////////////////////////////////////////////////////////

/// \todo BinarySearchTreeWrapper
