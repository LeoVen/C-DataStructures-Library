/**
 * @file BinarySearchTree.c
 * 
 * @author Leonardo Vencovsky (https://github.com/LeoVen)
 *
 * @date 31/10/2018
 */

#include "BinarySearchTree.h"

// NOT EXPOSED API

/// Node for the BinarySearchTree implementation.
typedef struct BinarySearchTreeNode_s
{
    int key;                               /*!< Node's key */
    unsigned count;                        /*!< Amount of repeated keys */
    struct BinarySearchTreeNode_s *right;  /*!< Pointer to right child */
    struct BinarySearchTreeNode_s *left;   /*!< Pointer to left child */
    struct BinarySearchTreeNode_s *parent; /*!< Pointer to parent Node */
} BinarySearchTreeNode_t, *BinarySearchTreeNode;

Status bst_make_node(BinarySearchTreeNode *node, int value);

Status bst_delete_recursive(BinarySearchTreeNode *node);

index_t bst_node_height(BinarySearchTreeNode node);

index_t bst_node_depth(BinarySearchTreeNode node);

BinarySearchTreeNode bst_node_find(BinarySearchTreeNode root, int value);

Status bst_display_raw(BinarySearchTreeNode node, index_t spaces);

Status bst_display_interactive(BinarySearchTreeNode node, index_t spaces);

Status bst_display_clean(BinarySearchTreeNode node, index_t spaces);

Status bst_traversal_preorder(BinarySearchTreeNode node);

Status bst_traversal_inorder(BinarySearchTreeNode node);

Status bst_traversal_postorder(BinarySearchTreeNode node);

Status bst_traversal_leaves(BinarySearchTreeNode node);

// END OF NOT EXPOSED API

Status bst_init(BinarySearchTree *bst)
{
    (*bst) = malloc(sizeof(BinarySearchTree_t));

    if (!(*bst))
        return DS_ERR_ALLOC;

    (*bst)->root = NULL;

    (*bst)->count = 0;

    return DS_OK;
}

Status bst_insert(BinarySearchTree bst, int value)
{
    if (bst == NULL)
        return DS_ERR_NULL_POINTER;

    BinarySearchTreeNode scan = bst->root;
    BinarySearchTreeNode before = NULL;

    while (scan != NULL)
    {
        before = scan;

        if (scan->key > value)
            scan = scan->left;
        else if (scan->key < value)
            scan = scan->right;
        else
        {
            (bst->count)++;

            (scan->count)++;

            return DS_OK;
        }
    }

    BinarySearchTreeNode node;

    Status st = bst_make_node(&node, value);

    if (st != DS_OK)
        return st;

    if (before == NULL)
    {
        node->parent = NULL;

        bst->root = node;

        (bst->count)++;

        return DS_OK;
    }

    node->parent = before;

    if (before->key < value)
        before->right = node;
    else
        before->left = node;

    (bst->count)++;

    return DS_OK;
}

Status bst_remove(BinarySearchTree bst, int element)
{
    if (bst == NULL)
        return DS_ERR_NULL_POINTER;

    if (bst_empty(bst))
        return DS_ERR_INVALID_OPERATION;

    BinarySearchTreeNode temp, node = bst_node_find(bst->root, element);

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
                bst->root = NULL;
            else
            {
                // This is the right leaf of a node
                if (node->parent->right == node)
                    node->parent->right = NULL;
                // This is the left leaf of a node
                else
                    node->parent->left = NULL;
            }

            free(node);
        }
        // Only right subtree. Need to update right subtree parent pointer.
        else if (node->left == NULL)
        {
            // Short-circuit the right subtree
            if (is_root)
            {
                bst->root = node->right;

                bst->root->parent = NULL;
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

            free(node);
        }
        // Only left subtree. Need to update left subtree parent pointer.
        else if (node->right == NULL)
        {
            // Short-circuit the left subtree
            if (is_root)
            {
                bst->root = node->left;

                bst->root->parent = NULL;
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

            free(node);
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
            unsigned temp_count = temp->count;

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

                free(temp);
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

                free(temp);
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

                free(temp);
            }
            // Undefined behaviour
            else
                return DS_ERR_UNEXPECTED_RESULT;

            // Finally switching values
            node->key = temp_key;
            node->count = temp_count;
        }
    }

    (bst->count)--;

    return DS_OK;
}

