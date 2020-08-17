#ifndef CLI_AUTOCOMPLETE
#define CLI_AUTOCOMPLETE

#if defined(_WIN32) || defined(_WIN64)
    #define OS_WINDOWS
#elif defined(__APPLE__) || defined(__unix__) || defined(__unix)
    #define OS_POSIX
#else
    #error unsupported platform
#endif

#if defined(OS_WINDOWS)
    #include <cstdint>
    #include <Windows.h>
    #include <conio.h>
#elif defined(OS_POSIX)
    #include <unistd.h>
    #include <termios.h>
    #include <csignal>
#endif
    #include <string>
    #include <vector>
    #include <map>
    #include <iostream>
    #include <fstream>
    #include <algorithm>

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
#elif defined(OS_POSIX)
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

typedef std::map<std::string, std::vector<std::string>> Dictionary;

#if defined(OS_POSIX)
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
#elif defined(OS_POSIX)
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
#elif defined(OS_POSIX)
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
#elif defined(OS_POSIX)
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
#elif defined(OS_POSIX)
    printf("\033[%d;%dH", y, x);
#endif
}

/**
 * Remove extra spaces to the left and right of the string.
 *
 * @param str Source string.
 * @return A new line equal to the original one without spaces on the left and right.
 */
std::string trim(std::string_view str) {
    std::string result(str);
    result.erase(0, result.find_first_not_of(" \n\r\t"));
    result.erase(result.find_last_not_of(" \n\r\t") + 1);
    return result;
}

/**
 * Parse config file to dictionary.
 *
 * @param file_path The path to the configuration file.
 * @return Tuple of dictionary with autocomplete rules, status of parsing and message.
 */
std::tuple<Dictionary, bool, std::string> parse_config_file(const std::string& file_path) {
    Dictionary dict;            // Dictionary with autocomplete rules

    std::map<int, std::string>
        root_words_by_tabsize;  // Root word memory map by tab size
    std::string line;           // Line for reading from file
    std::string token;          // Received word from string
    std::string root_word;      // Dictionary key for inserting as root word

    long tab_size = 0;        // Base length of a sequence of spaces in line
    long tab_count = 0;       // Count of tabs in line

    // Open config file by file path
    std::ifstream config_file(file_path);

    // Return with bad status if file has not been opened
    if (!config_file.is_open()) {
        return std::make_tuple(dict, false, "Error! Can't open " + file_path + " file.");
    }

    // Read all lines from config file
    while (std::getline(config_file, line)) {
        // Skip empty line
        if (line.empty()) {
            continue;
        }

        // Return with bad status if tab character has been founded
        if (std::count(line.begin(), line.end(), '\t') != 0) {
            return std::make_tuple(dict, false, "Error! Use a sequence of spaces instead of a tab character.");
        }

        // Getting count of spaces at the beginning of a line
        auto spaces = std::count(line.begin(), line.begin() + line.find_first_not_of(" "), ' ');

        // Setup base tabulation size if line with
        // spaces at the beginning has been founded
        if (spaces != 0 && tab_size == 0) {
            tab_size = spaces;
        }

        // Get word (token) from line
        token = trim(line);

        // Tabulation validation
        if (tab_size != 0 && spaces % tab_size != 0) {
            return std::make_tuple(dict, false, "Error! Tab length error was made.\nPossibly in line: " + line );
        }

        // Get count of tabulation
        tab_count = (tab_size == 0) ? 0 : (spaces / tab_size);

        // Remember the root word for a given number of tabs
        root_words_by_tabsize[tab_count] = token;

        // Get the root word for the current token
        root_word = (tab_count == 0) ? "" : root_words_by_tabsize[tab_count - 1];

        // Push token into the dictionary in the root word section if it is not there
        if (std::count(dict[root_word].begin(), dict[root_word].end(), token) == 0) {
            dict[root_word].push_back(token);
        }
    }

    // Close opened configuration file
    config_file.close();

    // Return tuple with success status
    return std::make_tuple(dict, true, "Success. The rule dictionary has been created.");
}

