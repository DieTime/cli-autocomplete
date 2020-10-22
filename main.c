#include <stdio.h>

//#include "include/tree.h"
//#include "include/token_vector.h"
//#include "include/autocomplete.h"
#include "include/vector.h"

//
//void print_tree(Node* head) {
//    printf("%s -> ", head->token);
//    node_vector_print(head->children);
//    for (unsigned i = 0; i < head->children->length; i++) {
//        print_tree(node_vector_get(head->children, i));
//    }
//}

void vector_print(Vector* vec) {
    printf("[ ");
    for (int i = 0; i < vec->length; i++) {
        int x = *(int*)vector_get(vec, i);
        printf("%d, ", x);
    }
    printf("]\n");
}

int main() {
    Vector* vec = vector_create(0, sizeof(int));

    int x = 0;
    vector_push(vec, &x);
    x = 2;
    vector_push(vec, &x);
    x = 4;
    vector_push(vec, &x);

    vector_print(vec);

    vector_free(vec);
//    Tree* tree = tree_create("../config.txt");
//
//    TokenVector* maybe = get_predictions(tree, "git ");
//    token_vector_print(maybe);
//    token_vector_free(maybe);
//
//    tree_free(tree);
    return 0;
}