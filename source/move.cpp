#include "move.hpp"

namespace chot {
    void move_container::push_back(chot::move move) {
        moves[move_count++] = move;
    }

    std::size_t move_container::size() const noexcept {
        return move_count;
    }

    chot::move move_container::operator[](std::size_t index) const noexcept {
        return moves[index];
    }
} // chot