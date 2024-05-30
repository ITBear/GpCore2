#pragma once

#include <type_traits>
#include <array>

namespace GPlatform {

template<class T>
struct is_std_array : std::false_type {};

template<class T, std::size_t N>
struct is_std_array<std::array<T, N>> : std::true_type {};

}// namespace GPlatform
