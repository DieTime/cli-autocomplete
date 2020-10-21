#include <stdio.h>

#include "include/tree.h"
#include "include/token_vector.h"

void print_tree(Node* head) {
    printf("%s -> ", head->token);
    node_vector_print(head->children);
    for (unsigned i = 0; i < head->children->length; i++) {
        print_tree(node_vector_get(head->children, i));
    }
}

int main() {
    Tree* tree = tree_create("../config.txt");
    print_tree(tree->head);
    tree_free(tree);

    TokenVector* tokens = token_vector_create("hello world ", ' ');
    token_vector_print(tokens);
    token_vector_free(tokens);
    return 0;
}