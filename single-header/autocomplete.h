#ifndef AUTOCOMPLETE_H
#define AUTOCOMPLETE_H

#ifdef _MSC_VER
#ifndef _CRT_SECURE_NO_WARNINGS
        #define _CRT_SECURE_NO_WARNINGS
    #endif
#endif

#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#define MAX_OF(x, y) (((x) > (y)) ? (x) : (y))

#if defined(_WIN32) || defined(__CYGWIN__) || defined(_WIN64)
    #ifndef OS_WINDOWS
        #define OS_WINDOWS

        #include <windows.h>
        #include <conio.h>
        #include <stdint.h>
    #endif
#elif defined(unix) || defined(__unix__) || defined(__unix)
    #ifndef OS_UNIX
        #define OS_UNIX

        #include <termios.h>
        #include <unistd.h>
        #include <sys/ioctl.h>
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
    #define COLOR_TYPE uint16_t
    #define DEFAULT_TITLE_COLOR 160
    #define DEFAULT_PREDICT_COLOR 8
    #define DEFAULT_MAIN_COLOR 7
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
    #define COLOR_TYPE char*
    #define DEFAULT_TITLE_COLOR "0;30;102"
    #define DEFAULT_PREDICT_COLOR "90"
    #define DEFAULT_MAIN_COLOR "0"
#endif
    #define SPACE 32
    #define TAB 9

/**
 * Vector implementation for contain
 * pointers of allocated data
 */
struct vector {
    void**   data;
    unsigned capacity;
    unsigned length;
};
typedef struct vector Vector;
typedef Vector Tokens;

/**
 * Node of rules tree
 *
 * Contains self token and
 * vector of children nodes
 */
struct node {
    char* token;
    struct vector* children;
};
typedef struct node Node;

enum predict_type {
    FAILURE  = 0,
    EXACTLY  = 1,
    PROBABLY = 2,
};
typedef enum predict_type PredictType;

/**
 * Structure containing vector of predictions
 * and prediction type
 */
struct predictions {
    PredictType type;
    Tokens* tokens;
};
typedef struct predictions Predictions;

/**
 * Tree structure which contains
 * self head node
 */
struct tree {
    Node* head;
};
typedef struct tree Tree;

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
char* custom_input(Tree* rules, char* title, COLOR_TYPE title_color, COLOR_TYPE predict_color,
                       COLOR_TYPE main_color, char* optional_brackets);

/**
 * Gives the user a simple convenient input
 * interface with autocomplete
 *
 * @param rules - Parsed rules from config file
 *
 * @return User input string
 */
char* input(Tree* rules);

/**
 * Function for checking if input
 * character in ignore list
 *
 * @param ch - Input character
 *
 * @return True if character in ignore list or False
 */
int is_ignore_key(int ch);

/**
 * Create node by token and token_length
 *
 * @param token - Node containing string
 * @param token_length - Length of string
 *
 * @return Created Node
 */
Node* node_create(char* token, unsigned token_length);

/**
 * Function for free Node
 * and all his children
 *
 * @param node - Node for deallocating
 */
void node_free(Node* node);

/**
 * Function for creating predictions
 * by rules and input string
 *
 * @param rules - Rules from config file
 * @param input - The entered string
 * @param optional_brackets - Characters which optional values begin
 *
 * @return Predictions for current input
 */
Predictions *predictions_create(Tree *rules, char *input, char *optional_brackets);

/**
 * Deallocating prediction struct
 * @param predict - Struct for deallocating
 */
void predictions_free(Predictions* predict);

/**
 * Split string to tokens by delimiter
 *
 * @param str - Input string
 * @param delimiter - Input delimiter character
 *
 * @return Vector of tokens
 */
Tokens* split(char *str, char delimiter);

/**
 * Function for token creation
 *
 * @param str - Input string
 * @param str_len - Length of input string
 *
 * @return Allocated token
 */
char* token_create(char* str, unsigned str_len);

