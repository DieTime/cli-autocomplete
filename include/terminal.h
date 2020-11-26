#pragma once
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
        #include <conio.h>
    #endif
#elif defined(unix) || defined(__unix__) || defined(__unix)
    #ifndef OS_UNIX
        #define OS_UNIX
        #define COLOR_TYPE char*

        #include <termios.h>
        #include <unistd.h>
        #include <sys/ioctl.h>
    #endif
#else
    #error Unknown environment!
#endif

short terminal_width();

void color_print(char* text, COLOR_TYPE color);

void clear_line();

void set_cursor_x(short x);

short get_cursor_y();

#if defined(OS_UNIX)
int _getch();
#endif

#endif //AUTOCOMPLETE_TERMINAL_H
