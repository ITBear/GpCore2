#pragma once

#include <type_traits>
#include <tuple>

namespace GPlatform {

template<typename>          struct is_tuple                                     : public std::false_type {};
template<typename... Ts>    struct is_tuple<std::tuple<Ts...>>                  : public std::true_type {};
template<typename... Ts>    struct is_tuple<const std::tuple<Ts...>>            : public std::true_type {};
template<typename... Ts>    struct is_tuple<volatile std::tuple<Ts...>>         : public std::true_type {};
template<typename... Ts>    struct is_tuple<const volatile std::tuple<Ts...>>   : public std::true_type {};

template <typename _Tp>
constexpr bool is_tuple_v = is_tuple<_Tp>::value;

}// namespace GPlatform
