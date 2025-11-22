#include "types.h"
#include <unordered_map>
#include <iostream>

inline Level* find_min_node(Level* root){
    while(root->left != nullptr){
        root = root->left;
    }
    return root;
}

inline Level* find_max_node(Level* root){
    while(root->right != nullptr){
        root = root->right;
    }
    return root;
}

template<Side side> void set_best(Level **best, Level *level);
template<> inline void set_best<Side::Buy>(Level **best_buy, Level *level)
{
    if (*best_buy == nullptr)
    {
        *best_buy = level;
    }
    else if (level->key > (*best_buy)->key)
    {
        *best_buy = level;
    }
}
template<> inline void set_best<Side::Sell>(Level **best_sell, Level *level)
{
    if (*best_sell == nullptr)
    {
        *best_sell = level;
    }
    else if (level->key < (*best_sell)->key)
    {
        *best_sell = level;
    }
}

template<Side side> void find_best(Level **best);
template<> inline void find_best<Side::Buy>(Level **best_buy)
{
    if ((*best_buy)->parent == nullptr)
    {
        *best_buy = (*best_buy)->left;
    }
    else
    {
        *best_buy = (*best_buy)->parent;
    }

    if (*best_buy != nullptr)
    {
        *best_buy = find_max_node(*best_buy);
    }
}
template<> inline void find_best<Side::Sell>(Level **best_sell)
{
    if ((*best_sell)->parent == nullptr)
    {
        *best_sell = (*best_sell)->right;
    }
    else
    {
        *best_sell = (*best_sell)->parent;
    }

    if (*best_sell != nullptr)
    {
        *best_sell = find_min_node(*best_sell);
    }
}

template<Side side> bool can_match(uint64_t limit, uint64_t market);

template<> inline bool can_match<Side::Buy>(uint64_t limit, uint64_t market)
{
    return market == 0 || market <= limit;
}

template<> inline bool can_match<Side::Sell>(uint64_t limit, uint64_t market)
{
    return market == 0 || market >= limit;
}

template<Side side> class PriceLevelTree
{
    Level *root = nullptr;
    std::unordered_map<uint64_t, Level *> levels;
    uint64_t last_best_price = 0;
    uint32_t count = 0;

public:
    uint64_t volume = 0;
    Level *best = nullptr;
    void add_level(Order *order)
    {
        if (levels.count(order->limit) == 0)
        {
            order->level = new Level(order);

            insert_to_bst(&root, order->level);

            set_best<side>(&best, order->level);

            levels.emplace(order->level->key, order->level);
        }
        else
        {
            order->level = levels.at(order->limit);
            ++order->level->count;
            order->level->volume += order->shares;
            append_to_dll(&order->level->order_tail, order);
        }
        ++count;
        volume += order->shares;
        if (best != nullptr)
            last_best_price = best->key;
    };

    inline void append_to_dll(Order **tail, Order *node)
    {
        (*tail)->next = node;
        node->prev = *tail;
        *tail = node;
    }


    void insert_to_bst(Level **root, Level *node)
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

    void cancel_order(Order *order)
    {
        auto level = order->level;
        if (order->prev == nullptr && order->next == nullptr)
        {
            remove_from_bst(&root, level);

            if (best == level)
            {
                find_best<side>(&best);
            }

            levels.erase(level->key);
            delete level;
        }
        else
        {
            --level->count;
            level->volume -= order->shares;
            remove_from_dll(&level->order_head,
                           &level->order_tail,
                            (order));
        }

        --count;
        volume -= order->shares;
        if (best != nullptr)
        {
            last_best_price = best->key;
        }
    }

    inline void remove_from_dll(Order **head, Order **tail, Order *node)
    {
        if (node->prev != nullptr)
        {
            node->prev->next = node->next;
        }
        if (node->next != nullptr)
        {
            node->next->prev = node->prev;
        }
        if (*head == node)
        {
            *head = node->next;
        }
        if (*tail == node)
        {
            *tail = node->prev;
        }
        node->next = nullptr;
        node->prev = nullptr;
    }

    void remove_from_bst(Level **root, Level *node)
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

    void in_order_traversal(Level *node)
    {
        if (node == nullptr)
            return;

        in_order_traversal(node->left);
        std::cout << node->key << " ";
        in_order_traversal(node->right);
    }

    template<typename C> void market_order(Order *order, C is_fill)
    {
        while (best != nullptr && can_match<side>(best->key, order->limit))
        {
            auto match = best->order_head;
            if (match->shares >= order->shares)
            {
                if (match->shares == order->shares)
                {
                    cancel_order(match);
                    is_fill(match->id_number);
                }
                else
                {
                    match->shares -= order->shares;
                    match->level->volume -= order->shares;
                    volume -= order->shares;
                }
                order->shares = 0;
                return;
            }
            order->shares -= match->shares;
            cancel_order(match);
            is_fill(match->id_number);
        }
    }

    void display_tree()
    {
        in_order_traversal(root);
    }

    void info_best()
    {
        if (best == nullptr)
            return;
        std::cout << "price: " << best->key << " count: " << best->count
                  << " volume: " << best->volume << std::endl;
    }
};
