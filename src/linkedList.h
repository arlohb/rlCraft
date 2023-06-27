#ifndef _LINKED_LIST_H
#define _LINKED_LIST_H

#include <cstddef>

typedef struct Node {
    void* val;
    struct Node* next;
} Node;

typedef struct {
    Node* head;
    Node* tail;
    size_t length;
} List;

void ListInit(List* list);
void ListAppend(List* list, void* val);

#define ListIter(list, node) for(Node* node = list.head; node; node = node->next)

#endif
