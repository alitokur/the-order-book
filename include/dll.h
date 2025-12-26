#pragma once
#include "types.h"

inline void append_to_dll(Order **tail, Order *node)
{
    (*tail)->next = node;
    node->prev = *tail;
    *tail = node;
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
