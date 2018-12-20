/**
 * @file AVLTree.c
 * 
 * @author Leonardo Vencovsky (https://github.com/LeoVen)
 *
 * @date 07/11/2018
 */

#include "AVLTree.h"

/// An AVLTree_s is a self-balancing binary search tree where the heights of
/// two child subtrees of any node differ by at most one. If at any time they
/// differ by more than one, rebalancing is done to restore this property using
/// left and right rotations that change the tree's structure while maintaining
/// the binary search tree property.
///
/// Lookup, insertion and deletion take <code> O(log N) </code> time in both
/// average and worst cases. The AVL tree is more strictly balanced
///
/// \par Advantages over BinarySearchTree_s
/// - The height of an AVLTree_s is always <code>log n</code> so all operations
/// like lookup, insert and remove are running on <code>O(log n)</code>.
///
/// \par Drawbacks
/// - Insertion and removal is slower because to maintain the AVL property many
/// rotations might be needed.
///
/// \par Functions
/// Located in the file AVLTree.c
struct AVLTree_s
{
    /// \brief Tree size.
    ///
    /// AVL tree's current amount of elements.
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
    /// The root element of an AVL tree.
    struct AVLTreeNode_s *root;

    /// \brief AVLTree_s interface.
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

/// \brief An AVLTree_s node.
///
/// Implementation detail. This node is like a binary search tree node but with
/// a height counter that is used to maintain the AVL property.
struct AVLTreeNode_s
{
    /// \brief Node's data.
    ///
    /// A pointer to the node's data.
    void *key;

    /// \brief Node's height.
    ///
    /// Used to maintain the AVL property.
    int8_t height;

    /// \brief A pointer to its right child.
    ///
    /// A pointer to its right child where its element is greater than the
    /// current node or NULL if there is no right subtree or if this node is
    /// a leaf.
    struct AVLTreeNode_s *right;

    /// \brief A pointer to its left child.
    ///
    /// A pointer to its left child where its element is smaller than the
    /// current node or NULL if there is no left subtree or if this node is
    /// a leaf.
    struct AVLTreeNode_s *left;

