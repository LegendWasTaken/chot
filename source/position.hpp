#ifndef CHOT_POSITION_HPP
#define CHOT_POSITION_HPP

#include "bitboard.hpp"
#include "piece.hpp"
#include "move.hpp"
#include "util.hpp"

#include <array>
#include <string_view>
#include <span>

#include <iostream>

namespace chot {

    class position {
    private:
        friend std::ostream &operator<<(std::ostream&, const chot::position&);

        std::string_view fen;
        std::array<chot::bitboard, 12> boards = {};
        std::optional<chot::square> en_passant = {};
        chot::castling castling = {};
        bool white_to_move = false;

    public:
        explicit position(std::string_view fen = "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1");

        position(const position &other) = default;

        [[nodiscard]] chot::move_container possible_moves() const noexcept;

        [[nodiscard]] chot::position apply_move(chot::move move) const noexcept;

        [[nodiscard]] bool is_check(bool for_white) const noexcept;

        [[nodiscard]] bool is_checkmate(bool for_white) const noexcept;

        [[nodiscard]] std::string_view original_fen() const noexcept;

        [[nodiscard]] bool occupied(chot::square square) const noexcept;

        [[nodiscard]] chot::castling &castling_rights() noexcept;

        [[nodiscard]] bool is_whites_turn() const noexcept;
    };

    inline std::ostream &operator<<(std::ostream &stream, const chot::position &position) {
        const auto pieces = std::array<std::string_view, 12>({
                                                                     "♟",
                                                                     "♞",
                                                                     "♝",
                                                                     "♜",
                                                                     "♛",
                                                                     "♚",
                                                                     "♙",
                                                                     "♘",
                                                                     "♗",
                                                                     "♖",
                                                                     "♕",
                                                                     "♔"});

        const auto print_character_at = [&](std::uint8_t file, std::uint8_t rank) {
            const auto test = bitboard::test(position.boards, chot::square(file, rank));
            stream << ' ';
            if (!test.is_occupied()) {
                stream << '.';
            } else {
                const auto index = detail::bit_index(test.raw_value());
                stream << pieces[index];
            }
        };

        stream << "  +-----------------+\n";
        for (auto rank = 8; rank >= 1; rank--) {
            stream << rank << " |";
            for (auto file = 0; file < 8; file++) {
                print_character_at(static_cast<uint8_t>(file), static_cast<uint8_t>(rank - 1));
            }
            stream << " |" << '\n';
        }
        stream << "  +-----------------+\n    A B C D E F G H\n";

        return stream;
    }

} // chot

#endif //CHOT_POSITION_HPP
