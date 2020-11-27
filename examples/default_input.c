#include <stdio.h>

#include "../include/autocomplete.h"

int main() {
    // Parsing the configuration file
    Tree* rules = tree_create("../config.txt");

    fprintf(
        stderr,
        "Attention! Please run the executable file only\n"
        "           through the command line!\n\n"

        "- To switch the prompts press UP or DOWN arrow.\n"
        "- To move cursor press LEFT or RIGHT arrow.\n"
        "- To edit input press DELETE or BACKSPACE key.\n"
        "- To apply current prompt press TAB key.\n\n"
    );

    // Listening process
    while (1) {
        // Get user input
        char* str = input(rules);
        printf("\n%s\n", str);

        // Stop listening if user need
        if (strcmp(str, "") == 0) {
            free(str);
            break;
        }

        // Free user input string
        free(str);
    }

    // Free rules
    tree_free(rules);

    return 0;
}
