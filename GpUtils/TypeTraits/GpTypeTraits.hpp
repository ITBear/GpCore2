#pragma once

#include <type_traits>
#include <array>
#include <tuple>

namespace GPlatform {

// --------- Is tuple ---------
template<typename>          struct is_tuple                                     : public std::false_type {};
template<typename... Ts>    struct is_tuple<std::tuple<Ts...>>                  : public std::true_type {};
template<typename... Ts>    struct is_tuple<const std::tuple<Ts...>>            : public std::true_type {};
template<typename... Ts>    struct is_tuple<volatile std::tuple<Ts...>>         : public std::true_type {};
template<typename... Ts>    struct is_tuple<const volatile std::tuple<Ts...>>   : public std::true_type {};

template <typename _Tp>
constexpr bool is_tuple_v = is_tuple<_Tp>::value;

// --------- Invocable ---------
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

// --------- Is std array ---------
template<class T>
struct is_std_array : std::false_type {};

template<class T, std::size_t N>
struct is_std_array<std::array<T, N>> : std::true_type {};

// --------- Is base of any ---------
template<typename Derived, typename... Bases>
struct is_base_of_any : std::disjunction<std::is_base_of<Bases, Derived>...> {};

template<typename Derived, typename... Bases>
constexpr bool is_base_of_any_v = is_base_of_any<Derived, Bases...>::value;

}// namespace GPlatform
