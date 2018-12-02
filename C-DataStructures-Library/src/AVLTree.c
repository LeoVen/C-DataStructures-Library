/**
 * @file AVLTree.c
 * 
 * @author Leonardo Vencovsky (https://github.com/LeoVen)
 *
 * @date 07/11/2018
 */

#include "AVLTree.h"

// NOT EXPOSED API

/// An \c AVLTreeNode is what an \c AVLTree is composed of. It it much like
/// a \c BinarySearchTreeNode with one pointer to its left child, one to its
/// right child and another to its parent, being NULL if that Node is the
/// \c root node. It also has a height value to easily implement the AVL
/// tree property.
typedef struct AVLTreeNode
{
    int key;                    /*!< Node's key */
    int count;                  /*!< Amount of keys */
    int height;				    /*!< Node's Height */
    struct AVLTreeNode *right;  /*!< Pointer to right child */
    struct AVLTreeNode *left;   /*!< Pointer to left child */
    struct AVLTreeNode *parent; /*!< Pointer to parent Node */
} AVLTreeNode_t, *AVLTreeNode;

Status avl_make_node(AVLTreeNode *node, int value);

Status avl_delete_node(AVLTreeNode *node);

Status avl_delete_recursive(AVLTreeNode *node);

AVLTreeNode avl_node_find(AVLTreeNode root, int value);

int avl_depth(AVLTreeNode node);

Status avl_display_raw(AVLTreeNode node, integer_t spaces);

Status avl_display_interactive(AVLTreeNode node, integer_t spaces);

Status avl_display_clean(AVLTreeNode node, integer_t spaces);

Status avl_display_quantity(AVLTreeNode node, integer_t spaces);

Status avl_traversal_preorder(AVLTreeNode node);

Status avl_traversal_inorder(AVLTreeNode node);

Status avl_traversal_postorder(AVLTreeNode node);

Status avl_traversal_leaves(AVLTreeNode node);

int avl_height(AVLTreeNode node);

int avl_height_update(AVLTreeNode node);

Status avl_rotate_right(AVLTreeNode *node_z);

Status avl_rotate_left(AVLTreeNode *node_z);

Status avl_rebalance(AVLTree avl, AVLTreeNode node);

// END OF NOT EXPOSED API

Status avl_init(AVLTree *avl)
{
    (*avl) = malloc(sizeof(AVLTree_t));

    if (!(*avl))
        return DS_ERR_ALLOC;

    (*avl)->root = NULL;

    (*avl)->size = 0;

    return DS_OK;
}

Status avl_insert(AVLTree avl, int value)
{
    if (avl == NULL)
        return DS_ERR_NULL_POINTER;

    Status st;

    if (avl_empty(avl))
    {
        AVLTreeNode node;

        st = avl_make_node(&node, value);

        if (st != DS_OK)
            return st;

        avl->root = node;

        (avl->size)++;
    }
    else
    {
        AVLTreeNode scan = avl->root;
        AVLTreeNode before = NULL;

        while (scan != NULL)
        {
            before = scan;

            if (scan->key > value)
                scan = scan->left;
            else if (scan->key < value)
                scan = scan->right;
            else
            {
                (scan->count)++;

                (avl->size)++;

                return DS_OK;
            }
        }

        AVLTreeNode node;

        st = avl_make_node(&node, value);

        if (st != DS_OK)
            return st;

        node->parent = before;

        if (before->key < value)
            before->right = node;
        else
            before->left = node;

        st = avl_rebalance(avl, node);

        if (st != DS_OK)
            return st;

        (avl->size)++;
    }

    return DS_OK;
}

