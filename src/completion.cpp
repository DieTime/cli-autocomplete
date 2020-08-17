#if defined(_WIN32) || defined(_WIN64)
    #define OS_WINDOWS
#elif defined(__APPLE__) || defined(__unix__) || defined(__unix)
    #define OS_POSIX
#else
    #error unsupported platform
#endif

#if defined(OS_WINDOWS)
    #include <conio.h>
#endif
    #include <string>
    #include <vector>
    #include <iostream>
    #include <algorithm>

#include "../include/autocomplete.h"

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
 * @param penult_word Penultimate word in user-entered line.
 * @param dict Vector of words.
 * @param optional_brackets String with symbols for optional values.
 * @return Vector with words starts with substring.
 */
std::vector<std::string> words_starts_with(std::string_view substr, std::string_view penult_word, Dictionary& dict,
                                           std::string_view optional_brackets) {
    std::vector<std::string> result;

    // Return if dictionary hasn't penult_word as key or
    // substr contain one of symbols for optional values
    if (!dict.count(penult_word.data()) ||
        substr.find_first_of(optional_brackets) != std::string::npos)
    {
        return result;
    }

    // Return all words that may come
    // after last_word if substr is empty
    if (substr.empty()) {
        return dict[penult_word.data()];
    }

    // Find strings starts with substring
    std::vector<std::string> candidates_list = dict[penult_word.data()];
    for (size_t i = 0 ; i < candidates_list.size(); i++) {
        if (candidates_list[i].find(substr) == 0) {
            result.push_back(dict[penult_word.data()][i]);
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

            // Skip if candidate contain one of symbols for optional values
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

        // Arrows and Delete keys handler
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
