#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../include/node.h"

Node* node_create(char* token, unsigned token_length) {
    // Allocate memory for node
    Node* n = (Node*)malloc(sizeof(Node));
    if (n == NULL) {
        fprintf(stderr, "[ERROR] Bad node vector memory allocation\n");
        exit(1);
    }

    // Allocate memory for token field
    n->token = (char*)malloc(sizeof(char) * token_length);
    if (n->token == NULL) {
        fprintf(stderr, "[ERROR] Bad node vector memory allocation\n");
        exit(1);
    }

    // Setup fields of node
    memcpy(n->token, token, sizeof(char) * token_length);

    n->children = vector_create(1);

    return n;
}

void node_free(Node* n) {
    // Free all child nodes
    for (unsigned i = 0; i < n->children->length; i++) {
        node_free(vector_get(n->children, i));
    }

    // Free memory for children, token and self
    vector_free(n->children);
    free(n->token);
    free(n);
}