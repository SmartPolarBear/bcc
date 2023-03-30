cmake_minimum_required(VERSION 3.25)
project(bcc)

set(CMAKE_CXX_STANDARD 23)

add_executable(bcc main.cpp)

add_subdirectory(bcc)
add_subdirectory(lexer)