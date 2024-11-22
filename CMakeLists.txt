cmake_minimum_required(VERSION 3.21)

# This file is heavily based on https://github.com/lefticus/cpp_starter_project
# and https://github.com/cpp-best-practices/cmake_template, but using the GitHub
# template feature is broken.

project(cojoNES CXX)
include(cmake/StandardProjectSettings.cmake)
include(cmake/PreventInSourceBuilds.cmake)
include(cmake/ProjectOptions.cmake)

# Ensure the final binary is built in the root of the build dir
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})

# Only set the cxx_standard if it is not set by someone else
if (NOT DEFINED CMAKE_CXX_STANDARD)
  set(CMAKE_CXX_STANDARD 20)
endif()

if(CMAKE_CXX_COMPILER_ID MATCHES ".*Clang")
  option(ENABLE_BUILD_WITH_TIME_TRACE "Enable -ftime-trace to generate time tracing .json files on clang" OFF)
  if(ENABLE_BUILD_WITH_TIME_TRACE)
    target_compile_options(project_options INTERFACE -ftime-trace)
  endif()
endif()

# Link this 'library' to use the warnings specified in CompilerWarnings.cmake
add_library(project_warnings INTERFACE)

# standard compiler warnings
include(cmake/CompilerWarnings.cmake)
set_project_warnings(project_warnings)

# sanitizer options if supported by compiler
include(cmake/Sanitizers.cmake)
enable_sanitizers(project_options)

# allow for static analysis options
include(cmake/StaticAnalyzers.cmake)

option(BUILD_SHARED_LIBS "Enable compilation of shared libraries" OFF)
option(ENABLE_TESTING "Enable Test Builds" ON)
option(ENABLE_FUZZING "Enable Fuzzing Builds" OFF)

include(Dependencies.cmake)
cojoNES_setup_dependencies()

# Extra target for tests.
add_executable(cojoNES_tests tests/test.cpp source/Cartridge.cpp source/CPU.cpp source/ROM.cpp source/System.cpp)
target_include_directories(cojoNES_tests PRIVATE dependencies/Catch2/src)
target_include_directories(cojoNES_tests PRIVATE source)
target_link_libraries(cojoNES_tests PRIVATE Catch2::Catch2WithMain)
target_link_system_libraries(cojoNES_tests PRIVATE fmt::fmt spdlog::spdlog)

# TODO: Implement fuzzing?

add_subdirectory(source)
