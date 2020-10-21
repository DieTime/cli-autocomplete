#ifndef AUTOCOMPLETE_H
#define AUTOCOMPLETE_H

struct token_vector {
    unsigned length;
    unsigned capacity;
    char**   data;
};
typedef struct token_vector TokenVector;

TokenVector* token_vector_create(char *str, char delimiter);

void token_vector_push(TokenVector* tv, char* t);

void token_vector_free(TokenVector* tv);

void token_vector_print(TokenVector* tv);

char* token_vector_get(TokenVector* tv, unsigned index);

#endif //AUTOCOMPLETE_H
