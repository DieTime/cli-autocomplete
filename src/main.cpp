#include <iostream>
#include <string>
#include <vector>

#include "predict.h"
#include "parser.h"

int main() {
    std::string config_file_path = "../src/config.txt";
    std::string optional_brackets = "[";

    auto [dict, status, message] = parse_config_file(config_file_path);

    if (status) {
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