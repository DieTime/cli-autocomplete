#include <stdio.h>

#include "include/autocomplete.h"

int main() {
    // Parsing the configuration file
    Tree* rules = tree_create("../config.txt");

#if defined(OS_WINDOWS)
    // https://stackoverflow.com/questions/4053837/colorizing-text-in-the-console-with-c#answer-4053879
    COLOR_TYPE title_color = 160;
    COLOR_TYPE predict_color = 8;
    COLOR_TYPE main_color = 7;
#elif defined(OS_UNIX)
    // https://stackoverflow.com/questions/2616906/how-do-i-output-coloured-text-to-a-linux-terminal#answer-45300654
    // Set the value that goes between \033 and m ( \033{your_value}m )
    COLOR_TYPE title_color = "0;30;102";
    COLOR_TYPE predict_color = "90";
    COLOR_TYPE main_color = "0";
#endif

    // Characters which optional values begin ( optional parameter )
    char* optional_brackets = "[{<";

    fprintf(
        stderr,
        "Attention! Please run the executable file only\n"
        "           through the command line!\n\n"

        "- To switch the prompts press UP or DOWN arrow.\n"
        "- To move cursor press LEFT or RIGHT arrow.\n"
        "- To edit input press DELETE or BACKSPACE key.\n"
        "- To apply current prompt press TAB key.\n\n"
    );

    // Value for line title
    unsigned command_counter = 0;

    // Title for custom input function
    char title[64] = {0};

    // Listening process
    while (1) {
        // Prepare line title ( optional parameter )
        sprintf(title, "%s [%u]", "git", command_counter);

        char* str = custom_input(rules, title, title_color, predict_color, main_color, optional_brackets);
        printf("\n%s\n", str);
        free(str);

        command_counter += 1;
    }

    tree_free(rules);
    return 0;
}
