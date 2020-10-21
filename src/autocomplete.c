#include <assert.h>
#include <stdlib.h>
#include <string.h>

#include "../include/autocomplete.h"

char** tokenize(char* str, char delimiter) {
    int  token_count = 0;
    char last_char = delimiter;

    // Find count of tokens for memory allocation
    for (int i = 0; str[i] != '\0'; i++) {
        if (last_char != delimiter && str[i] == delimiter) {
            token_count += 1;
        }
        last_char = str[i];
    }

    if (last_char != ' ') {
        token_count += 1;
    }

    // Allocate memory for tokens array
    char** result = (char**)malloc(sizeof(char*) * (token_count + 1));
    assert(result != NULL && "memory allocation error");

    int idx = 0;
    last_char = delimiter;

    // Finding tokens and placing them in an array
    for (int i = 0; str[i] != '\0'; i++) {
        if (last_char == delimiter && str[i] != delimiter) {
            size_t token_length = 0;

            while (str[i + token_length] != delimiter && str[i + token_length] != '\0') {
                token_length += 1;
            }

            result[idx] = (char*)malloc(sizeof(char) * (token_length + 1));
            assert(result[idx] != NULL && "memory allocation error");

            memcpy(result[idx], str + i, token_length);
            result[idx][token_length] = '\0';

            idx += 1;
            i += token_length;
        }

        last_char = str[i];
    }

    // Checking the number of received tokens
    assert(idx == token_count && "the number of lines does not match the expected");
    result[idx] = "\0";

    return result;
}

void free_tokens(char** tokens) {
    int counter = 0;

    // Free all tokens in array
    while (tokens[counter][0] != '\0') {
        free(tokens[counter++]);
    }
    free(tokens[counter]);

    // Free tokens array
    free(tokens);
}