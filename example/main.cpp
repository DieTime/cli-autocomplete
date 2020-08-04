#include <iostream>
#include <string>

#include "../include/autocomplete.h"

int main() {
    // Path to the configuration file
    std::string config_file_path = "../config.txt";

    // Characters which optional values begin ( optional parameter )
    std::string optional_brackets = "[";

    // Optional parameters
    #if defined(OS_WINDOWS)
        // https://stackoverflow.com/questions/4053837/colorizing-text-in-the-console-with-c#answer-4053879
        uint16_t title_color = 160; // by default 10
        uint16_t predict_color = 8; // by default 8
        uint16_t default_color = 7; // by default 7
    #elif defined(OS_UNIX)
        // https://stackoverflow.com/questions/2616906/how-do-i-output-coloured-text-to-a-linux-terminal#answer-45300654
        // Set the value that goes between \033 and m ( \033{your_value}m )
        std::string title_color = "0;30;102";  // by default 92
        std::string predict_color = "90";      // by default 90
        std::string default_color = "0";       // by default 90
    #endif

    // Value for line title
    size_t command_counter = 0;

    // Parsing the configuration file
    auto [dict, status, message] = parse_config_file(config_file_path);

    // If status is True start listening
    if (status) {
        std::cerr << "Attention! Please run the executable file only" << std::endl
                  << "through the command line!\n\n";

        std::cerr << "- To switch the prompts press UP or DOWN arrow." << std::endl;
        std::cerr << "- To move cursor press LEFT or RIGHT arrow." << std::endl;
        std::cerr << "- To edit input press DELETE or BACKSPACE key." << std::endl;
        std::cerr << "- To apply current prompt press TAB key.\n\n";

        // Listening process
        while (true) {
            // Prepare line title ( optional parameter )
            std::string line_title = "git [" + std::to_string(command_counter) + "]:";

            // Listening user input with highlighting prompts
            std::string command = input(dict, line_title, optional_brackets,
                                        title_color, predict_color, default_color);

            // Do something with the entered string...
            std::cout << std::endl << command << std::endl << std::endl;

            command_counter++;
        }
    }
    // Print an error message if parsing return bad status
    else {
        std::cerr << message << std::endl;
    }

    return 0;
}