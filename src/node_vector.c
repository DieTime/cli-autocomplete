#include <stdlib.h>
#include <stdio.h>

#include "../include/node_vector.h"

NodeVector* node_vector_create(unsigned length) {
    // Allocate memory for vector
    NodeVector* nv = (NodeVector*)malloc(sizeof(NodeVector));
    if (nv == NULL) {
        fprintf(stderr, "[ERROR] Bad node vector memory allocation\n");
        exit(1);
    }

    // Setup vector capacity and length
    nv->length = 0;
    nv->capacity = length;

    // Allocate memory for data field
    nv->data = (Node**)malloc(sizeof(Node*) * length);
    if (nv->data == NULL) {
        fprintf(stderr, "[ERROR] Bad node vector memory allocation\n");
        exit(1);
    }

    return nv;
}

void node_vector_push(NodeVector* nv, Node* n) {
    // Reallocate longer memory if capacity runs out
    if (nv->length >= nv->capacity) {
        nv->capacity = (nv->capacity + 1) * 2;
        nv->data = (Node**)realloc(nv->data,sizeof(Node*) * nv->capacity);
        if (nv->data == NULL) {
            fprintf(stderr, "[ERROR] Bad node vector memory reallocation\n");
            exit(1);
        }
    }

    // Add node and increase length
    nv->data[nv->length++] = n;
}

void node_vector_print(NodeVector* nv) {
    fprintf(stdout, "[");

    char* format = " \"%s\"";
    for (unsigned i = 0; i < nv->length; i++) {
        fprintf(stdout, format, node_vector_get(nv, i)->token);
        format = ", \"%s\"";
    }

    fprintf(stdout, " ]\n");
}

Node* node_vector_get(NodeVector* nv, unsigned index) {
    // Error message if index not within array bounds
    if (index >= nv->length) {
        fprintf(stderr, "[ERROR] Out of bounds an node vector\n");
        exit(1);
    }

    // Return node by index
    return nv->data[index];
}

void node_vector_set(NodeVector* nv, unsigned index, Node* item) {
    // Error message if index not within array bounds
    if (index >= nv->length) {
        fprintf(stderr, "[ERROR] Out of bounds an node vector\n");
        exit(1);
    }

    // Set node by index
    nv->data[index] = item;
}

void node_vector_free(NodeVector* nv) {
    // Free memory for data field and self
    free(nv->data);
    free(nv);
}