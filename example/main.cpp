#include <iostream>
#include <string>

#include "../include/autocomplete.h"

int main() {
    std::string config_file_path = "../config.txt";
    std::string optional_brackets = "[";
    size_t counter = 0;

    auto [dict, status, message] = parse_config_file(config_file_path);

    if (status) {
        std::cerr << "Attention! Please run the executable file only" << std::endl
                  << "through the command line!\n\n";

        while (true) {
            std::string line_title = "git [" + std::to_string(counter) + "]:";

            std::string command = input(dict, line_title, optional_brackets);
            std::cout << std::endl << command << std::endl << std::endl;

            counter++;
        }
    }
    else {
        std::cerr << message << std::endl;
    }

    return 0;
}