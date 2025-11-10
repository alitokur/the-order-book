
#include "types.h"
#include "DLL.h"
#include <cstdint>
#include <unordered_map>
#include <iostream>

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

            /// TODO: set best price

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
    };
};
