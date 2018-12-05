#include <iostream>
#include <fstream>

#include <string>
#include <string_view>
#include <unordered_set>

//External libs
#include <fmt/core.h>
#include <fmt/format.h>
#include <fmt/ostream.h>

#include "spdlog/spdlog.h"
#include "spdlog/sinks/stdout_color_sinks.h"


std::ostream& operator<<(std::ostream& os, const std::unordered_set<int>& set) {
    os << "[ ";
    for (const int val : set) {
        os << val << " ";
    }
    return os << "]";
}

int main() {
    auto console = spdlog::stdout_color_mt("console");
    auto err_log = spdlog::stderr_color_mt("stderr");
    console->set_level(spdlog::level::info);
    err_log->set_level(spdlog::level::info);

    console->info("Advent of Code - Day 01");

    constexpr std::string_view inputFilename {"../input.txt"};
    std::ifstream inFile {inputFilename.data()};
    if (!inFile) {
        err_log->error("Error opening file \"{}\"", inputFilename);
        return 1;
    }

    std::vector<int> frequencies {};

    //*
    int freq {0};
    int buff {0};
    while (inFile >> buff) {
        freq += buff;
        frequencies.push_back(buff);
    }

    console->info("*  => {}", freq);

    //**
    bool foundDuplicate {false};
    std::unordered_set<int> set {};
    freq = 0;
    set.insert(freq);
    while (!foundDuplicate) {
        for (auto f : frequencies) {
            freq += f;
            console->debug("Checking if {} has been seen", freq);
            if (set.count(freq)) {
                console->info("** => {}", freq);
                foundDuplicate = true;
                break;
            }
            set.insert(freq);
        }
        console->debug(set);
    }

    return 0;
}