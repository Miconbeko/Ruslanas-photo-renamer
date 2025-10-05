cmake_minimum_required(VERSION 3.23)
project(ruslanas-photo-ultimate VERSION 1.0)

set(CMAKE_CXX_STANDARD 17)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})

add_subdirectory(OpenXLSX)
add_subdirectory(src)
