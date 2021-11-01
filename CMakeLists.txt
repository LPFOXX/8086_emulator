cmake_minimum_required(VERSION 3.20)

project(8086_emulator)

set(CMAKE_CXX_STANDARD 17)

include_directories(include)

option(LPEM8086_BUILD_TESTS "Builds tests" ON)

if(LPEM8086_BUILD_TESTS)
    add_subdirectory(test)
endif()

add_library(8086_emulator src/8086.cpp)
