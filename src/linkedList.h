#pragma once

#include <cstddef>

struct Node {
    void* val;
    struct Node* next;
};

struct List {
    Node* head;
    Node* tail;
    size_t length;
};

void ListInit(List* list);
void ListAppend(List* list, void* val);

#define ListIter(list, node) for(Node* node = list.head; node; node = node->next)