/**
 * Function for deallocating
 * vector of tokens
 *
 * @param tokens - Vector for deallocating
 */
void tokens_free(Tokens* tokens);

/**
 * Function for checking if string
 * contains special characters
 *
 * @param str - Input string
 * @param chars - Special characters
 *
 * @return True if contains or False
 */
int contain_chars(const char* str, const char* chars);

/**
 * Function for getting current
 * terminal width (cols count)
 *
 * @return Count of terminal cols
 */
short terminal_width();

/**
 * Printing text with color in terminal
 *
 * @param text - Printable text
 * @param color - Color for printing
 */
void color_print(char* text, COLOR_TYPE color);

/**
 * Function for clear all content
 * in current line
 */
void clear_line();

/**
 * Set cursor X position in current row
 *
 * @param x - Position for moving
 */
void set_cursor_x(short x);

/**
 * Function for getting current
 * cursor Y position
 *
 * @return Current cursor Y position
 */
short get_cursor_y();

#if defined(OS_UNIX)
/**
 * Implementation of getch() function
 * for UNIX systems
 *
 * @return Pressed keyboard character
 */
int _getch();
#endif

/**
 * Parsing configuration file for autocomplete
 * rules in the form of a tree
 *
 * @param filepath - Path to configuration file
 *
 * @return Rules in the form of a tree
 */
Tree* tree_create(const char* filepath);

/**
 * Function for tree deallocating
 *
 * @param tree - Tree structure for deallocating
 */
void tree_free(Tree* tree);

/**
 * Function for allocating vector
 *
 * @param length - Initial length of vector
 * @return Vector structure
 */
Vector* vector_create(unsigned length);

/**
 * Function for peak value from
 * vector by index
 *
 * @param vec - Vector for getting info
 * @param index - Position of element
 *
 * @return Pointer of data by index
 */
void* vector_get(Vector* vec, unsigned index);

/**
 * Function for setting value in
 * vector by index
 *
 * @param vec - Vector for setting info
 * @param index - Position of element
 * @param item - Pointer of data for setting
 */
void vector_set(Vector* vec, unsigned index, void* item);

/**
 * Function for append data to vector
 *
 * @param vec - Vector for pushing
 * @param item - Item for pushing
 */
void vector_push(Vector* vec, void* item);

/**
 * Function for deallocating vector
 * @param vec - Vector for deallocating
 */
void vector_free(Vector* vec);

