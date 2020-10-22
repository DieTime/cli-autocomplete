#ifndef AUTOCOMPLETE_VECTOR_H
#define AUTOCOMPLETE_VECTOR_H

struct vector {
    void*    data;
    unsigned capacity;
    unsigned length;
    unsigned item_size;
};
typedef struct vector Vector;

Vector* vector_create(unsigned length, unsigned item_size);

void* vector_get(Vector* vec, unsigned index);

void vector_set(Vector* vec, unsigned index, void* item);

void vector_push(Vector* vec, void* item);

void vector_free(Vector* vec);

#endif //AUTOCOMPLETE_VECTOR_H