Status bst_remove_all(BinarySearchTree bst, int element, unsigned *total)
{
    if (bst == NULL)
        return DS_ERR_NULL_POINTER;

    if (bst_empty(bst))
        return DS_ERR_INVALID_OPERATION;

    BinarySearchTreeNode temp, node = bst_node_find(bst->root, element);

    if (node == NULL)
        return DS_ERR_NOT_FOUND;

    *total = node->count;

    Status st;

    bool is_root = node->parent == NULL;

    if (node->left == NULL && node->right == NULL)
    {
        if (is_root)
            bst->root = NULL;
        else
        {
            if (node->parent->right == node)
                node->parent->right = NULL;
            else
                node->parent->left = NULL;
        }

        free(node);
    }
    else if (node->left == NULL)
    {
        if (is_root)
        {
            bst->root = node->right;

            bst->root->parent = NULL;
        }
        else
        {
            node->right->parent = node->parent;

            if (node->parent->right == node)
                node->parent->right = node->right;
            else
                node->parent->left = node->right;
        }

        free(node);
    }
    else if (node->right == NULL)
    {
        if (is_root)
        {
            bst->root = node->left;

            bst->root->parent = NULL;
        }
        else
        {
            node->left->parent = node->parent;

            if (node->parent->right == node)
                node->parent->right = node->left;
            else
                node->parent->left = node->left;
        }

        free(node);
    }
    else
    {
        temp = node->right;

        while (temp->left != NULL)
            temp = temp->left;

        int temp_key = temp->key;
        unsigned temp_count = temp->count;

        if (temp->left == NULL && temp->right == NULL)
        {
            if (temp->parent->right == temp)
                temp->parent->right = NULL;
            else
                temp->parent->left = NULL;

            free(temp);
        }
        else if (temp->left == NULL)
        {
            temp->right->parent = temp->parent;

            if (temp->parent->right == temp)
                temp->parent->right = temp->right;
            else
                temp->parent->left = temp->right;

            free(temp);
        }
        else if (temp->right == NULL)
        {
            temp->left->parent = temp->parent;

            if (temp->parent->right == temp)
                temp->parent->right = temp->left;
            else
                temp->parent->left = temp->left;

            free(temp);
        }
        else
            return DS_ERR_UNEXPECTED_RESULT;

        node->key = temp_key;
        node->count = temp_count;
    }

    (bst->count)--;

    return DS_OK;
}

Status bst_pop(BinarySearchTree bst, int *result)
{
    if (bst == NULL)
        return DS_ERR_NULL_POINTER;

    if (bst_empty(bst))
        return DS_ERR_INVALID_OPERATION;

    *result = bst_peek(bst);

    Status st = bst_remove(bst, *result);

    if (st != DS_OK)
        return st;

    return DS_OK;
}

Status bst_display(BinarySearchTree bst, int display)
{
    if (bst == NULL)
        return DS_ERR_NULL_POINTER;

    if (bst_empty(bst))
        return DS_ERR_INVALID_OPERATION;

    Status st;

    if (display)
    {
        printf("\n+----------------------+");
        printf("\n|  Binary Search Tree  |");
        printf("\n+----------------------+");
    }

    switch (display)
    {
        case 0:
            printf("\n\n");
            st = bst_display_raw(bst->root, 0);
            break;
        case 1:
            printf("\n<PARENT(DATA)[D-DEPTH|H-HEIGHT]\n\n");
            st = bst_display_interactive(bst->root, 0);
            break;
        default:
            printf("\n<PARENT(DATA)[D-DEPTH|H-HEIGHT]\n\n");
            st = bst_display_clean(bst->root, 0);
            break;
    }

    printf("\n");

    if (st != DS_OK)
        return st;

    return DS_OK;
}

Status bst_display_raw(BinarySearchTreeNode node, index_t spaces)
{
    if (node == NULL)
        return DS_OK;

    bst_display_raw(node->right, spaces + 1);

    for (index_t i = 0; i < spaces * 6; i++)
        printf(" ");

    printf("%d\n", node->key);

    bst_display_raw(node->left, spaces + 1);

    return DS_OK;
}

Status bst_display_interactive(BinarySearchTreeNode node, index_t spaces)
{
    if (node == NULL)
        return DS_OK;

    bst_display_interactive(node->right, spaces + 1);

    for (index_t i = 0; i < spaces; i++)
        printf("|-------");

    printf("<%d(%d)[D-%llu|H-%llu]\n", (node->parent == NULL) ? 0 : node->parent->key,
            node->key, bst_node_depth(node), bst_node_height(node) - 1);

    bst_display_interactive(node->left, spaces + 1);

    return DS_OK;
}

Status bst_display_clean(BinarySearchTreeNode node, index_t spaces)
{
    if (node == NULL)
        return DS_OK;

    bst_display_clean(node->right, spaces + 1);

    for (index_t i = 0; i < spaces; i++)
        printf("|       ");

    if (node->parent != NULL)
        printf("< %d ( %d )\n", node->parent->key, node->key);
    else
        printf("< %d ( %d )\n", 0, node->key);

    bst_display_clean(node->left, spaces + 1);

    return DS_OK;
}

Status bst_delete(BinarySearchTree *bst)
{
    if (*bst == NULL)
        return DS_ERR_NULL_POINTER;

    Status st = bst_delete_recursive(&((*bst)->root));

    if (st != DS_OK)
        return st;

    free(*bst);

    *bst = NULL;

    return DS_OK;
}

