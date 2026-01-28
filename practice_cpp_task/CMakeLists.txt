cmake_minimum_required(VERSION 3.10)

project(MouseDriverTask)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(mouse_driver embedded_coding_mouse_driver_task.cpp)

target_link_libraries(mouse_driver pthread)
