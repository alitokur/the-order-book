
#pragma once

#include <cstdint>

enum class Side : bool
{
    Sell = false,
    Buy = true
};

struct Order
{
    const uint64_t id_number = 0;
    const Side buy_or_sell = Side::Sell;
    uint32_t shares = 0;
    const uint64_t limit = 0;
    uint64_t entry_time = 0;
    uint64_t event_time = 0;

    Order(uint64_t _order_id,
          Side _side,
          uint32_t _shares,
          uint64_t price_,
          uint64_t _entry_time,
          uint64_t _event_time) :
        id_number(_order_id),
        buy_or_sell(_side),
        shares(_shares),
        limit(price_),
        entry_time(_entry_time),
        event_time(_event_time)
    {
    }
};