char* custom_input(Tree* rules, char* title, COLOR_TYPE title_color, COLOR_TYPE predict_color,
                   COLOR_TYPE main_color, char* optional_brackets) {
    // Initialize buffer for reading
    short buff_len = 0;
    short buff_cap = terminal_width();
    char* buff = (char*)calloc((size_t)buff_cap, sizeof(char));
    if (buff == NULL) {
        fprintf(stderr, "[ERROR] Couldn't allocate memory for buffer\n");
        exit(1);
    }

    // Cursor offset in buffer for moving
    int offset = 0;

    // Current hint number
    int hint_num = 0;

    // Calculate title length
    int title_len = (short)strlen(title);

    while (1) {
        // Print title with title color
        clear_line();
        color_print(title, title_color);
        printf(title_len != 0 ? " " : "");

        // Get length of last word in input
        short space_offset = 0;
        while ((buff_len - space_offset) != 0 && buff[buff_len - space_offset - 1] != ' ') {
            space_offset += 1;
        }

        // Print current buffer
        color_print(buff, main_color);

        // Get predictions
        Predictions* pred = predictions_create(rules, buff, optional_brackets);

        // Print prediction by hint_num with color
        if (pred->type != FAILURE) {
            char* prediction = (char*)vector_get(pred->tokens, hint_num % pred->tokens->length);

            // Print info message before prediction
            // if prediction is probably
            if (pred->type == PROBABLY) {
                color_print("  maybe you mean: ", predict_color);
            }

            // Print trimmed or not trimmed prediction depending on the type
            color_print(prediction + (pred->type == EXACTLY) * space_offset, predict_color);
        }

        // Move cursor to buffer end
        short x = (short)(buff_len + title_len + (title_len != 0) + 1 - offset);
        set_cursor_x(x);

        // Read character from console
        int ch = _getch();

        // Wait next symbol if character in ignore keys
        if (is_ignore_key(ch)) {
            continue;
        }

        // Return buffer if ENTER was pressed
        else if (ch == ENTER) {
            // Free predictions and break
            predictions_free(pred);
            break;
        }

        // Keyboard interrupt handler for Windows
        #if defined(OS_WINDOWS)
        else if (ch == CTRL_C) {
            predictions_free(pred);
            tree_free(rules);
            free(buff);
            exit(0);
        }
        #endif
        // Edit buffer like backspace if BACKSPACE was pressed
        else if (ch == BACKSPACE) {
            if (buff_len != 0 && buff_len - offset >= 1) {
                // Delete character from buffer
                for (unsigned i = (unsigned int)(buff_len - offset - 1); i < buff_cap - 1; i++) {
                    buff[i] = buff[i + 1];
                }
                buff_len -= 1;
            }
        }

        // Apply prediction if TAB was pressed
        else if (ch == TAB) {
            if (pred->type != FAILURE) {
                char* prediction = (char*)vector_get(pred->tokens, hint_num % pred->tokens->length);
                unsigned predict_len = (unsigned int)strlen(prediction);

                // Make sure the candidate has no optional brackets
                if (!contain_chars(prediction, optional_brackets)) {

                    // Handle buffer overflow
                    if (buff_len + predict_len - space_offset >= buff_cap) {
                        fprintf(stderr, "\n[ERROR] Input string more then terminal width\n");
                        exit(1);
                    }

                    // Append prediction to buffer
                    for (unsigned i = 0; i < predict_len; i++) {
                        buff[buff_len - space_offset] = prediction[i];
                        buff_len++;
                    }

                    // Fix buffer length
                    buff_len = buff_len - space_offset;

                    // Append end characters to buffer
                    buff[buff_len++] = ' ';
                    buff[buff_len] = '\0';
                }
            }
        }

        // Arrows and Delete keys handler
        else if (
            ch == SPECIAL_SEQ_1
        #if defined(OS_WINDOWS)
            || ch
        #elif defined(OS_UNIX)
            && _getch()
        #endif
            == SPECIAL_SEQ_2
        ) {
            switch (_getch()) {
                case LEFT:
                    // Increase offset from the end of the buffer if left key pressed
                    offset = (offset < buff_len) ? (offset + 1) : buff_len;
                    break;
                case RIGHT:
                    // Decrease offset from the end of the buffer if left key pressed
                    offset = (offset > 0) ? offset - 1 : 0;
                    break;
                case UP:
                    // Increase hint number
                    hint_num = hint_num + 1;
                    clear_line();
                    break;
                case DOWN:
                    // Decrease hint number
                    hint_num = hint_num - 1;
                    clear_line();
                    break;
                case DEL:
                    // Edit buffer like DELETE key
                #if defined(OS_UNIX)
                    if (_getch() == DEL_AFTER)
                #endif
                {
                    if (buff_len != 0 && offset != 0) {
                        // Delete character from buffer
                        for (unsigned i = (unsigned int)(buff_len - offset); i < buff_cap - 1; i++) {
                            buff[i] = buff[i + 1];
                        }
                        buff_len -= 1;
                        offset -= 1;
                    }
                }
                    break;
                default:
                    break;
            }
        }

        // Add character to buffer considering
        // offset if any key was pressed
        else {
            hint_num = ch == SPACE ? 0 : hint_num;
            buff[buff_len++] = (char)ch;
        }

        // Free predictions
        predictions_free(pred);
    }

    return buff;
}

char* input(Tree* rules) {
    return custom_input(rules, "", DEFAULT_TITLE_COLOR, DEFAULT_PREDICT_COLOR,
                        DEFAULT_MAIN_COLOR, "");
}