    /// \brief Pointer to parent node.
    ///
    /// Pointer to parent node or NULL if this is the root node.
    struct AVLTreeNode_s *parent;
};

/// \brief A type for an AVL tree node.
///
/// Defines a type to a <code> struct AVLTreeNode_s </code>.
typedef struct AVLTreeNode_s AVLTreeNode_t;

/// \brief A pointer type for an AVL tree node.
///
/// Defines a pointer type to a <code> struct AVLTreeNode_s </code>.
typedef struct AVLTreeNode_s *AVLTreeNode;

///////////////////////////////////////////////////// NOT EXPOSED FUNCTIONS ///

static AVLTreeNode_t *
avl_new_node(void *element);

static void
avl_free_node(AVLTreeNode_t *node, free_f function);

static void
avl_free_node_shallow(AVLTreeNode_t *node);

static void
avl_free_tree(AVLTreeNode_t *root, free_f function);

static void
avl_free_tree_shallow(AVLTreeNode_t *root);

static AVLTreeNode_t *
avl_find(AVLTree_t *tree, void *element);

// Rotations, re-balancing and other things to maintain the AVL properties
static int8_t
avl_node_height(AVLTreeNode_t *node);

static int8_t
avl_height_update(AVLTreeNode_t *node);

static void
avl_rotate_right(AVLTreeNode_t **Z);

static void
avl_rotate_left(AVLTreeNode_t **Z);

static void
avl_rebalance(AVLTree_t *tree, AVLTreeNode_t *node);

// Displaying modes
static void
avl_display_tree(AVLTreeNode_t *root, integer_t height, display_f function);

static void
avl_display_height(AVLTreeNode_t *root, integer_t height, display_f function);

static void
avl_display_simple(AVLTreeNode_t *root, integer_t height, display_f function);

static void
avl_display_treeview(AVLTreeNode_t *root, integer_t depth, char *path,
        display_f function, bool direction);

// Traversal
static void
avl_traversal_preorder(AVLTreeNode_t *root, display_f function);

static void
avl_traversal_inorder(AVLTreeNode_t *root, display_f function);

static void
avl_traversal_postorder(AVLTreeNode_t *root, display_f function);

static void
avl_traversal_leaves(AVLTreeNode_t *root, display_f function);

////////////////////////////////////////////// END OF NOT EXPOSED FUNCTIONS ///

/// Initializes a new AVLTree_s with \c size, \c limit, and \c version_id to 0,
/// \c root to NULL and sets its interface.
///
/// \par Interface Requirements
/// - None
///
/// \param interface An interface defining all necessary functions for the
/// AVL tree to operate.
///
/// \return A new AVLTree_s or NULL if allocation failed.
AVLTree_t *
avl_new(Interface_t *interface)
{
    AVLTree_t *tree = malloc(sizeof(AVLTree_t));

    if (!tree)
        return NULL;

    tree->size = 0;
    tree->limit = 0;
    tree->version_id = 0;
    tree->root = NULL;

    tree->interface = interface;

    return tree;
}

/// Frees an AVLTree_s, freeing all of its elements using the interface's free
/// function.
///
/// \par Interface Requirements
/// - free
///
/// \param tree The AVL tree to be freed from memory.
void
avl_free(AVLTree_t *tree)
{
    avl_free_tree(tree->root, tree->interface->free);

    free(tree);
}

/// Frees an AVLTree_s, freeing all of its nodes, leaving its elements intact.
///
/// \par Interface Requirements
/// - None
///
/// \param tree The AVL tree to be freed from memory.
void
avl_free_shallow(AVLTree_t *tree)
{
    avl_free_tree_shallow(tree->root);

    free(tree);
}

/// Frees an AVLTree_s, freeing all of its elements using the interface's free
/// function but leaves the tree's structure without a change.
///
/// \par Interface Requirements
/// - free
///
/// \param tree The AVL tree to have all of its elements freed from
/// memory.
void
avl_erase(AVLTree_t *tree)
{
    avl_free_tree(tree->root, tree->interface->free);

    tree->root = NULL;
    tree->size = 0;
}

/// Frees an AVLTree_s, freeing all of its nodes, leaving its elements intact.
/// The AVLTree_s structure is not changed.
///
/// \par Interface Requirements
/// - None
///
/// \param tree The AVL tree to have all of its nodes freed from memory.
void
avl_erase_shallow(AVLTree_t *tree)
{
    avl_free_tree_shallow(tree->root);

    tree->root = NULL;
    tree->size = 0;
}

/// Changes the AVL tree's interface.
///
/// \par Interface Requirements
/// - None
///
/// \param tree AVLTree_s reference.
/// \param new_interface A new interface for the specified AVL tree.
void
avl_config(AVLTree_t *tree, Interface_t *new_interface)
{
    tree->interface = new_interface;
}

/// Returns the amount of elements in the AVL tree.
///
/// \par Interface Requirements
/// - None
///
/// \param tree AVLTree_s reference.
///
/// \return The amount of elements in the AVL tree.
integer_t
avl_size(AVLTree_t *tree)
{
    return tree->size;
}

/// Returns the AVL tree's maximum number of elements defined by the user.
///
/// \par Interface Requirements
/// - None
///
/// \param tree AVLTree_s reference.
///
/// \return The AVL tree's maximum number of elements.
integer_t
avl_limit(AVLTree_t *tree)
{
    return tree->limit;
}

/// Sets a limit to the amount of elements in the AVL tree. To remove the limit
/// set.
///
/// \par Interface Requirements
/// - None
///
/// \param tree AVLTree_s reference.
/// \param limit The specified limit
///
/// \return false if the limit is less than the tree's current size and greater
/// than 0. Returns true if the limit was successfully set.
bool
avl_set_limit(AVLTree_t *tree, integer_t limit)
{
    if (tree->size > limit && limit > 0)
        return false;

    tree->limit = limit;

    return true;
}

/// Adds a new element in the specified AVL tree. The tree does not accepts
/// duplicate values.
///
/// \par Interface Requirements
/// - compare
///
/// \param tree AVLTree_s reference.
/// \param element The element to be added to the AVL tree.
///
/// \return True if the element was added to the tree.
/// \return False if the element is already present in the AVL tree, if the
/// tree has a limited size or if any allocations failed.
bool
avl_insert(AVLTree_t *tree, void *element)
{
    if (avl_full(tree))
        return false;

    if (avl_empty(tree))
    {
        tree->root = avl_new_node(element);

        if (!tree->root)
            return false;
    }
    else
    {
        AVLTreeNode scan = tree->root;
        AVLTreeNode parent = scan;

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

        AVLTreeNode_t *node;

        if (tree->interface->compare(parent->key, element) < 0)
        {
            parent->right = avl_new_node(element);

            if (!parent->right)
                return false;

            parent->right->parent = parent;
            node = parent->right;
        }
        else
        {
            parent->left = avl_new_node(element);

            if (!parent->left)
                return false;

            parent->left->parent = parent;
            node = parent->left;
        }

        avl_rebalance(tree, node);
    }

    tree->size++;
    tree->version_id++;

    return true;
}

/// Removes an element, if present, that matches a given element from the
/// specified AVL tree.
///
/// \par Interface Requirements
/// - compare
/// - free
///
/// \param tree AVLTree_s reference.
/// \param element The element to be removed has to match this element.
///
/// \return True if the element was removed.
/// \return False if the element was not found.
bool
avl_remove(AVLTree_t *tree, void *element)
{
    AVLTreeNode_t *temp = NULL,
                  *unbalanced = NULL,
                  *node = avl_find(tree, element);

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
            // Probably unbalanced node
            unbalanced = node->parent;

            // This is the right leaf of a node
            if (node->parent->right == node)
                node->parent->right = NULL;
            // This is the left leaf of a node
            else
                node->parent->left = NULL;
        }

