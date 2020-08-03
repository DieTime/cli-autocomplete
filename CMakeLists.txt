cmake_minimum_required(VERSION 3.16)
project(CLI-Autocomplete VERSION 1.0.0)

set(CMAKE_CXX_STANDARD 17)

file(GLOB_RECURSE SOURCES src/*.cpp)
file(GLOB_RECURSE INCLUDE include/*.h)
file(GLOB_RECURSE TEST_FILES tests/*.*)
file(GLOB_RECURSE EXAMPLE example/*.cpp)

add_executable(tests ${SOURCES} ${TEST_FILES} ${INCLUDE})
if (WIN32)
  set_target_properties(tests PROPERTIES
      RUNTIME_OUTPUT_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/tests/")
elseif (UNIX)
  set_target_properties(tests PROPERTIES
      RUNTIME_OUTPUT_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/tests/${CMAKE_BUILD_TYPE}/")
endif()

add_executable(example ${SOURCES} ${EXAMPLE} ${INCLUDE})
if (WIN32)
  set_target_properties(example PROPERTIES
      RUNTIME_OUTPUT_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/example/")
elseif (UNIX)
  set_target_properties(example PROPERTIES
      RUNTIME_OUTPUT_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/example/${CMAKE_BUILD_TYPE}/")
endif()
