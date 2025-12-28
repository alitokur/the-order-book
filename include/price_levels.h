#include "types.h"
#include <unordered_map>
#include <iostream>
#include "pool.h"
#include "dll.h"
#include "bst.h"

template<Side side> void set_best(Level** best, Level* level);
template<> inline void set_best<Side::Buy>(Level** best_buy, Level* level)
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

template<> inline void set_best<Side::Sell>(Level** best_sell, Level* level)
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

template<Side side> void find_best(Level** best);

template<> inline void find_best<Side::Buy>(Level** best_buy)
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

template<> inline void find_best<Side::Sell>(Level** best_sell)
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

template<Side side> class PriceLevel
{
    Level* root = nullptr;
    std::unordered_map<uint64_t, Level*> levels;
    uint64_t last_best_price = 0;
    uint32_t count = 0;
    Pool pool;

public:
    uint64_t volume = 0;
    Level* best = nullptr;
    void add_level(Order* order)
    {
        if (levels.count(order->limit) == 0)
        {
            // If you wanr use new operator, remember to delete
            // order->level = new Level(order);
            order->level = pool.get_level(order);

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

    void cancel_order(Order* order)
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
            pool.release_level(level);
            // delete level;
        }
        else
        {
            --level->count;
            level->volume -= order->shares;
            remove_from_dll(&level->order_head, &level->order_tail, (order));
        }

        --count;
        volume -= order->shares;
        if (best != nullptr)
        {
            last_best_price = best->key;
        }
    }

    void in_order_traversal(Level* node)
    {
        if (node == nullptr)
            return;

        in_order_traversal(node->left);
        std::cout << node->key << " ";
        in_order_traversal(node->right);
    }

    template<typename C> void market_order(Order* order, C is_fill)
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
