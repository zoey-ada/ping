cmake_minimum_required(VERSION 3.6)

project(demo-game)

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/../bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/../bin)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/../bin)

set(bin_dir ${CMAKE_RUNTIME_OUTPUT_DIRECTORY})
set(third_party_libs_dir ${CMAKE_CURRENT_SOURCE_DIR}/libraries)

set_property(GLOBAL PROPERTY USE_FOLDERS ON)

include("cmake/macros.cmake")
configure_msvc_runtime()

add_subdirectory("source")
