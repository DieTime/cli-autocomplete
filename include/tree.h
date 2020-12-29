#ifndef AUTOCOMPLETE_TREE_H
#define AUTOCOMPLETE_TREE_H

#include "node.h"

#if defined(_WIN32) || defined(__CYGWIN__) || defined(_WIN64)
    #if defined(BUILD_SHARED)
        #define LIB extern __declspec(dllexport)
    #else
        #define LIB
    #endif
#elif defined(__APPLE__) || defined(__unix__) || defined(__unix) || defined(unix) || defined(__linux__)
    #define LIB extern __attribute__((visibility("default")))
#else
    #error unsupported platform
#endif

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
LIB Tree* tree_create(const char* filepath);

/**
 * Function for tree deallocating
 *
 * @param tree - Tree structure for deallocating
 */
LIB void tree_free(Tree* tree);

#endif // AUTOCOMPLETE_TREE_H