#include <position.hpp>

#include <iostream>
#include <string>
#include <sstream>
#include <fstream>

struct uci_state {
    chot::position position;
};

chot::square parse_square(std::string_view str) {
    const auto file = static_cast<std::uint8_t>(str[0] - 'a');
    const auto rank = static_cast<std::uint8_t>(str[1] - '0' - 1);
    return {file, rank};
}

std::optional<chot::piece::type> parse_promotion_type(std::string_view str) {
    if (str[0] == 'N') {
        return chot::piece::type::white_knight;
    } else if (str[0] == 'B') {
        return chot::piece::type::white_bishop;
    } else if (str[0] == 'R') {
        return chot::piece::type::white_rook;
    } else if (str[0] == 'Q') {
        return chot::piece::type::white_queen;
    }

    // Handle the black pieces
    if (str[0] == 'n') {
        return chot::piece::type::black_knight;
    } else if (str[0] == 'b') {
        return chot::piece::type::black_bishop;
    } else if (str[0] == 'r') {
        return chot::piece::type::black_rook;
    } else if (str[0] == 'q') {
        return chot::piece::type::black_queen;
    } else {
        return std::nullopt;
    }
}

chot::move parse_move(std::string_view str, chot::position *position) {
    auto from = parse_square(str.substr(0, 2));
    auto to = parse_square(str.substr(2, 2));
    auto promotion = parse_promotion_type(str.substr(4));

    // Check if it's castling
    const auto castling_move = [from, to, position]() -> std::optional<chot::move> {
        auto &castling = position->castling_rights();
        const auto white_to_move = position->is_whites_turn();
        const auto rank_for_castling = white_to_move ? chot::rank::first : chot::rank::eighth;

        if (from.rank() == rank_for_castling && to.rank() == rank_for_castling) {
            if (from.file() == chot::file::e) {
                if (to.file() == chot::file::g && castling.kingside(white_to_move)) {
                    castling.disallow_kingside(white_to_move);
                    return chot::move {
                        .from = from,
                        .to = to,
                        .short_castle = true,
                    };
                } else if (to.file() == chot::file::c && castling.queenside(white_to_move)) {
                    castling.disallow_queenside(white_to_move);
                    return chot::move {
                            .from = from,
                            .to = to,
                            .long_castle = true,
                    };
                }
            }
        }

        return std::nullopt;
    }();

    return castling_move.value_or(chot::move {
        .from = from,
        .to = to,
        .takes = position->occupied(to),
        .promotion = promotion
    });
}

void handle_uci_command(std::string_view command, uci_state *state) {
    auto in_stream = std::istringstream(std::string(command));
    auto token = std::string();

    while (in_stream >> token) {
        if (token == "uci") {
            std::cout << "id name Chot" << std::endl;
            std::cout << "id author Caio Wakamatsu" << std::endl;
            std::cout << "uciok" << std::endl;
        } else if (token == "isready") {
            std::cout << "readyok" << std::endl;
        } else if (token == "ucinewgame") {
            // dont really need to do anything here i dont think
        } else if (token == "position") {

            // Figure out starting pos
            auto pos_type = std::string();
            in_stream >> pos_type;
            if (pos_type == "startpos") {
                state->position = chot::position();
            } else if (pos_type == "fen") {
                auto fen = std::string();
                in_stream >> fen;
                state->position = chot::position(fen);
            }

            // Parse moves if any
            auto moves_token = std::string();
            in_stream >> moves_token;
            if (moves_token == "moves") {
                auto move_token = std::string();
                while (in_stream >> move_token) {
                    auto move = parse_move(move_token, &state->position);
                    state->position = state->position.apply_move(move);
                }
            }
        } else if (token == "go") {
            auto moves = state->position.possible_moves();
            const auto move_index = std::rand() % moves.size();
            std::cout << "bestmove " << moves[move_index] << std::endl;
        }
    }
}

int main() {
    auto state = uci_state();

    auto input = std::string();
    while (std::getline(std::cin, input)) {
        handle_uci_command(input, &state);
    }
}
