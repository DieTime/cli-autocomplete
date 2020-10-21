#pragma once
#ifndef AUTOCOMPLETE_TREE_H
#define AUTOCOMPLETE_TREE_H

#include "node.h"

struct tree {
    Node* head;
};
typedef struct tree Tree;

Tree* tree_create(const char* filepath);

void tree_free(Tree* tree);

#endif // AUTOCOMPLETE_TREE_H