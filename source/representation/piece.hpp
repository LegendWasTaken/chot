#ifndef CHOT_PIECE_HPP
#define CHOT_PIECE_HPP

#include <cctype>
#include <cstdint>
#include <limits>

namespace chot {
    namespace piece {
        enum class type : std::uint8_t {
            white_pawn = 0,
            white_knight = 1,
            white_bishop = 2,
            white_rook = 3,
            white_queen = 4,
            white_king = 5,
            black_pawn = 6,
            black_knight = 7,
            black_bishop = 8,
            black_rook = 9,
            black_queen = 10,
            black_king = 11
        };

        [[nodiscard]] inline uint8_t material_value(type type) {
            switch (static_cast<uint8_t>(type) % 6) {
                case 0:
                    return 1;
                case 1:
                    return 3;
                case 2:
                    return 3;
                case 3:
                    return 5;
                case 4:
                    return 9;
                case 5:
                    return std::numeric_limits<std::uint8_t>::max();
            }
            return 255; // unreachable
        }

        [[nodiscard]] inline type from(char letter) {
            const auto lowercase = static_cast<char>(std::tolower(letter));
            const auto is_white = bool(std::isupper(letter));
            const auto offset = static_cast<std::uint8_t>([lowercase, is_white]() {
                const auto white_offset = is_white ? 0 : 6;
                switch (lowercase) {
                    case 'p':
                        return 0 + white_offset;
                    case 'n':
                        return 1 + white_offset;
                    case 'b':
                        return 2 + white_offset;
                    case 'r':
                        return 3 + white_offset;
                    case 'q':
                        return 4 + white_offset;
                    case 'k':
                        return 5 + white_offset;
                    default:
                        return 0;
                }
            }());
            return static_cast<type>(offset);
        }
    }

    class castling {
    private:
        std::uint8_t rights = 0;
    public:
        castling() = default;

        castling(bool white_kingside, bool white_queenside, bool black_kingside, bool black_queenside) {
            if (white_kingside) rights |= 1;
            if (white_queenside) rights |= 2;
            if (black_kingside) rights |= 4;
            if (black_queenside) rights |= 8;
        }

        [[nodiscard]] bool white_kingside() const noexcept { return rights & 1; }
        [[nodiscard]] bool white_queenside() const noexcept { return rights & 2; }
        [[nodiscard]] bool black_kingside() const noexcept { return rights & 4; }
        [[nodiscard]] bool black_queenside() const noexcept { return rights & 8; }
        [[nodiscard]] bool kingside(bool is_white) const noexcept { return is_white ? white_kingside() : black_kingside(); }
        [[nodiscard]] bool queenside(bool is_white) const noexcept { return is_white ? white_queenside() : black_queenside(); }
        void disallow_kingside(bool is_white) noexcept { rights &= ~(std::uint8_t(is_white ? 1 : 4)); }
        void disallow_queenside(bool is_white) noexcept { rights &= ~(std::uint8_t(is_white ? 2 : 8)); }
        void disallow(bool is_white) noexcept {
            disallow_kingside(is_white);
            disallow_queenside(is_white);
        }
    };
}

#endif //CHOT_PIECE_HPP
