#ifndef AUTOCOMPLETE_TOKEN_VECTOR_H
#define AUTOCOMPLETE_TOKEN_VECTOR_H

struct token_vector {
    unsigned length;
    unsigned capacity;
    char**   data;
};
typedef struct token_vector TokenVector;

char* token_create(char* str, unsigned str_len);

TokenVector* token_vector_create(unsigned capacity);

TokenVector* token_vector_parse(char *str, char delimiter);

void token_vector_push(TokenVector* tv, char* t);

void token_vector_free(TokenVector* tv);

void token_vector_print(TokenVector* tv);

char* token_vector_get(TokenVector* tv, unsigned index);

#endif // AUTOCOMPLETE_TOKEN_VECTOR_H
