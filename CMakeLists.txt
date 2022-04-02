cmake_minimum_required(VERSION 3.20)

project(8086_emulator)

set(CMAKE_CXX_STANDARD 17)

option(LPEM8086_BUILD_TESTS "Builds tests" ON)

add_subdirectory(lib)
add_subdirectory(test)
