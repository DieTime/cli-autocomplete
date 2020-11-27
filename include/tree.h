#pragma once
#ifndef AUTOCOMPLETE_TREE_H
#define AUTOCOMPLETE_TREE_H

#include "node.h"

/**
 * Tree structure which contains
 * self head node
 */
struct tree {
    Node* head;
};
typedef struct tree Tree;

/**
 * Parsing configuration file for autocomplete
 * rules in the form of a tree
 *
 * @param filepath - Path to configuration file
 *
 * @return Rules in the form of a tree
 */
Tree* tree_create(const char* filepath);

/**
 * Function for tree deallocating
 *
 * @param tree - Tree structure for deallocating
 */
void tree_free(Tree* tree);

#endif // AUTOCOMPLETE_TREE_H