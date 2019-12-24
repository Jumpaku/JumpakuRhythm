cmake_minimum_required(VERSION 3.5)
project(music_data)

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin)

set(CMAKE_CXX_STANDARD 14)

find_package(Armadillo REQUIRED)
find_package(Boost 1.65.1 REQUIRED COMPONENTS iostreams system filesystem)
message(STATUS ${Boost_LIBRARIES})

include_directories(${ARMADILLO_INCLUDE_DIR})
include_directories(${Boost_INCLUDE_DIRS})
include_directories(${CMAKE_SOURCE_DIR}/music_data_lib)
include_directories(/home/wave/src/)
include_directories(/home/gnuplot-iostream/)
add_subdirectory(music_data_lib)
add_subdirectory(main)