/**
 * Get the minimum of two numbers.
 *
 * @param a First value.
 * @param b Second value.
 * @return Minimum of two numbers.
 */
size_t min_of(size_t a, size_t b) {
    return (a < b) ? a : b;
}

/**
 * Find strings in vector starts with substring.
 *
 * @param substr String with which the word should begin.
 * @param last_word Last word in user-entered line.
 * @param dict Vector of words.
 * @param optional_brackets String with symbols for optional values.
 * @return Vector with words starts with substring.
 */
std::vector<std::string> words_starts_with(std::string_view substr, std::string_view last_word, Dictionary& dict,
                                           std::string_view optional_brackets) {
    std::vector<std::string> result;

    // Return if dictionary hasn't last_word as key or
    // substr contain one of symbols for optional values
    if (!dict.count(last_word.data()) || substr.find_first_of(optional_brackets) != std::string::npos) {
        return result;
    }

    // Return all words that may come
    // after last_word if substr is empty
    if (substr.empty()) {
        return dict[last_word.data()];
    }

    // Find strings starts with substring
    std::vector<std::string> candidates_list = dict[last_word.data()];
    for (size_t i = 0 ; i < candidates_list.size(); i++) {
        if (candidates_list[i].find(substr) == 0) {
            result.push_back(dict[last_word.data()][i]);
        }
    }

    return result;
}

/**
 * Find strings in vector similar to a substring (max 1 error).
 *
 * @param substr String with which the word should begin.
 * @param penult_word Penultimate word in user-entered line.
 * @param dict Vector of words.
 * @param optional_brackets String with symbols for optional values.
 * @return Vector with words similar to a substring.
 */
std::vector<std::string> words_similar_to(std::string_view substr, std::string_view penult_word, Dictionary& dict,
                                          std::string_view optional_brackets) {
    std::vector<std::string> result;

    // Return if substr is empty
    if (substr.empty()) {
        return result;
    }

    // Find strings starts similar to a substring
    std::vector<std::string> candidates_list = dict[penult_word.data()];
    for (size_t i = 0 ; i < candidates_list.size(); i++) {
        int errors = 0;

        // Current candidate in vector
        std::string candidate = candidates_list[i];

        // Character-by-character check
        for (size_t j = 0; j < substr.length(); j++) {

            // Skip if substr contain one of symbols for optional values
            if (optional_brackets.find_first_of(candidate[j]) != std::string::npos) {
                errors = 2;
                break;
            }

            if (substr[j] != candidate[j]) {
                errors += 1;
            }

            if (errors > 1) {
                break;
            }
        }

        // Add candidate to vector if errors less or equal then 1
        if (errors <= 1) {
            result.push_back(candidate);
        }
    }

    return result;
}

/**
 * Get the position of the beginning of the last word.
 *
 * @param str String with words.
 * @return Position of the beginning of the last word.
 */
size_t get_last_word_pos(std::string_view str) {
    // Return 0 if the string consists entirely of spaces
    if (std::count(str.begin(), str.end(), ' ') == str.length()) {
        return 0;
    }

    // Get position of last space symbol
    auto last_word_pos = str.rfind(' ');

    // Return 0 if space symbol not founded, else return position + 1
    return (last_word_pos == std::string::npos) ? 0 : last_word_pos + 1;
}

/**
 * Get the last word in string.
 *
 * @param str String with words.
 * @return Pair Position of the beginning of the
 *         last word and the last word in string.
 */
std::pair<size_t, std::string> get_last_word(std::string_view str) {
    // Get position of the beginning of the last word
    size_t last_word_pos = get_last_word_pos(str);

    // Get last word from string
    auto last_word = str.substr(last_word_pos);

    // Return pair
    return std::make_pair(last_word_pos, last_word.data());
}

/**
 * Get the penultimate words.
 *
 * @param str String with words.
 * @return Pair Position of the beginning of the penultimate
 *         word and the penultimate word in string.
 */
