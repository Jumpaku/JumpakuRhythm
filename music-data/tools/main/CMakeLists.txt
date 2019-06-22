cmake_minimum_required(VERSION 3.5)
add_executable(main main.cpp wavfile.cpp analysis.cpp)
set(LIBWAVE /home/wave/src/wave/libwave.a)
target_link_libraries(main music_data_lib ${LIBWAVE} ${ARMADILLO_LIBRARY})