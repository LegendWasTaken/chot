#include <iostream>
#include <numeric>
#include "position.hpp"

int main() {
    auto position = chot::position("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1");
//    auto position = chot::position("rnbqkbnr/pppprppp/8/8/8/2b5/P1P2PPP/RNBQK2R w KQkq - 0 1");
    std::cout << position;

    auto moves = position.possible_moves();
    for (size_t i = 0; i < moves.size(); i++) {
        std::cout << moves[i] << std::endl;
    }
    std::cout << "Total possible moves: " << moves.size() << std::endl;

    return 0;
}
