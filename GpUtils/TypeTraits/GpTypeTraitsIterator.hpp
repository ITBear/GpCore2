#pragma once

#include "GpTypeTraitsArray.hpp"
#include "../Types/Numerics/GpNumericTypes.hpp"

//TODO remove from C++20
#if  (__cplusplus >= 202002L) && __has_include(<concepts>)
#   include <concepts>
#   undef NO_CONCEPT_random_access_iterator
#else
#   include <vector>
#   include <string>
#   include <string_view>
#   include <experimental/type_traits>
#   define NO_CONCEPT_random_access_iterator
#endif//#if  __has_include(<concepts>)

namespace GPlatform {

template<class T>
using has_iterator_v                            = typename T::iterator;

template<typename T, typename OTHER = void>
using detected_iter_t                           = std::experimental::detected_or_t<OTHER, has_iterator_v, T>;

#if defined(NO_CONCEPT_random_access_iterator)
    template <typename T>
    static constexpr bool has_random_access_iter_v =    std::is_same_v<std::string, T>
                                                     || std::is_same_v<std::string_view, T>
                                                     || std::is_same_v<std::vector<std::byte>, T>
                                                     || std::is_same_v<std::vector<u_int_8>, T>
                                                     || is_std_array<T>::value;
#else
    template <typename T>
    static constexpr bool has_random_access_iter_v  = std::random_access_iterator<detected_iter_t<T, void>>;
#endif

}//GPlatform
