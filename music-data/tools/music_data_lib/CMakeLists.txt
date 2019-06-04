cmake_minimum_required(VERSION 3.5)
project(music_data_lib)
add_library(music_data_lib STATIC music_data_lib.cpp)
target_link_libraries(music_data_lib ${ARMADILLO_LIBRARY})