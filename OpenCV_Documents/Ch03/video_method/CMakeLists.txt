cmake_minimum_required(VERSION 3.0)
project(video_method)
find_package(OpenCV REQUIRED)
include_directories(${OpenCV_INCLUDE_DIRS})
add_executable(video_method main.cpp)
target_link_libraries(video_method ${OpenCV_LIBS})