Status bst_erase(BinarySearchTree *bst)
{
    if ((*bst) == NULL)
        return DS_ERR_NULL_POINTER;

    Status st = bst_delete(bst);

    if (st != DS_OK)
        return st;

    st = bst_init(bst);

    if (st != DS_OK)
        return st;

    return DS_OK;
}

bool bst_empty(BinarySearchTree bst)
{
    return bst->count == 0;
}

bool bst_contains(BinarySearchTree bst, int value)
{
    if (bst == NULL)
        return false;

    if (bst_empty(bst))
        return false;

    BinarySearchTreeNode node = bst->root;

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

int bst_max(BinarySearchTree bst)
{
    if (bst == NULL)
        return 0;

    if (bst_empty(bst))
        return 0;

    BinarySearchTreeNode scan = bst->root;

    while (scan->right != NULL)
        scan = scan->right;

    return scan->key;
}

int bst_min(BinarySearchTree bst)
{
    if (bst == NULL)
        return 0;

    if (bst_empty(bst))
        return 0;

    BinarySearchTreeNode scan = bst->root;

    while (scan->left != NULL)
        scan = scan->left;

    return scan->key;
}

int bst_peek(BinarySearchTree bst)
{
    if (bst == NULL)
        return 0;

    if (bst_empty(bst))
        return 0;

    return bst->root->key;
}

index_t bst_height(BinarySearchTree bst)
{
    if (bst == NULL)
        return -1;

    return bst_node_height(bst->root);
}

Status bst_traversal(BinarySearchTree bst, int traversal)
{
    if (bst == NULL)
        return DS_ERR_NULL_POINTER;

    if (bst_empty(bst))
        return DS_ERR_INVALID_OPERATION;

    Status st;

    switch (traversal)
    {
        case -1:
            printf("\nPreorder Traversal\n");
            st = bst_traversal_preorder(bst->root);
            break;
        case 0:
            printf("\nInorder Traversal\n");
            st = bst_traversal_inorder(bst->root);
            break;
        case 1:
            printf("\nPostorder Traversal\n");
            st = bst_traversal_postorder(bst->root);
            break;
        default:
            printf("\nLeaves Traversal\n");
            st = bst_traversal_leaves(bst->root);
            break;
    }

    printf("\n");

    if (st != DS_OK)
        return st;

    return DS_OK;
}

// NOT EXPOSED API

Status bst_delete_recursive(BinarySearchTreeNode *node)
{
    if (*node == NULL)
        return DS_OK;

    bst_delete_recursive(&((*node)->right));

    bst_delete_recursive(&((*node)->left));

    free((*node));

    return DS_OK;
}

Status bst_make_node(BinarySearchTreeNode *node, int value)
{
    (*node) = malloc(sizeof(BinarySearchTreeNode_t));

    if (!(*node))
        return DS_ERR_ALLOC;

    (*node)->key = value;
    (*node)->count = 1;

    (*node)->left = NULL;
    (*node)->right = NULL;

    (*node)->parent = NULL;

    return DS_OK;
}

index_t bst_node_height(BinarySearchTreeNode node)
{
    if (node == NULL)
        return 0;

    index_t r_height = bst_node_height(node->right);

    index_t l_height = bst_node_height(node->left);

    return (l_height > r_height) ? l_height + 1 : r_height + 1;
}

index_t bst_node_depth(BinarySearchTreeNode node)
{
    if (node == NULL)
        return 0;

    BinarySearchTreeNode scan = node;

    index_t depth = 0;

    while (scan->parent != NULL)
    {
        scan = scan->parent;

        depth++;
    }

    return depth;
}

BinarySearchTreeNode bst_node_find(BinarySearchTreeNode root, int value)
{
    if (root == NULL)
        return NULL;

    BinarySearchTreeNode scan = root;

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

Status bst_traversal_preorder(BinarySearchTreeNode node)
{
    if (node == NULL)
        return DS_OK;

    printf(" %d", node->key);

    bst_traversal_preorder(node->left);

    bst_traversal_preorder(node->right);

    return DS_OK;
}

Status bst_traversal_inorder(BinarySearchTreeNode node)
{
    if (node == NULL)
        return DS_OK;

    bst_traversal_inorder(node->left);

    printf(" %d", node->key);

    bst_traversal_inorder(node->right);

    return DS_OK;
}

Status bst_traversal_postorder(BinarySearchTreeNode node)
{
    if (node == NULL)
        return DS_OK;

    bst_traversal_postorder(node->left);

    bst_traversal_postorder(node->right);

    printf(" %d", node->key);

    return DS_OK;
}

Status bst_traversal_leaves(BinarySearchTreeNode node)
{
    if (node->left != NULL)
    {
        bst_traversal_leaves(node->left);
    }
    if (node->right != NULL)
    {
        bst_traversal_leaves(node->right);
    }
    if (node->left == NULL && node->right == NULL)
    {
        printf(" %d", node->key);
    }

    return DS_OK;
}

// END OF NOT EXPOSED API
