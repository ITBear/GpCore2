#pragma once

#include "../GpCore_global.hpp"
#include "../Constexpr/GpConstexprIterator.hpp"
#include <optional>

namespace GPlatform {
namespace Algo {

template<typename T>
[[nodiscard]] constexpr std::optional<size_t>   FindElementId (const T& aContainer, const typename T::value_type& aElement) noexcept
{
    static_assert(has_random_access_iter_v<T>, "Type has no random access iterator");

    size_t id = 0;

    for (const typename T::value_type& v: aContainer)
    {
        if (v == aElement)
        {
            return id;
        }

        id++;
    }

    return std::nullopt;
}

}//Algo
}//GPlatform
