
#pragma once

#include <cstdint>
#include "BST.h"
#include "DLL.h"

enum class Side : bool
{
    Sell = false,
    Buy = true
};

inline Side operator!(Side side)
{
    return static_cast<Side>(!static_cast<bool>(side));
}

struct Level;

struct Order : DLLNode
{
    const uint64_t id_number = 0;
    const Side buy_or_sell = Side::Sell;
    uint32_t shares = 0;
    const uint64_t limit = 0;
    uint64_t entry_time = 0;
    uint64_t event_time = 0;
    Level *level = nullptr;

    /// TODO: add double linked list here

    Order(uint64_t _order_id,
          Side _side,
          uint32_t _shares,
          uint64_t price_,
          uint64_t _entry_time,
          uint64_t _event_time) :
        DLLNode(),
        id_number(_order_id),
        buy_or_sell(_side),
        shares(_shares),
        limit(price_),
        entry_time(_entry_time),
        event_time(_event_time)
    {
    }
};

struct Level : BSTNode<uint64_t>
{

    uint32_t count = 0; const uint32_t _padding = 0; uint64_t volume = 0; Order *order_head = nullptr; Order *order_tail = nullptr;
    explicit Level(Order *order) :
        BSTNode<uint64_t>(order->limit),
        count(1),
        volume(order->shares),
        order_head(order),
        order_tail(order)
    {
    }
};
