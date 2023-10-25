cmake_minimum_required(VERSION 3.26)
project(local_v2 C)

set(CMAKE_C_STANDARD 11)

add_executable(local_v2 main.c
        neuralnet.c
        neuralnet.h
        proportional.c
        proportional.h)
