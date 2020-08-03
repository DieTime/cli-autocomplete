#if defined(_WIN32) || defined(_WIN64)
    #define OS_WINDOWS
#elif defined(__APPLE__) || defined(__unix__) || defined(__unix)
    #define OS_UNIX
#else
    #error unsupported platform
#endif

#if defined(OS_WINDOWS)
    #include <Windows.h>
#elif defined(OS_UNIX)
    #include <unistd.h>
    #include <termios.h>
    #include <csignal>
#endif

#include <iostream>

#include "../include/autocomplete.h"

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

    short width = --info.dwSize.X;
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
 * Sets the console color.
 *
 * @param color System code of target color.
 * @return Input parameter os.
 */
#if defined(OS_WINDOWS)
std::string set_console_color(uint16_t color) {
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), color);
    return "";
#elif defined(OS_UNIX)
std::string set_console_color(std::string color) {
    return "\033[" + color + "m";
#endif
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
    short i = 0, pow = 1, y = 0;

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