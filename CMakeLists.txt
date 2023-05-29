cmake_minimum_required(VERSION 3.22)
project(test4_1)

set(CMAKE_CXX_STANDARD 17)

include_directories(.)

add_executable(test4_1
        main.cpp
        Node.cpp
        Node.hpp)
