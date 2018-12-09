#include <iostream>
#include <fstream>
#include <string_view>

#include <sstream>
#include <string>
#include <thread>
#include <future>

// External libs
#include <fmt/core.h>
#include <fmt/format.h>
#include <fmt/ostream.h>

#include "spdlog/spdlog.h"
#include "spdlog/sinks/stdout_color_sinks.h"


unsigned long react(std::string str);


int main() {
    auto console = spdlog::stdout_color_mt("console");
    auto err_log = spdlog::stderr_color_mt("stderr");
//    console->set_level(spdlog::level::debug);
//    err_log->set_level(spdlog::level::debug);

    console->info("Advent of Code - Day 05");

    constexpr std::string_view inputFilename {"../input.txt"};
    std::ifstream inFile {inputFilename.data()};
    if (!inFile) {
        err_log->error("Error opening file \"{}\"", inputFilename);
        return 1;
    }

    std::string origPolymer {};
    std::getline(inFile, origPolymer);
    std::string polymer {origPolymer};
    console->debug("Original -> {}", polymer);
    for (size_t i = 1; i < polymer.length(); ++i) {
        if ((std::islower(polymer[i]) && (polymer[i - 1] == std::toupper(polymer[i]))) ||
            (std::isupper(polymer[i]) && (polymer[i - 1] == std::tolower(polymer[i])))) {
            polymer.erase(i - 1, 2);
            i -= 2;
        }
    }
    console->debug("Processed -> {}", polymer);
    console->info("*  -> {}", std::async(react, polymer).get());


    std::array<std::future<size_t>, 26> testChamber {};
    for (size_t j = 0; j < testChamber.size(); ++j) {
        std::string str {origPolymer};
        char ch {static_cast<char>('a' + j)};
        str.erase(
                std::remove_if(
                        str.begin(), str.end(),
                        [ch](unsigned char x) { return x == ch || std::tolower(x) == ch; }
                ), str.end());
        testChamber[j] = std::async(react, str);
    }

    size_t min {testChamber[0].get()};
    for (size_t k = 1; k < testChamber.size(); ++k) {
        size_t tmp = testChamber[k].get();
        if (tmp < min) {
            min = tmp;
        }
    }
    console->info("** -> {}", min);


    return 0;
}


size_t react(std::string str) {
    auto console = spdlog::get("console");
    for (size_t i = 1; i < str.length(); ++i) {
        if ((std::islower(str[i]) && (str[i - 1] == std::toupper(str[i]))) ||
            (std::isupper(str[i]) && (str[i - 1] == std::tolower(str[i])))) {
            str.erase(i - 1, 2);
            i -= 2;
        }
    }
    console->debug("Result {} \nProcessed, final size: {}", str, str.length());
    return str.length();
}
