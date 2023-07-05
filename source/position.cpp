#include "position.hpp"

#include "move_gen.hpp"

namespace chot {
    position::position(std::string_view fen) : fen(fen) {
        auto it = fen.begin();

        { // Piece locations
            auto file = std::uint8_t(0);
            auto rank = std::uint8_t(7);

            while (it != fen.end()) {
                const auto character = *it;

                if (character == ' ') {
                    break;
                } else if (character == '/') {
                    file = 0;
                    rank -= 1;
                } else if ('0' <= character && character <= '9') {
                    file += static_cast<uint8_t>(character - '0');
                } else {
                    const auto piece = chot::piece::from(character);
                    const auto index = static_cast<uint8_t>(piece);
                    boards[index].set(chot::square(file, rank));
                    file += 1;
                }
                it++;
            }
        }

        {
            it++;
            if (*(it++) == 'w') {
                white_to_move = true;
            }
        }

        auto castling_rights = std::array<bool, 4>();
        {
            if (*(++it) != '-') {
                while (*(it) != ' ') {
                    const auto index_to_set = [character = *(it)]() {
                        switch (character) {
                            case 'K':
                                return 0;
                            case 'Q':
                                return 1;
                            case 'k':
                                return 2;
                            case 'q':
                                return 3;
                            default:
                                throw std::runtime_error("invalid castling fen");
                        }
                    };
                    castling_rights[index_to_set()] = true;
                    it++;
                }
            } else {
                it += 2;
            }
        }
        castling = chot::castling(castling_rights[0], castling_rights[1], castling_rights[2], castling_rights[3]);

        {
            if (*++it != '-') {
                const auto file = static_cast<uint8_t>(*(it++) - 'a');
                const auto rank = static_cast<uint8_t>((*(it++) - '0'));
                en_passant = chot::square(file, rank);
            }
        }
    }

    chot::move_container chot::position::possible_moves() const noexcept {
        auto moves = chot::move_container();

        {
            auto pawn_moves = chot::move_gen::pawns(boards, white_to_move, en_passant);
            for (size_t i = 0; i < pawn_moves.size(); i++) {
                moves.push_back(pawn_moves[i]);
            }
        }

        {
            auto knight_moves = chot::move_gen::knights(boards, white_to_move);
            for (size_t i = 0; i < knight_moves.size(); i++) {
                moves.push_back(knight_moves[i]);
            }
        }

        {
            auto bishop_moves = chot::move_gen::bishops(boards, white_to_move);
            for (size_t i = 0; i < bishop_moves.size(); i++) {
                moves.push_back(bishop_moves[i]);
            }
        }

        {
            auto rook_moves = chot::move_gen::rooks(boards, white_to_move);
            for (size_t i = 0; i < rook_moves.size(); i++) {
                moves.push_back(rook_moves[i]);
            }
        }

        {
            auto queen_moves = chot::move_gen::queens(boards, white_to_move);
            for (size_t i = 0; i < queen_moves.size(); i++) {
                moves.push_back(queen_moves[i]);
            }
        }

        {
            auto king_moves = chot::move_gen::king(boards, white_to_move, castling);
            for (size_t i = 0; i < king_moves.size(); i++) {
                moves.push_back(king_moves[i]);
            }
        }

        // Filter out moves that result in check
        auto legal_moves = chot::move_container();

        for (size_t i = 0; i < moves.size(); i++) {
            auto after_move = this->apply_move(moves[i]);
            if ((moves[i].short_castle || moves[i].long_castle) && this->is_check(white_to_move)) {
                continue;
            }

            if (moves[i].short_castle) {
                const auto rank = white_to_move ? rank::first : rank::eighth;
                // Is the king going through a check?
                const auto square_to_check = chot::square(file::f, rank);
                // Create fake moves for king to square to check
                const auto proxy_move = chot::move {
                    .from = chot::square(file::e, rank),
                    .to = square_to_check
                };
                auto after_proxy_move = this->apply_move(proxy_move);
                if (after_proxy_move.is_check(white_to_move)) {
                    continue;
                }
            } else if (moves[i].long_castle) {
                const auto rank = white_to_move ? rank::first : rank::eighth;
                const auto square_to_check = chot::square(file::d, rank);
                const auto proxy_move = chot::move {
                        .from = chot::square(file::e, rank),
                        .to = square_to_check
                };
                auto after_proxy_move = this->apply_move(proxy_move);
                if (after_proxy_move.is_check(white_to_move)) {
                    continue;
                }
            }

            if (!after_move.is_check(white_to_move)) {
                legal_moves.push_back(moves[i]);
            }
        }

        return legal_moves;
    }

