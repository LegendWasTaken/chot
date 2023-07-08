#ifndef CHOT_SQUARE_HPP
#define CHOT_SQUARE_HPP

#include <cstdint>
#include <ostream>

namespace chot {
    using square_index = std::uint8_t;

    enum class file : std::uint8_t {
        a,
        b,
        c,
        d,
        e,
        f,
        g,
        h,
    };

    enum class rank : std::uint8_t {
        first,
        second,
        third,
        fourth,
        fifth,
        sixth,
        seventh,
        eighth
    };

    class square {
    private:
        constexpr static std::uint8_t file_shift = 0x0;
        constexpr static std::uint8_t rank_shift = 0x4;

        std::uint8_t data = 0; // & 0x3 = file, & 0x38 = rank

        [[nodiscard]] static std::uint8_t data_for_index(chot::square_index index) noexcept {
            const auto file = index % 8;
            const auto rank = index / 8;
            return (file << file_shift) | (rank << rank_shift);
        }

    public:
        __attribute__((always_inline)) square(std::uint8_t file, std::uint8_t rank) :
                data((file << file_shift) | (rank << rank_shift)) {
        }

        __attribute__((always_inline)) square(chot::file file, chot::rank rank) :
                data((static_cast<std::uint8_t>(file) << file_shift) | static_cast<std::uint8_t>(rank) << rank_shift) {
        };

        __attribute__((always_inline)) explicit square(chot::square_index index): data(data_for_index(index)) {
        }

        [[nodiscard]] chot::square_index index() const noexcept {
            const auto file_num = (data >> file_shift) & 0x7;
            const auto rank_num = (data >> rank_shift) & 0x7;
            return file_num + rank_num * 8;
        }

        [[nodiscard]] chot::file file() const noexcept {
            const auto file_num = (data >> file_shift) & 0x7;
            return static_cast<chot::file>(file_num);
        }


        [[nodiscard]] chot::rank rank() const noexcept {
            const auto rank_num = (data >> rank_shift) & 0x7;
            return static_cast<chot::rank>(rank_num);
        }

        [[nodiscard]] bool operator==(const chot::square &rhs) const noexcept {
            return data == rhs.data;
        }

        [[nodiscard]] bool operator!=(const chot::square &rhs) const noexcept {
            return !this->operator==(rhs);
        }
    };

    inline std::ostream &operator<<(std::ostream &stream, const chot::square &square) {
        const auto file = static_cast<std::uint8_t>(square.file());
        const auto rank = static_cast<std::uint8_t>(square.rank());
        stream << char(file + 'a') << rank + 1;

        return stream;
    }

} // chot

#endif //CHOT_SQUARE_HPP
