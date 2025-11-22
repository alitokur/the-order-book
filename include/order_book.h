#include <cstdint>
#include <unordered_map>
#include <iostream>
#include "types.h"
#include "tree.h"
#include "robin_hood.h"

class OrderBook
{
private:
    std::unordered_map<uint64_t, Order> orders;
    PriceLevelTree<Side::Sell> asks;
    PriceLevelTree<Side::Buy> bids;

public:
    inline void add_buy_order(uint64_t order_id,
                              uint32_t quantity,
                              uint32_t price,
                              uint64_t entry_time,
                              uint64_t event_time)
    {
        orders.emplace(std::piecewise_construct,
                       std::forward_as_tuple(order_id),
                       std::forward_as_tuple(order_id,
                                             Side::Buy,
                                             quantity,
                                             price,
                                             entry_time,
                                             event_time));
        // orders.try_emplace(order_id,
        //                order_id,
        //                Side::Buy,
        //                quantity,
        //                price,
        //                entry_time,
        //                event_time);
        if (asks.best != nullptr && price >= asks.best->key)
        {
            asks.market_order(&orders.at(order_id),
                              [&](uint64_t id) { orders.erase(id); });
            if (orders.at(order_id).shares == 0)
            {
                orders.erase(order_id);
                return;
            }
        }
        bids.add_level(&orders.at(order_id));
    }

    inline void add_sell_order(uint64_t order_id,
                               uint32_t quantity,
                               uint32_t price,
                               uint64_t entry_time,
                               uint64_t event_time)
    {
        orders.emplace(std::piecewise_construct,
                       std::forward_as_tuple(order_id),
                       std::forward_as_tuple(order_id,
                                             Side::Sell,
                                             quantity,
                                             price,
                                             entry_time,
                                             event_time));

        if (bids.best != nullptr && price <= bids.best->key)
        {
            bids.market_order(&orders.at(order_id),
                              [&](uint64_t id) { orders.erase(id); });
            if (orders.at(order_id).shares == 0)
            {
                orders.erase(order_id);
                return;
            }
        }
        asks.add_level(&orders.at(order_id));
    }

    inline void add_order(uint64_t order_id,
                          Side side,
                          uint32_t quantity,
                          uint32_t price,
                          uint64_t entry_time,
                          uint64_t event_time)
    {
        switch (side)
        {
        case Side::Sell:
            return add_sell_order(order_id,
                                  quantity,
                                  price,
                                  entry_time,
                                  event_time);
        case Side::Buy:
            return add_buy_order(order_id,
                                 quantity,
                                 price,
                                 entry_time,
                                 event_time);
        }
    }


    inline void cancel_order(uint64_t order_id){
        auto order = &orders.at(order_id);
        switch(order->buy_or_sell){
            case Side::Sell:{
                asks.cancel_order(order);
                break;
            }
            case Side::Buy:{
                bids.cancel_order(order);
                break;
            }
        }
        orders.erase(order_id);
    }

    inline void execute_order(uint64_t order_id, uint32_t quantity)
    {
        auto order = &orders.at(order_id);
        if (quantity > order->shares)
        {
            std::cout << "ERROR";
            return;
        }
        order->shares -= quantity;
        order->level->volume -= quantity;
        switch (order->buy_or_sell)
        {
        case Side::Sell: {
            asks.volume -= quantity;
            break;
        };
        case Side::Buy: {
            bids.volume -= quantity;
            break;
        }
        }

        if (order->shares == 0)
        {
            switch (order->buy_or_sell)
            {
            case Side::Sell: {
                asks.cancel_order(order);
                break;
            }
            case Side::Buy: {
                bids.cancel_order(order);
                break;
            }
            }
            orders.erase(order_id);
        }
    }

    /// combination of: cancel + add
    inline void modify_order(uint64_t old_order_id,
                             uint64_t new_order_id,
                             uint32_t quantity,
                             uint64_t price,
                             Side side,
                             uint64_t entry_time,
                             uint64_t event_time)
    {
        cancel_order(old_order_id);
        add_order(new_order_id, side, quantity, price, entry_time, event_time);
    }

    /// DEBUG
    inline void display_orders()
    {
        // std::cout << "----Orders----" << std::endl;
        // for (const auto &order : orders)
        // {
        //     const char* side = (order.second.buy_or_sell == Side::Buy) ? "Buy" : "Sell";
        //     std::cout << "order_id: " << order.first << " side: " << side
        //               << " quantity: " << order.second.shares
        //               << " price: " << order.second.limit << std::endl;
        // }

        std::cout << "----Bids Tree----" << std::endl;
        bids.display_tree();
        std::cout << std::endl;
        std::cout << "----Asks Tree----" << std::endl;
        asks.display_tree();
        std::cout << std::endl;
        
        std::cout << "Best Ask Info: " << std::endl;
        asks.info_best();
        
        std::cout << "Best Bid Info: " << std::endl;
        bids.info_best();
 
    }
    /// DEBUG 
};
