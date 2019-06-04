cmake_minimum_required(VERSION 3.5)
add_executable(main main.cpp)
target_link_libraries(main music_data_lib ${ARMADILLO_LIBRARY})