    chot::position position::apply_move(move move) const noexcept {
        auto applied = *this;

        const auto from_index = bitboard::test(applied.boards, move.from).piece_index();
        applied.en_passant = std::nullopt;

        if (move.takes) {
            if (move.en_passant) {
                const auto to_index = bitboard::test(applied.boards, en_passant.value()).piece_index();
                applied.boards[to_index].unset(en_passant.value());
                applied.en_passant = std::nullopt;
            } else {
                const auto to_index = bitboard::test(applied.boards, move.to).piece_index();
                applied.boards[to_index].unset(move.to);

                if (to_index % 6 == static_cast<std::uint8_t>(piece::type::white_rook)) {
                    // If we're taking a rook, update the castling availability
                    const auto required_rank = white_to_move ? rank::eighth : rank::first;
                    if (move.to.rank() == required_rank) {
                        if (move.to.file() == file::h) {
                            applied.castling.disallow_kingside(!white_to_move);
                        } else if (move.to.file() == file::a) {
                            applied.castling.disallow_queenside(!white_to_move);
                        }
                    }
                }
            }
        }
        applied.boards[from_index].unset(move.from);


        // If it's a pawn and there's a promotion inbound
        if (from_index % 6 == static_cast<std::uint8_t>(piece::type::white_pawn) && move.promotion.has_value()) {
            applied.boards[static_cast<std::uint8_t>(move.promotion.value())].set(move.to);
        } else {
            applied.boards[from_index].set(move.to);
        }

        // If it's a pawn and moved 2 squares
        if (from_index % 6 == static_cast<std::uint8_t>(piece::type::white_pawn) &&
                std::abs(static_cast<std::int32_t>(move.from.rank()) - static_cast<std::int32_t>(move.to.rank())) == 2) {
            // it *might* be en passant, only set if there are pawns next to self
            auto pawn_to_side = false;

            if (move.to.file() != file::a) {
                // Check left
                const auto left = chot::square(move.to.index() - 1);
                if (chot::bitboard::test(boards, left).piece_index() == static_cast<std::uint8_t>(
                        white_to_move ? piece::type::black_pawn : piece::type::white_pawn)) {
                    pawn_to_side = true;
                }
            }

            if (move.to.file() != file::h) {
                // Check right
                const auto right = chot::square(move.to.index() + 1);
                if (chot::bitboard::test(boards, right).piece_index() == static_cast<std::uint8_t>(
                        white_to_move ? piece::type::black_pawn : piece::type::white_pawn)) {
                    pawn_to_side = true;
                }
            }

            if (pawn_to_side) {
                applied.en_passant = move.to;
            }
        }

        // No more castling if it's a king move
        if (from_index == static_cast<std::uint8_t>(applied.white_to_move ? piece::type::white_king : piece::type::black_king)) {
            applied.castling.disallow(applied.white_to_move);
        }

        // No more castling if it's a rook move (on that side)
        if (from_index == static_cast<std::uint8_t>(applied.white_to_move ? piece::type::white_rook : piece::type::black_rook)) {
            if (move.from.file() == file::a && applied.castling.queenside(applied.white_to_move)) {
                applied.castling.disallow_queenside(applied.white_to_move);
            } else if (move.from.file() == file::h && applied.castling.kingside(applied.white_to_move)) {
                applied.castling.disallow_kingside(applied.white_to_move);
            }
        }

        if (move.short_castle) {
            const auto rook_index = white_to_move ?
                                    chot::square(chot::file::h, chot::rank::first).index() :
                                    chot::square(chot::file::h, chot::rank::eighth).index();
            const auto position_index = white_to_move ?
                                        chot::square(chot::file::f, chot::rank::first).index() :
                                        chot::square(chot::file::f, chot::rank::eighth).index();
            const auto rook_board = static_cast<std::uint8_t>(white_to_move ? piece::type::white_rook
                                                                            : piece::type::black_rook);
            applied.boards[rook_board].unset(rook_index);
            applied.boards[rook_board].set(position_index);
            applied.castling.disallow(applied.white_to_move);
        }

        if (move.long_castle) {
            const auto rook_index = white_to_move ?
                                    chot::square(chot::file::a, chot::rank::first).index() :
                                    chot::square(chot::file::a, chot::rank::eighth).index();
            const auto position_index = white_to_move ?
                                        chot::square(chot::file::d, chot::rank::first).index() :
                                        chot::square(chot::file::d, chot::rank::eighth).index();
            const auto rook_board = static_cast<std::uint8_t>(white_to_move ? piece::type::white_rook
                                                                            : piece::type::black_rook);
            applied.boards[rook_board].unset(rook_index);
            applied.boards[rook_board].set(position_index);
            applied.castling.disallow(applied.white_to_move);
        }

        applied.white_to_move = !applied.white_to_move;

        return applied;
    }

