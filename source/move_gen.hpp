#ifndef CHOT_MOVE_GEN_HPP
#define CHOT_MOVE_GEN_HPP

#include "piece.hpp"
#include "bitboard.hpp"
#include "move.hpp"

namespace chot::move_gen {
    namespace detail {
        [[nodiscard]] inline bool
        process_move(std::array<chot::bitboard, 12> boards, bool is_white, chot::move_container &moves,
                     chot::square from, chot::square to) {
            const auto test = chot::bitboard::test(boards, to);
            if (test.is_occupied()) {
                if (test.is_capturable(is_white)) {
                    moves.push_back(chot::move{
                            .from = from,
                            .to = to,
                            .takes = true,
                    });
                }
                return false;
            } else {
                moves.push_back(chot::move{
                        .from = from,
                        .to = to,
                });
                return true;
            }
        }

        inline void sliding_piece(std::array<chot::bitboard, 12> boards, bool is_white, chot::move_container &moves,
                                  chot::square from, std::span<const int> dxs, std::span<const int> dys) {
            const auto rank = static_cast<std::uint8_t>(from.rank());
            const auto file = static_cast<std::uint8_t>(from.file());
            for (int i = 0; i < dxs.size(); i++) {
                const auto dx = dxs[i];
                const auto dy = dys[i];

                for (
                        int target_file = file + dx, target_rank = rank + dy;
                        0 <= target_file && target_file < 8 &&
                        0 <= target_rank && target_rank < 8;
                        target_file += dx, target_rank += dy) {
                    const auto can_continue = process_move(boards, is_white, moves, from,
                                                           chot::square(target_file, target_rank));
                    if (!can_continue) {
                        break;
                    }
                }
            }
        }

        void enumerate_pawn_moves(
                std::array<chot::bitboard, 12> boards,
                bool is_white,
                std::optional<chot::square> en_passant,
                chot::square_index index,
                std::function<void(chot::move move)> callback);

        void enumerate_knight_moves(
                std::array<chot::bitboard, 12> boards,
                bool is_white,
                chot::square_index index,
                std::function<void(chot::move move)> callback);

        void enumerate_bishop_moves(
                std::array<chot::bitboard, 12> boards,
                bool is_white,
                chot::square_index index,
                std::function<void(chot::move move)> callback);

        void enumerate_rook_moves(
                std::array<chot::bitboard, 12> boards,
                bool is_white,
                chot::square_index index,
                std::function<void(chot::move move)> callback);

        void enumerate_queen_moves(
                std::array<chot::bitboard, 12> boards,
                bool is_white,
                chot::square_index index,
                std::function<void(chot::move move)> callback);

        void enumerate_king_moves(
                std::array<chot::bitboard, 12> boards,
                bool is_white,
                chot::castling rights,
                chot::square_index index,
                std::function<void(chot::move move)> callback);
    }

    [[nodiscard]] chot::move_container
    pawns(std::array<chot::bitboard, 12> boards, bool is_white, std::optional<chot::square> en_passant);

    [[nodiscard]] chot::move_container knights(std::array<chot::bitboard, 12> boards, bool is_white);

    [[nodiscard]] chot::move_container bishops(std::array<chot::bitboard, 12> boards, bool is_white);

    [[nodiscard]] chot::move_container rooks(std::array<chot::bitboard, 12> boards, bool is_white);

    [[nodiscard]] chot::move_container queens(std::array<chot::bitboard, 12> boards, bool is_white);

    [[nodiscard]] chot::move_container king(std::array<chot::bitboard, 12> boards, bool is_white, chot::castling rights);
} // chot

#endif //CHOT_MOVE_GEN_HPP
