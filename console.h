#pragma once
#ifndef AUTO_COMPLETE_CONSOLE_H
#define AUTO_COMPLETE_CONSOLE_H

#if defined(_WIN32) || defined(_WIN64)
    #define OS_WINDOWS
#elif defined(__APPLE__) || defined(__unix__) || defined(__unix)
    #define OS_UNIX
#else
    #error unsupported platform
#endif

#if defined(OS_WINDOWS)
    #include <conio.h>
    #include <Windows.h>
#elif defined(OS_UNIX)
    #include <unistd.h>
    #include <termios.h>
    #include <csignal>
#endif

#include <iostream>
#include <string>
#include <vector>

#if defined(OS_WINDOWS)
    #define CLEAR_LINE "\r" + std::string(64, ' ')
    #define ENTER 13
    #define BACKSPACE 8
    #define CTRL_C 3
    #define LEFT 75
    #define RIGHT 77
    #define DEL 83
#elif defined(OS_UNIX)
    #define CLEAR_LINE "\33[2K"
    #define ENTER 10
    #define BACKSPACE 127
    #define LEFT 68
    #define RIGHT 67
    #define DEL 51
#endif
    #define TAB 9

#if defined(OS_UNIX)
/**
 * Read key without press ENTER
 *
 * @return Code of key on keyboard.
 */
int _getch() {
    int ch;
    struct termios oldt, newt;
    tcgetattr( STDIN_FILENO, &oldt );
    newt = oldt;
    newt.c_lflag &= ~( ICANON | ECHO );
    tcsetattr( STDIN_FILENO, TCSANOW, &newt );
    ch = getchar();
    tcsetattr( STDIN_FILENO, TCSANOW, &oldt );
    return ch;
}
#endif

/**
 * Sets the console color to gray
 *
 * @param os Output stream.
 * @return input parameter os.
 */
std::ostream& gray(std::ostream& os) {
#if defined(OS_WINDOWS)
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 8);
#elif defined(OS_UNIX)
    std::cout << "\033[90m";
#endif
    return os;
}

/**
 * Sets the console color to white
 *
 * @param os Output stream.
 * @return input parameter os.
 */
std::ostream& white(std::ostream& os) {
#if defined(OS_WINDOWS)
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 7);
#elif defined(OS_UNIX)
    std::cout << "\033[37m";
#endif
    return os;
}

/**
 * Gets current terminal cursor position
 *
 * @return Y position of terminal cursor.
 */
int cursorY() {
#if defined(OS_WINDOWS)
    CONSOLE_SCREEN_BUFFER_INFO info;
    GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &info);
    return info.dwCursorPosition.Y;
#elif defined(OS_UNIX)
    char buf[30]={0};
    int i, pow;
    char ch;
    struct termios term, restore;
    int y = 0;

    tcgetattr(0, &term);
    tcgetattr(0, &restore);
    term.c_lflag &= ~(ICANON|ECHO);
    tcsetattr(0, TCSANOW, &term);

    write(1, "\033[6n", 4);

    for (i = 0, ch = 0; ch != 'R'; i++) {
        read(0, &ch, 1);
        buf[i] = ch;
    }

    for (i-- , pow = 1; buf[i] != '['; i--, pow *= 10) {
        y += ( buf[i] - '0' ) * pow;
    }

    tcsetattr(0, TCSANOW, &restore);
    return y;
#endif
}

/**
 * Move terminal cursor at position x
 *
 * @param x Position to move.
 * @return void.
 */
void gotoX(size_t x) {
#if defined(OS_WINDOWS)
    COORD xy;
    xy.X = x - 1;
    xy.Y = cursorY();
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), xy);
#elif defined(OS_UNIX)
    printf("\033[%d;%dH", cursorY(), (int)x);
#endif
}

#endif //AUTO_COMPLETE_CONSOLE_H
