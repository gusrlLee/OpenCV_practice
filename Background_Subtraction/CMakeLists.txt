cmake_minimum_required(VERSION 3.0)
project(mysubtract)

find_package(OpenCV REQUIRED)

include_directories(${OpenCV_INCLUDE_DIRS})
add_executable(${PROJECT_NAME} test.cpp)
target_link_libraries(${PROJECT_NAME} ${OpenCV_LIBS})
