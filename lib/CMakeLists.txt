cmake_minimum_required(VERSION 3.20)

include_directories(./include)

add_library(8086_emulator 
	src/8086.cpp
	"src/Memory.cpp"
	"src/Processor.cpp")
