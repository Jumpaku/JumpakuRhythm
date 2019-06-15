cmake_minimum_required(VERSION 3.5)
project(music_data)

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin)

set(CMAKE_CXX_STANDARD 14)

find_package(Armadillo REQUIRED)

include_directories(${ARMADILLO_INCLUDE_DIR})
include_directories(${CMAKE_SOURCE_DIR}/music_data_lib)
include_directories(/home/wave/src/)
add_subdirectory(music_data_lib)
add_subdirectory(main)