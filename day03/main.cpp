#include <iostream>
#include <fstream>

#include <string>
#include <string_view>
#include <sstream>
#include <array>
#include <map>

// External libs
#include <fmt/core.h>
#include <fmt/format.h>
#include <fmt/ostream.h>

#include "spdlog/spdlog.h"
#include "spdlog/sinks/stdout_color_sinks.h"


template<class InputIt, class ForwardIt, class BinOp>
void for_each_token(InputIt first, InputIt last, ForwardIt s_first, ForwardIt s_last, BinOp binary_op);


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

    int id, x, y, l, h;
    auto matrix = std::array<std::array<uint_fast8_t, 1000>, 1000>();
    std::map<int, std::tuple<int, int, int, int>> cache {};
    constexpr char delims[] = " \n\t\r\f#@,:x";
    std::string line;
    std::stringstream tmp;
    while (std::getline(inFile, line)) {
        for_each_token(std::cbegin(line), std::cend(line),
                       std::cbegin(delims), std::cend(delims),
                       [&tmp](auto first, auto second) {
                           if (first != second) {
                               tmp << std::string(first, second) << " ";
                           }
                       });
        tmp >> id >> x >> y >> l >> h;
        console->debug("id {}, x {}, y {}, l {}, h {}", id, x, y, l, h);
        cache.insert_or_assign(id, std::tuple(x, y, l, h));

        int maxY {y + h};
        for (auto i = y; i < maxY; ++i) {
            int maxX {x + l};
            for (auto j = x; j < maxX; ++j) {
                matrix[i][j]++;
            }
        }
    }

    uint total {};
    for (auto row : matrix) {
        for (auto cell : row) {
            if (cell > 1) { ++total; }
        }
    }

    console->info("*  {}", total);

    int intactID {};
    for (auto claim : cache) {
        auto[tx, ty, tl, th] = claim.second;
        int maxY {ty + th};
        bool intact = true;
        for (auto i = ty; i < maxY && intact; ++i) {
            int maxX {tx + tl};
            for (auto j = tx; j < maxX; ++j) {
                if (matrix[i][j] > 1) {
                    intact = false;
                    break;
                }
            }
        }
        if (intact) {
            intactID = claim.first;
            break;
        }
    }

    console->info("** {}", intactID);


    return 0;
}

template<class InputIt, class ForwardIt, class BinOp>
void for_each_token(InputIt first, InputIt last, ForwardIt s_first, ForwardIt s_last, BinOp binary_op) {
    while (first != last) {
        const auto pos = find_first_of(first, last, s_first, s_last);
        binary_op(first, pos);
        if (pos == last) break;
        first = next(pos);
    }
}
