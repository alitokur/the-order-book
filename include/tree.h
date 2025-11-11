
#include "types.h"
#include "DLL.h"
#include <cstdint>
#include <unordered_map>
#include <iostream>

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

template<Side side> class PriceLevelTree
{
    Level *root = nullptr;
    std::unordered_map<uint64_t, Level *> levels;
    Level *best = nullptr;
    uint64_t last_best_price = 0;
    uint32_t count = 0;
    uint64_t volume = 0;

public:
    void add_level(Order *order)
    {
        if (levels.count(order->limit) == 0)
        {
            std::cout << "empty price level... creating for: " << order->limit
                      << std::endl;
            order->level = new Level(order);

            insert_to_bst(reinterpret_cast<BSTNode<uint64_t> **>(&root),
                          static_cast<BSTNode<uint64_t> *>(order->level));

            set_best<side>(&best, order->level);

            levels.emplace(order->level->key, order->level);
        }
        else
        {
            std::cout << "adding to linked list for " << order->limit << std::endl;
            order->level = levels.at(order->limit);
            ++order->level->count;
            order->level->volume += order->shares;
            append_to_dll(reinterpret_cast<DLLNode**>(&order->level->order_tail),
                          static_cast<DLLNode*>(order));
        }
        ++count;
        volume += order->shares;
        if(best != nullptr) last_best_price = best->key;
    };

    void display_tree(){
        in_order_traversal(reinterpret_cast<BSTNode<uint64_t> *>(root));
    }

    void info_best()
    {
        std::cout << "price: " << best->key << " count: " << best->count
                  << " volume: " << best->volume << std::endl;
    }
};
