cmake_minimum_required(VERSION 3.17)
project(StegaGenerator)

set(CMAKE_CXX_STANDARD 17)

add_executable(StegaGenerator main.cpp string_util.cpp string_util.h rgb.cpp rgb.h)