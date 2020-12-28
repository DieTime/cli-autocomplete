#ifndef AUTOCOMPLETE_AUTOCOMPLETE_H
#define AUTOCOMPLETE_AUTOCOMPLETE_H

#include <string.h>

#include "tree.h"
#include "terminal.h"

#if defined(_WIN32) || defined(__CYGWIN__) || defined(_WIN64)
    #ifndef OS_WINDOWS
        #define OS_WINDOWS
        #if defined(BUILD_SHARED)
            #define LIB extern __declspec(dllexport)
        #else
            #define LIB
        #endif
    #endif
#elif defined(unix) || defined(__unix__) || defined(__unix)
    #ifndef OS_UNIX
        #define OS_UNIX
        #define LIB extern __attribute__((visibility("default")))
    #endif
#else
    #error Unknown environment!
#endif

#if defined(OS_WINDOWS)
    #define ENTER 13
    #define BACKSPACE 8
    #define LEFT 75
    #define RIGHT 77
    #define UP 72
    #define DOWN 80
    #define DEL 83
    #define CTRL_C 3
    #define SPECIAL_SEQ_1 0
    #define SPECIAL_SEQ_2 224
#elif defined(OS_UNIX)
    #define ENTER 10
    #define BACKSPACE 127
    #define LEFT 68
    #define RIGHT 67
    #define UP 65
    #define DOWN 66
    #define DEL 51
    #define DEL_AFTER 126
    #define SPECIAL_SEQ_1 27
    #define SPECIAL_SEQ_2 91
#endif
#define SPACE 32
#define TAB 9

/**
 * Gives the user a customizable convenient input
 * interface with autocomplete
 *
 * @param rules - Parsed rules from config file
 * @param title - The line printed before the input cursor
 * @param title_color - Color type for title printing
 * @param predict_color - Color type for prediction printing
 * @param main_color - Color type for user input printing
 * @param optional_brackets - Characters which optional values begin
 *
 * @return User input string
 */
LIB char* custom_input(Tree* rules, char* title, COLOR_TYPE title_color, COLOR_TYPE predict_color,
            COLOR_TYPE main_color, char* optional_brackets);

/**
 * Gives the user a simple convenient input
 * interface with autocomplete
 *
 * @param rules - Parsed rules from config file
 *
 * @return User input string
 */
LIB char* input(Tree* rules);

/**
 * Function for checking if input
 * character in ignore list
 *
 * @param ch - Input character
 *
 * @return True if character in ignore list or False
 */
LIB int is_ignore_key(int ch);

#endif //AUTOCOMPLETE_AUTOCOMPLETE_H