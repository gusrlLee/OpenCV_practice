cmake_minimum_required(VERSION 3.0)
set(CMAKE_CXX_STANDARD 11)
project(bitwise)
find_package(OpenCV REQUIRED)
include_directories(${OpenCV_INCLUDE_DIRS})
add_executable(${PROJECT_NAME} main.cpp)
target_link_libraries(${PROJECT_NAME} ${OpenCV_LIBS})


