cmake_minimum_required(VERSION 3.15)
project(connect4-minimax CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(connect4
    src/main.cpp
    src/Board.cpp
    src/Board.h
)