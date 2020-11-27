#include "../include/autocomplete.h"
#include "../include/predictions.h"

char* custom_input(Tree* rules, char* title, COLOR_TYPE title_color,COLOR_TYPE predict_color,
                   COLOR_TYPE main_color, char* optional_brackets) {
    // Initialize buffer for reading
    short buff_len = 0;
    short buff_cap = terminal_width();
    char* buff = (char*)calloc(buff_cap, sizeof(char));
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
                for (unsigned i = buff_len - offset - 1; i < buff_cap - 1; i++) {
                    buff[i] = buff[i + 1];
                }
                buff_len -= 1;
            }
        }

        // Apply prediction if TAB was pressed
        else if (ch == TAB) {
            if (pred->type != FAILURE) {
                char* prediction = (char*)vector_get(pred->tokens, hint_num % pred->tokens->length);
                unsigned predict_len = strlen(prediction);

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
                    buff_len = (short)(buff_len - space_offset);

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
                            for (unsigned i = buff_len - offset; i < buff_cap - 1; i++) {
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

