#ifndef AUTOCOMPLETE_TERMINAL_H
#define AUTOCOMPLETE_TERMINAL_H

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#if defined(_WIN32) || defined(__CYGWIN__)
    #ifndef OS_WINDOWS
        #define OS_WINDOWS
        #define COLOR_TYPE uint16_t
        #if defined(BUILD_SHARED)
            #define LIB extern __declspec(dllexport)
        #else
            #define LIB
        #endif

        #include <windows.h>
        #include <conio.h>
    #endif
#elif defined(unix) || defined(__unix__) || defined(__unix)
    #ifndef OS_UNIX
        #define OS_UNIX
        #define COLOR_TYPE char*
        #define LIB extern __attribute__((visibility("default")))

        #include <termios.h>
        #include <unistd.h>
        #include <sys/ioctl.h>
    #endif
#else
    #error Unknown environment!
#endif

#if defined(OS_WINDOWS)
    // https://stackoverflow.com/questions/4053837/colorizing-text-in-the-console-with-c#answer-4053879
    #define DEFAULT_TITLE_COLOR 160
    #define DEFAULT_PREDICT_COLOR 8
    #define DEFAULT_MAIN_COLOR 7
#elif defined(OS_UNIX)
    // https://stackoverflow.com/questions/2616906/how-do-i-output-coloured-text-to-a-linux-terminal#answer-45300654
    #define DEFAULT_TITLE_COLOR "0;30;102"
    #define DEFAULT_PREDICT_COLOR "90"
    #define DEFAULT_MAIN_COLOR "0"
#endif

/**
 * Function for getting current
 * terminal width (cols count)
 *
 * @return Count of terminal cols
 */
LIB short terminal_width();

/**
 * Printing text with color in terminal
 *
 * @param text - Printable text
 * @param color - Color for printing
 */
LIB void color_print(char* text, COLOR_TYPE color);

/**
 * Function for clear all content
 * in current line
 */
LIB void clear_line();

/**
 * Set cursor X position in current row
 *
 * @param x - Position for moving
 */
LIB void set_cursor_x(short x);

/**
 * Function for getting current
 * cursor Y position
 *
 * @return Current cursor Y position
 */
LIB short get_cursor_y();

#if defined(OS_UNIX)
/**
 * Implementation of getch() function
 * for UNIX systems
 *
 * @return Pressed keyboard character
 */
LIB int _getch();
#endif

#endif //AUTOCOMPLETE_TERMINAL_H
