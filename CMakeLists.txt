cmake_minimum_required(VERSION 3.15)
project(autocomplete C)

set(CMAKE_C_STANDARD 99)

file(GLOB SOURCES include/*.h src/*.c)

add_executable(autocomplete main.c ${SOURCES})