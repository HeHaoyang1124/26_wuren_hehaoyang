cmake_minimum_required(VERSION 3.10)

project(Control)

find_package(Eigen3 REQUIRED)

add_executable(Q1 Q1.cpp)

target_link_libraries(Q1 Eigen3::Eigen)