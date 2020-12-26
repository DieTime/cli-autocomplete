cmake_minimum_required(VERSION 3.13)
project(CLI-Autocomplete VERSION 2.0.0)

set(CMAKE_C_STANDARD 99)

file(GLOB_RECURSE SOURCES src/*.c)
file(GLOB_RECURSE INCLUDE include/*.h)
set(EXAMPLE_1 examples/default_input.c)
set(EXAMPLE_2 examples/custom_input.c)

if (MINGW OR UNIX)
    set(LIB_NAME cliac)
elseif(MSVC)
    set(LIB_NAME libcliac)
endif()

if (MSVC)
    set(DIR_NAME msvc)
elseif (MINGW)
    set(DIR_NAME mingw)
elseif (UNIX)
    set(DIR_NAME unix)
endif ()

if (MSVC OR MINGW)
    set(SHARED_OUTPUT_COMMAND RUNTIME_OUTPUT_DIRECTORY)
elseif (UNIX)
    set(SHARED_OUTPUT_COMMAND LIBRARY_OUTPUT_DIRECTORY)
endif ()

add_executable(default ${EXAMPLE_1} ${SOURCES} ${INCLUDE})
add_executable(custom ${EXAMPLE_2} ${SOURCES} ${INCLUDE})
add_library(static ${SOURCES} ${INCLUDE})
add_library(shared SHARED ${SOURCES} ${INCLUDE})

set_target_properties(default PROPERTIES
        RUNTIME_OUTPUT_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/builds/examples/${DIR_NAME}")
target_compile_options(default PRIVATE -DBUILD_STATIC)

set_target_properties(custom PROPERTIES
        RUNTIME_OUTPUT_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/builds/examples/${DIR_NAME}")
target_compile_options(custom PRIVATE -DBUILD_STATIC)

set_target_properties(static PROPERTIES
        ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/builds/libs/static/${DIR_NAME}"
        OUTPUT_NAME ${LIB_NAME})
set_target_properties(shared PROPERTIES
        ${SHARED_OUTPUT_COMMAND} "${CMAKE_CURRENT_SOURCE_DIR}/builds/libs/shared/${DIR_NAME}"
        OUTPUT_NAME ${LIB_NAME})
add_custom_command(
        TARGET static POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_SOURCE_DIR}/include/
        ${CMAKE_SOURCE_DIR}/builds/libs/static/${DIR_NAME}/include/)
add_custom_command(
        TARGET shared POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_SOURCE_DIR}/include/
        ${CMAKE_SOURCE_DIR}/builds/libs/shared/${DIR_NAME}/include/)

target_compile_options(static PRIVATE -DBUILD_STATIC)
target_compile_options(shared PRIVATE -DBUILD_SHARED)