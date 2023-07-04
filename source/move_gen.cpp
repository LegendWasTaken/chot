#include "move_gen.hpp"

#include "lut.hpp"

namespace chot::move_gen {
    namespace detail {
        void enumerate_pawn_moves(
                std::array<chot::bitboard, 12> boards,
                bool is_white,
                std::optional<chot::square> en_passant,
                chot::square_index i,
                std::function<void(chot::move move)> callback) {
            const auto square = chot::square(i);
            const auto color_sign = is_white ? 1 : -1;
            const auto initial_rank = is_white ? rank::second : rank::seventh;
            // Regular move forward
            if (!chot::bitboard::test(boards, i + 8 * color_sign).is_occupied()) {
                callback(chot::move {
                        .from = square,
                        .to = chot::square(i + 8 * color_sign)
                });

                if (square.rank() == initial_rank) {
                    if (!chot::bitboard::test(boards, i + 16 * color_sign).is_occupied()) {
                        callback(chot::move {
                                .from = square,
                                .to = chot::square(i + 16 * color_sign)
                        });
                    }
                }
            }

            // French move.png
            if (en_passant.has_value()) {
                const auto left = chot::square(i + 1);
                const auto right = chot::square(i - 1);
                if ((left.rank() == square.rank() && *en_passant == left) || (right.rank() == square.rank() && *en_passant == right)) {
                    callback(chot::move{
                            .from = square,
                            .to = chot::square(en_passant.value().index() + 8 * color_sign),
                            .takes = true,
                    });
                }
            }

            // Captures
            {
                if (square.file() != file::a) {
                    const auto left = chot::square(i - 1 + 8 * color_sign);
                    if (static_cast<std::uint8_t>(left.rank()) == static_cast<std::uint8_t>(square.rank()) + 1 * color_sign) {
                        if (chot::bitboard::test(boards, left).is_capturable(is_white)) {
                            callback(chot::move {
                                    .from = square,
                                    .to = left,
                                    .takes = true,
                            });
                        }
                    }
                }

                if (square.file() != file::h) {
                    const auto right = chot::square(i + 1 + 8 * color_sign);
                    if (static_cast<std::uint8_t>(right.rank()) == static_cast<std::uint8_t>(square.rank()) + 1 * color_sign) {
                        if (chot::bitboard::test(boards, right).is_capturable(is_white)) {
                            callback(chot::move {
                                    .from = square,
                                    .to = right,
                                    .takes = true,
                            });
                        }
                    }
                }
            }
        }

        void enumerate_knight_moves(
                std::array<chot::bitboard, 12> boards,
                bool is_white,
                chot::square_index i,
                std::function<void(chot::move move)> callback) {
            const auto square = chot::square(i);
            const auto move_bits = lut::knight_lut[i];
            for (std::uint64_t j = 0; j < 64; j++) {
                if (move_bits & std::uint64_t(1) << j) {
                    const auto target = chot::square(j);
                    const auto at = chot::bitboard::test(boards, target);
                    if (!at.is_occupied()) {
                        // No piece
                        callback(chot::move {
                                .from = square,
                                .to = chot::square(j),
                        });
                    } else if (at.is_capturable(is_white)) {
                        // Enemy piece
                        callback(chot::move {
                                .from = square,
                                .to = chot::square(j),
                                .takes = true,
                        });
                    }
                }
            }
        }

        void enumerate_bishop_moves(
                std::array<chot::bitboard, 12> boards,
                bool is_white,
                chot::square_index i,
                std::function<void(chot::move move)> callback) {
            auto moves = chot::move_container(); // Todo: Update this to use callbacks instead of storing then iterating

            const auto dxs = std::array<int, 4>({1, -1, 1, -1});
            const auto dys = std::array<int, 4>({1, 1, -1, -1});
            detail::sliding_piece(boards, is_white, moves, chot::square(i), dxs, dys);
            for (size_t j = 0; j < moves.size(); j++) {
                callback(moves[j]);
            }
        }

        void enumerate_rook_moves(
                std::array<chot::bitboard, 12> boards,
                bool is_white,
                chot::square_index i,
                std::function<void(chot::move move)> callback) {
            auto moves = chot::move_container(); // Todo: Update this to use callbacks instead of storing then iterating

            const auto dxs = std::array<int, 4>({0, 0, 1, -1});
            const auto dys = std::array<int, 4>({1, -1, 0, 0});
            detail::sliding_piece(boards, is_white, moves, chot::square(i), dxs, dys);
            for (size_t j = 0; j < moves.size(); j++) {
                callback(moves[j]);
            }
        }

        void enumerate_queen_moves(
                std::array<chot::bitboard, 12> boards,
                bool is_white,
                chot::square_index i,
                std::function<void(chot::move move)> callback) {
            auto moves = chot::move_container(); // Todo: Update this to use callbacks instead of storing then iterating

            const auto dxs = std::array<int, 8>({0, 0, 1, -1, 1, -1, 1, -1});
            const auto dys = std::array<int, 8>({1, -1, 0, 0, 1, 1, -1, -1});
            detail::sliding_piece(boards, is_white, moves, chot::square(i), dxs, dys);
            for (size_t j = 0; j < moves.size(); j++) {
                callback(moves[j]);
            }
        }

