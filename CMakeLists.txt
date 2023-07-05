cmake_minimum_required(VERSION 3.24)
project(chot)

set(CMAKE_CXX_STANDARD 20)

add_library(chot source/square.hpp source/move.cpp source/move.hpp source/piece.hpp source/bitboard.cpp source/bitboard.hpp source/position.cpp source/position.hpp source/move_gen.cpp source/move_gen.hpp source/util.hpp source/lut.hpp)
add_executable(chot-driver source/main.cpp)
target_link_libraries(chot-driver PUBLIC chot)

add_executable(chot-perft tests/perft.cpp)
target_link_libraries(chot-perft PUBLIC chot)
target_include_directories(chot-perft PUBLIC source)
