#include "linkedList.h"
#include <stdlib.h>

void ListInit(List* list) {
    list->head = NULL;
    list->tail = NULL;
    list->length = 0;
}

void ListAppend(List* list, void* val) {
    Node* node = (Node*)malloc(sizeof(Node));
    node->val = val;
    node->next = NULL;

    if (!list->head)
        list->head = node;
    else
        list->tail->next = node;

    list->tail = node;

    list->length++;
}
