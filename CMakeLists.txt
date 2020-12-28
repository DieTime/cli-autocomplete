cmake_minimum_required(VERSION 3.13)
project(CLI-Autocomplete VERSION 2.0.0)

set(CMAKE_C_STANDARD 99)

file(GLOB_RECURSE SOURCES src/*.c)

set(EXAMPLE_DEFAULT examples/default_input.c)
set(EXAMPLE_CUSTOM examples/custom_input.c)

if (MSVC)
    set(DIR_NAME msvc)
elseif(MINGW)
    set(DIR_NAME mingw)
elseif(UNIX)
    set(DIR_NAME unix)
endif ()

add_executable(default_example ${EXAMPLE_DEFAULT} ${SOURCES})
add_executable(custom_example ${EXAMPLE_CUSTOM} ${SOURCES})

set_target_properties(default_example PROPERTIES
        RUNTIME_OUTPUT_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/builds/examples/${DIR_NAME}")

set_target_properties(custom_example PROPERTIES
        RUNTIME_OUTPUT_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/builds/examples/${DIR_NAME}")