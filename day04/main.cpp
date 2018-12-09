#include <iostream>
#include <fstream>

#include <string>
#include <string_view>
#include <sstream>
#include <map>
#include <algorithm>

// External libs
#include <fmt/core.h>
#include <fmt/format.h>
#include <fmt/ostream.h>

#include "spdlog/spdlog.h"
#include "spdlog/sinks/stdout_color_sinks.h"


struct Entry {
    int year;
    int month;
    int day;
    int hour;
    int minute;
    std::string message;

    bool operator<(const Entry& lhr) {
        if (this->year == lhr.year) {
            if (this->month == lhr.month) {
                if (this->day == lhr.day) {
                    if (this->hour == lhr.hour) {
                        return this->minute < lhr.minute;
                    } else {
                        return this->hour < lhr.hour;
                    }
                } else {
                    return this->day < lhr.day;
                }
            } else {
                return this->month < lhr.month;
            }
        } else {
            return this->year < lhr.year;
        }
    };
};


template<class InputIt, class ForwardIt, class BinOp>
void for_each_token(InputIt first, InputIt last, ForwardIt s_first, ForwardIt s_last, BinOp binary_op);


int main() {
    auto console = spdlog::stdout_color_mt("console");
    auto err_log = spdlog::stderr_color_mt("stderr");
//    console->set_level(spdlog::level::debug);
//    err_log->set_level(spdlog::level::debug);

    console->info("Advent of Code - Day 04");

    constexpr std::string_view inputFilename {"../input.txt"};
    std::ifstream inFile {inputFilename.data()};
    if (!inFile) {
        err_log->error("Error opening file \"{}\"", inputFilename);
        return 1;
    }

    Entry entry {};
    std::vector<Entry> entries {};
    constexpr char delims[] = " \n\t\r\f#[]-:";
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
        tmp >> entry.year >> entry.month >> entry.day >> entry.hour >> entry.minute;
        getline(tmp, entry.message);
        tmp.clear();

        entries.emplace_back(std::move(entry));
    }

    std::map<int, std::map<int, int>> guardLog {};

    std::sort(entries.begin(), entries.end());
    int currGuardID {};
    int startedSleeping {};
    for (const auto& e : entries) {
//        console->debug("[{}-{}-{} {}:{}] {}", e.year, e.month, e.day, e.hour, e.minute, e.message);
        std::stringstream ss {e.message};
        std::string first {};
        ss >> first;
        if (first == "Guard") {
            ss >> currGuardID;
            console->debug("Guard {} began shift at {}", currGuardID, e.minute);
        } else if (first == "falls") {
            startedSleeping = e.minute;
            console->debug("Guard {} fell asleep {}", currGuardID, e.minute);
        } else if (first == "wakes") {
            for (int i = startedSleeping; i < e.minute; ++i) {
                guardLog[currGuardID][i]++;
            }
            console->debug("Guard {} woke up at {}", currGuardID, e.minute);
        }
    }

    int maxID {};
    int max {};
    for (const auto& g : guardLog) {
        int sum {};
        for (auto m : g.second) {
            sum += m.second;
        }
        console->debug("Guard {} slept {}m", g.first, sum);
        if (sum > max) {
            max = sum;
            maxID = g.first;
        }
    }

    max = 0;
    int maxMinute {};
    for (auto m : guardLog[maxID]) {
        if (m.second > max) {
            max = m.second;
            maxMinute = m.first;
        }
    }
    console->info("*  -> {}", maxMinute * maxID);


    std::pair<int, int> maxPair {};
    max = 0;
    for (auto g : guardLog) {
        for (auto m : g.second) {
            console->debug("Guard {}, minute {} -> {}", g.first, m.first, m.second);
            if (m.second > max) {
                max = m.second;
                maxPair = std::make_pair(g.first, m.first);
            }
        }
    }
    console->debug("Guard {} - minute {}", maxPair.first, maxPair.second);
    console->info("** -> {}", maxPair.first * maxPair.second);


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
