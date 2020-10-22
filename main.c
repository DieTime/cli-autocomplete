#include <stdio.h>

#include "include/tree.h"
#include "include/autocomplete.h"

int main() {
    Tree* tree = tree_create("../config.txt");

    Tokens* vector = get_predictions(tree, "git c");
    for (int i = 0; i < vector->length; i++) {
        printf("%s, ", (char*)vector_get(vector, i));
    }
    printf("\n");

    free_tokens(vector);
    tree_free(tree);
    return 0;
}