std::pair<size_t, std::string> get_penult_word(std::string_view str) {
    // Setup end position for searching
    size_t end_pos = min_of(str.find_last_not_of(' ') + 2, str.length());

    // Get start position of last word in string
    size_t last_word = get_last_word_pos(str.substr(0, end_pos));

    // Prepare variables
    size_t penult_word_pos = 0;
    std::string penult_word = "";

    // Setup penultimate if start of last word not equal 0
    if (last_word != 0) {
        // Get start position of penultimate word
        penult_word_pos = str.find_last_of(' ', last_word - 2);

        // Set penultimate word if position was founded
        if (penult_word_pos != std::string::npos) {
            penult_word = str.substr(penult_word_pos, last_word - penult_word_pos - 1);
        }
        // Else set penultimate word as everything that went to the last word
        else {
            penult_word = str.substr(0, last_word - 1);
        }
    }

    // Trim penultimate word
    penult_word = trim(penult_word);

    // Return pair
    return std::make_pair(penult_word_pos, penult_word);
}

/**
 * Get the word-prediction by the index.
 *
 * @param buffer String with user input.
 * @param dict Dictionary with rules.
 * @param number Index of word-prediction.
 * @param optional_brackets String with symbols for optional values.
 * @return Tuple of word-prediction, phrase for output, substring of buffer
 *         preceding before phrase, start position of last word.
 */
std::tuple<std::string, std::string, std::string, size_t>
get_prediction(std::string_view buffer, Dictionary& dict, size_t number, std::string_view optional_brackets) {
    // Get last word info
    auto [last_word_pos, last_word] = get_last_word(buffer);

    // Get penultimate word from string
    auto [_, penult_word] = get_penult_word(buffer);

    std::string prediction; // word-prediction
    std::string phrase;     // phrase for output
    std::string prefix;     // substring of buffer preceding before phrase

    // Find prediction
    std::vector<std::string> starts_with = words_starts_with(last_word, penult_word, dict, optional_brackets);

    // Setup variables if words starts with substring was founded
    if (!starts_with.empty()) {
        prediction = starts_with[number % starts_with.size()];
        phrase = prediction;
        prefix = buffer.substr(0, last_word_pos);
    }
    // If words starts with substring wasn't founded
    else {
        // Find similar words
        std::vector<std::string> similar = words_similar_to(last_word, penult_word, dict, optional_brackets);

        // Setup variables if similar words was founded
        if (!similar.empty()) {
            prediction = similar[number % similar.size()];
            phrase = " maybe you mean " + prediction + "?";
            prefix = buffer;
        }
    }

    // Return tuple
    return std::make_tuple(prediction, phrase, prefix, last_word_pos);
}

/**
 * Printing user input with prompts.
 *
 * @param buffer String - User input.
 * @param dict Vector of words.
 * @param line_title Line title of CLI when entering command.
 * @param number Hint number.
 * @param optional_brackets String with symbols for optional values.
 * @param title_color System code of title color     (line title color).
 * @param predict_color System code of predict color (prediction color).
 * @param default_color System code of default color (user input color).
 * @return Void.
 */
