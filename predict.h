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
 * Find index of first string in vector starts with substring
 *
 * @param substr String with which the word should begin.
 * @param dict Vector of words.
 * @return index of word in vector, or -1 if not founded.
 */
static int startswith(const std::string& substr, const std::vector<std::string> &dict) {
    if (substr.empty()) {
        return -1;
    }

    // Find string starts with substring
    for (size_t i = 0 ; i < dict.size(); i++) {
        if (dict[i].find(substr) == 0) {
            return i;
        }
    }

    return -1;
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

void printWithPrediction(const std::string& buffer, const std::vector<std::string>& dict ) {
    // Try predict word in user input
    size_t last_word = last_word_pos(buffer);

    int index = startswith(buffer.substr(last_word), dict);

    // Printing prediction to the
    // console if it was found
    if (index != -1) {
        std::cout << '\r' << buffer.substr(0, last_word) << gray << dict[index] << white;
    } else {
        std::cout << CLEAR_LINE;
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
    std::string buffer;
    int offset = 0;
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
            printWithPrediction(buffer, dict);
            gotoX(buffer.length() - offset + 1);
        }
        
        // Apply prediction if TAB was pressed
        else if (ch == TAB) {
            // Get last word position
            size_t last_word = last_word_pos(buffer);

            // If a prediction exists
            int index = startswith(buffer.substr(last_word), dict);
            if (index != -1) {
                // Apply prediction
                buffer = buffer.substr(0, last_word) + dict[index] + " ";
            }

            offset = 0;

            std::cout << CLEAR_LINE;
            printWithPrediction(buffer, dict);
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
                gotoX(buffer.length() - offset + 1);
                break;
            case RIGHT:
                offset = (offset > 0) ? offset - 1 : 0;
                gotoX(buffer.length() - offset + 1);
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
                    printWithPrediction(buffer, dict);
                    gotoX(buffer.length() - offset + 1);\
                }
            default:
                break;
        }

        // Add character to buffer if any key was pressed
        else if (!std::count(ignore.begin(), ignore.end(), ch)) {
            buffer.insert(buffer.end() - offset, ch);
            printWithPrediction(buffer, dict);
            gotoX(buffer.length() - offset + 1);
        }
    }
}

#endif //AUTO_COMPLETE_PREDICT_H