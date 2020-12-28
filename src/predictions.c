#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#include "../include/predictions.h"

char* token_create(char* str, unsigned str_len) {
    // Allocate memory for token
    char* token = (char*)malloc(sizeof(char) * str_len + 1);
    if (token == NULL) {
        fprintf(stderr, "[ERROR] Bad token memory allocation\n");
        exit(0);
    }

    // Paste data to token
    memcpy(token, str, str_len);
    token[str_len] = '\0';

    // Return token
    return token;
}

Tokens* split(char *str, char delimiter) {
    // Create token vector
    Tokens* vec = vector_create(1);

    char prev_char = delimiter;

    // Finding tokens and placing them in an array
    for (unsigned i = 0; str[i] != '\0'; i++) {

        // Skip spaces while token not found
        if (prev_char == delimiter && str[i] != delimiter) {
            // Get length of current token
            unsigned token_length = 0;

            while (str[i + token_length] != delimiter && str[i + token_length] != '\0') {
                token_length += 1;
            }

            // Add token to vector
            vector_push(vec, token_create(str + i, token_length));

            // Move string cursor right
            i += token_length - 1;
        }

        // Remember prev character
        prev_char = str[i];
    }

    // Add empty string to vector if detected delimiter in the end
    if (prev_char == delimiter) {
        char* token = token_create("", 1);
        vector_push(vec, token);
    }

    // Return token vector
    return vec;
}

Predictions *predictions_create(Tree *rules, char *input, char *optional_brackets) {
    // Initialize result predictions
    Predictions* pred = (Predictions*)malloc(sizeof(Predictions));
    if (pred == NULL) {
        fprintf(stderr, "[ERROR] Bad prediction memory allocation\n");
        exit(0);
    }
    pred->type = EXACTLY;
    pred->tokens = vector_create(1);

    // Split input string to tokens
    Tokens* tokens = split(input, ' ');

    Node* curr_node = rules->head;
    Node* prev_node = rules->head;
    Vector* curr_children = curr_node->children;

    // Don't show predictions if last word contains optional brackets
    if (contain_chars((char*)vector_get(tokens, tokens->length - 1), optional_brackets)) {
        pred->type = FAILURE;
        return pred;
    }

    // Finding vector in tree by input tokens
    for (unsigned i = 0; i < tokens->length - 1; i++) {
        for (unsigned j = 0; j < curr_children->length; j++) {
            Node* temp_node = (Node*)vector_get(curr_children, j);
            char* token = (char*)vector_get(tokens, i);

            if (strcmp(temp_node->token, token) == 0) {
                curr_node = temp_node;
                curr_children = curr_node->children;
                break;
            }
        }

        // If further search makes no sense
        if (prev_node == curr_node) {
            pred->type = FAILURE;
            break;
        }

        // Remember node
        prev_node = curr_node;
    }

    // Search words starts with last token
    // if children nodes was found
    if (pred->type != FAILURE) {
        for (unsigned i = 0; i < curr_children->length; i++) {
            char* probably_token = ((Node*)vector_get(curr_children, i))->token;
            char* last_token = (char*)vector_get(tokens, tokens->length - 1);

            if (strncmp(last_token, probably_token, strlen(last_token)) == 0) {
                vector_push(pred->tokens, token_create(probably_token, (unsigned int) strlen(probably_token)));
            }
        }
    }

    // Set EXACTLY type for predictions if words was found
    // or search probably words without optional brackets
    // if words wasn't found
    if (pred->tokens->length > 0) {
        pred->type = EXACTLY;
    } else if (pred->type != FAILURE) {
        for (unsigned i = 0; i < curr_children->length; i++) {
            char *probably_token = ((Node *) vector_get(curr_children, i))->token;
            char *last_token = (char *) vector_get(tokens, tokens->length - 1);

            // Skip if candidate contain one of symbols for optional values
            if (contain_chars(probably_token, optional_brackets)) {
                continue;
            }

            // Total misses in probably token
            unsigned miss = 0;

            // Counting misses (no more than 2)
            for (unsigned j = 0; j < strlen(last_token); j++) {
                if (last_token[j] != probably_token[j]) {
                    if (++miss == 2) {
                        break;
                    }
                }
            }

            // Adding a word to predictions
            // if there are less than 2 misses
            if (miss < 2) {
                vector_push(pred->tokens, token_create(probably_token, (unsigned int)strlen(probably_token)));
            }
        }

        // Set type depending on the success of the search
        pred->type = pred->tokens->length > 0 ? PROBABLY : FAILURE;
    }

    // Free input tokens
    tokens_free(tokens);

    // Return result
    return pred;
}

void predictions_free(Predictions* predict) {
    // Free all tokens
    tokens_free(predict->tokens);

    // Free self
    free(predict);
}

void tokens_free(Tokens* tokens) {
    // Free all input tokens vector
    for (unsigned i = 0; i < tokens->length; i++) {
        free(vector_get(tokens, i));
    }

    // Free self
    vector_free(tokens);
}

int contain_chars(const char* str, const char* chars) {
    // Checking for given characters in a string
    for (unsigned i = 0; str[i] != '\0'; i++) {
        for (unsigned j = 0; chars[j] != '\0'; j++) {
            if (str[i] == chars[j]) {
                return 1;
            }
        }
    }

    return 0;
}