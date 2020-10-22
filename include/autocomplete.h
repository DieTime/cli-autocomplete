#ifndef AUTOCOMPLETE_AUTOCOMPLETE_H
#define AUTOCOMPLETE_AUTOCOMPLETE_H

#include "tree.h"
#include "vector.h"

typedef Vector Tokens;

char* token_create(char* str, unsigned str_len);

void free_tokens(Tokens* tokens);

Tokens* split(char *str, char delimiter);

Tokens* get_predictions(Tree* t, char* input);


#endif // AUTOCOMPLETE_AUTOCOMPLETE_H