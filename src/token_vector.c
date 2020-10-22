#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "../include/token_vector.h"

TokenVector* token_vector_create(unsigned capacity) {
    // Allocate memory for self
    TokenVector* tv = (TokenVector*)malloc(sizeof(TokenVector));
    if (tv == NULL) {
        fprintf(stderr, "[ERROR] Bad token vector memory allocation\n");
        exit(1);
    }

    // Allocate memory for data field
    tv->data = (char**)malloc(sizeof(char*) * capacity);
    if (tv->data == NULL) {
        fprintf(stderr, "[ERROR] Bad token vector memory allocation\n");
        exit(1);
    }

    // Setup fields
    tv->length = 0;
    tv->capacity = capacity;

    return tv;
}

TokenVector* token_vector_parse(char *str, char delimiter) {
    // Create token vector
    TokenVector* tv = token_vector_create(1);

    char prev_char = delimiter;

    // Finding tokens and placing them in an array
    for (unsigned i = 0; str[i] != '\0'; i++) {
        if (prev_char == delimiter && str[i] != delimiter) {
            // Get length of current token
            unsigned token_length = 0;
            while (str[i + token_length] != delimiter && str[i + token_length] != '\0') {
                token_length += 1;
            }

            // Create token
            char* token = token_create(str + i, token_length);

            // Add token to vector
            token_vector_push(tv, token);

            // Move string cursor right
            i += token_length - 1;
        }

        // Remember prev character
        prev_char = str[i];
    }

    // Add empty string to vector if detected delimiter in the end
    if (prev_char == delimiter) {
        char* token = (char*)malloc(sizeof(char));
        if (token == NULL) {
            fprintf(stderr, "[ERROR] Bad token vector memory allocation\n");
            exit(1);
        }
        token[0] = '\0';
        token_vector_push(tv, token);
    }

    // Return token vector
    return tv;
}

void token_vector_push(TokenVector* tv, char* t) {
    // Reallocate longer memory if capacity runs out
    if (tv->length >= tv->capacity) {
        tv->capacity = (tv->capacity + 1) * 2;
        tv->data = (char**)realloc(tv->data,sizeof(char*) * tv->capacity);
        if (tv->data == NULL) {
            fprintf(stderr, "[ERROR] Bad token vector memory reallocation\n");
            exit(1);
        }
    }

    // Add node and increase length
    tv->data[tv->length++] = t;
}

void token_vector_free(TokenVector* tv) {
    // Free all tokens
    for (unsigned i = 0; i < tv->length; i++) {
        free(tv->data[i]);
    }

    // Free data and self
    free(tv->data);
    free(tv);
}

void token_vector_print(TokenVector* tv) {
    fprintf(stdout, "[");

    char* format = " \"%s\"";
    for (unsigned i = 0; i < tv->length; i++) {
        fprintf(stdout, format, token_vector_get(tv, i));
        format = ", \"%s\"";
    }

    fprintf(stdout, " ]\n");
}

char* token_vector_get(TokenVector* tv, unsigned index) {
    // Error message if index not within array bounds
    if (index >= tv->length) {
        fprintf(stderr, "[ERROR] Out of bounds an token vector\n");
        exit(1);
    }

    // Return node by index
    return tv->data[index];
}

char* token_create(char* str, unsigned str_len) {
    char* token = (char*)malloc(sizeof(char) * str_len + 1);
    if (token == NULL) {
        fprintf(stderr, "[ERROR] Memory token memory allocation\n");
        exit(0);
    }

    memcpy(token, str, str_len);
    token[str_len] = '\0';

    return token;
}