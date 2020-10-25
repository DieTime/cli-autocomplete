#include <stdio.h>

#include "include/tree.h"
#include "include/predictions.h"

// TODO Fix error checking if tab diff > 1

int main() {
    Tree* tree = tree_create("../config.txt");

    Predictions* pred = predictions_create(tree, "git commit ");
    for (int i = 0; i < pred->tokens->length; i++) {
        printf("\"%s\", ", (char*)vector_get(pred->tokens, i));
    }
    printf("\n");
    printf("status = %d\n", pred->type);

    predictions_free(pred);
    tree_free(tree);
    return 0;
}
