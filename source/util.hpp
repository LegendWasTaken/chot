#ifndef CHOT_UTIL_HPP
#define CHOT_UTIL_HPP

#include <concepts>

namespace chot::detail {
    template<typename T>
    requires std::integral<T>
    std::uint8_t bit_index(T value) {
        auto index = std::uint8_t(0);

        while (value != 0) {
            value >>= 1;
            index++;
        }

        return index - 1;
    }
}

#endif //CHOT_UTIL_HPP
