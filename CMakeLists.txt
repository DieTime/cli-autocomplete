cmake_minimum_required(VERSION 3.16)
project(CLI-AutoComplete)

set(CMAKE_CXX_STANDARD 17)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/build")

file(GLOB_RECURSE HEADERS src/**.h)
file(GLOB_RECURSE SOURCES src/*.cpp)

add_executable(CLI-AutoComplete ${HEADERS} ${SOURCES})