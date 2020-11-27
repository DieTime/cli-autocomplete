#ifdef _MSC_VER
    #ifndef _CRT_SECURE_NO_WARNINGS
        #define _CRT_SECURE_NO_WARNINGS
    #endif
#endif

#include <stdio.h>
#include <stdlib.h>

#include "../include/tree.h"

Tree* tree_create(const char* filepath) {
    int character; // Variable for reading char by char

    unsigned buff_length = 0;  // Max token length
    unsigned buff_counter = 0; // Variable for getting max token length

    // Try open file for reading
    FILE* config = fopen(filepath, "r");
    if (config == NULL) {
        fprintf(stderr, "[ERROR] Can't open file %s\n", filepath);
        exit(0);
    }

    // Check for empty config file
    character = fgetc(config);
    if (character == EOF) {
        fclose(config);
        fprintf(stderr, "[ERROR] %s file is empty\n", filepath);
        exit(0);
    }
    ungetc(character, config);

    // Get max token length
    while (character != EOF) {
        buff_counter = 0;

        while (character == ' ') {
            character = fgetc(config);
        }

        // Count length of new token
        while (character != '\n' && character != '\r' && character != EOF) {
            buff_counter += 1;
            character = fgetc(config);
        }

        // Update token length if we need
        if (buff_counter > buff_length) {
            // +1 for null terminated symbol
            buff_length = buff_counter + 1;
        }

        while (character != ' ' && character != EOF) {
            character = fgetc(config);
        }
    }

    // Create buffer for parsing
    char* buff = (char*)malloc(sizeof(char) * buff_length);
    if (buff == NULL) {
        fprintf(stderr, "[ERROR] Bad buffer memory allocation\n");
        fclose(config);
        exit(1);
    }
    unsigned buff_i;

    unsigned space_counter;       // Counter of spaces before token
    unsigned tab_length = 0;      // Count of spaces in one tabulation
    unsigned tab_count;           // Count of tabulations in line
    unsigned line_counter = 0;    // Counter of lines in config file

    // Initialize tree and tree head
    Tree* tree = (Tree*)malloc(sizeof(Tree));
    if (tree == NULL) {
        fprintf(stderr, "[ERROR] Bad tree memory allocation\n");
        fclose(config);
        free(buff);
        exit(1);
    }
    tree->head = node_create("\0", 1);

    // Vector of root nodes for parsing
    Vector* root_nodes = vector_create(1);
    vector_push(root_nodes, (void*)tree->head);

    // Set file cursor to the start
    fseek(config, SEEK_SET, 0);

    int error = 0;  // Flag for error handling

    // Start parsing
    character = fgetc(config);
    while (character != EOF) {
        line_counter += 1;
        space_counter = 0;
        buff_i = 0;

        // Don't use tab character!!!
        if (character == '\t') {
            fprintf(stderr, "[ERROR] Tab character detected in line %d, use a sequence of spaces\n", line_counter);
            error = 1;
            break;
        }

        // Get space count
        while (character == ' ') {
            space_counter += 1;
            character = fgetc(config);
        }

        // Init correct tab length
        if (space_counter > tab_length && tab_length == 0) {
            tab_length = space_counter;
        }

        // Check count of spaces for correctness
        if (space_counter % MAX_OF(tab_length, 1) != 0) {
            fprintf(stderr, "[ERROR] Incorrect tab length in line %d\n", line_counter);
            error = 1;
            break;
        }

        // Get tabulations count;
        tab_count = space_counter / MAX_OF(tab_length, 1);

        // Get token
        while (character != '\n' && character != '\r' && character != EOF) {
            // Handle space symbol in token error
            if (character == ' ' || character == '\t') {
                error = 1;
                break;
            }

            buff[buff_i++] = (char)character;
            character = fgetc(config);
        }

        // Handle error when getting token
        if (error) {
            fprintf(stderr, "[ERROR] Token in config file must not have spaces, line %d\n", line_counter);
            break;
        }

        buff[buff_i] = '\0';

        // Create node by token
        Node* n = node_create(buff, buff_length);

        // Push node to tree
        if (tab_count >= root_nodes->length) {
            fprintf(stderr, "[ERROR] The token on line %d does not belong to any token\n", line_counter);
            error = 1;
            break;
        }
        vector_push(((Node*)vector_get(root_nodes, tab_count))->children, (void*)n);

        // Add token to root tokens vector by index as tab count
        if (tab_count + 1 < root_nodes->length) {
            vector_set(root_nodes, tab_count + 1, n);
        }
        else if (tab_count + 1 == root_nodes->length) {
            vector_push(root_nodes, (void*)n);
        }

        // Go to next line
        while (character != ' ' && character != EOF) {
            character = fgetc(config);
        }
    }

    // Remove temporary variables
    fclose(config);
    vector_free(root_nodes);
    free(buff);

    // Free tree and exit if error
    if (error) {
        tree_free(tree);
        exit(1);
    }

    // Return tree
    return tree;
}

void tree_free(Tree* t) {
    // Free all nodes from the head and self
    node_free(t->head);
    free(t);
}