        avl_free_node(node, tree->interface->free);
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
            // Probably unbalanced node
            unbalanced = node->parent;

            // Linking the subtree parent pointer
            node->right->parent = node->parent;

            // This is the right child of a node
            if (node->parent->right == node)
                node->parent->right = node->right;
            // This is the left child of a node
            else
                node->parent->left = node->right;
        }

        avl_free_node(node, tree->interface->free);
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
            // Probably unbalanced node
            unbalanced = node->parent;

            // Linking the subtree parent pointer
            node->left->parent = node->parent;

            // This is the right child of a node
            if (node->parent->right == node)
                node->parent->right = node->left;
            // This is the left child of a node
            else
                node->parent->left = node->left;
        }

        avl_free_node(node, tree->interface->free);
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

        // Unbalanced node will be passed into balance function
        unbalanced = temp->parent;

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
        avl_free_node_shallow(temp);

        tree->interface->free(node->key);

        // Finally switching values
        node->key = temp_key;
    }

    if (unbalanced != NULL)
        avl_rebalance(tree, unbalanced);

    tree->size--;
    tree->version_id++;

    return true;
}

/// Removes the root element and frees it from memory.
///
/// \par Interface Requirements
/// - compare
/// - free
///
/// \param tree AVLTree_s reference.
///
/// \return True if the element was removed.
/// \return False if the element was not found.
bool
avl_pop(AVLTree_t *tree)
{
    void *element = avl_peek(tree);

    if (!element)
        return false;

    return avl_remove(tree, element);
}

/// Checks if the specified AVL tree is empty based on its \c size counter.
///
/// \par Interface Requirements
/// - None
///
/// \param tree AVLTree_s reference.
///
/// \return True if \c size equals 0, otherwise false.
bool
avl_empty(AVLTree_t *tree)
{
    return tree->size == 0;
}

/// Checks if the specified AVL tree is full. The tree can only be full if the
/// limit is greater than 0 and the tree's size equals the limit.
///
/// \par Interface Requirements
/// - None
///
/// \param tree AVLTree_s reference.
///
/// \return True if \c limit is greater than 0 and the tree's size equals the
/// tree's limit, otherwise false.
bool
avl_full(AVLTree_t *tree)
{
    return tree->limit > 0 && tree->size >= tree->limit;
}

