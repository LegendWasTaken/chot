#ifndef CHOT_BITBOARD_HPP
#define CHOT_BITBOARD_HPP

#include <array>
#include <cstdint>
#include <span>

#include "square.hpp"

namespace chot {
    struct test_result {
    private:
        std::uint16_t data;
    public:
        explicit test_result(std::uint64_t data);

        [[nodiscard]] bool is_occupied() const noexcept;

        [[nodiscard]] bool is_white() const noexcept;

        [[nodiscard]] bool is_capturable(bool is_white) const noexcept;

        [[nodiscard]] std::uint16_t raw_value() const noexcept;

        [[nodiscard]] std::uint8_t piece_index() const noexcept;
    };

    class bitboard {
    private:
        std::uint64_t data = 0;
    public:
        void set(chot::square_index index);

        void set(chot::square square);

        void unset(chot::square_index index);

        void unset(chot::square square);

        [[nodiscard]] bool occupied(chot::square_index index) const noexcept;

        [[nodiscard]] bool occupied(chot::square square) const noexcept;

        [[nodiscard]] static test_result test(std::span<const bitboard> boards, chot::square_index index) noexcept;

        [[nodiscard]] static test_result test(std::span<const bitboard> boards, chot::square square) noexcept;

        [[nodiscard]] bool static occupied(std::span<const bitboard> boards, chot::square_index index) noexcept;

        [[nodiscard]] bool static occupied(std::span<const bitboard> boards, chot::square square) noexcept;

        [[nodiscard]] std::uint64_t bits() const noexcept;
    };

} // chot

#endif //CHOT_BITBOARD_HPP
