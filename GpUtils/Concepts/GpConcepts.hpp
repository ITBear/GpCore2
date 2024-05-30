#pragma once

#include <iterator>
#include <type_traits>

namespace GPlatform::Concepts {

template <typename T>
concept HasRandomAccessIter = requires(T t)
{
    typename T::iterator;
    requires std::random_access_iterator<typename T::iterator>;
};

template <typename T>
concept HasContiguousIter = requires(T t)
{
    typename T::iterator;
    requires std::contiguous_iterator<typename T::iterator>;
};

template <typename T>
concept HasForwardIter = requires(T t)
{
    typename T::iterator;
    requires std::forward_iterator<typename T::iterator>;
};

template <typename T, size_t Size>
concept SizeOfValueType = requires()
{
    requires sizeof(typename T::value_type) == Size;
};

template <typename T>
concept IsPointer = requires()
{
    requires std::is_pointer_v<T>;
};

template <typename T>
concept IsReference = requires()
{
    requires std::is_reference_v<T>;
};

template <typename T>
concept IsIntergal = requires()
{
    requires
        std::is_integral_v<T>
    && !std::is_same_v<T, bool>;
};

template <typename T>
concept IsFloatingPoint = requires()
{
    requires std::is_floating_point_v<T>;
};

template <typename T>
concept IsArithmetic = requires()
{
    requires
        (std::is_integral_v<T> || std::is_floating_point_v<T>)
    && !std::is_same_v<T, bool>;
};

template <typename T>
concept IsSigned = requires()
{
    requires std::is_signed_v<T>;
};

template <typename T>
concept IsIntegralUpTo128 =
       std::is_integral_v<T>
    && (sizeof(T) <= 16);

template <typename T>
concept IsIntegralUpTo64 =
       std::is_integral_v<T>
    && (sizeof(T) <= 8);

}// namespace GPlatform::Concepts
