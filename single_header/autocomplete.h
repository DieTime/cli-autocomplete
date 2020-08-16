#pragma once
#ifndef CLI_AUTOCOMPLETE_AUTOCOMPLETE_H
#define CLI_AUTOCOMPLETE_AUTOCOMPLETE_H

#if defined(_WIN32) || defined(_WIN64)
    #define OS_WINDOWS
    #if defined(BUILD_STATIC)
        #define SHARED_LIB
    #elif defined(BUILD_DLL)
        #define SHARED_LIB extern __declspec(dllexport)
    #else
        #define SHARED_LIB extern __declspec(dllimport)
    #endif
#elif defined(__APPLE__) || defined(__unix__) || defined(__unix)
    #define OS_POSIX
    #define SHARED_LIB extern __attribute__((visibility("default")))
#else
    #error unsupported platform
#endif

#if defined(OS_WINDOWS)
    #include <cstdint>
#endif
    #include <string>
    #include <vector>
    #include <map>
    #include <iostream>

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
SHARED_LIB int _getch();
#endif

#if defined(OS_WINDOWS)
/**
 * Get count of terminal cols.
 *
 * @return Width of terminal.
 */
SHARED_LIB size_t console_width();
#endif

/**
 * Clear terminal line.
 *
 * @param os Output stream.
 * @return input parameter os.
 */
SHARED_LIB std::ostream& clear_line(std::ostream& os);

/**
 * Sets the console color.
 *
 * @param color System code of target color.
 * @return Input parameter os.
 */
#if defined(OS_WINDOWS)
SHARED_LIB std::string set_console_color(uint16_t color);
#elif defined(OS_POSIX)
SHARED_LIB std::string set_console_color(std::string color);
#endif

/**
 * Gets current terminal cursor position.
 *
 * @return Y position of terminal cursor.
 */
SHARED_LIB short cursor_y_pos();

/**
 * Move terminal cursor at position x and y.
 *
 * @param x X position to move.
 * @param x Y position to move.
 * @return Void.
 */
SHARED_LIB void goto_xy(short x, short y);

/**
 * Get the minimum of two numbers.
 *
 * @param a First value.
 * @param b Second value.
 * @return Minimum of two numbers.
 */
SHARED_LIB size_t min_of(size_t a, size_t b);

/**
 * Find strings in vector starts with substring.
 *
 * @param substr String with which the word should begin.
 * @param last_word Last word in user-entered line.
 * @param dict Vector of words.
 * @param optional_brackets String with symbols for optional values.
 * @return Vector with words starts with substring.
 */
SHARED_LIB std::vector<std::string> words_starts_with(std::string_view substr, std::string_view last_word,
                                                      Dictionary& dict, std::string_view optional_brackets);

/**
 * Find strings in vector similar to a substring (max 1 error).
 *
 * @param substr String with which the word should begin.
 * @param last_word Last word in user-entered line.
 * @param dict Vector of words.
 * @param optional_brackets String with symbols for optional values.
 * @return Vector with words similar to a substring.
 */
SHARED_LIB std::vector<std::string> words_similar_to(std::string_view substr, std::string_view last_word,
                                                     Dictionary& dict, std::string_view optional_brackets);

/**
 * Get the position of the beginning of the last word.
 *
 * @param str String with words.
 * @return Position of the beginning of the last word.
 */
SHARED_LIB size_t get_last_word_pos(std::string_view str);

/**
 * Get the last word in string.
 *
 * @param str String with words.
 * @return Pair Position of the beginning of the
 *         last word and the last word in string.
 */
SHARED_LIB std::pair<size_t, std::string> get_last_word(std::string_view str);

/**
 * Get the penultimate words.
 *
 * @param str String with words.
 * @return Pair Position of the beginning of the penultimate
 *         word and the penultimate word in string.
 */
SHARED_LIB std::pair<size_t, std::string> get_penult_word(std::string_view str);

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
SHARED_LIB std::tuple<std::string, std::string, std::string, size_t>
    get_prediction(std::string_view buffer, Dictionary& dict, size_t number, std::string_view optional_brackets);

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
SHARED_LIB void print_with_prompts(std::string_view buffer, Dictionary& dict, std::string_view line_title, size_t number,
                                   std::string_view optional_brackets, uint16_t title_color, uint16_t predict_color,
                                   uint16_t default_color);
#else
SHARED_LIB void print_with_prompts(std::string_view buffer, Dictionary& dict, std::string_view line_title, size_t number,
                                   std::string_view optional_brackets, std::string title_color, std::string predict_color,
                                   std::string default_color);
#endif
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
SHARED_LIB std::string input(Dictionary& dict, std::string_view line_title = "", std::string_view optional_brackets = "",
                  uint16_t title_color = 10, uint16_t predict_color = 8, uint16_t default_color = 7);
#else
SHARED_LIB std::string input(Dictionary& dict, std::string_view line_title = "", std::string_view optional_brackets = "",
                             std::string title_color = "92", std::string predict_color = "90", std::string default_color = "0");
#endif

/**
 * Remove extra spaces to the left and right of the string.
 *
 * @param str Source string.
 * @return A new line equal to the original one without spaces on the left and right.
 */
SHARED_LIB std::string trim(std::string_view str);

/**
 * Parse config file to dictionary.
 *
 * @param file_path The path to the configuration file.
 * @return Tuple of dictionary with autocomplete rules, status of parsing and message.
 */
SHARED_LIB std::tuple<Dictionary, bool, std::string> parse_config_file(const std::string& file_path);

#endif //CLI_AUTOCOMPLETE_AUTOCOMPLETE_H