    bool position::is_check(bool for_white) const noexcept {
        // Instead of seeing all possible moves for the other side (!for_white)
        // We enumerate all possible moves for all pieces at the (for_white) kings location (except annoying pawns),
        // then we check if any of those squares are covered by that type
        // squares_type_can_go_to_from(for_white_king_location) ∩ type_locations != ∅
        // Pawns are an annoying exceptions, due to the nature of their movement. So they must be hardcoded to a degree
        const auto king_board_index = static_cast<std::uint8_t>(for_white ? piece::type::white_king : piece::type::black_king);
        const auto king_index = detail::bit_index(boards[king_board_index].bits());

        auto in_check = false;

        // Knight
        move_gen::detail::enumerate_knight_moves(boards, for_white, king_index, [this, &in_check, for_white](chot::move move){
            const auto target_square = move.to;
            const auto board_index = static_cast<std::uint8_t>(for_white ? piece::type::black_knight : piece::type::white_knight);
            if (boards[board_index].occupied(target_square)) {
                in_check = true;
            }
        });

        if (in_check) {
            return true;
        }

        // Bishop
        move_gen::detail::enumerate_bishop_moves(boards, for_white, king_index, [this, &in_check, for_white](chot::move move){
            const auto target_square = move.to;
            const auto board_index = static_cast<std::uint8_t>(for_white ? piece::type::black_bishop : piece::type::white_bishop);
            if (boards[board_index].occupied(target_square)) {
                in_check = true;
            }
        });

        if (in_check) {
            return true;
        }

        // Rook
        move_gen::detail::enumerate_rook_moves(boards, for_white, king_index, [this, &in_check, for_white](chot::move move){
            const auto target_square = move.to;
            const auto board_index = static_cast<std::uint8_t>(for_white ? piece::type::black_rook : piece::type::white_rook);
            if (boards[board_index].occupied(target_square)) {
                in_check = true;
            }
        });

        if (in_check) {
            return true;
        }

        // Queen - Maybe this can be added in the rook / bishop checks? save some time
        move_gen::detail::enumerate_queen_moves(boards, for_white, king_index, [this, &in_check, for_white](chot::move move){
            const auto target_square = move.to;
            const auto board_index = static_cast<std::uint8_t>(for_white ? piece::type::black_queen : piece::type::white_queen);
            if (boards[board_index].occupied(target_square)) {
                in_check = true;
            }
        });

        if (in_check) {
            return true;
        }

         // Pawns
        {
            const auto color_sign = for_white ? 1 : -1;
            const auto king_square = chot::square(king_index);
            const auto enemy_pawn_type = for_white ? piece::type::black_pawn : piece::type::white_pawn;

            if (king_square.file() != file::a) {
                const auto left = chot::square(king_index - 1 + 8 * color_sign);
                if (bitboard::test(boards, left).piece_index() == static_cast<std::uint8_t>(enemy_pawn_type)) {
                    in_check = true;
                }
            }

            if (king_square.file() != file::h) {
                const auto right = chot::square(king_index + 1 + 8 * color_sign);
                if (bitboard::test(boards, right).piece_index() == static_cast<std::uint8_t>(enemy_pawn_type)) {
                    in_check = true;
                }
            }
        }

        return in_check;
    }

    bool position::is_checkmate(bool for_white) const noexcept {
        return false;
    }

    std::string_view position::original_fen() const noexcept {
        return fen;
    }
} // chot