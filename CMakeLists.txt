cmake_minimum_required(VERSION 3.20)
project(0xb100d)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(server src/main.cpp)
add_executable(tests tests/main.cpp)

