#include "engine.hpp"

#include <iostream>

namespace chot::engine {
    namespace pos_eval {
        float legal_move_eval(chot::position &position) {
            return static_cast<float>(position.possible_moves().size()) / 218.0f;
        }

        float material_advantage(chot::position &position) {
            return static_cast<float>(position.material_delta() * (position.is_whites_turn() ? 1 : -1));
        }

        float eval_for_player(chot::position &position) {
            return
//                    legal_move_eval(position) +
                            material_advantage(position);
        }
    }

    namespace detail {
        float minmax(chot::position &position, std::int32_t depth) {
            const auto moves = position.possible_moves();
            if (depth == 0 || moves.size() == 0) { // is == 0 a checkmate?
                return pos_eval::eval_for_player(position);
            }
            auto best_value = std::numeric_limits<float>::infinity() * (position.is_whites_turn() ? -1.0f : 1.0f);
            for (size_t i = 0; i < moves.size(); i++) {
                auto pos = position.apply_move(moves[i]);
                best_value = position.is_whites_turn() ?
                        std::max(minmax(pos, depth - 1), best_value) :
                        std::min(minmax(pos, depth - 1), best_value);
            }

            return best_value;
        }

        float alpha_beta_minmax(chot::position node, std::int32_t depth, float alpha, float beta) {
            const auto moves = node.possible_moves();
            if (depth == 0 || moves.size() == 0) {
                return pos_eval::eval_for_player(node);
            }

            if (node.is_whites_turn()) {
                auto value = -std::numeric_limits<float>::infinity();
                for (size_t i = 0; i < moves.size(); i++) {
                    value = std::max(value, alpha_beta_minmax(node.apply_move(moves[i]), depth - 1, alpha, beta));
                    alpha = std::max(alpha, value);
                    if (value >= beta) {
                        break; // beta cutoff
                    }
                }
                return value;
            } else {
                auto value = std::numeric_limits<float>::infinity();
                for (size_t i = 0; i < moves.size(); i++) {
                    value = std::min(value, alpha_beta_minmax(node.apply_move(moves[i]), depth - 1, alpha, beta));
                    beta = std::min(beta, value);
                    if (value <= alpha) {
                        break; // alpha cutoff
                    }
                }
                return value;
            }
        }
    }


    chot::move best_move(chot::position &pos) {
        constexpr auto inf = std::numeric_limits<float>::infinity();

        const auto moves = pos.possible_moves();
        auto evaluations = std::vector<std::pair<float, chot::move>>();
        for (size_t i = 0; i < moves.size(); i++) {
            auto to_check = pos.apply_move(moves[i]);
            evaluations.emplace_back(detail::alpha_beta_minmax(to_check, 3, -inf, inf), moves[i]);
            std::cout << "Eval: " << evaluations.back().first << ", Move: " << evaluations.back().second << std::endl;
        }

        std::sort(evaluations.begin(), evaluations.end(), [is_white = pos.is_whites_turn()](auto lhs, auto rhs){
            return is_white ? lhs.first > rhs.first : lhs.first < rhs.first;
        });

        return evaluations[0].second;
    }
} // chot