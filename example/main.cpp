#include <iostream>
#include <string>

#include "../include/completion.h"

int main() {
    std::string config_file_path = "../config.txt";
    std::string optional_brackets = "[";

    auto [dict, status, message] = parse_config_file(config_file_path);

    if (status) {
        std::cout << set_predict_color << "Attention! Please run the executable file only" << std::endl
                                       << "through the command line!\n\n";

        while (true) {
            std::string command = input(dict, optional_brackets);
            std::cout << std::endl << command << std::endl;
        }
    }
    else {
        std::cerr << message << std::endl;
    }

    return 0;
}