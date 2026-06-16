cmake_minimum_required(VERSION 3.15)
project(chip8)
include(FetchContent)

add_executable(chip8 src/main.cpp src/chip8.cpp)
target_include_directories(chip8 PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/include")