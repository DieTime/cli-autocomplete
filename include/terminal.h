#ifndef AUTOCOMPLETE_TERMINAL_H
#define AUTOCOMPLETE_TERMINAL_H

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#if defined(_WIN32) || defined(__CYGWIN__)
    #ifndef OS_WINDOWS
        #define OS_WINDOWS
        #define COLOR_TYPE uint16_t
        #include <windows.h>
    #endif
#elif defined(unix) || defined(__unix__) || defined(__unix)
    #ifndef OS_UNIX
        #define OS_UNIX
        #define COLOR_TYPE char*
        #include <termios.h>
        #include <unistd.h>
    #endif
#else
    #error Unknown environment!
#endif

void color_print(char* text, COLOR_TYPE color);

int getch_();

#endif //AUTOCOMPLETE_TERMINAL_H
