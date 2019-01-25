cmake_minimum_required(VERSION 3.10)
project(Images-Processing)

set(CMAKE_CXX_STANDARD 14)

add_executable(Images-Processing main.cpp Image.cpp Image.h helpers.h helpers.cpp Filter.cpp Filter.h)