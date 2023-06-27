#include "linkedList.h"
#include <cstdlib>

void ListInit(List* list) {
    list->head = NULL;
    list->tail = NULL;
    list->length = 0;
}

void ListAppend(List* list, void* val) {
    Node* node = new Node;
    node->val = val;
    node->next = NULL;

    if (!list->head)
        list->head = node;
    else
        list->tail->next = node;

    list->tail = node;

    list->length++;
}
