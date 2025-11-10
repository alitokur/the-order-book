#pragma once

struct DLLNode
{
    DLLNode *prev;
    DLLNode *next;
    DLLNode() : prev(nullptr), next(nullptr) {}
};

inline void append_to_dll(DLLNode **tail, DLLNode *node)
{
    (*tail)->next = node;
    node->prev = *tail;
    *tail = node;
}
