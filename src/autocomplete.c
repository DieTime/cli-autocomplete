#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#include "../include/autocomplete.h"

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

Tokens* split(char *str, char delimiter) {
    // Create token vector
    Tokens* vec = vector_create(1);

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
            vector_push(vec, token);

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
        vector_push(vec, token);
    }

    // Return token vector
    return vec;
}

Tokens* get_predictions(Tree* t, char* input) {
    Tokens* predictions = vector_create(1);

    Tokens* tokens = split(input, ' ');
    Node* curr_node = t->head;
    Node* prev_node = t->head;

    for (unsigned i = 0; i < tokens->length - 1; i++) {
        for (unsigned j = 0; j < curr_node->children->length; j++) {
            Node* temp_node = (Node*)vector_get(curr_node->children, j);
            char* token = (char*)vector_get(tokens, i);

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
        char* probably_token = ((Node*)vector_get(curr_node->children, i))->token;
        char* last_token = (char*)vector_get(tokens, tokens->length - 1);
        if (strcmp(last_token, " ") == 0 || strncmp(last_token, probably_token, strlen(last_token)) == 0) {
            vector_push(predictions, token_create(probably_token, strlen(probably_token)));
        }
    }

    free_tokens(tokens);

    return predictions;
}

void free_tokens(Tokens* tokens) {
    for (int i = 0; i < tokens->length; i++) {
        free(vector_get(tokens, i));
    }
    vector_free(tokens);
}