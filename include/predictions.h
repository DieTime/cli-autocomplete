#ifndef AUTOCOMPLETE_PREDICTIONS_H
#define AUTOCOMPLETE_PREDICTIONS_H

#include "tree.h"
#include "vector.h"

typedef Vector Tokens;

enum predict_type {
    FAILURE  = 0,
    EXACTLY  = 1,
    PROBABLY = 2,
};
typedef enum predict_type PredictType;

struct predictions {
    PredictType type;
    Tokens* tokens;
};
typedef struct predictions Predictions;

Predictions* predictions_create(Tree* t, char* input);

void predictions_free(Predictions* predict);

Tokens* split(char *str, char delimiter);

char* token_create(char* str, unsigned str_len);

void tokens_free(Tokens* tokens);

#endif // AUTOCOMPLETE_PREDICTIONS_H