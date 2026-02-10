cmake_minimum_required(VERSION 3.20)

project(worksheet4_ex3)

# Create static library
add_library(adder STATIC adder.cpp)

# Create executable
add_executable(worksheet4_ex3 main.cpp)

# Link static library into executable
target_link_libraries(worksheet4_ex3 adder)
