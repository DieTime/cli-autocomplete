#include <assert.h>
#include <stdlib.h>
#include <stdio.h>
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
    token_count += 1;

    // Allocate memory for tokens array
    char** result = (char**)malloc(sizeof(char*) * (token_count + 1));
    if (result == NULL) {
        fprintf(stderr, "[ERROR] Memory allocation error when tokenize string\n");
        exit(0);
    }

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
            if (result[idx] == NULL) {
                fprintf(stderr, "[ERROR] Memory allocation error when tokenize string\n");
                exit(0);
            }

            memcpy(result[idx], str + i, token_length);
            result[idx][token_length] = '\0';

            idx += 1;
            i += token_length - 1;
        }

        last_char = str[i];
    }


    if (last_char == ' ') {
        result[idx] = (char*)malloc(sizeof(char) * 2);
        memcpy(result[idx], " \0", 2);
        idx += 1;
    }

    // Checking the number of received tokens
    result[idx] = "\0";

    return result;
}

void free_tokens(char** tokens) {
    int counter = 0;

    // Free all tokens in array
    while (tokens[counter][0] != '\0') {
        free(tokens[counter++]);
    }

    // Free tokens array
    free(tokens);
}