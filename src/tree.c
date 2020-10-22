#ifdef _MSC_VER
#define _CRT_SECURE_NO_WARNINGS
#endif

#include <stdio.h>
#include <stdlib.h>

#include "../include/tree.h"

Tree* tree_create(const char* filepath) {
    // Get max token length
    unsigned buff_length = 64;

    // Create buffer for parsing
    char* buff = (char*)malloc(sizeof(char) * buff_length);
    if (buff == NULL) {
        fprintf(stderr, "[ERROR] Bad buffer memory allocation\n");
        exit(1);
    }
    unsigned buff_i;

    unsigned space_counter;    // Counter of spaces before token
    unsigned tab_length = 0;   // Count of spaces in one tabulation
    unsigned tab_count;        // Count of tabulations in line
    unsigned line_counter = 0; // Counter of lines in config file

    char character; // Variable for reading char by char

    // Initialize tree and tree head
    Tree* tree = (Tree*)malloc(sizeof(Tree));
    if (tree == NULL) {
        fprintf(stderr, "[ERROR] Bad tree memory allocation\n");
        exit(1);
    }
    tree->head = node_create("\0", buff_length);

    // Vector of root nodes for parsing
    Vector* root_nodes = vector_create(1);
    vector_push(root_nodes, (void*)tree->head);

    // Try open file for reading
    FILE* config = fopen(filepath, "r");
    if (config == NULL) {
        fprintf(stderr, "[ERROR] Can't open file %s\n", filepath);
        free(buff);
        tree_free(tree);
        vector_free(root_nodes);
        return NULL;
    }

    // Start parsing
    character = (char)fgetc(config);
    while (character != EOF) {
        line_counter += 1;
        space_counter = 0;
        buff_i = 0;

        // Don't use tab character!!!
        if (character == '\t') {
            fprintf(stderr, "[ERROR] Tab character detected in line %d, use a sequence of spaces\n", line_counter);
            fclose(config);
            vector_free(root_nodes);
            tree_free(tree);
            return NULL;
        }

        // Get space count
        while (character == ' ') {
            space_counter += 1;
            character = (char)fgetc(config);
        }

        // Init correct tab length
        if (space_counter > tab_length && tab_length == 0) {
            tab_length = space_counter;
        }

        // Check count of spaces for correctness
        if (space_counter % MAX_OF(tab_length, 1) != 0) {
            fprintf(stderr, "[ERROR] Incorrect tab length in line %d\n", line_counter);
            fclose(config);
            vector_free(root_nodes);
            tree_free(tree);
            free(buff);
            return NULL;
        }

        // Get tabulations count;
        tab_count = space_counter / MAX_OF(tab_length, 1);

        // Get token
        while (character != '\n' && character != '\r' && character != EOF) {
            buff[buff_i++] = character;
            character = (char)fgetc(config);
        }
        buff[buff_i] = '\0';

        // Create node by token
        Node* n = node_create(buff, buff_length);

        // Push node to tree
        vector_push(((Node*)vector_get(root_nodes, tab_count))->children, (void*)n);

        // Add token to root tokens vector by index as tab count
        if (tab_count + 1 < root_nodes->length) {
            vector_set(root_nodes, tab_count + 1, n);
        }
        else if (tab_count + 1 == root_nodes->length) {
            vector_push(root_nodes, (void*)n);
        } else {
            fprintf(stderr, "[ERROR] The token on line %d does not belong to any token\n", line_counter);
            fclose(config);
            vector_free(root_nodes);
            tree_free(tree);
            free(buff);
            return NULL;
        }

        // Go to next line
        while (character != ' ' && character != EOF) {
            character = (char)fgetc(config);
        }
    }

    // Remove temporary variables
    fclose(config);
    vector_free(root_nodes);
    free(buff);

    // Return tree
    return tree;
}

void tree_free(Tree* t) {
    // Free all nodes from the head and self
    node_free(t->head);
    free(t);
}
