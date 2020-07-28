cmake_minimum_required(VERSION 3.16)
project(auto_complete)

set(CMAKE_CXX_STANDARD 14)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/build")

add_executable(auto_complete main.cpp predict.h console.h)