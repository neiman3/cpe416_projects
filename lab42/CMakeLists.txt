cmake_minimum_required(VERSION 3.26)
project(lab4 C)

set(CMAKE_C_STANDARD 11)

add_executable(lab4 lab4_part2.c
        particle.c
        particle.h
        proportional.c
        proportional.h
        progressbar.c
        progressbar.h
        test_data.h)