        void enumerate_king_moves(
                std::array<chot::bitboard, 12> boards,
                bool is_white,
                chot::castling rights,
                chot::square_index i,
                std::function<void(chot::move move)> callback) {
            const auto square = chot::square(i);

            const auto dxs = std::array<int, 8>({0, 0, 1, -1, 1, -1, 1, -1});
            const auto dys = std::array<int, 8>({1, -1, 0, 0, 1, 1, -1, -1});
            const auto file = static_cast<std::uint8_t>(square.file());
            const auto rank = static_cast<std::uint8_t>(square.rank());
            for (int di = 0; di < dxs.size(); di++) {
                const auto dx = dxs[di];
                const auto dy = dys[di];
                if (0 <= file + dx && file + dx < 8 && 0 <= rank + dy && rank + dy < 8) {
                    const auto target = chot::square(file + dx, rank + dy);

                    const auto test = chot::bitboard::test(boards, target);
                    if (test.is_occupied()) {
                        if (test.is_capturable(is_white)) {
                            callback(chot::move {
                                    .from = square,
                                    .to = target,
                                    .takes = true,
                            });
                        }
                    } else {
                        callback(chot::move {
                                .from = square,
                                .to = target,
                        });
                    }
                }
            }

            // Short castle
            if (rights.kingside(is_white)) {
                const auto bishop_test = chot::bitboard::test(boards, i + 1);
                const auto knight_test = chot::bitboard::test(boards, i + 2);
                if (!bishop_test.is_occupied() && !knight_test.is_occupied()) {
                    callback(chot::move {
                            .from = square,
                            .to = chot::square(i + 2),
                            .short_castle = true,
                    });
                }
            }

            // Long castle
            if (rights.queenside(is_white)) {
                const auto queen_test = chot::bitboard::test(boards, i - 1);
                const auto bishop_test = chot::bitboard::test(boards, i - 2);
                const auto knight_test = chot::bitboard::test(boards, i - 3);
                if (!queen_test.is_occupied() && !bishop_test.is_occupied() && !knight_test.is_occupied()) {
                    callback(chot::move {
                            .from = square,
                            .to = chot::square(i - 2),
                            .long_castle = true,
                    });
                }
            }
        }
    }

    chot::move_container pawns(std::array<chot::bitboard, 12> boards, bool is_white, std::optional<chot::square> en_passant) {
        auto moves = chot::move_container();

        const auto board_offset = is_white ? 0 : 6;
        const auto &board = boards[board_offset + static_cast<std::uint8_t>(piece::type::white_pawn)];
        for (std::uint64_t i = 0; i < 64; i++) {
            const auto square = chot::square(i);
            if (board.occupied(square)) {
                detail::enumerate_pawn_moves(boards, is_white, en_passant, i, [&moves](chot::move move){
                    moves.push_back(move);
                });
            }
        }

        return moves;
    }

    chot::move_container knights(std::array<chot::bitboard, 12> boards, bool is_white) {
        auto moves = chot::move_container();

        const auto board_offset = is_white ? 0 : 6;
        const auto &board = boards[board_offset + static_cast<std::uint8_t>(piece::type::white_knight)];
        for (std::uint64_t i = 0; i < 64; i++) {
            const auto square = chot::square(i);
            if (board.occupied(square)) {
                detail::enumerate_knight_moves(boards, is_white, i, [&moves](chot::move move){
                    moves.push_back(move);
                });
            }
        }

        return moves;
    }

    chot::move_container bishops(std::array<chot::bitboard, 12> boards, bool is_white) {
        auto moves = chot::move_container();

        const auto board_offset = is_white ? 0 : 6;
        const auto &board = boards[board_offset + static_cast<std::uint8_t>(piece::type::white_bishop)];
        for (std::uint64_t i = 0; i < 64; i++) {
            const auto square = chot::square(i);
            if (board.occupied(square)) {
                detail::enumerate_bishop_moves(boards, is_white, i, [&moves](chot::move move){
                    moves.push_back(move);
                });
            }
        }

        return moves;
    }

    chot::move_container rooks(std::array<chot::bitboard, 12> boards, bool is_white) {
        auto moves = chot::move_container();

        const auto board_offset = is_white ? 0 : 6;
        const auto &board = boards[board_offset + static_cast<std::uint8_t>(piece::type::white_rook)];
        for (std::uint64_t i = 0; i < 64; i++) {
            const auto square = chot::square(i);
            if (board.occupied(square)) {
                detail::enumerate_rook_moves(boards, is_white, i, [&moves](chot::move move){
                    moves.push_back(move);
                });
            }
        }

        return moves;
    }

    chot::move_container queens(std::array<chot::bitboard, 12> boards, bool is_white) {
        auto moves = chot::move_container();

        const auto board_offset = is_white ? 0 : 6;
        const auto &board = boards[board_offset + static_cast<std::uint8_t>(piece::type::white_queen)];
        for (std::uint64_t i = 0; i < 64; i++) {
            const auto square = chot::square(i);
            if (board.occupied(square)) {
                detail::enumerate_queen_moves(boards, is_white, i, [&moves](chot::move move){
                    moves.push_back(move);
                });
            }
        }

        return moves;
    }

    chot::move_container king(std::array<chot::bitboard, 12> boards, bool is_white, chot::castling rights) {
        auto moves = chot::move_container();

        const auto board_offset = is_white ? 0 : 6;
        const auto &board = boards[board_offset + static_cast<std::uint8_t>(piece::type::white_king)];
        for (std::uint64_t i = 0; i < 64; i++) {
            const auto square = chot::square(i);
            if (board.occupied(square)) {
                detail::enumerate_king_moves(boards, is_white, rights, i, [&moves](chot::move move){
                    moves.push_back(move);
                });
            }
        }

        return moves;
    }
} // chot