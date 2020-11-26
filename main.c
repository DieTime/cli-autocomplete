#include <stdio.h>

//#include "include/tree.h"
//#include "include/predictions.h"
//#include "include/terminal.h"
#include "include/autocomplete.h"

int main() {
    Tree* rules = tree_create("../config.txt");

#if defined(OS_WINDOWS)
    COLOR_TYPE title_color = 160;
    COLOR_TYPE predict_color = 8;
    COLOR_TYPE main_color = 7;
#elif defined(OS_UNIX)
    COLOR_TYPE title_color = "0;30;102";
    COLOR_TYPE predict_color = "90";
    COLOR_TYPE main_color = "0";
#endif
    char* optional_brackets = "[{<";

    char* str = input(rules, "GIT", title_color, predict_color, main_color, optional_brackets);
    printf("%s", str);
    free(str);
//
//    Predictions* pred = predictions_create(rules, "git init ", optional_brackets);
//    predictions_free(pred);

    tree_free(rules);
    return 0;
}
