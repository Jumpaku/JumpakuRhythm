cmake_minimum_required(VERSION 3.5)
add_executable(extract_notes main.cpp DetectNotes.cpp BSplineCurve.cpp ExtractNotes.cpp)
target_link_libraries(extract_notes gen_data_lib ${OpenCV_LIBS})