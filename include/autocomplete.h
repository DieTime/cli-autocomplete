#ifndef AUTOCOMPLETE_AUTOCOMPLETE_H
#define AUTOCOMPLETE_AUTOCOMPLETE_H

#include "tree.h"
#include "token_vector.h"

TokenVector* get_predictions(Tree* t, char* input);

#endif // AUTOCOMPLETE_AUTOCOMPLETE_H