int is_ignore_key(int ch) {
    int ignore_keys[] =
    #if defined(OS_WINDOWS)
            {1, 2, 19, 24, 26};
    #elif defined(OS_UNIX)
            {1, 2, 4, 24};
    #endif

    // Calculate length of ignore keys array
    unsigned len = sizeof(ignore_keys) / sizeof(int);

    // Check if character is ignore key
    for (unsigned i = 0; i < len; i++) {
        if (ch == ignore_keys[i]) {
            return 1;
        }
    }

    return 0;
}

Node* node_create(char* token, unsigned token_length) {
    // Allocate memory for node
    Node* n = (Node*)malloc(sizeof(Node));
    if (n == NULL) {
        fprintf(stderr, "[ERROR] Bad node vector memory allocation\n");
        exit(1);
    }

    // Allocate memory for token field
    n->token = (char*)malloc(sizeof(char) * token_length);
    if (n->token == NULL) {
        fprintf(stderr, "[ERROR] Bad node vector memory allocation\n");
        exit(1);
    }

    // Setup fields of node
    memcpy(n->token, token, sizeof(char) * token_length);

    n->children = vector_create(1);

    return n;
}

void node_free(Node* n) {
    // Free all child nodes
    for (unsigned i = 0; i < n->children->length; i++) {
        node_free(vector_get(n->children, i));
    }

    // Free memory for children, token and self
    vector_free(n->children);
    free(n->token);
    free(n);
}

char* token_create(char* str, unsigned str_len) {
    // Allocate memory for token
    char* token = (char*)malloc(sizeof(char) * str_len + 1);
    if (token == NULL) {
        fprintf(stderr, "[ERROR] Bad token memory allocation\n");
        exit(0);
    }

    // Paste data to token
    memcpy(token, str, str_len);
    token[str_len] = '\0';

    // Return token
    return token;
}

Tokens* split(char *str, char delimiter) {
    // Create token vector
    Tokens* vec = vector_create(1);

    char prev_char = delimiter;

    // Finding tokens and placing them in an array
    for (unsigned i = 0; str[i] != '\0'; i++) {

        // Skip spaces while token not found
        if (prev_char == delimiter && str[i] != delimiter) {
            // Get length of current token
            unsigned token_length = 0;

            while (str[i + token_length] != delimiter && str[i + token_length] != '\0') {
                token_length += 1;
            }

            // Add token to vector
            vector_push(vec, token_create(str + i, token_length));

            // Move string cursor right
            i += token_length - 1;
        }

        // Remember prev character
        prev_char = str[i];
    }

    // Add empty string to vector if detected delimiter in the end
    if (prev_char == delimiter) {
        char* token = token_create("", 1);
        vector_push(vec, token);
    }

    // Return token vector
    return vec;
}

