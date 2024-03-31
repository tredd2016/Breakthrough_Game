cmake_minimum_required(VERSION 3.15)
project(Breakthrough)

set(INCLUDE_PATHS asio/include crow/Crow-1.1.0/include)

add_executable(Breakthrough main.cpp breakthrough_ai.cpp)

target_include_directories(Breakthrough PUBLIC ${INCLUDE_PATHS})