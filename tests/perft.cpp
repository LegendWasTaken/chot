#include <iostream>
#include "position.hpp"

std::uint64_t perft(chot::position pos, int depth, bool display_debug = false) {
    if (depth == 0) {
        return 1;
    } // The base case is very important

    const auto moves = pos.possible_moves();
    auto under = std::uint64_t(0);
    for (size_t i = 0; i < moves.size(); i++) {
        const auto at_move = perft(pos.apply_move(moves[i]), depth - 1);
        under += at_move;
        if (display_debug) {
            std::cout << moves[i] << " - " << at_move << std::endl;
        }
    }

    return under;
}

void test_position(chot::position pos, int depth, std::uint64_t expected_node_count) {
    const auto node_count = perft(pos, depth);
    std::cout << "[Fen: \"" << pos.original_fen() << "\", depth: " << depth << "]";
    std::cout << " - [ Got: " << node_count << ", Expected: " << expected_node_count << "]";
    if (node_count != expected_node_count) {
        std::cout << " - FAILED" << std::endl;
        perft(pos, depth, true);
    } else {
        std::cout << " - SUCCESS" << std::endl;
    }

}

int main() {
    std::cout << "Running perft tests (Starting pos)\n--------------" << std::endl;
    test_position(chot::position(), 0, 1);
    test_position(chot::position(), 1, 20);
    test_position(chot::position(), 2, 400);
    test_position(chot::position(), 3, 8'902);
    test_position(chot::position(), 4, 197'281);
    test_position(chot::position(), 5, 4'865'609);

    std::cout << "Running perft tests (Weird positions)\n--------------" << std::endl;
//    test_position(chot::position("rnbqkbnr/p1pppppp/8/1p6/2P5/P7/1P1PPPPP/RNBQKBNR b KQkq c3 0 2"), 2, 481);
//    test_position(chot::position("rnbqkbnr/p1pppppp/8/8/2p5/P7/1P1PPPPP/RNBQKBNR w KQkq - 0 3"), 1, 20);
}