Predictions *predictions_create(Tree *rules, char *input, char *optional_brackets) {
    // Initialize result predictions
    Predictions* pred = (Predictions*)malloc(sizeof(Predictions));
    if (pred == NULL) {
        fprintf(stderr, "[ERROR] Bad prediction memory allocation\n");
        exit(0);
    }
    pred->type = EXACTLY;
    pred->tokens = vector_create(1);

    // Split input string to tokens
    Tokens* tokens = split(input, ' ');

    Node* curr_node = rules->head;
    Node* prev_node = rules->head;
    Vector* curr_children = curr_node->children;

    // Don't show predictions if last word contains optional brackets
    if (contain_chars((char*)vector_get(tokens, tokens->length - 1), optional_brackets)) {
        pred->type = FAILURE;
        return pred;
    }

    // Finding vector in tree by input tokens
    for (unsigned i = 0; i < tokens->length - 1; i++) {
        for (unsigned j = 0; j < curr_children->length; j++) {
            Node* temp_node = (Node*)vector_get(curr_children, j);
            char* token = (char*)vector_get(tokens, i);

            if (strcmp(temp_node->token, token) == 0) {
                curr_node = temp_node;
                curr_children = curr_node->children;
                break;
            }
        }

        // If further search makes no sense
        if (prev_node == curr_node) {
            pred->type = FAILURE;
            break;
        }

        // Remember node
        prev_node = curr_node;
    }

    // Search words starts with last token
    // if children nodes was found
    if (pred->type != FAILURE) {
        for (unsigned i = 0; i < curr_children->length; i++) {
            char* probably_token = ((Node*)vector_get(curr_children, i))->token;
            char* last_token = (char*)vector_get(tokens, tokens->length - 1);

            if (strncmp(last_token, probably_token, strlen(last_token)) == 0) {
                vector_push(pred->tokens, token_create(probably_token, (unsigned int)strlen(probably_token)));
            }
        }
    }

    // Set EXACTLY type for predictions if words was found
    // or search probably words without optional brackets
    // if words wasn't found
    if (pred->tokens->length > 0) {
        pred->type = EXACTLY;
    } else if (pred->type != FAILURE) {
        for (unsigned i = 0; i < curr_children->length; i++) {
            char *probably_token = ((Node *) vector_get(curr_children, i))->token;
            char *last_token = (char *) vector_get(tokens, tokens->length - 1);

            // Skip if candidate contain one of symbols for optional values
            if (contain_chars(probably_token, optional_brackets)) {
                continue;
            }

            // Total misses in probably token
            unsigned miss = 0;

            // Counting misses (no more than 2)
            for (unsigned j = 0; j < strlen(last_token); j++) {
                if (last_token[j] != probably_token[j]) {
                    if (++miss == 2) {
                        break;
                    }
                }
            }

            // Adding a word to predictions
            // if there are less than 2 misses
            if (miss < 2) {
                vector_push(pred->tokens, token_create(probably_token, (unsigned int)strlen(probably_token)));
            }
        }

        // Set type depending on the success of the search
        pred->type = pred->tokens->length > 0 ? PROBABLY : FAILURE;
    }

    // Free input tokens
    tokens_free(tokens);

    // Return result
    return pred;
}

void predictions_free(Predictions* predict) {
    // Free all tokens
    tokens_free(predict->tokens);

    // Free self
    free(predict);
}

void tokens_free(Tokens* tokens) {
    // Free all input tokens vector
    for (unsigned i = 0; i < tokens->length; i++) {
        free(vector_get(tokens, i));
    }

    // Free self
    vector_free(tokens);
}

int contain_chars(const char* str, const char* chars) {
    // Checking for given characters in a string
    for (unsigned i = 0; str[i] != '\0'; i++) {
        for (unsigned j = 0; chars[j] != '\0'; j++) {
            if (str[i] == chars[j]) {
                return 1;
            }
        }
    }

    return 0;
}

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

#if defined(OS_UNIX)
int _getch() {
    int character;
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

    return character;
}
#endif

short terminal_width() {
#if defined(OS_WINDOWS)
    // Handle current terminal
    HANDLE h_console = GetStdHandle(STD_OUTPUT_HANDLE);
    if (h_console == NULL) {
        fprintf(stderr, "[ERROR] Couldn't handle terminal\n");
        exit(1);
    }

    // Get current attributes
    CONSOLE_SCREEN_BUFFER_INFO console_info;
    if (GetConsoleScreenBufferInfo(h_console, &console_info) == 0) {
        fprintf(stderr, "[ERROR] Couldn't get terminal info\n");
        exit(1);
    }

    // Return current width
    return console_info.dwSize.X;
#elif defined(OS_UNIX)
    struct winsize t_size;

    if (ioctl(STDOUT_FILENO, TIOCGWINSZ, &t_size) == -1) {
        fprintf(stderr, "[ERROR] Couldn't get terminal info\n");
        exit(1);
    }

    return (short)t_size.ws_col;
#endif
}