/// Checks if a given element is in the AVL tree.
///
/// \par Interface Requirements
/// - None
///
/// \param tree AVLTree_s reference.
/// \param element The element to be searched in the tree.
///
/// \return True if the the AVL tree contains the element, otherwise false.
bool
avl_contains(AVLTree_t *tree, void *element)
{
    AVLTreeNode_t *node = avl_find(tree, element);

    return node != NULL;
}

/// Returns the root element if present.
///
/// \par Interface Requirements
/// - None
///
/// \param tree AVLTree_s reference.
///
/// \return Returns the root element or NULL if the tree is empty.
void *
avl_peek(AVLTree_t *tree)
{
    if (avl_empty(tree))
        return NULL;

    return tree->root->key;
}

/// Returns the maximum element in the tree or NULL if it is empty.
///
/// \par Interface Requirements
/// - None
///
/// \param tree AVLTree_s reference.
///
/// \return The maximum element or NULL if the tree is empty.
void *
avl_max(AVLTree_t *tree)
{
    if (avl_empty(tree))
        return NULL;

    AVLTreeNode_t *scan = tree->root;

    while (scan->right != NULL)
        scan = scan->right;

    return scan->key;
}

/// Returns the minimum element in the tree or NULL if it is empty.
///
/// \par Interface Requirements
/// - None
///
/// \param tree AVLTree_s reference.
///
/// \return The minimum element or NULL if the tree is empty.
void *
avl_min(AVLTree_t *tree)
{
    if (avl_empty(tree))
        return NULL;

    AVLTreeNode_t *scan = tree->root;

    while (scan->left != NULL)
        scan = scan->left;

    return scan->key;
}

/// Displays an AVLTree_s in the console. There are currently four modes:
/// - -1 Displays the tree with \c avl_display_tree.
/// - 0 Displays the tree with \c avl_display_simple.
/// - 1 Displays the tree with \c avl_display_height
/// - Any other values defaults to \c avl_display_treeview
///
/// \par Interface Requirements
/// - display
///
///
/// \param tree AVLTree_s reference.
/// \param display_mode The way the tree is to be displayed.
void
avl_display(AVLTree_t *tree, int display_mode)
{
    if (display_mode)
    {
        printf("\n+--------------------------------------------------+");
        printf("\n|                     AVL Tree                     |");
        printf("\n+--------------------------------------------------+\n");
    }

    if (avl_empty(tree) && display_mode)
    {
        printf(" EMPTY\n");
    }

    if (display_mode == -1)
    {
        avl_display_tree(tree->root, 0, tree->interface->display);
    }
    else if (display_mode == 0)
    {
        avl_display_simple(tree->root, 0, tree->interface->display);
    }
    else if (display_mode == 1)
    {
        avl_display_height(tree->root, 1, tree->interface->display);
    }
    else
    {
        char path[5000] = {};

        avl_display_treeview(tree->root, 0, path, tree->interface->display,
                false);
        printf("\n");
    }
}

/// Displays an AVLTree_s in the console. There are currently four modes:
/// - -1 Pre-order traversal.
/// - 0 In-order traversal.
/// - 1 Post-order traversal.
/// - Any other values defaults to leaves traversal
///
/// \par Interface Requirements
/// - display
///
///
/// \param tree AVLTree_s reference.
/// \param traversal_mode The way the tree is to be traversed.
void
avl_traversal(AVLTree_t *tree, int traversal_mode)
{
    switch (traversal_mode)
    {
        case -1:
            printf("Pre-order Traversal\n");
            avl_traversal_preorder(tree->root, tree->interface->display);
            break;
        case 0:
            printf("In-order Traversal\n");
            avl_traversal_inorder(tree->root, tree->interface->display);
            break;
        case 1:
            printf("Post-order Traversal\n");
            avl_traversal_postorder(tree->root, tree->interface->display);
            break;
        default:
            printf("Leaves Traversal\n");
            avl_traversal_leaves(tree->root, tree->interface->display);
            break;
    }

    printf("\n");
}

