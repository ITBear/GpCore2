#pragma once

#include "../GpCore_global.hpp"

namespace GPlatform{
namespace Algo {

template<typename T, typename... Ts>
[[nodiscard]] constexpr T	Average (Ts... aArgs) noexcept
{
    const T sum = (T(aArgs) + ...);
    const T cnt = sizeof...(Ts);

    return sum / cnt;
}

}//Algo
}//GPlatform
