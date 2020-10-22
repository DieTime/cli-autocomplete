#include <stdio.h>

#include "include/tree.h"
#include "include/token_vector.h"
#include "include/autocomplete.h"

void print_tree(Node* head) {
    printf("%s -> ", head->token);
    node_vector_print(head->children);
    for (unsigned i = 0; i < head->children->length; i++) {
        print_tree(node_vector_get(head->children, i));
    }
}

int main() {
    Tree* tree = tree_create("../config.txt");

    TokenVector* maybe = get_predictions(tree, "git ");
    token_vector_print(maybe);
    token_vector_free(maybe);

    tree_free(tree);
    return 0;
}