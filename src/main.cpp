#include <iostream>
#include <string>
#include <vector>

#include "predict.h"

// TODO dictionary from file

int main() {
    std::vector<std::string> dict = {
        "push",
        "puts",
        "append",
    };

    while (true) {
        std::string command = input(dict);
        std::cout << command << std::endl;
    }

    return 0;
}