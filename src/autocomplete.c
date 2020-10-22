#include <string.h>

#include "../include/autocomplete.h"

TokenVector* get_predictions(Tree* t, char* input) {
    TokenVector* predictions = token_vector_create(1);

    TokenVector* tokens = token_vector_parse(input, ' ');
    Node* curr_node = t->head;
    Node* prev_node = t->head;

    for (unsigned i = 0; i < tokens->length - 1; i++) {
        for (unsigned j = 0; j < curr_node->children->length; j++) {
            Node* temp_node = node_vector_get(curr_node->children, j);
            char* token = token_vector_get(tokens, i);

            if (strcmp(temp_node->token, token) == 0) {
                curr_node = temp_node;
                break;
            }
        }

        if (prev_node == curr_node) {
            return predictions;
        }

        prev_node = curr_node;
    }

    for (unsigned i = 0; i < curr_node->children->length; i++) {
        char* probably_token = node_vector_get(curr_node->children, i)->token;
        char* last_token = token_vector_get(tokens, tokens->length - 1);
        if (strcmp(last_token, " ") == 0 || strncmp(last_token, probably_token, strlen(last_token)) == 0) {
            token_vector_push(predictions, token_create(probably_token, strlen(probably_token)));
        }
    }

    return predictions;
}