Status avl_remove(AVLTree avl, int value)
{
    if (avl == NULL)
        return DS_ERR_NULL_POINTER;

    if (avl_empty(avl))
        return DS_ERR_INVALID_OPERATION;

    AVLTreeNode temp, unbalanced = NULL, node = avl_node_find(avl->root, value);

    if (node == NULL)
        return DS_ERR_NOT_FOUND;

    Status st;

    bool is_root = node->parent == NULL;

    if (node->count > 1)
    {
        (node->count)--;
    }
    else
    {
        // Deleting a leaf. No need to update parent pointers.
        if (node->left == NULL && node->right == NULL)
        {
            // Deleting last element
            if (is_root)
                avl->root = NULL;
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

            st = avl_delete_node(&node);

            if (st != DS_OK)
                return st;
        }
        // Only right subtree. Need to update right subtree parent pointer.
        else if (node->left == NULL)
        {
            // Short-circuit the right subtree
            if (is_root)
            {
                avl->root = node->right;

                avl->root->parent = NULL;
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

            st = avl_delete_node(&node);

            if (st != DS_OK)
                return st;
        }
        // Only left subtree. Need to update left subtree parent pointer.
        else if (node->right == NULL)
        {
            // Short-circuit the left subtree
            if (is_root)
            {
                avl->root = node->left;

                avl->root->parent = NULL;
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

            st = avl_delete_node(&node);

            if (st != DS_OK)
                return st;
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
            int temp_key = temp->key;
            int temp_count = temp->count;

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
            // Undefined behaviour
            else
                return DS_ERR_UNEXPECTED_RESULT;

            // Delete temp node
            st = avl_delete_node(&temp);

            if (st != DS_OK)
                return st;

            // Finally switching values
            node->key = temp_key;
            node->count = temp_count;
        }
    }

    (avl->size)--;

    if (unbalanced != NULL)
    {
        st = avl_rebalance(avl, unbalanced);

        if (st != DS_OK)
            return st;
    }

    return DS_OK;
}

Status avl_pop(AVLTree avl, int *result)
{
    if (avl == NULL)
        return DS_ERR_NULL_POINTER;

    if (avl_empty(avl))
        return DS_ERR_INVALID_OPERATION;

    *result = avl_peek(avl);

    Status st = avl_remove(avl, *result);

    if (st != DS_OK)
        return st;

    return DS_OK;
}

Status avl_display(AVLTree avl, int display)
{
    if (avl == NULL)
        return DS_ERR_NULL_POINTER;

    if (avl_empty(avl))
        return DS_ERR_INVALID_OPERATION;

    Status st;

    if (display)
    {
        printf("\n+--------------------------------------------------+");
        printf("\n|                     AVL Tree                     |");
        printf("\n+--------------------------------------------------+\n");
    }

    switch (display)
    {
        case -1:
            printf("\n<PARENT(DATA)[D-DEPTH|H-HEIGHT]\n\n");
            st = avl_display_clean(avl->root, 0);
            break;
        case 0:
            printf("\n");
            st = avl_display_raw(avl->root, 0);
            break;
        case 1:
            printf("\n<PARENT(DATA)[D-DEPTH|H-HEIGHT]\n\n");
            st = avl_display_interactive(avl->root, 0);
            break;
        default:
            printf("\n<PARENT(DATA)[QUANTITY]\n\n");
            st = avl_display_quantity(avl->root, 0);
            break;
    }

    printf("\n");

    if (st != DS_OK)
        return st;

    return DS_OK;
}

Status avl_delete(AVLTree *avl)
{
    if (*avl == NULL)
        return DS_ERR_NULL_POINTER;

    Status st = avl_delete_recursive(&((*avl)->root));

    if (st != DS_OK)
        return st;

    free(*avl);

    *avl = NULL;

    return DS_OK;
}

Status avl_erase(AVLTree *avl)
{
    if ((*avl) == NULL)
        return DS_ERR_NULL_POINTER;

    Status st = avl_delete(avl);

    if (st != DS_OK)
        return st;

    st = avl_init(avl);

    if (st != DS_OK)
        return st;

    return DS_OK;
}

int avl_max(AVLTree avl)
{
    if (avl == NULL)
        return 0;

    if (avl_empty(avl))
        return 0;

    AVLTreeNode scan = avl->root;

    while (scan->right != NULL)
        scan = scan->right;

    return scan->key;
}

int avl_min(AVLTree avl)
{
    if (avl == NULL)
        return 0;

    if (avl_empty(avl))
        return 0;

    AVLTreeNode scan = avl->root;

    while (scan->left != NULL)
        scan = scan->left;

    return scan->key;
}

int avl_peek(AVLTree avl)
{
    if (avl == NULL)
        return 0;

    if (avl_empty(avl))
        return 0;

    return avl->root->key;
}

bool avl_contains(AVLTree avl, int value)
{
    if (avl == NULL)
        return false;

    if (avl_empty(avl))
        return false;

    AVLTreeNode node = avl->root;

    while (node != NULL)
    {
        if (node->key < value)
            node = node->right;
        else if (node->key > value)
            node = node->left;
        else
            return true;
    }

    return false;
}

bool avl_empty(AVLTree avl)
{
    return avl->size == 0;
}

Status avl_traversal(AVLTree avl, int traversal)
{
    if (avl == NULL)
        return DS_ERR_NULL_POINTER;

    if (avl_empty(avl))
        return DS_ERR_INVALID_OPERATION;

    Status st;

    switch (traversal)
    {
        case -1:
            printf("\nPreorder Traversal\n");
            st = avl_traversal_preorder(avl->root);
            break;
        case 0:
            printf("\nInorder Traversal\n");
            st = avl_traversal_inorder(avl->root);
            break;
        case 1:
            printf("\nPostorder Traversal\n");
            st = avl_traversal_postorder(avl->root);
            break;
        default:
            printf("\nLeaves Traversal\n");
            st = avl_traversal_leaves(avl->root);
            break;
    }

    printf("\n");

    if (st != DS_OK)
        return st;

    return DS_OK;
}

// NOT EXPOSED API

Status avl_make_node(AVLTreeNode *node, int value)
{
    (*node) = malloc(sizeof(AVLTreeNode_t));

    if (!(*node))
        return DS_ERR_ALLOC;

    (*node)->key = value;
    (*node)->count = 1;
    (*node)->height = 0;

    (*node)->left = NULL;
    (*node)->right = NULL;
    (*node)->parent = NULL;

    return DS_OK;
}

Status avl_delete_node(AVLTreeNode *node)
{
    if (*node == NULL)
        return DS_ERR_NULL_POINTER;

    free(*node);

    *node = NULL;

    return DS_OK;
}

Status avl_delete_recursive(AVLTreeNode *node)
{
    if (*node == NULL)
        return DS_OK;

    avl_delete_recursive(&((*node)->left));

    avl_delete_recursive(&((*node)->right));

    free((*node));

    return DS_OK;
}

AVLTreeNode avl_node_find(AVLTreeNode root, int value)
{
    if (root == NULL)
        return NULL;

    AVLTreeNode scan = root;

    while (scan != NULL)
    {
        if (scan->key < value)
            scan = scan->right;
        else if (scan->key > value)
            scan = scan->left;
        else
            return scan;
    }

    return NULL;
}

int avl_depth(AVLTreeNode node)
{
    if (node == NULL)
        return 0;

    AVLTreeNode scan = node;

    int depth = 0;

    while (scan->parent != NULL)
    {
        scan = scan->parent;

        depth++;
    }

    return depth;
}

Status avl_display_raw(AVLTreeNode node, integer_t spaces)
{
    if (node == NULL)
        return DS_OK;

    avl_display_raw(node->right, spaces + 1);

    for (integer_t i = 0; i < spaces * 6; i++)
        printf(" ");

    printf("%d\n", node->key);

    avl_display_raw(node->left, spaces + 1);

    return DS_OK;
}

Status avl_display_interactive(AVLTreeNode node, integer_t spaces)
{
    if (node == NULL)
        return DS_OK;

    avl_display_interactive(node->right, spaces + 1);

    for (integer_t i = 0; i < spaces; i++)
        printf("|-------");

    printf("<%d(%d)[D-%d|H-%d]\n", (node->parent) ? node->parent->key : 0,
            node->key, avl_depth(node), node->height);

    avl_display_interactive(node->left, spaces + 1);

    return DS_OK;
}

Status avl_display_clean(AVLTreeNode node, integer_t spaces)
{
    if (node == NULL)
        return DS_OK;

    avl_display_clean(node->right, spaces + 1);

    for (integer_t i = 0; i < spaces; i++)
        printf("|       ");

    printf("<%d(%d)[D-%d|H-%d]\n", (node->parent) ? node->parent->key : 0,
            node->key, avl_depth(node), node->height);

    avl_display_clean(node->left, spaces + 1);

    return DS_OK;
}

Status avl_display_quantity(AVLTreeNode node, integer_t spaces)
{
    if (node == NULL)
        return DS_OK;

    avl_display_quantity(node->right, spaces + 1);

    for (integer_t i = 0; i < spaces; i++)
        printf("|_______");

    printf("<%d(%d)[%d]\n", (node->parent) ? node->parent->key : 0,
           node->key, node->count);

    avl_display_quantity(node->left, spaces + 1);

    return DS_OK;
}

Status avl_traversal_preorder(AVLTreeNode node)
{
    if (node == NULL)
        return DS_OK;

    printf(" %d", node->key);

    avl_traversal_preorder(node->left);

    avl_traversal_preorder(node->right);

    return DS_OK;
}

Status avl_traversal_inorder(AVLTreeNode node)
{
    if (node == NULL)
        return DS_OK;

    avl_traversal_inorder(node->left);

    printf(" %d", node->key);

    avl_traversal_inorder(node->right);

    return DS_OK;
}

Status avl_traversal_postorder(AVLTreeNode node)
{
    if (node == NULL)
        return DS_OK;

    avl_traversal_postorder(node->left);

    avl_traversal_postorder(node->right);

    printf(" %d", node->key);

    return DS_OK;
}

Status avl_traversal_leaves(AVLTreeNode node)
{
    if (node->left != NULL)
    {
        avl_traversal_leaves(node->left);
    }
    if (node->right != NULL)
    {
        avl_traversal_leaves(node->right);
    }
    if (node->left == NULL && node->right == NULL)
    {
        printf(" %d", node->key);
    }

    return DS_OK;
}

int avl_height(AVLTreeNode node)
{
    if (node == NULL)
        return 0;

    return node->height;
}

int avl_height_update(AVLTreeNode node)
{
    if (node == NULL)
        return 0;

    int height_l = (node->left == NULL) ? 0 : node->left->height;
    int height_r = (node->right == NULL) ? 0 : node->right->height;

    return 1 + ((height_l >= height_r) ? height_l : height_r);
}

/// Right rotation
/// 
/// T1, T2, T3 and T4 are subtrees.
/// 
///        Z                                      Y
///       / \                                   /   \
///      Y   T4      Right Rotate (Z)          X      Z
///     / \          - - - - - - - - ->      /  \    /  \
///    X   T3                               T1  T2  T3  T4
///   / \
/// T1   T2
/// 
/// 
Status avl_rotate_right(AVLTreeNode *node_z)
{
    // X won't change
    AVLTreeNode node_zp = (*node_z)->parent; // Z parent
    AVLTreeNode node_y = (*node_z)->left;    // Y
    AVLTreeNode sub3 = node_y->right;        // T3

    // Rotate
    node_y->right = (*node_z);
    (*node_z)->left = sub3;

    // Modify parents
    (*node_z)->parent = node_y;
    node_y->parent = node_zp;

    if (sub3 != NULL)
        sub3->parent = (*node_z);

    // Update heights
    (*node_z)->height = avl_height_update(*node_z);

    node_y->height = avl_height_update(node_y);

    // Update parent of root
    if (node_zp != NULL)
    {
        if (node_zp->key > node_y->key)
            node_zp->left = node_y;
        else
            node_zp->right = node_y;

        // Update height
        node_zp->height = avl_height_update(node_zp);
    }

    // New root node
    *node_z = node_y;

    return DS_OK;
}

/// Left rotation
///
/// T1, T2, T3 and T4 are subtrees.
///
///   Z                                Y
///  /  \                            /   \
/// T1   Y     Left Rotate(Z)       Z      X
///     /  \   - - - - - - - ->    / \    / \
///    T2   X                     T1  T2 T3  T4
///        / \
///      T3  T4
///
Status avl_rotate_left(AVLTreeNode *node_z)
{
    // X won't change
    AVLTreeNode node_zp = (*node_z)->parent; // Z parent
    AVLTreeNode node_y = (*node_z)->right;   // Y
    AVLTreeNode sub2 = node_y->left;         // T2

    // Rotate
    node_y->left = (*node_z);
    (*node_z)->right = sub2;

    // Modify parents
    (*node_z)->parent = node_y;
    node_y->parent = node_zp;

    if (sub2 != NULL)
        sub2->parent = (*node_z);

    // Update heights
    (*node_z)->height = avl_height_update(*node_z);

    node_y->height = avl_height_update(node_y);

    // Update parent of root
    if (node_zp != NULL)
    {
        if (node_zp->key > node_y->key)
            node_zp->left = node_y;
        else
            node_zp->right = node_y;

        // Update height
        node_zp->height = avl_height_update(node_zp);
    }

    // New root node
    *node_z = node_y;

    return DS_OK;
}

// Rebalances the AVLTree
Status avl_rebalance(AVLTree avl, AVLTreeNode node)
{
    if (avl == NULL || node == NULL)
        return DS_ERR_NULL_POINTER;

    Status st;

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

        balance = avl_height(scan->right) - avl_height(scan->left);

        // Right skewed
        if (balance >= 2)
        {
            child = scan->right;

            // Right Left skewed
            if (avl_height(child->right) < avl_height(child->left))
            {
                st = avl_rotate_right(&child);

                if (st != DS_OK)
                    return st;
            }

            st = avl_rotate_left(&scan);

            if (st != DS_OK)
                return st;
        }
        // Left skewed
        else if (balance <= -2)
        {
            child = scan->left;

            // Left Right skewed
            if (avl_height(child->left) < avl_height(child->right))
            {
                st = avl_rotate_left(&child);

                if (st != DS_OK)
                    return st;
            }

            st = avl_rotate_right(&scan);

            if (st != DS_OK)
                return st;
        }

        if (is_root)
        {
            avl->root = scan;

            is_root = false;
        }

        // Goes up the tree until it is fully balanced
        scan = scan->parent;
    }

    return DS_OK;
}

// END OF NOT EXPOSED API