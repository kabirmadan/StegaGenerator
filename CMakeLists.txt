cmake_minimum_required(VERSION 3.17)
project(StegaGenerator)

set(CMAKE_CXX_STANDARD 17)

add_executable(StegaGenerator src/main.cpp src/string_util.cpp src/string_util.h src/rgb.cpp src/rgb.h src/third_party/bitmap_image.hpp)