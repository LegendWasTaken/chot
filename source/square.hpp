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

        [[nodiscard]] static std::uint8_t data_for_index(chot::square_index index) noexcept;

    public:
        square(std::uint8_t file, std::uint8_t rank);

        square(chot::file file, chot::rank rank);

        explicit square(chot::square_index index);

        [[nodiscard]] static square from_bits(std::uint64_t bits);

        [[nodiscard]] chot::square_index index() const noexcept;

        [[nodiscard]] chot::file file() const noexcept;

        [[nodiscard]] chot::rank rank() const noexcept;

        [[nodiscard]] bool operator==(const chot::square &rhs) const noexcept;

        [[nodiscard]] bool operator!=(const chot::square &rhs) const noexcept;
    };

    inline std::ostream &operator<<(std::ostream &stream, const chot::square &square) {
        const auto file = static_cast<std::uint8_t>(square.file());
        const auto rank = static_cast<std::uint8_t>(square.rank());
        stream << char(file + 'a') << rank + 1;

        return stream;
    }

} // chot

#endif //CHOT_SQUARE_HPP
