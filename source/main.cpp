#include <iostream>
#include <numeric>
#include "position.hpp"


//.apply_move(
//            chot::move {
//                .from = chot::square(chot::file::e, chot::rank::fifth),
//                .to = chot::square(chot::file::d, chot::rank::fourth)
//            }
//
int main() {
    auto position = chot::position("7r/8/p5R1/P3k3/8/2K5/6n1/8 b - - 18 121");

    auto moves = position.possible_moves();
    for (size_t i = 0; i < moves.size(); i++) {
        std::cout << moves[i] << std::endl;
    }
    std::cout << "Total possible moves: " << moves.size() << std::endl;

    return 0;
}
