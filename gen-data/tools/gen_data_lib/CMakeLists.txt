cmake_minimum_required(VERSION 3.5)
project(gen_data_lib)
add_library(gen_data_lib STATIC gen_data_lib.cpp)
target_link_libraries(gen_data_lib ${OpenCV_LIBS})