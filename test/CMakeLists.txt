cmake_minimum_required(VERSION 3.20)

if(LPEM8086_BUILD_TESTS)
	enable_testing()

	include_directories(vendor/google_test/googletest/include)
	include_directories(${PROJECT_SOURCE_DIR}/lib/include)
	include_directories(./include)

	set(BUILD_GMOCK OFF CACHE BOOL "Test lib only" FORCE)
	set(INSTALL_GTEST CACHE BOOL "Do not install test library" FORCE)

	add_subdirectory(vendor/google_test)

	file(GLOB_RECURSE ADD_SRC *.cpp)

	message(STATUS ${ADD_SRC})

	add_executable(8086_emulator_test
		8086_ModRMByte.cpp
		CheckParityTest.cpp
		GetRegisterTest.cpp
		m8086Test.cpp
		SetRegisterTest.cpp
		${ADD_SRC}
	)
	
	# link google test dependencies
	target_link_libraries(8086_emulator_test gtest gtest_main)

	# link library as a dependency
	target_link_libraries(8086_emulator_test 8086_emulator)

	add_test(NAME tests COMMAND 8086_emulator_test)
endif()
