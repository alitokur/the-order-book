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

inline void remove_from_dll(DLLNode** head, DLLNode** tail, DLLNode* node){
    if(node->prev != nullptr){
        node->prev->next = node->next;
    }
    if(node->next != nullptr){
        node->next->prev = node->prev;
    }
    if(*head == node){
        *head = node->next;
    }
    if(*tail == node){
        *head = node->prev;
    }
    node->next = nullptr;
    node->prev = nullptr;
}
