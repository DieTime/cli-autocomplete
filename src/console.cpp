#pragma once
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
    #define ENTER 13
    #define BACKSPACE 8
    #define CTRL_C 3
    #define LEFT 75
    #define RIGHT 77
    #define DEL 83
    #define UP 72
    #define DOWN 80
    #define SPACE 32
#elif defined(OS_UNIX)
    #define ENTER 10
    #define BACKSPACE 127
    #define SPACE 32
    #define LEFT 68
    #define RIGHT 67
    #define UP 65
    #define DOWN 66
    #define DEL 51
#endif
    #define TAB 9

#if defined(OS_UNIX)
/**
 * Read key without press ENTER.
 *
 * @return Code of key on keyboard.
 */
int _getch() {
    int ch;
    struct termios old_termios, new_termios;
    tcgetattr( STDIN_FILENO, &old_termios );
    new_termios = old_termios;
    new_termios.c_lflag &= ~(ICANON | ECHO );
    tcsetattr( STDIN_FILENO, TCSANOW, &new_termios );
    ch = getchar();
    tcsetattr( STDIN_FILENO, TCSANOW, &old_termios );
    return ch;
}
#endif

#if defined(OS_WINDOWS)
/**
 * Get count of terminal cols.
 *
 * @return Width of terminal.
 */
size_t console_width() {
    CONSOLE_SCREEN_BUFFER_INFO info;
    GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &info);

    short width = info.dwSize.X - 1;
    return size_t((width < 0) ? 0 : width);
}
#endif

/**
 * Clear terminal line.
 *
 * @param os Output stream.
 * @return input parameter os.
 */
std::ostream& clear_line(std::ostream& os) {
#if defined(OS_WINDOWS)
    size_t width = console_width();
    os << '\r' << std::string(width, ' ');
#elif defined(OS_UNIX)
    std::cout << "\033[2K";
#endif
    return os;
}

/**
 * Sets the console color to gray.
 *
 * @param os Output stream.
 * @return input parameter os.
 */
std::ostream& set_predict_color(std::ostream& os) {
#if defined(OS_WINDOWS)
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 8);
#elif defined(OS_UNIX)
    std::cout << "\033[90m";
#endif
    return os;
}

/**
 * Sets the console color to default.
 *
 * @param os Output stream.
 * @return Input parameter os.
 */
std::ostream& set_primary_color(std::ostream& os) {
#if defined(OS_WINDOWS)
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 7);
#elif defined(OS_UNIX)
    std::cout << "\033[0m";
#endif
    return os;
}

/**
 * Gets current terminal cursor position.
 *
 * @return Y position of terminal cursor.
 */
short cursor_y_pos() {
#if defined(OS_WINDOWS)
    CONSOLE_SCREEN_BUFFER_INFO info;
    GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &info);
    return info.dwCursorPosition.Y;
#elif defined(OS_UNIX)
    struct termios term, restore;
    char ch, buf[30] = {0};
    int i = 0, pow = 1;
    short y = 0;

    tcgetattr(0, &term);
    tcgetattr(0, &restore);
    term.c_lflag &= ~(ICANON|ECHO);
    tcsetattr(0, TCSANOW, &term);

    write(1, "\033[6n", 4);

    for (ch = 0; ch != 'R'; i++) {
        read(0, &ch, 1);
        buf[i] = ch;
    }

    i -= 2;
    while (buf[i] != ';') {
        i -= 1;
    }

    i -= 1;
    while (buf[i] != '[') {
        y = y + ( buf[i] - '0' ) * pow;
        pow *= 10;
        i -= 1;
    }

    tcsetattr(0, TCSANOW, &restore);
    return y;
#endif
}

/**
 * Move terminal cursor at position x and y.
 *
 * @param x X position to move.
 * @param x Y position to move.
 * @return Void.
 */
void goto_xy(short x, short y) {
#if defined(OS_WINDOWS)
    COORD xy {--x, y};
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), xy);
#elif defined(OS_UNIX)
    printf("\033[%d;%dH", y, x);
#endif
}