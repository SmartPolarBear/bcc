cmake_minimum_required(VERSION 3.25)

target_sources(bcc PRIVATE parser.cpp)

target_include_directories(bcc PRIVATE include)