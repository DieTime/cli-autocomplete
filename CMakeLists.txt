cmake_minimum_required(VERSION 3.16)
project(autocomplete C)

set(CMAKE_C_STANDARD 99)

file(GLOB SOURCES src/*.h src/*.c)

add_executable(autocomplete main.c ${SOURCES})