#include <iostream>
#include <fstream>
#include <string_view>

#include <sstream>
#include <string>
#include <vector>
#include <limits>
#include <algorithm>

// External libs
#include <fmt/core.h>
#include <fmt/format.h>
#include <fmt/ostream.h>

#include "spdlog/spdlog.h"
#include "spdlog/sinks/stdout_color_sinks.h"


typedef std::pair<int, int> Point;


template<typename T>
std::ostream& operator<<(std::ostream& out, const std::vector<T>& v) {
    out << "[";
    size_t last = v.size() - 1;
    for (size_t i = 0; i < v.size(); ++i) {
        out << v[i];
        if (i != last)
            out << ", ";
    }
    out << "]";
    return out;
}

std::ostream& operator<<(std::ostream& out, const Point& v) {
    return out << "(" << v.first << ", " << v.second << ")";
}

static inline bool cmp_by_x(const Point& a, const Point& b);

static inline bool cmp_by_y(const Point& a, const Point& b);

static inline unsigned manhattan_dist(const Point& a, const Point& b);


int main() {
    auto console = spdlog::stdout_color_mt("console");
    auto err_log = spdlog::stderr_color_mt("stderr");
    console->set_level(spdlog::level::debug);
    err_log->set_level(spdlog::level::debug);

    console->info("Advent of Code - Day 06");

    constexpr std::string_view inputFilename {"../input.txt"};
    std::ifstream inFile {inputFilename.data()};
    if (!inFile) {
        err_log->error("Error opening file \"{}\"", inputFilename);
        return 1;
    }

    std::vector<Point> coords {};
    int x {}, y {}, maxX {}, maxY {};
    char comma;
    while (inFile >> x >> comma >> y) {
        if (x > maxX) { maxX = x; }
        if (y > maxY) { maxY = y; }
        coords.emplace_back(std::make_pair(x, y));
    }
    console->debug(coords);
    console->debug("maxX {}, maxY {}", maxX, maxY);


    return 0;
}

static inline bool cmp_by_x(const Point& a, const Point& b) {
    if (a.first < b.first) {
        return true;
    } else if (a.first > b.first) {
        return false;
    } else {
        return a.second < b.second;
    }
}

static inline bool cmp_by_y(const Point& a, const Point& b) {
    if (a.second < b.second) {
        return true;
    } else if (a.second > b.second) {
        return false;
    } else {
        return a.first < b.first;
    }
}

static inline unsigned manhattan_dist(const Point& a, const Point& b) {
    return static_cast<uint>(std::abs(a.first - b.first) +
                             std::abs(a.second - b.second));
}
