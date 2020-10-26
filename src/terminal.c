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
        fprintf(stderr, "[ERROR] Couldn't reset terminal mode");
        exit(1);
    }
#elif defined(OS_UNIX)
    struct termios old_attr, new_attr;

    // Backup terminal attributes
    if (tcgetattr( STDIN_FILENO, &old_attr ) == -1) {
        fprintf(stderr, "[ERROR] Couldn't get terminal attributes");
        exit(1);
    }

    // Disable echo
    new_attr = old_attr;
    new_attr.c_lflag &= ~(ICANON | ECHO );
    if (tcsetattr( STDIN_FILENO, TCSANOW, &new_attr) == -1) {
        fprintf(stderr, "[ERROR] Couldn't set terminal attributes");
        exit(1);
    }

    // Get input character
    character = getchar();

    // Restore terminal attributes
    if (tcsetattr( STDIN_FILENO, TCSANOW, &old_attr) == -1) {
        fprintf(stderr, "[ERROR] Couldn't reset terminal attributes");
        exit(1);
    }
#endif

    return character;
}
