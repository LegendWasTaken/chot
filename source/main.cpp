#include <iostream>
#include <numeric>
#include "representation/position.hpp"
#include "engine/engine.hpp"


//.apply_move(
//            chot::move {
//                .from = chot::square(chot::file::e, chot::rank::fifth),
//                .to = chot::square(chot::file::d, chot::rank::fourth)
//            }
//
int main() {
    auto position = chot::position("rnbqkbnr/1pBppppp/8/p7/3P4/8/PPP1PPPP/RN1QKBNR b KQkq - 0 1");
    auto material_advantage = chot::engine::pos_eval::material_advantage(position);
    std::cout << material_advantage << std::endl;


    auto best_move = chot::engine::best_move(position);
    std::cout << "Best move: " << best_move << std::endl;

    return 0;
}
