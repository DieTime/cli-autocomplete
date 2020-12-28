cmake_minimum_required(VERSION 3.13)
project(CLI-Autocomplete VERSION 2.0.0)

set(CMAKE_C_STANDARD 99)

file(GLOB_RECURSE SOURCES src/*.c)
file(GLOB_RECURSE INCLUDE include/*.h)

set(EXAMPLE_1 examples/default_input.c)
set(EXAMPLE_2 examples/custom_input.c)

add_executable(default ${EXAMPLE_1} ${SOURCES} ${INCLUDE})
add_executable(custom ${EXAMPLE_2} ${SOURCES} ${INCLUDE})

set_target_properties(default PROPERTIES
        RUNTIME_OUTPUT_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/builds/examples/${DIR_NAME}")

set_target_properties(custom PROPERTIES
        RUNTIME_OUTPUT_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/builds/examples/${DIR_NAME}")