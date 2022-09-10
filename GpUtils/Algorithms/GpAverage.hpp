#pragma once

#include "../GpUtils_global.hpp"

namespace GPlatform::Algo {

template<typename T, typename... Ts>
[[nodiscard]] constexpr T   Average (Ts... aArgs) noexcept
{
    const T sum = (T(aArgs) + ...);
    const T cnt = sizeof...(Ts);

    return sum / cnt;
}

}//namespace GPlatform::Algo
