#include "bitboard.hpp"

#include "util.hpp"

namespace chot {

    void bitboard::set(chot::square_index index) {
        data |= std::uint64_t(1) << index;
    }

    void bitboard::set(chot::square square) {
        set(square.index());
    }

    void bitboard::unset(chot::square_index index) {
        data &= ~(std::uint64_t(1) << index);
    }

    void bitboard::unset(chot::square square) {
        unset(square.index());
    }

    bool bitboard::occupied(chot::square_index index) const noexcept {
        return data & (std::uint64_t(1) << index);
    }

    bool bitboard::occupied(chot::square square) const noexcept {
        return occupied(square.index());
    }

    chot::test_result bitboard::test(std::span<const bitboard> boards, chot::square_index index) noexcept {
        auto result = std::uint16_t();

        for (std::uint16_t i = 0; i < boards.size(); i++) {
            const auto set = boards[i].occupied(index);
            result |= std::uint16_t(set) << i;
        }

        return chot::test_result(result);
    }

    chot::test_result bitboard::test(std::span<const bitboard> boards, chot::square square) noexcept {
        return test(boards, square.index());
    }

    bool bitboard::occupied(std::span<const bitboard> boards, chot::square_index index) noexcept {
        for (const auto &board : boards) {
            if (board.occupied(index)) {
                return true;
            }
        }

        return false;
    }

    bool bitboard::occupied(std::span<const bitboard> boards, chot::square square) noexcept {
        return occupied(boards, square.index());
    }

    std::uint64_t bitboard::bits() const noexcept {
        return data;
    }

    test_result::test_result(std::uint64_t data) : data(data) {

    }

    bool test_result::is_occupied() const noexcept {
        return data != 0;
    }

    bool test_result::is_white() const noexcept {
        return data < 6;
    }

    bool test_result::is_capturable(bool is_white) const noexcept {
        const auto bit_index = chot::detail::bit_index(data);
        if (is_white) {
            return bit_index >= 6;
        } else {
            return bit_index < 6;
        }
    }

    std::uint16_t test_result::raw_value() const noexcept {
        return data;
    }

    std::uint8_t test_result::piece_index() const noexcept {
        return chot::detail::bit_index(data);
    }
} // chot