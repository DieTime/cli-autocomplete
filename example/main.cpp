#include <iostream>
#include <string>

#include "../include/autocomplete.h"

int main() {
    std::string config_file_path = "../config.txt";

    auto [dict, status, message] = parse_config_file(config_file_path);

    if (status) {
        while (true) {
            // Hearing user input with highlighting prompts
            std::string command = input(dict);

            // Do something with the entered string...
            std::cout << std::endl << command << std::endl << std::endl;
        }
    }
    else {
        std::cerr << message << std::endl;
    }

    return 0;
}