cmake_minimum_required(VERSION 3.20)
project(8086_emulator)

set(CMAKE_CXX_STANDARD 17)

include_directories(include)

add_library(8086_emulator include/m8086.hpp)
