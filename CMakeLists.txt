cmake_minimum_required(VERSION 3.24)
project(chot)

set(CMAKE_CXX_STANDARD 20)

add_library(chot source/representation/square.hpp source/representation/move.cpp source/representation/move.hpp source/representation/piece.hpp source/representation/bitboard.cpp source/representation/bitboard.hpp source/representation/position.cpp source/representation/position.hpp source/representation/move_gen.cpp source/representation/move_gen.hpp source/util.hpp source/representation/lut.hpp)
add_executable(chot-driver source/main.cpp)
target_link_libraries(chot-driver PUBLIC chot)

add_executable(chot-perft tests/perft.cpp)
target_link_libraries(chot-perft PUBLIC chot)
target_include_directories(chot-perft PUBLIC source)

add_executable(chot-uci source/uci.cpp)
target_link_libraries(chot-uci PUBLIC chot)
target_include_directories(chot-uci PUBLIC source)
# position startpos moves d2d4 g8f6 c2c4 g7g6 b1c3 f8g7 e2e4 d7d6  g1f3 e8g8
