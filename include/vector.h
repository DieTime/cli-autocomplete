#pragma once
#ifndef AUTOCOMPLETE_VECTOR_H
#define AUTOCOMPLETE_VECTOR_H

#define MAX_OF(x, y) (((x) > (y)) ? (x) : (y))

struct vector {
    void**   data;
    unsigned capacity;
    unsigned length;
};
typedef struct vector Vector;

Vector* vector_create(unsigned length);

void* vector_get(Vector* vec, unsigned index);

void vector_set(Vector* vec, unsigned index, void* item);

void vector_push(Vector* vec, void* item);

void vector_free(Vector* vec);

#endif //AUTOCOMPLETE_VECTOR_H
