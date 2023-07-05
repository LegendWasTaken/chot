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

int total = 0;

void test_position(chot::position pos, int depth, std::uint64_t expected_node_count) {
    const auto node_count = perft(pos, depth);
    total += node_count;
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
    auto start = std::chrono::steady_clock::now();

    std::cout << "Running perft tests (Starting pos)\n--------------" << std::endl;
//    test_position(chot::position(), 0, 1);
//    test_position(chot::position(), 1, 20);
//    test_position(chot::position(), 2, 400);
//    test_position(chot::position(), 3, 8'902);
//    test_position(chot::position(), 4, 197'281);
//    test_position(chot::position(), 5, 4'865'609);

    std::cout << "Running perft tests (Kiwipete)\n--------------" << std::endl;
//    test_position(chot::position("r3k2r/p1ppqpb1/bn2pnp1/3PN3/1p2P3/2N2Q1p/PPPBBPPP/R3K2R w KQkq - 0 1"), 0, 1);
//    test_position(chot::position("r3k2r/p1ppqpb1/bn2pnp1/3PN3/1p2P3/2N2Q1p/PPPBBPPP/R3K2R w KQkq - 0 1"), 1, 48);
//    test_position(chot::position("r3k2r/p1ppqpb1/bn2pnp1/3PN3/1p2P3/2N2Q1p/PPPBBPPP/R3K2R w KQkq - 0 1"), 2, 2'039);
//    test_position(chot::position("r3k2r/p1ppqpb1/bn2pnp1/3PN3/1p2P3/2N2Q1p/PPPBBPPP/R3K2R w KQkq - 0 1"), 3, 97'862);
//    test_position(chot::position("r3k2r/p1ppqpb1/bn2pnp1/3PN3/1p2P3/2N2Q1p/PPPBBPPP/R3K2R w KQkq - 0 1"), 4, 4'085'603);
//    test_position(chot::position("r3k2r/p1ppqpb1/bn2pnp1/3PN3/1p2P3/2N2Q1p/PPPBBPPP/R3K2R w KQkq - 0 1"), 5, 193'690'690);

    std::cout << "Running perft tests (Position 3)\n--------------" << std::endl;
//    test_position(chot::position("8/2p5/3p4/KP5r/1R3p1k/8/4P1P1/8 w - - "), 0, 1);
//    test_position(chot::position("8/2p5/3p4/KP5r/1R3p1k/8/4P1P1/8 w - - "), 1, 14);
//    test_position(chot::position("8/2p5/3p4/KP5r/1R3p1k/8/4P1P1/8 w - - "), 2, 191);
//    test_position(chot::position("8/2p5/3p4/KP5r/1R3p1k/8/4P1P1/8 w - - "), 3, 2'812);
//    test_position(chot::position("8/2p5/3p4/KP5r/1R3p1k/8/4P1P1/8 w - - "), 4, 43'238);
//    test_position(chot::position("8/2p5/3p4/KP5r/1R3p1k/8/4P1P1/8 w - - "), 5, 674'624);
//    test_position(chot::position("8/2p5/3p4/KP5r/1R3p1k/8/4P1P1/8 w - - "), 6, 11'030'083);

    std::cout << "Running perft tests (Position 4)\n--------------" << std::endl;
//    test_position(chot::position("r3k2r/Pppp1ppp/1b3nbN/nP6/BBP1P3/q4N2/Pp1P2PP/R2Q1RK1 w kq - 0 1"), 0, 1);
//    test_position(chot::position("r3k2r/Pppp1ppp/1b3nbN/nP6/BBP1P3/q4N2/Pp1P2PP/R2Q1RK1 w kq - 0 1"), 1, 6);
//    test_position(chot::position("r3k2r/Pppp1ppp/1b3nbN/nP6/BBP1P3/q4N2/Pp1P2PP/R2Q1RK1 w kq - 0 1"), 2, 264);
//    test_position(chot::position("r3k2r/Pppp1ppp/1b3nbN/nP6/BBP1P3/q4N2/Pp1P2PP/R2Q1RK1 w kq - 0 1"), 3, 9'467);
//    test_position(chot::position("r3k2r/Pppp1ppp/1b3nbN/nP6/BBP1P3/q4N2/Pp1P2PP/R2Q1RK1 w kq - 0 1"), 4, 422'333);
//    test_position(chot::position("r3k2r/Pppp1ppp/1b3nbN/nP6/BBP1P3/q4N2/Pp1P2PP/R2Q1RK1 w kq - 0 1"), 5, 15'833'292);

    std::cout << "Running perft tests (Position 5)\n--------------" << std::endl;
//    test_position(chot::position("rnbq1k1r/pp1Pbppp/2p5/8/2B5/8/PPP1NnPP/RNBQK2R w KQ - 1 8"), 0, 1);
//    test_position(chot::position("rnbq1k1r/pp1Pbppp/2p5/8/2B5/8/PPP1NnPP/RNBQK2R w KQ - 1 8"), 1, 44);
//    test_position(chot::position("rnbq1k1r/pp1Pbppp/2p5/8/2B5/8/PPP1NnPP/RNBQK2R w KQ - 1 8"), 2, 1'486);
//    test_position(chot::position("rnbq1k1r/pp1Pbppp/2p5/8/2B5/8/PPP1NnPP/RNBQK2R w KQ - 1 8"), 3, 62'379);
    test_position(chot::position("rnbq1k1r/pp1Pbppp/2p5/8/2B5/8/PPP1NnPP/RNBQK2R w KQ - 1 8"), 4, 2'103'487);

    std::cout << "Running perft tests (Position 6)\n--------------" << std::endl;
//    test_position(chot::position("r4rk1/1pp1qppp/p1np1n2/2b1p1B1/2B1P1b1/P1NP1N2/1PP1QPPP/R4RK1 w - - 0 10"), 0, 1);
//    test_position(chot::position("r4rk1/1pp1qppp/p1np1n2/2b1p1B1/2B1P1b1/P1NP1N2/1PP1QPPP/R4RK1 w - - 0 10"), 1, 46);
//    test_position(chot::position("r4rk1/1pp1qppp/p1np1n2/2b1p1B1/2B1P1b1/P1NP1N2/1PP1QPPP/R4RK1 w - - 0 10"), 2, 2'079);
//    test_position(chot::position("r4rk1/1pp1qppp/p1np1n2/2b1p1B1/2B1P1b1/P1NP1N2/1PP1QPPP/R4RK1 w - - 0 10"), 3, 89'890);
    auto end = std::chrono::steady_clock::now();
    auto delta = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();
    std::cout << "Took " << delta << "ms. To iterate through " << total << " nodes" << std::endl;
}
