cmake_minimum_required(VERSION 3.16)
project(CLI-AutoComplete)

set(CMAKE_CXX_STANDARD 17)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/build")

file(GLOB_RECURSE SOURCES src/*.cpp)
file(GLOB_RECURSE HEADERS include/*.h)

file(GLOB_RECURSE TESTS_SOURCES tests/*.cpp)
file(GLOB_RECURSE TESTS_HEADERS tests/*.h)

add_executable(autocomplete ${HEADERS} ${SOURCES})
add_executable(tests ${TESTS_HEADERS} ${TESTS_SOURCES})