///////////////////////////////////////////////////// NOT EXPOSED FUNCTIONS ///

static AVLTreeNode_t *
avl_new_node(void *element)
{
    AVLTreeNode_t *node = malloc(sizeof(AVLTreeNode_t));

    if (!node)
        return NULL;

    node->key = element;
    node->height = 0;

    node->left = NULL;
    node->right = NULL;
    node->parent = NULL;

    return node;
}

static void
avl_free_node(AVLTreeNode_t *node, free_f function)
{
    function(node->key);

    free(node);
}

static void
avl_free_node_shallow(AVLTreeNode_t *node)
{
    free(node);
}

static void
avl_free_tree(AVLTreeNode_t *root, free_f function)
{
    AVLTreeNode_t *scan = root;
    AVLTreeNode_t *up = NULL;

    while (scan != NULL)
    {
        if (scan->left != NULL)
        {
            AVLTreeNode_t *left = scan->left;

            scan->left = up;
            up = scan;
            scan = left;
        }
        else if (scan->right != NULL)
        {
            AVLTreeNode_t *right = scan->right;

            scan->left = up;
            scan->right = NULL;
            up = scan;
            scan = right;
        }
        else
        {
            if (up == NULL)
            {
                avl_free_node(scan, function);
                scan = NULL;
            }

            while (up != NULL)
            {
                avl_free_node(scan, function);

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
avl_free_tree_shallow(AVLTreeNode_t *root)
{
    AVLTreeNode_t *scan = root;
    AVLTreeNode_t *up = NULL;

    while (scan != NULL)
    {
        if (scan->left != NULL)
        {
            AVLTreeNode_t *left = scan->left;

            scan->left = up;
            up = scan;
            scan = left;
        }
        else if (scan->right != NULL)
        {
            AVLTreeNode_t *right = scan->right;

            scan->left = up;
            scan->right = NULL;
            up = scan;
            scan = right;
        }
        else
        {
            if (up == NULL)
            {
                avl_free_node_shallow(scan);
                scan = NULL;
            }

            while (up != NULL)
            {
                avl_free_node_shallow(scan);

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

static AVLTreeNode_t *
avl_find(AVLTree_t *tree, void *element)
{
    AVLTreeNode_t *scan = tree->root;

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

// Gets the current node's height
static int8_t
avl_node_height(AVLTreeNode_t *node)
{
    if (node == NULL)
        return 0;

    return node->height;
}

// Used to update the node's height
static int8_t
avl_height_update(AVLTreeNode_t *node)
{
    if (node == NULL)
        return 0;

    int8_t height_l = (node->left == NULL) ? (int8_t)0 : node->left->height;
    int8_t height_r = (node->right == NULL) ? (int8_t)0 : node->right->height;

    return (int8_t)1 + ((height_l >= height_r) ? height_l : height_r);
}

static void
avl_rotate_right(AVLTreeNode_t **Z)
{
    AVLTreeNode_t *root = *Z;
    AVLTreeNode_t *new_root = root->left;

    // First changing parent
    if (root->parent != NULL)
    {
        if (root->parent->left == root)
            root->parent->left = new_root;
        else
            root->parent->right = new_root;
    }

    new_root->parent = root->parent;

    root->parent = new_root;
    root->left = new_root->right;

    if (root->left)
        root->left->parent = root;

    new_root->right = root;

    // The order of these two updates matter
    root->height = avl_height_update(root);
    new_root->height = avl_height_update(new_root);

    // New root node
    *Z = new_root;
}

static void
avl_rotate_left(AVLTreeNode_t **Z)
{
    AVLTreeNode_t *root = *Z;
    AVLTreeNode_t *new_root = root->right;

    // First changing parent
    if (root->parent != NULL)
    {
        if (root->parent->right == root)
            root->parent->right = new_root;
        else
            root->parent->left = new_root;
    }

    new_root->parent = root->parent;

    root->parent = new_root;
    root->right = new_root->left;

    if (root->right)
        root->right->parent = root;

    new_root->left = root;

    // The order of these two updates matter
    root->height = avl_height_update(root);
    new_root->height = avl_height_update(new_root);

    // New root node
    *Z = new_root;
}

// Rebalances the AVLTree
static void
avl_rebalance(AVLTree_t *tree, AVLTreeNode_t *node)
{
    AVLTreeNode scan = node, child = NULL;

    int balance;

    bool is_root = false;

    while (scan != NULL)
    {
        // If we are changing the root then change avl->root = scan
        if (scan->parent == NULL)
            is_root = true;

        // Updates scan height
        scan->height = avl_height_update(scan);

        balance = avl_node_height(scan->right) - avl_node_height(scan->left);

        // Right skewed
        if (balance >= 2)
        {
            child = scan->right;

            // Right Left skewed
            if (avl_node_height(child->right) < avl_node_height(child->left))
            {
                avl_rotate_right(&(scan->right));
            }

            avl_rotate_left(&scan);
        }
        // Left skewed
        else if (balance <= -2)
        {
            child = scan->left;

            // Left Right skewed
            if (avl_node_height(child->left) < avl_node_height(child->right))
            {
                avl_rotate_left(&(scan->left));
            }

            avl_rotate_right(&scan);
        }

        if (is_root)
        {
            tree->root = scan;

            is_root = false;
        }

        // Goes up the tree until it is fully balanced
        scan = scan->parent;
    }

}

static void
avl_display_tree(AVLTreeNode_t *root, integer_t height, display_f function)
{
    if (root == NULL)
        return;

    avl_display_tree(root->right, height + 1, function);

    for (integer_t i = 0; i < height; i++)
        printf("|------- ");

    function(root->key);

    printf("\n");

    avl_display_tree(root->left, height + 1, function);
}

static void
avl_display_height(AVLTreeNode_t *root, integer_t height, display_f function)
{
    if (root == NULL)
        return;

    avl_display_height(root->right, height + 1, function);

    for (integer_t i = 0; i < height - 1; i++)
        printf("        ");

    printf("|---%d---< ", root->height);

    function(root->key);
    printf("\n");

    avl_display_height(root->left, height + 1, function);
}

static void
avl_display_simple(AVLTreeNode_t *root, integer_t height, display_f function)
{
    if (root == NULL)
        return;

    avl_display_simple(root->right, height + 1, function);

    for (integer_t i = 0; i < height; i++)
        printf("        ");

    function(root->key);
    printf("\n");

    avl_display_simple(root->left, height + 1, function);
}

static void
avl_display_treeview(AVLTreeNode_t *root, integer_t depth, char *path,
                     display_f function, bool direction)
{
    const integer_t spaces = 8;

    if (root == NULL)
        return;

    depth++;

    avl_display_treeview(root->right, depth, path, function, true);

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

    avl_display_treeview(root->left, depth, path, function, false);
}

static void
avl_traversal_preorder(AVLTreeNode_t *root, display_f function)
{
    if (root == NULL)
        return;

    function(root->key);
    printf(" ");

    avl_traversal_preorder(root->left, function);

    avl_traversal_preorder(root->right, function);
}

static void
avl_traversal_inorder(AVLTreeNode_t *root, display_f function)
{
    if (root == NULL)
        return;

    avl_traversal_inorder(root->left, function);

    function(root->key);
    printf(" ");

    avl_traversal_inorder(root->right, function);
}

static void
avl_traversal_postorder(AVLTreeNode_t *root, display_f function)
{
    if (root == NULL)
        return;

    avl_traversal_postorder(root->left, function);

    avl_traversal_postorder(root->right, function);

    function(root->key);
    printf(" ");
}

static void
avl_traversal_leaves(AVLTreeNode_t *root, display_f function)
{
    if (root == NULL)
        return;

    avl_traversal_leaves(root->left, function);

    avl_traversal_leaves(root->right, function);

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

/// \todo AVLTreeIterator

///////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////// Wrapper ///
///////////////////////////////////////////////////////////////////////////////

/// \todo AVLTreeWrapper
