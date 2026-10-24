cmake_minimum_required(VERSION 3.15)
project(connect4-minimax CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if(NOT CMAKE_BUILD_TYPE AND NOT CMAKE_CONFIGURATION_TYPES)
  set(CMAKE_BUILD_TYPE Release)
endif()

add_executable(connect4
    src/main.cpp
    src/Board.cpp
    src/Board.h
    src/AI.h
    src/AI.cpp
    src/Game.h
    src/Game.cpp
    src/Experiments.h
    src/Experiments.cpp
    src/Random.h)

    
if(MSVC)
  target_compile_options(connect4 PRIVATE /W4)
else()
  target_compile_options(connect4 PRIVATE -Wall -Wextra)
endif()
