#ifndef CHOT_ENGINE_HPP
#define CHOT_ENGINE_HPP

#include <representation/position.hpp>

namespace chot::engine {
    namespace pos_eval {
        [[nodiscard]] float legal_move(chot::position &position);

        [[nodiscard]] float material_advantage(chot::position &position);

        [[nodiscard]] float eval_for_player(chot::position &position);
    }

    namespace detail {
        [[nodiscard]] float minmax(chot::position &position, std::int32_t depth);
    }

    [[nodiscard]] chot::move best_move(chot::position &position);
} // chot

#endif //CHOT_ENGINE_HPP
