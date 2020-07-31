#pragma once
#ifndef CLI_AUTOCOMPLETE_PARSER_H
#define CLI_AUTOCOMPLETE_PARSER_H

#include <vector>
#include <string>
#include <fstream>
#include <algorithm>
#include <map>

typedef std::map<std::string, std::vector<std::string>> Dictionary;

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

    size_t tab_size = 0;        // Base length of a sequence of spaces in line
    size_t tab_count = 0;       // Count of tabs in line

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
        size_t spaces = std::count(line.begin(), line.begin() + line.find_first_not_of(" "), ' ');

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

#endif //CLI_AUTOCOMPLETE_PARSER_H
