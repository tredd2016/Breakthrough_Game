cmake_minimum_required(VERSION 3.15)
project(Breakthrough)

set(INCLUDE_PATHS ../../boost_1_84_0 ../../crow/Crow-1.1.0/include)

add_executable(Breakthrough main.cpp breakthrough_ai.cpp)

target_include_directories(Breakthrough PUBLIC ${INCLUDE_PATHS})