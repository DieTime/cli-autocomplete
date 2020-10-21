#include <stdio.h>

#include "include/tree.h"

void print_tree(Node* head) {
    printf("%s -> ", head->token);
    node_vector_print(head->children);
    for (size_t i = 0; i < head->children->length; i++) {
        print_tree(node_vector_get(head->children, i));
    }
}

int main() {
    Tree* tree = tree_create("../config.txt");
    print_tree(tree->head);
    tree_free(tree);
    return 0;
}