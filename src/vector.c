#include <stdlib.h>
#include <stdio.h>

#include "../include/vector.h"

Vector* vector_create(unsigned length) {
    // Allocate memory for self
    Vector* vec = (Vector*)malloc(sizeof(Vector));
    if (vec == NULL) {
        fprintf(stderr, "[ERROR] Bad vector memory allocation\n");
        exit(1);
    }

    // Setup vector capacity and length
    vec->length = 0;
    vec->capacity = MAX_OF(length, 1);

    // Allocate memory for data field
    vec->data = (void**)malloc(sizeof(void*) * length);
    if (vec->data == NULL) {
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
    return vec->data[index];
}

void vector_set(Vector* vec, unsigned index, void* item) {
    // Error message if index not within array bounds
    if (index >= vec->length) {
        fprintf(stderr, "[ERROR] Out of bounds an vector\n");
        exit(1);
    }

    // Set item by index
    vec->data[index] = item;
}

void vector_push(Vector* vec, void* item) {
    // Reallocate longer memory if capacity runs out
    if (vec->length >= vec->capacity) {
        vec->capacity = (vec->capacity + 1) * 2;
        vec->data = (void**)realloc(vec->data, vec->capacity * sizeof(void*));
        if (vec->data == NULL) {
            fprintf(stderr, "[ERROR] Bad vector memory reallocation\n");
            exit(1);
        }
    }

    // Add node and increase length
    vec->data[vec->length++] = item;
}

void vector_free(Vector* vec) {
    // Free memory for data field and self
    free(vec->data);
    free(vec);
}