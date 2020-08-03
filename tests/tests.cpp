#define CATCH_CONFIG_MAIN
#include "catch.h"
#include "../include/completion.h"

void lines_to_file(const std::string& filename, const std::vector<std::string>& lines) {
    std::ofstream os(filename);

    for (const auto& line : lines) {
        os << line << std::endl;
    }

    os.close();
}

TEST_CASE( "trim function" ) {
    SECTION( "works correctly for the left side" ) {
        REQUIRE( trim("hello") == "hello" );
        REQUIRE( trim(" hello") == "hello" );
        REQUIRE( trim("  hello") == "hello" );
    }

    SECTION( "works correctly for the left side" ) {
        REQUIRE( trim("hello") == "hello" );
        REQUIRE( trim("hello ") == "hello" );
        REQUIRE( trim("hello  ") == "hello" );
    }

    SECTION( "works correctly for both sides" ) {
        REQUIRE( trim(" hello ") == "hello" );
        REQUIRE( trim("  hello  ") == "hello" );
    }

    SECTION( "does not touch spaces within a string" ) {
        REQUIRE( trim(" my string ") == "my string" );
        REQUIRE( trim("  my  long  string  ") == "my  long  string" );
    }
}

TEST_CASE( "parse_config_file function" ) {
    SECTION( "file not exists" ) {
        std::string file_path = "file_not_exists.txt";
        auto [dict, status, message] = parse_config_file(file_path);

        REQUIRE( dict.empty() );
        REQUIRE( !status );
        REQUIRE( message ==  "Error! Can't open " + file_path + " file.");
    }

    SECTION( "file contains tab symbol" ) {
        std::vector<std::string> lines = {
            "default",
            "\tcommand_1",
            "\tcommand_2"
        };

        lines_to_file("rules.txt", lines);
        auto [_, status, message] = parse_config_file("rules.txt");

        REQUIRE(!status);
        REQUIRE( message ==  "Error! Use a sequence of spaces instead of a tab character.");
    }

    SECTION( "error in tabulation length" ) {
        std::vector<std::string> lines = {
            "default",
            "    command_1",
            "  command_2",
        };

        lines_to_file("rules.txt", lines);
        auto [_, status, message] = parse_config_file("rules.txt");

        REQUIRE(!status);
        REQUIRE( message ==  "Error! Tab length error was made.\nPossibly in line: " + lines.back() );
    }

    SECTION( "file is correct" ) {
        std::vector<std::string> lines = {
            "default",
            "    command_1",
            "        command_2",
            "            command_3",
            "    command_4",
        };

        lines_to_file("rules.txt", lines);
        auto [dict, status, message] = parse_config_file("rules.txt");

        REQUIRE( dict["default"] == std::vector<std::string>({"command_1", "command_4"}) );
        REQUIRE( dict["command_1"] == std::vector<std::string>({"command_2"}) );
        REQUIRE( dict["command_2"] == std::vector<std::string>({"command_3"}) );
        REQUIRE( status );
        REQUIRE( message ==  "Success. The rule dictionary has been created." );
    }

    SECTION( "push value into dict[""] if tab count == 0" ) {
        std::vector<std::string> lines = {
            "default",
            "    command_1",
            "        command_2",
            "            command_3",
            "    command_4",
        };

        lines_to_file("rules.txt", lines);
        auto [dict, status, message] = parse_config_file("rules.txt");

        REQUIRE( dict[""] == std::vector<std::string>({"default"}) );
    }
}

TEST_CASE( "min_of function" ) {
    SECTION( "works with size_t values" ) {
        REQUIRE( min_of(0, 1) == 0 );
        REQUIRE( min_of(1, 0) == 0 );
        REQUIRE( min_of(0, 0) == 0 );
        REQUIRE( min_of(1, 1) == 1 );
    }
}

TEST_CASE( "words_starts_with function" ) {
    std::vector<std::string> lines = {
        "default",
        "    --[command_1]",
        "        command_2",
        "            command_3",
        "    command_4",
    };

    lines_to_file("rules.txt", lines);
    auto [dict, _, __] = parse_config_file("rules.txt");

    SECTION( "return empty vector if dictionary hasn't last_word as key" ) {
        auto words = words_starts_with("comm", "github", dict, "");
        REQUIRE( words.empty() );
    }

    SECTION( "return empty vector if substr contain one of symbols for optional values" ) {
        auto words = words_starts_with("--[", "default", dict, "[");
        REQUIRE( words.empty() );
    }

    SECTION( "return all words that may come after last_word if substr is empty" ) {
        auto words = words_starts_with("", "default", dict, "");
        REQUIRE( words == std::vector<std::string>({"--[command_1]", "command_4"}) );

        words = words_starts_with("", "--[command_1]", dict, "");
        REQUIRE( words == std::vector<std::string>({"command_2"}) );
    }

    SECTION ( "return correct result" ) {
        auto words = words_starts_with("comm", "command_2", dict, "");
        REQUIRE( words == std::vector<std::string>({"command_3"}) );
    }
}

