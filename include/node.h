#pragma once
#ifndef AUTOCOMPLETE_NODE_H
#define AUTOCOMPLETE_NODE_H

#include "vector.h"

/**
 * Node of rules tree
 *
 * Contains self token and
 * vector of children nodes
 */
struct node {
    char* token;
    struct vector* children;
};
typedef struct node Node;


/**
 * Create node by token and token_length
 *
 * @param token - Node containing string
 * @param token_length - Length of string
 *
 * @return Created Node
 */
Node* node_create(char* token, unsigned token_length);


/**
 * Function for free Node
 * and all his children
 *
 * @param node - Node for deallocating
 */
void node_free(Node* node);

#endif // AUTOCOMPLETE_NODE_H
