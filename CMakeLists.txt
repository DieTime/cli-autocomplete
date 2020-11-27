cmake_minimum_required(VERSION 3.13)
project(autocomplete C)

set(CMAKE_C_STANDARD 11)
set (CMAKE_C_FLAGS "-Wall")

if (UNIX)
    set (CMAKE_C_FLAGS "-fsanitize=address")
endif (UNIX)

file(GLOB SOURCES include/*.h src/*.c)

add_executable(autocomplete main.c ${SOURCES})