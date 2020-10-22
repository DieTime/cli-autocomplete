#pragma once
#ifndef AUTOCOMPLETE_NODE_H
#define AUTOCOMPLETE_NODE_H

#include "vector.h"

struct node {
    char* token;
    struct node_vector* children;
};
typedef struct node Node;

Node* node_create(char* token, unsigned token_length);

void node_free(Node* node);

#endif // AUTOCOMPLETE_NODE_H
