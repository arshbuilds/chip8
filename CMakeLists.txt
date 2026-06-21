cmake_minimum_required(VERSION 3.15)

project(chip8)

find_package(SDL2 REQUIRED)

add_executable(
    chip8
    src/main.cpp
    src/chip8.cpp
    src/platform.cpp
)

target_include_directories(
    chip8
    PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/include"
)

target_link_libraries(
    chip8
    SDL2::SDL2
)