TEST_CASE( "words_similar_to function" ) {
    std::vector<std::string> lines = {
        "default",
        "    --[command_1]",
        "        command_2",
        "            command_3",
        "            command_4",
        "    command_5",
    };

    lines_to_file("rules.txt", lines);
    auto [dict, _, __] = parse_config_file("rules.txt");

    SECTION( "return empty vector if substr is empty" ) {
        auto words = words_similar_to("", "default", dict, "");
        REQUIRE( words.empty() );
    }

    SECTION( "return empty vector if substr contain one of symbols for optional values" ) {
        auto words = words_similar_to("-+[", "default", dict, "[");
        REQUIRE( words.empty() );
    }

    SECTION ( "return correct result" ) {
        auto words = words_similar_to("conm", "command_2", dict, "");
        REQUIRE( words == std::vector<std::string>({"command_3", "command_4"}) );
    }
}

TEST_CASE( "get_last_word_pos function" ) {
    SECTION( "return 0 if the string consists entirely of spaces" ) {
        REQUIRE( get_last_word_pos(" ") == 0 );
        REQUIRE( get_last_word_pos("     ") == 0 );
    }

    SECTION( "return string length value if user write ' ' in the end" ) {
        REQUIRE( get_last_word_pos("hello world ") == 12 );
        REQUIRE( get_last_word_pos("hello ") == 6 );
        REQUIRE( get_last_word_pos("hello  ") == 7 );
        REQUIRE( get_last_word_pos("  hello  ") == 9 );
    }

    SECTION( "return first character index of last word if user don't write ' ' in the end" ) {
        REQUIRE( get_last_word_pos("hello world") == 6 );
        REQUIRE( get_last_word_pos("hello") == 0 );
    }
}

TEST_CASE( "get_penult_word function" ) {
    SECTION( "the space entered at the end by the user is a word" ) {
        REQUIRE( get_penult_word("  hello world  ").second == "world" );
        REQUIRE( get_penult_word("  hello world ").second == "world" );
    }

    SECTION("return penultimate word if user don't write ' ' in the end" ) {
        REQUIRE( get_penult_word("hello").second == "" );
        REQUIRE( get_penult_word("hello world").second == "hello" );
        REQUIRE( get_penult_word("  hello world").second == "hello" );
    }
}

TEST_CASE( "get_prediction function" ) {
    std::vector<std::string> lines = {
        "default",
        "    --[command_1]",
        "        command_2",
        "            command_3",
        "            command_4",
        "    command_5",
        "        command_6",
        "        command_7",
    };

    lines_to_file("rules.txt", lines);
    auto [dict, _, __] = parse_config_file("rules.txt");

    SECTION( "return empty string if penultimate word isn't key of dictionary" ) {
        auto [prediction, _, __, ___] = get_prediction("default command ", dict, 0, "");
        REQUIRE( prediction.empty() );
    }

    SECTION( "work correct if buffer is empty" ) {
        auto [prediction, _, __, ___] = get_prediction("", dict, 0, "");
        REQUIRE( prediction == "default" );
    }

    SECTION( "work with orthography errors" ) {
        auto [prediction, _, __, ___] = get_prediction("default command_5 conma", dict, 0, "");
        REQUIRE( prediction == "command_6" );
    }

    SECTION( "change hint number work correctly" ) {
        auto prediction = get_prediction("default command_5 comma", dict, 0, "");
        REQUIRE( std::get<0>(prediction) == "command_6" );

        prediction = get_prediction("default command_5 comma", dict, 1, "");
        REQUIRE( std::get<0>(prediction) == "command_7" );
    }

    SECTION( "work correct with optional brackets" ) {
        auto [prediction, _, __, ___] = get_prediction("default --[", dict, 0, "[");
        REQUIRE( prediction == "" );
    }

    SECTION( "prediction runs until the user enters a ' ' at the end of the line" ) {
        auto [prediction, _, __, ___] = get_prediction("default", dict, 0, "[");
        REQUIRE( prediction == "default" );
    }
}

