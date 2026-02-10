cmake_minimum_required(VERSION 3.20)
project(worksheet4_ex4 LANGUAGES CXX)

add_library(adder SHARED adder.cpp)

target_compile_definitions(adder PRIVATE ADDER_EXPORTS)

add_executable(worksheet4_ex4 main.cpp)

target_link_libraries(worksheet4_ex4 PRIVATE adder)
