#pragma once
#ifndef AUTOCOMPLETE_VECTOR_H
#define AUTOCOMPLETE_VECTOR_H

#define MAX_OF(x, y) (((x) > (y)) ? (x) : (y))

/**
 * Vector implementation for contain
 * pointers of allocated data
 */
struct vector {
    void**   data;
    unsigned capacity;
    unsigned length;
};
typedef struct vector Vector;

/**
 * Function for allocating vector
 *
 * @param length - Initial length of vector
 * @return Vector structure
 */
Vector* vector_create(unsigned length);

/**
 * Function for peak value from
 * vector by index
 *
 * @param vec - Vector for getting info
 * @param index - Position of element
 *
 * @return Pointer of data by index
 */
void* vector_get(Vector* vec, unsigned index);

/**
 * Function for setting value in
 * vector by index
 *
 * @param vec - Vector for setting info
 * @param index - Position of element
 * @param item - Pointer of data for setting
 */
void vector_set(Vector* vec, unsigned index, void* item);

/**
 * Function for append data to vector
 *
 * @param vec - Vector for pushing
 * @param item - Item for pushing
 */
void vector_push(Vector* vec, void* item);

/**
 * Function for deallocating vector
 * @param vec - Vector for deallocating
 */
void vector_free(Vector* vec);

#endif //AUTOCOMPLETE_VECTOR_H
