#pragma once

#include <type_traits>
#include <tuple>

namespace GPlatform {

template<typename T>
struct invocable_traits;

// For lambdas
template <typename T>
struct invocable_traits: public invocable_traits<decltype(&T::operator())>
{
};

// All other cases
template <typename T, typename ResT, typename... Args>
struct invocable_traits<ResT(T::*)(Args...) const>
{
    using result_type       = ResT;
    using argument_types    = std::tuple<Args...>;
};

}// namespace GPlatform
