#include <cstdint>
#include <unordered_map>
#include "types.h"

class OrderBook
{
private:
    std::unordered_map<uint64_t, Order> orders;

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
};
