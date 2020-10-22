#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "../include/vector.h"

Vector vector_create(unsigned length, unsigned item_size) {
    Vector vec;

    // Setup vector capacity and length
    vec.length = 0;
    vec.capacity = length;
    vec.item_size = item_size;

    // Allocate memory for data field
    vec.data = malloc(length * item_size);
    if (vec.data == NULL) {
        fprintf(stderr, "[ERROR] Bad vector memory allocation\n");
        exit(1);
    }

    return vec;
}

void* vector_get(Vector* vec, unsigned index) {
    // Error message if index not within array bounds
    if (index >= vec->length) {
        fprintf(stderr, "[ERROR] Out of bounds an vector\n");
        exit(1);
    }

    // Return node by index
    return vec->data + index * vec->item_size;
}

void vector_set(Vector* vec, unsigned index, void* item) {
    // Error message if index not within array bounds
    if (index >= vec->length) {
        fprintf(stderr, "[ERROR] Out of bounds an vector\n");
        exit(1);
    }

    memcpy(vec->data + index * vec->item_size, item, vec->item_size);
}

void vector_push(Vector* vec, void* item) {
    // Reallocate longer memory if capacity runs out
    if (vec->length >= vec->capacity) {
        vec->capacity = (vec->capacity + 1) * 2;
        vec->data = (void*)realloc(vec->data, vec->capacity * vec->item_size);
        if (vec->data == NULL) {
            fprintf(stderr, "[ERROR] Bad vector memory reallocation\n");
            exit(1);
        }
    }

    // Add node and increase length
    memcpy(vec->data + vec->length * vec->item_size, item, vec->item_size);
    vec->length += 1;
}

void vector_free(Vector* vec) {
    // Free memory for data field
    free(vec->data);
}