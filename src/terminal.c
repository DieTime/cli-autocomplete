#include "../include/terminal.h"

void color_print(char* text, COLOR_TYPE color) {
#if defined(OS_WINDOWS)
    HANDLE h_console = GetStdHandle(STD_OUTPUT_HANDLE);
    if (h_console == NULL) {
        fprintf(stderr, "[ERROR] Couldn't handle terminal\n");
        exit(1);
    }

    CONSOLE_SCREEN_BUFFER_INFO console_info;
    COLOR_TYPE backup;

    // Save current attributes
    if (GetConsoleScreenBufferInfo(h_console, &console_info) == 0) {
        fprintf(stderr, "[ERROR] Couldn't get terminal info\n");
        exit(1);
    }
    backup = console_info.wAttributes;

    // Print colored text
    if (SetConsoleTextAttribute(h_console, color) == 0) {
        fprintf(stderr, "[ERROR] Couldn't set terminal color\n");
        exit(1);
    }

    printf("%s", text);

    // Restore original color
    if (SetConsoleTextAttribute(h_console, backup) == 0) {
        fprintf(stderr, "[ERROR] Couldn't reset terminal color\n");
        exit(1);
    }
#elif defined(OS_UNIX)
    //Set new terminal color
    printf("\033[");
    printf("%s", color);
    printf("m");

    // Print colored text
    printf("%s", text);

    //Resets the text to default color
    printf("\033[0m");
#endif
}

int getch_() {
    int character;

#if defined(OS_WINDOWS)
    DWORD mode, n;

    HANDLE h_console = GetStdHandle(STD_INPUT_HANDLE );
    if (h_console == NULL) {
        fprintf(stderr, "[ERROR] Couldn't handle terminal\n");
        exit(1);
    }

    // Backup terminal mode
    if (GetConsoleMode(h_console, &mode ) == 0) {
        fprintf(stderr, "[ERROR] Couldn't get terminal mode\n");
        exit(1);
    }

    // Disable echo
    if (SetConsoleMode(h_console, mode & ~(ENABLE_LINE_INPUT | ENABLE_ECHO_INPUT)) == 0) {
        fprintf(stderr, "[ERROR] Couldn't set terminal mode\n");
        exit(1);
    }

    // Read character
    if (ReadConsole(h_console, &character, 1, &n, NULL) == 0) {
        fprintf(stderr, "[ERROR] Couldn't get character from terminal\n");
        exit(1);
    }

    // Validate reading
    if (n != 1) {
        fprintf(stderr, "[ERROR] The character was read from the terminal incorrectly\n");
        exit(1);
    }

    // Reset terminal mode
    if (SetConsoleMode(h_console, mode) == 0) {
        fprintf(stderr, "[ERROR] Couldn't reset terminal mode\n");
        exit(1);
    }
#elif defined(OS_UNIX)
    struct termios old_attr, new_attr;

    // Backup terminal attributes
    if (tcgetattr(STDIN_FILENO, &old_attr ) == -1) {
        fprintf(stderr, "[ERROR] Couldn't get terminal attributes\n");
        exit(1);
    }

    // Disable echo
    new_attr = old_attr;
    new_attr.c_lflag &= ~(ICANON | ECHO );
    if (tcsetattr(STDIN_FILENO, TCSANOW, &new_attr) == -1) {
        fprintf(stderr, "[ERROR] Couldn't set terminal attributes\n");
        exit(1);
    }

    // Get input character
    character = getchar();

    // Restore terminal attributes
    if (tcsetattr(STDIN_FILENO, TCSANOW, &old_attr) == -1) {
        fprintf(stderr, "[ERROR] Couldn't reset terminal attributes\n");
        exit(1);
    }
#endif

    return character;
}

void clear_line() {
#if defined(OS_WINDOWS)
    HANDLE h_console = GetStdHandle(STD_OUTPUT_HANDLE);
    if (h_console == NULL) {
        fprintf(stderr, "[ERROR] Couldn't handle terminal\n");
        exit(1);
    }

    // Save current attributes
    CONSOLE_SCREEN_BUFFER_INFO console_info;
    if (GetConsoleScreenBufferInfo(h_console, &console_info) == 0) {
        fprintf(stderr, "[ERROR] Couldn't get terminal info\n");
        exit(1);
    }

    short width = console_info.dwSize.X;

    printf("\r");
    for (short i = 0; i < width - 1; i++) {
        printf(" ");
    }
#elif defined(OS_UNIX)
    printf("\033[2K");
#endif
}

void set_cursor(int x, int y) {
#if defined(OS_WINDOWS)
    HANDLE h_console = GetStdHandle(STD_OUTPUT_HANDLE);
    if (h_console == NULL) {
        fprintf(stderr, "[ERROR] Couldn't handle terminal\n");
        exit(1);
    }

    // Create position
    COORD xy;
    xy.X = x - 1;
    xy.Y = y;

    // Set cursor position
    if (SetConsoleCursorPosition(h_console, xy) == 0) {
        fprintf(stderr, "[ERROR] Couldn't set terminal cursor position\n");
        exit(1);
    }
#elif defined(OS_UNIX)
    printf("\033[%d;%dH", y, x);
#endif
}

short get_cursor_y() {
#if defined(OS_WINDOWS)
    HANDLE h_console = GetStdHandle(STD_OUTPUT_HANDLE);
    if (h_console == NULL) {
        fprintf(stderr, "[ERROR] Couldn't handle terminal\n");
        exit(1);
    }

    // Get terminal info
    CONSOLE_SCREEN_BUFFER_INFO console_info;
    if (GetConsoleScreenBufferInfo(h_console, &console_info) == 0) {
        fprintf(stderr, "[ERROR] Couldn't get terminal Y position\n");
        exit(1);
    }

    // Return Y position
    return console_info.dwCursorPosition.Y;
#elif defined(OS_UNIX)
    struct termios old_attr, new_attr;
    char ch, buf[30] = {0};
    int i = 0, pow = 1, y = 0;

    // Backup terminal attributes
    if (tcgetattr(STDIN_FILENO, &new_attr) == -1) {
        fprintf(stderr, "[ERROR] Couldn't get terminal attributes\n");
        exit(1);
    }

    // Disable echo
    old_attr = new_attr;
    old_attr.c_lflag &= ~(ICANON | ECHO);
    if (tcsetattr(STDIN_FILENO, TCSANOW, &old_attr) == -1) {
        fprintf(stderr, "[ERROR] Couldn't set terminal attributes\n");
        exit(1);
    }

    // Get info about cursor
    if (write(STDOUT_FILENO, "\033[6n", 4) != 4) {
        fprintf(stderr, "[ERROR] Couldn't get cursor information\n");
        exit(1);
    }

    // Get ^[[{this};1R value

    for (ch = 0; ch != 'R'; i++) {
        if (read(STDIN_FILENO, &ch, 1) == 1) {
            fprintf(stderr, "[ERROR] Couldn't read cursor information");
            exit(1);
        }
        buf[i] = ch;
    }

    i -= 2;
    while (buf[i] != ';') {
        i -= 1;
    }

    i -= 1;
    while (buf[i] != '[') {
        y = y + (buf[i] - '0') * pow;
        pow *= 10;
        i -= 1;
    }

    // Reset attributes
    if (tcsetattr(0, TCSANOW, &new_attr) == -1) {
        fprintf(stderr, "[ERROR] Couldn't reset terminal attributes\n");
        exit(1);
    }

    return (short)y;
#endif
}