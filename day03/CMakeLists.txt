cmake_minimum_required(VERSION 3.12)
project(AoC2018)


add_subdirectory(lib/fmt)
add_subdirectory(lib/spdlog)

if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release)
endif()

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}")
set(CMAKE_CXX_FLAGS_DEBUG "-Wall")
set(CMAKE_CXX_FLAGS_RELEASE "-O3")

add_executable(AoC2018 main.cpp)
target_link_libraries(AoC2018 PRIVATE fmt::fmt spdlog::spdlog)

