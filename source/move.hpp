#ifndef CHOT_MOVE_HPP
#define CHOT_MOVE_HPP

#include <array>
#include <optional>
#include <ostream>

#include "square.hpp"
#include "piece.hpp"

namespace chot {

    struct move {
    public:
        chot::square from = {0, 0};
        chot::square to = {0, 0};
        bool check = false;
        bool takes = false;
        bool short_castle = false;
        bool long_castle = false;
        bool en_passant = false;
        std::optional<chot::piece::type> promotion = std::nullopt;
    };

    struct move_container {
    private:
        // https://www.chessprogramming.org/Encoding_Moves#MoveIndex
        constexpr static std::uint8_t max_legal_moves = 218;
        std::array<move, max_legal_moves> moves = {};
        std::uint8_t move_count = 0;

    public:
        move_container() = default;

        void push_back(chot::move move);

        [[nodiscard]] std::size_t size() const noexcept;

        [[nodiscard]] chot::move operator[](std::size_t index) const noexcept;
    };

    inline std::ostream &operator<<(std::ostream &stream, const chot::move &move) {
        if (move.from.index() == 0 && move.to.index() == 0) {
            stream << "invalid";
        } else {
            if (move.short_castle) {
                stream << "O-O";
            } else if (move.long_castle) {
                stream << "O-O-O";
            } else {
                stream << move.from;
                if (move.takes) {
//                    stream << 'x';
                }
                stream << move.to;
                if (move.check) {
                    stream << '+';
                }
            }
        }

        return stream;
    }

} // chot

#endif //CHOT_MOVE_HPP
