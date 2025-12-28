#include "types.h"

inline Level* find_min_node(Level* root)
{
    while (root->left != nullptr)
    {
        root = root->left;
    }
    return root;
}

inline Level* find_max_node(Level* root)
{
    while (root->right != nullptr)
    {
        root = root->right;
    }
    return root;
}

inline void insert_to_bst(Level** root, Level* node)
{
    if (*root == nullptr)
    {
        *root = node;
        return;
    }

    auto current = *root;
    while (current != nullptr)
    {
        if (node->key < current->key)
        {
            if (current->left == nullptr)
            {
                current->left = node;
                node->parent = current;
                return;
            }
            current = current->left;
        }
        else
        {
            if (current->right == nullptr)
            {
                current->right = node;
                node->parent = current;
                return;
            }
            current = current->right;
        }
    }
}

inline void remove_from_bst(Level** root, Level* node)
{

    bool no_left = node->left == nullptr;
    bool no_right = node->right == nullptr;

    /*
     *           A     |     F
     *          / \    |    / \
     *         B   C   |  null null
     */
    if (no_left && no_right)
    {
        if (*root == node) // F
        {
            *root = nullptr;
        }
        else if (node->parent->left == node) // B
        {
            node->parent->left = nullptr;
        }
        else // C
        {
            node->parent->right = nullptr;
        }
    }
    /*
     *           A     |
     *          / \    |
     *         B null  |
     */
    else if (no_right)
    {
        node->left->parent = node->parent;

        if (*root == node)
        {
            *root = node->left;
        }
        else if (node->parent->left == node)
        {
            node->parent->left = node->left;
        }
        else
        {
            node->parent->right = node->left;
        }
    }
    /*
     *           A     |
     *          / \    |
     *       null  B   |
     */
    else if (no_left)
    {
        node->right->parent = node->parent;
        if (*root == node)
        {
            *root = node->right;
        }
        else if (node->parent->left == node)
        {
            node->parent->left = node->right;
        }
        else
        {
            node->parent->right = node->right;
        }
    }
    /*
     *           A     |
     *          / \    |
     *         A  B    |
     */
    else
    {
        if (node->right->left == nullptr)
        {
            node->right->parent = node->parent;
            node->right->left = node->left;
            node->left->parent = node->right;
            if (*root == node)
            {
                *root = node->right;
            }
            else if (node->parent->left == node)
            {
                node->parent->left = node->right;
            }
            else
            {
                node->parent->right = node->right;
            }
        }
        else
        {
            auto min_node = find_min_node(node->right);
            if (min_node->right != nullptr)
            {
                min_node->parent->left = min_node->right;
                min_node->right->parent = min_node->parent;
            }
            else if (min_node != node->right)
            {
                min_node->parent->left = nullptr;
            }
            min_node->parent = node->parent;
            min_node->left = node->left;
            min_node->left->parent = min_node;
            min_node->right = node->right;
            min_node->right->parent = min_node;
            if (*root == node)
            {
                *root = min_node;
            }
            else if (node->parent->left == node)
            {
                node->parent->left = min_node;
            }
            else
            {
                node->parent->right = min_node;
            }
        }
    }
}
