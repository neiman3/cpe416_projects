cmake_minimum_required(VERSION 3.13)  # CMake version check
project(simple_example)               # Create project "simple_example"
set(CMAKE_CXX_STANDARD 14)            # Enable c++14 standard

# Add main.cpp file of the project root directory as a source file
set(SOURCE_FILES neuralnet.c neuralnet.h progressbar.c progressbar.h proportional.c proportional.h training_data.h
        particle.c
        lab4_part2.c)

# Add executable target with source files listed in SOURCE_FILES variable
add_executable(simple_example ${SOURCE_FILES})