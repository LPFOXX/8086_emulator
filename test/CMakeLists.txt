cmake_minimum_required(VERSION 3.20)
project(8086_emulator_test)

set(CMAKE_CXX_STANDARD 17)

include_directories(../include)
include_directories(vendor/google_test/googletest/include)

set(BUILD_GMOCK OFF CACHE BOOL "Test lib only" FORCE)
set(INSTALL_GTEST CACHE BOOL "Do not install test library" FORCE)

add_subdirectory(vendor/google_test)

add_executable(8086_emulator_test 8086_ModRMByte.cpp 8086TestFixture.hpp CheckParityTest.cpp GetRegisterTest.cpp m8086Test.cpp SetRegisterTest.cpp)
target_link_libraries(8086_emulator_test gtest gtest_main)