#if defined(OS_WINDOWS)
void print_with_prompts(std::string_view buffer, Dictionary& dict, std::string_view line_title, size_t number,
                        std::string_view optional_brackets, uint16_t title_color, uint16_t predict_color,
                        uint16_t default_color) {
#else
void print_with_prompts(std::string_view buffer, Dictionary& dict, std::string_view line_title, size_t number,
                        std::string_view optional_brackets, std::string title_color, std::string predict_color,
                        std::string default_color) {
#endif
    // Get prediction phrase and substring of buffer preceding before phrases
    auto [_, phrase, prefix, __] = get_prediction(buffer, dict, number, optional_brackets);

    std::string delimiter = line_title.empty() ? "" : " ";

    // Output prediction
    std::cout << clear_line;

    std::cout << '\r' << set_console_color(title_color) << line_title
              << set_console_color(default_color) << delimiter << prefix
              << set_console_color(predict_color) << phrase;

    std::cout << '\r' << set_console_color(title_color) << line_title
              << set_console_color(default_color) << delimiter << buffer;
}

/**
 * Reading user input with autocomplete.
 *
 * @param dict Vector of words.
 * @param line_title Line title of CLI when entering command.
 * @param optional_brackets String with symbols for optional values.
 * @param title_color System code of title color     (line title color).
 * @param predict_color System code of predict color (prediction color).
 * @param default_color System code of default color (user input color).
 * @return User input.
 */
#if defined(OS_WINDOWS)
std::string input(Dictionary& dict, std::string_view line_title, std::string_view optional_brackets,
                  uint16_t title_color, uint16_t predict_color, uint16_t default_color) {
#else
std::string input(Dictionary& dict, std::string_view line_title, std::string_view optional_brackets,
                  std::string title_color, std::string predict_color, std::string default_color) {
#endif
    std::string buffer;       // User input
    size_t offset = 0;        // Cursor offset from the end of the buffer
    size_t number = 0;        // Hint number
    short y = cursor_y_pos(); // Get Y cursor position in terminal

    // Ignore key codes
    #if defined(OS_WINDOWS)
    std::vector<int> ignore_keys({1, 2, 19, 24, 26});
    #elif defined(OS_POSIX)
    std::vector<int> ignore_keys({1, 2, 4, 24});
    #endif

    while (true) {
        // Printing user input with prompts
        print_with_prompts(buffer, dict, line_title, number, optional_brackets,
                           title_color, predict_color, default_color);

        // Move cursor
        short x = short(buffer.length() + line_title.length() + !line_title.empty() + 1 - offset);
        goto_xy(x, y);

        // Read character from console
        int ch = _getch();

        // Return buffer if ENTER was pressed
        if (ch == ENTER) {
            return buffer;
        }

        // Keyboard interrupt handler for Windows
        #if defined(OS_WINDOWS)
        else if (ch == CTRL_C) {
            exit(0);
        }
        #endif

        // Edit buffer like backspace if BACKSPACE was pressed
        else if (ch == BACKSPACE) {
            if (!buffer.empty() && buffer.length() - offset >= 1) {
                buffer.erase(buffer.length() - offset - 1, 1);
            }
        }

        // Apply prediction if TAB was pressed
        else if (ch == TAB) {
            // Get prediction and start position of last word
            auto [prediction, _, __, last_word_pos] = get_prediction(buffer, dict, number, optional_brackets);

            // Add prediction to user input if it exists and
            // there are no symbols for optional values
            if (!prediction.empty() && prediction.find_first_of(optional_brackets) == std::string::npos) {
                buffer = buffer.substr(0, last_word_pos) + prediction + " ";
            }

            // Reset cursor offset and hint number
            offset = 0;
            number = 0;
        }

        // Left and Right key handler
        #if defined(OS_WINDOWS)
        else if (ch == 0 || ch == 224)
        #elif defined(OS_POSIX)
        else if (ch == 27 && _getch() == 91)
        #endif
            switch (_getch()) {
                case LEFT:
                    // Increase offset from the end of the buffer if left key pressed
                    offset = (offset < buffer.length()) ? offset + 1 : buffer.length();
                    break;
                case RIGHT:
                    // Decrease offset from the end of the buffer if left key pressed
                    offset = (offset > 0) ? offset - 1 : 0;
                    break;
                case UP:
                    // Increase hint number
                    number = number + 1;
                    std::cout << clear_line;
                    break;
                case DOWN:
                    // Decrease hint number
                    number = number - 1;
                    std::cout << clear_line;
                    break;
                case DEL:
                    // Edit buffer like DELETE key
                    #if defined(OS_POSIX)
                    if (_getch() == 126)
                    #endif
                    {
                        if (!buffer.empty() && offset != 0) {
                            buffer.erase(buffer.length() - offset, 1);
                            offset -= 1;
                        }
                    }
                default:
                    break;
            }

        // Add character to buffer considering offset if any key was pressed
        else if (!std::count(ignore_keys.begin(), ignore_keys.end(), ch)) {
            buffer.insert(buffer.end() - offset, (char)ch);

            if (ch == SPACE) {
                number = 0;
            }
        }
    }
}

#endif //CLI_AUTOCOMPLETE