void clear_line() {
#if defined(OS_WINDOWS)
    // Get current terminal width
    short width = terminal_width();
    if (width < 1) {
        fprintf(stderr, "[ERROR] Size of terminal is too small\n");
        exit(1);
    }

    // Create long empty string
    char* empty = (char*)malloc(sizeof(char) * width);
    memset(empty, ' ', width);
    empty[width - 1] = '\0';

    // Clear line
    printf("\r%s\r", empty);

    // Free line
    free(empty);
#elif defined(OS_UNIX)
    printf("\033[2K\r");
#endif
}

void set_cursor_x(short x) {
#if defined(OS_WINDOWS)
    HANDLE h_console = GetStdHandle(STD_OUTPUT_HANDLE);
    if (h_console == NULL) {
        fprintf(stderr, "[ERROR] Couldn't handle terminal\n");
        exit(1);
    }

    // Create position
    COORD xy;
    xy.X = x - 1;
    xy.Y = get_cursor_y();

    // Set cursor position
    if (SetConsoleCursorPosition(h_console, xy) == 0) {
        fprintf(stderr, "[ERROR] Couldn't set terminal cursor position\n");
        exit(1);
    }
#elif defined(OS_UNIX)
    printf("\033[%d;%dH", get_cursor_y(), x);
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
        if (read(STDIN_FILENO, &ch, 1) != 1) {
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

Tree* tree_create(const char* filepath) {
    int character; // Variable for reading char by char

    unsigned buff_length = 0;  // Max token length
    unsigned buff_counter = 0; // Variable for getting max token length

    // Try open file for reading
    FILE* config = fopen(filepath, "r");
    if (config == NULL) {
        fprintf(stderr, "[ERROR] Can't open file %s\n", filepath);
        exit(0);
    }

    // Check for empty config file
    character = fgetc(config);
    if (character == EOF) {
        fclose(config);
        fprintf(stderr, "[ERROR] %s file is empty\n", filepath);
        exit(0);
    }
    ungetc(character, config);

    // Get max token length
    while (character != EOF) {
        buff_counter = 0;

        while (character == ' ') {
            character = fgetc(config);
        }

        // Count length of new token
        while (character != '\n' && character != '\r' && character != EOF) {
            buff_counter += 1;
            character = fgetc(config);
        }

        // Update token length if we need
        if (buff_counter > buff_length) {
            // +1 for null terminated symbol
            buff_length = buff_counter + 1;
        }

        while (character != ' ' && character != EOF) {
            character = fgetc(config);
        }
    }

    // Create buffer for parsing
    char* buff = (char*)malloc(sizeof(char) * buff_length);
    if (buff == NULL) {
        fprintf(stderr, "[ERROR] Bad buffer memory allocation\n");
        fclose(config);
        exit(1);
    }
    unsigned buff_i;

    unsigned space_counter;       // Counter of spaces before token
    unsigned tab_length = 0;      // Count of spaces in one tabulation
    unsigned tab_count;           // Count of tabulations in line
    unsigned line_counter = 0;    // Counter of lines in config file

    // Initialize tree and tree head
    Tree* tree = (Tree*)malloc(sizeof(Tree));
    if (tree == NULL) {
        fprintf(stderr, "[ERROR] Bad tree memory allocation\n");
        fclose(config);
        free(buff);
        exit(1);
    }
    tree->head = node_create("\0", 1);

    // Vector of root nodes for parsing
    Vector* root_nodes = vector_create(1);
    vector_push(root_nodes, (void*)tree->head);

    // Set file cursor to the start
    fseek(config, SEEK_SET, 0);

    int error = 0;  // Flag for error handling

    // Start parsing
    character = fgetc(config);
    while (character != EOF) {
        line_counter += 1;
        space_counter = 0;
        buff_i = 0;

        // Don't use tab character!!!
        if (character == '\t') {
            fprintf(stderr, "[ERROR] Tab character detected in line %d, use a sequence of spaces\n", line_counter);
            error = 1;
            break;
        }

        // Get space count
        while (character == ' ') {
            space_counter += 1;
            character = fgetc(config);
        }

        // Init correct tab length
        if (space_counter > tab_length && tab_length == 0) {
            tab_length = space_counter;
        }

        // Check count of spaces for correctness
        if (space_counter % MAX_OF(tab_length, 1) != 0) {
            fprintf(stderr, "[ERROR] Incorrect tab length in line %d\n", line_counter);
            error = 1;
            break;
        }

        // Get tabulations count;
        tab_count = space_counter / MAX_OF(tab_length, 1);

        // Get token
        while (character != '\n' && character != '\r' && character != EOF) {
            // Handle space symbol in token error
            if (character == ' ' || character == '\t') {
                error = 1;
                break;
            }

            buff[buff_i++] = (char)character;
            character = fgetc(config);
        }

        // Handle error when getting token
        if (error) {
            fprintf(stderr, "[ERROR] Token in config file must not have spaces, line %d\n", line_counter);
            break;
        }

        buff[buff_i] = '\0';

        // Create node by token
        Node* n = node_create(buff, buff_length);

        // Push node to tree
        if (tab_count >= root_nodes->length) {
            fprintf(stderr, "[ERROR] The token on line %d does not belong to any token\n", line_counter);
            error = 1;
            break;
        }
        vector_push(((Node*)vector_get(root_nodes, tab_count))->children, (void*)n);

        // Add token to root tokens vector by index as tab count
        if (tab_count + 1 < root_nodes->length) {
            vector_set(root_nodes, tab_count + 1, n);
        }
        else if (tab_count + 1 == root_nodes->length) {
            vector_push(root_nodes, (void*)n);
        }

        // Go to next line
        while (character != ' ' && character != EOF) {
            character = fgetc(config);
        }
    }

    // Remove temporary variables
    fclose(config);
    vector_free(root_nodes);
    free(buff);

    // Free tree and exit if error
    if (error) {
        tree_free(tree);
        exit(1);
    }

    // Return tree
    return tree;
}

void tree_free(Tree* t) {
    // Free all nodes from the head and self
    node_free(t->head);
    free(t);
}

Vector* vector_create(unsigned length) {
    // Allocate memory for self
    Vector* vec = (Vector*)malloc(sizeof(Vector));
    if (vec == NULL) {
        fprintf(stderr, "[ERROR] Bad vector memory allocation\n");
        exit(1);
    }

    // Setup vector capacity and length
    vec->length = 0;
    vec->capacity = MAX_OF(length, 1);

    // Allocate memory for data field
    vec->data = (void**)malloc(sizeof(void*) * length);
    if (vec->data == NULL) {
        fprintf(stderr, "[ERROR] Bad vector memory allocation\n");
        exit(1);
    }

    return vec;
}

void* vector_get(Vector* vec, unsigned index) {
    // Error message if index not within array bounds
    if (index >= vec->length) {
        fprintf(stderr, "[ERROR] Out of bounds an vector\n");
        exit(1);
    }

    // Return node by index
    return vec->data[index];
}

void vector_set(Vector* vec, unsigned index, void* item) {
    // Error message if index not within array bounds
    if (index >= vec->length) {
        fprintf(stderr, "[ERROR] Out of bounds an vector\n");
        exit(1);
    }

    // Set item by index
    vec->data[index] = item;
}

void vector_push(Vector* vec, void* item) {
    // Reallocate longer memory if capacity runs out
    if (vec->length >= vec->capacity) {
        vec->capacity = (vec->capacity + 1) * 2;
        vec->data = (void**)realloc(vec->data, vec->capacity * sizeof(void*));
        if (vec->data == NULL) {
            fprintf(stderr, "[ERROR] Bad vector memory reallocation\n");
            exit(1);
        }
    }

    // Add node and increase length
    vec->data[vec->length++] = item;
}

void vector_free(Vector* vec) {
    // Free memory for data field and self
    free(vec->data);
    free(vec);
}

#endif //AUTOCOMPLETE_H
