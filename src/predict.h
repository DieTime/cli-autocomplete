#pragma once
#ifndef AUTO_COMPLETE_PREDICT_H
#define AUTO_COMPLETE_PREDICT_H

#if defined(_WIN32) || defined(_WIN64)
    #define OS_WINDOWS
#elif defined(__APPLE__) || defined(__unix__) || defined(__unix)
    #define OS_UNIX
#else
    #error unsupported platform
#endif

#include <string>
#include <vector>
#include <iostream>
#include <algorithm>

#include "console.h"

 /**
 * Find indices of strings in vector starts with substring
 *
 * @param substr String with which the word should begin.
 * @param dict Vector of words.
 * @return Vector with indices of words starts with substring.
 */
static std::vector<size_t> starts_with(const std::string& substr, const std::vector<std::string> &dict) {
    std::vector<size_t> result;

    if (substr.empty()) {
        return result;
    }

    // Find strings starts with substring
    for (size_t i = 0 ; i < dict.size(); i++) {
        if (dict[i].find(substr) == 0) {
            result.push_back(i);
        }
    }

    return result;
}

/**
* Find indices of strings in vector similar to a substring (max 1 error)
*
* @param substr String with which the word should begin.
* @param dict Vector of words.
* @return Vector with indices of words similar to a substring.
*/
static std::vector<size_t> maybe(const std::string& substr, const std::vector<std::string> &dict) {
    std::vector<size_t> result;

    if (substr.empty()) {
        return result;
    }

    // Find strings starts similar to a substring
    for (size_t i = 0 ; i < dict.size(); i++) {
        int errors = 0;
        for (size_t j = 0; j < substr.length(); j++) {
            if (substr[j] != dict[i][j]) {
                errors += 1;
            }
            if (errors > 1) {
                break;
            }
        }

        if (errors <= 1) {
            result.push_back(i);
        }
    }

    return result;
}

/**
 * Get the position of the start of the last word
 *
 * @param str String for searching.
 * @return Position of last word.
 */
static size_t last_word_pos(const std::string& str) {
    auto last_word = str.rfind(' ');
    return (last_word == std::string::npos) ? 0 : last_word + 1;
}

/**
 * Printing user input with prompts
 *
 * @param buffer String - User input.
 * @param dict Vector of words.
 * @param number Hint number.
 */
void print_with_prompts(const std::string& buffer, const std::vector<std::string>& dict, size_t number) {
    // Try predict word in user input
    size_t last_word = last_word_pos(buffer);

    std::vector<size_t> indices = starts_with(buffer.substr(last_word), dict);

    // Printing prediction to the
    // console if it was found
    if (!indices.empty()) {
        size_t word_index = indices[number % indices.size()];

        std::cout << '\r' << buffer.substr(0, last_word)
                  << gray << dict[word_index] << white;
    }
    // Printing possible words to the
    // console if it was found
    else {
        std::vector<size_t> possible = maybe(buffer.substr(last_word), dict);

        if (!possible.empty()) {
            size_t possible_index = possible[number % possible.size()];
            std::cout << '\r' << buffer << gray << " maybe you mean \""
                      << dict[possible_index] << "\"?" << white;
        }
        else {
             std::cout << CLEAR_LINE;
        }
    }

    // Printing current buffer to the console
    std::cout << '\r' << buffer;
}

/**
 * Reading user input with autocomplete
 *
 * @param dict Vector of words.
 * @return user input.
 */
std::string input(std::vector<std::string> dict) {
    std::string buffer; // User input
    size_t offset = 0;  // Cursor offset from the end of the buffer
    size_t number = 0;  // Hint number

    // Ignore key codes
    #if defined(OS_WINDOWS)
        std::vector<int> ignore({1, 2, 19, 24, 26});
    #elif defined(OS_UNIX)
        std::vector<int> ignore({1, 2, 4, 24});
    #endif

    while (true) {
        // Read character from console
        int ch = _getch();

        // Return buffer if ENTER was pressed
        if (ch == ENTER) {
            std::cout << std::endl;
            return buffer;
        }

        // Keyboard interrupt handler for Windows
        #if defined(OS_WINDOWS)
            else if (ch == CTRL_C) {
            exit(0);
        }
        #endif
        
        // Change buffer and clear console line
        // if BACKSPACE was pressed
        else if (ch == BACKSPACE) {
            if (!buffer.empty() && buffer.length() - offset >= 1) {
                buffer.erase(buffer.length() - offset - 1, 1);
            }
            std::cout << CLEAR_LINE;
            print_with_prompts(buffer, dict, number);
            goto_x(buffer.length() - offset + 1);
        }
        
        // Apply prediction if TAB was pressed
        else if (ch == TAB) {
            // Get last word position
            size_t last_word = last_word_pos(buffer);

            // If a prediction exists
            std::vector<size_t> indices = starts_with(buffer.substr(last_word), dict);
            if (!indices.empty()) {
                // Apply prediction
                size_t word_index = indices[number % indices.size()];
                buffer = buffer.substr(0, last_word) + dict[word_index] + " ";
            }
            else {
                std::vector<size_t> possible = maybe(buffer.substr(last_word), dict);
                if (!possible.empty()) {
                    size_t possible_index = possible[number % possible.size()];
                    buffer = buffer.substr(0, last_word) + dict[possible_index] + " ";
                }
            }

            offset = 0;
            number = 0;

            std::cout << CLEAR_LINE;
            print_with_prompts(buffer, dict, number);
        }
        
        // Left and Right key handler
        #if defined(OS_WINDOWS)
        else if (ch==224)
        #elif defined(OS_UNIX)
        else if (ch == 27 && _getch() == 91)
        #endif
        switch (_getch()) {
            case LEFT:
                offset = (offset < (int)buffer.length()) ? offset + 1 : (int)buffer.length();
                goto_x(buffer.length() - offset + 1);
                break;
            case RIGHT:
                offset = (offset > 0) ? offset - 1 : 0;
                goto_x(buffer.length() - offset + 1);
                break;
            case UP:
                number = number + 1;
                print_with_prompts(buffer, dict, number);
                break;
            case DOWN:
                number = number - 1;
                print_with_prompts(buffer, dict, number);
                break;
            case DEL:
                #if defined(OS_UNIX)
                if (_getch() == 126)
                #endif
                {
                    if (!buffer.empty() && offset != 0) {
                        buffer.erase(buffer.length() - offset, 1);
                        offset -= 1;
                    }
                    std::cout << CLEAR_LINE;
                    print_with_prompts(buffer, dict, number);
                    goto_x(buffer.length() - offset + 1);
                }
            default:
                break;
        }

        // Add character to buffer if any key was pressed
        else if (!std::count(ignore.begin(), ignore.end(), ch)) {
            buffer.insert(buffer.end() - offset, (char)ch);
            print_with_prompts(buffer, dict, number);
            goto_x(buffer.length() - offset + 1);

            if (ch == SPACE) {
                number = 0;
            }
        }
    }
}

#endif //AUTO_COMPLETE_PREDICT_H