#include <iostream>
#include <fstream>

#include <string>
#include <string_view>
#include <vector>
#include <unordered_set>

//External libs
#include <fmt/core.h>
#include <fmt/format.h>
#include <fmt/ostream.h>

#include "spdlog/spdlog.h"
#include "spdlog/sinks/stdout_color_sinks.h"


int main() {
    auto console = spdlog::stdout_color_mt("console");
    auto err_log = spdlog::stderr_color_mt("stderr");
//    console->set_level(spdlog::level::debug);
//    err_log->set_level(spdlog::level::debug);

    console->info("Advent of Code - Day 02");

    constexpr std::string_view inputFilename {"../input.txt"};
    std::ifstream inFile {inputFilename.data()};
    if (!inFile) {
        err_log->error("Error opening file \"{}\"", inputFilename);
        return 1;
    }


    std::vector<std::string> input {};

    //*
    std::unordered_multiset<char> chars {};
    int n2 {0};
    int n3 {0};
    bool found2 {};
    bool found3 {};
    std::string buff {};
    while (inFile >> buff) {
        input.push_back(buff);
        chars.clear();
        found2 = false;
        found3 = false;
        for (char ch : buff) {
            chars.insert(ch);
        }

        for (auto ch : chars) {
            if (!found2 && chars.count(ch) == 2) {
                ++n2;
                found2 = true;
            }
            if (!found3 && chars.count(ch) == 3) {
                ++n3;
                found3 = true;
            }
            if (found2 && found3) {
                break;
            }
        }
    }

    console->info("*  => {}", n2 * n3);


    //**
    uint diff {};
    size_t diffPos {0};
    bool terminate {false};
    for (size_t i = 0; i < input.size() && !terminate; ++i) {
        for (size_t j = i + 1; j < input.size() - 1 && !terminate; ++j) {
            diff = 0;
            for (size_t k = 0; k < input[i].length(); ++k) {
                if (input[i][k] != input[j][k]) {
                    ++diff;
                    if (diff == 1) {
                        diffPos = k;
                    } else {
                        break;
                    }
                }
            }
            console->debug("Compared {} with {}", input[i], input[j]);
            if (diff == 1) {
                console->debug("Deleting different char in pos: {}", diffPos);
                console->info("** => {}", input[i].erase(diffPos, 1));
                terminate = true;
            }
        }
    }


    return 0;
}