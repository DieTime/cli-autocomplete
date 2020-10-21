#include <stdio.h>

#include "include/tree.h"
#include "include/autocomplete.h"

void print_tree(Node* head) {
    printf("%s -> ", head->token);
    node_vector_print(head->children);
    for (size_t i = 0; i < head->children->length; i++) {
        print_tree(node_vector_get(head->children, i));
    }
}

int main() {
//    Tree* tree = tree_create("../config.txt");
//    print_tree(tree->head);
//    tree_free(tree);

    char** tokens = tokenize("hello world", ' ');
    int i = 0;
    while (tokens[i][0] != '\0') {
        printf("\"%s\"\n", tokens[i++]);
    }
    free_tokens(tokens);
    return 0;
}