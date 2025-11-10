
template<typename T> struct BSTNode
{
    T key;
    BSTNode *parent;
    BSTNode *left;
    BSTNode *right;

    BSTNode() : key(), parent(nullptr), left(nullptr), right(nullptr) {}

    explicit BSTNode(T _key) :
        key(_key), parent(nullptr), left(nullptr), right(nullptr)
    {
    }
};

template<typename T> void insert_to_bst(BSTNode<T> **root, BSTNode<T> *node)
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




