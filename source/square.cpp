#include "square.hpp"

#include "util.hpp"

namespace chot {
    std::uint8_t square::data_for_index(chot::square_index index) noexcept {
        const auto file = index % 8;
        const auto rank = index / 8;
        return (file << file_shift) | (rank << rank_shift);
    }

    square::square(std::uint8_t file, std::uint8_t rank) :
            data((file << file_shift) | (rank << rank_shift)) {
    }

    square::square(chot::file file, chot::rank rank) :
            data((static_cast<std::uint8_t>(file) << file_shift) | static_cast<std::uint8_t>(rank) << rank_shift) {
    }

    square::square(chot::square_index index) : data(data_for_index(index)) {
    }

    square square::from_bits(std::uint64_t bits) {
        return square(detail::bit_index(bits));
    }

    square_index square::index() const noexcept {
        const auto file_num = (data >> file_shift) & 0x7;
        const auto rank_num = (data >> rank_shift) & 0x7;
        return file_num + rank_num * 8;
    }

    file square::file() const noexcept {
        const auto file_num = (data >> file_shift) & 0x7;
        return static_cast<chot::file>(file_num);
    }

    rank square::rank() const noexcept {
        const auto rank_num = (data >> rank_shift) & 0x7;
        return static_cast<chot::rank>(rank_num);
    }

    bool square::operator==(const square &rhs) const noexcept {
        return data == rhs.data;
    }

    bool square::operator!=(const square &rhs) const noexcept {
        return !this->operator==(rhs);
    }


} // chot