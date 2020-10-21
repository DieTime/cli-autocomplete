#pragma once
#ifndef AUTOCOMPLETE_NODE_VECTOR_H
#define AUTOCOMPLETE_NODE_VECTOR_H

#include "node.h"

struct node_vector {
    struct node** data;
    unsigned capacity;
    unsigned length;
};
typedef struct node_vector NodeVector;

NodeVector* node_vector_create(unsigned length);

struct node* node_vector_get(NodeVector* nv, unsigned index);

void node_vector_set(NodeVector* nv, unsigned index, struct node* item);

void node_vector_push(NodeVector* nv, struct node* n);

void node_vector_print(NodeVector* nv);

void node_vector_free(NodeVector* nv);

#endif // AUTOCOMPLETE_NODE_VECTOR_H
